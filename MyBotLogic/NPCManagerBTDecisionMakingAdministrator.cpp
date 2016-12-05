#include "NPCManagerBTDecisionMakingAdministrator.h"
#include "NPCManager.h"
#include "Map.h"
#include <iostream>

using namespace BehaviourTree;



NPCManagerBTDecisionMakingAdministrator::NPCManagerBTDecisionMakingAdministrator() :
    m_behaviorTreeRoot{BehaviourTree::BlocFabric::initiateRootAsCompositeBloc<BehaviourTree::BlocTernary>("DecisionMakingNpcManagerRoot")}
{
    this->init();
}


void NPCManagerBTDecisionMakingAdministrator::init()
{
    BlocTernary* ternarySelectorRoot = m_behaviorTreeRoot.getRoot()->as<BlocTernary>();

    // First Layer => has a goal? 
    // yes : assign an AStar
    // no : go to next layer : is the area explored ?

    // Has a goal check bloc
    ternarySelectorRoot->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        Map *myMap = Map::get();

        myMap->visitTile(m_currentNpc->getCurrentTileId());
        if(!m_currentNpc->hasGoal() && m_goalMap.find(m_currentNpc->getId()) != end(m_goalMap))
        {
            unsigned int goalTile = m_goalMap[m_currentNpc->getId()];
            m_currentNpc->setGoal(goalTile);
            return general::result::SUCCESS;
        }
        else if(m_currentNpc->hasGoal())
        {
            return general::result::SUCCESS;
        }
        return general::result::FAIL;
    },
            "HasAGoal"
        )
    );

    // This node is executed if the npc has a goal assigned
    // YES : assign AStar
    ternarySelectorRoot->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        // If the npc has already a path we don't do the A* star again
        if(m_currentNpc->getPathSize() <= 1)
        {
            m_currentNpc->calculPath();
            // If the A* has failed
            if(m_currentNpc->getPathSize() <= 1)
                return general::result::FAIL;
        }
        return general::result::SUCCESS;
    },
            "AStar"
        )
    );

    // Second Layer node creation
    BlocRef exploredAreaTernaryRef = BlocFabric::createCompositeBloc<BlocTernarySelector>("ExploredAreaNode");

    BlocTernarySelector* exploredAreaTernarySelector = exploredAreaTernaryRef->as<BlocTernarySelector>();



    // Connection of the first layer with the second layer (NO case)
    ternarySelectorRoot->connect(exploredAreaTernaryRef);


    // Second Layer => is the area explored ? 
    // This layer is reached only if the npc hasn't found a goal
    // yes : go to the next layer : a door is there ?
    // no : regular explore assignement

    // the area is explored checker bloc
    exploredAreaTernarySelector->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        // If the area is explored then is there a door ? (call first children) 
        // If not then we need to keep exploring (call second children)
        return /*is the area explored ?*/true ? general::result::SUCCESS : general::result::FAIL;
    },
            "IsTheAreaExplored"
        )
    );

    // This node is called if the zone has not been explored
    // NO : Regular Explore Assignement
    std::string exploredAreaNoCaseOrRegularExploreBlocName{"RegularExplore"};

    exploredAreaTernarySelector->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        // Make the npc explore the zone he is in
        return general::result::SUCCESS;
    },
            exploredAreaNoCaseOrRegularExploreBlocName
        )
    );


    // Third Layer node creation
    BlocRef aDoorIsThereTernaryRef = BlocFabric::createCompositeBloc<BlocTernarySelector>("aDoorIsThereNode");

    BlocTernarySelector* aDoorIsThereTernarySelector = aDoorIsThereTernaryRef->as<BlocTernarySelector>();


    // connection of the second layer with the third layer (YES case)
    // before because the yes case must be before the no case.
    exploredAreaTernarySelector->connectBefore(aDoorIsThereTernaryRef, exploredAreaNoCaseOrRegularExploreBlocName);

    // This layer is reached only if the npc has no goal and the zone has been entirely explored
    // Third Layer => is there a door ? 
    // Yes : Change area
    // No : Find hidden door

    // Is there a checker bloc
    aDoorIsThereTernarySelector->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        // If there is a door in the zone then we need to try to go through it (call first children)
        // If not then there must be a hidden door and we need to find it (call second children)
        return /*Is there a door?*/true ? general::result::SUCCESS : general::result::FAIL;
    },
            "IsThereADoor"
        )
    );

    // This node is called if there is no goal, the zone has been and a door is visible in the zone
    //YES : change area
    aDoorIsThereTernarySelector->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        // Create a Mission that in the end will make the npc reach the other side of the door
        return general::result::SUCCESS;
    },
            "ChangeArea"
        )
    );


    // This node is called if there is no goal, the zone has been and a door is not visible in the zone
    //NO : Find hidden door
    aDoorIsThereTernarySelector->connect(
        BlocFabric::createGeneralAction(
            [this]() {
        // If there  is no door in the zone there must be an hidden door and the npc needs to search for it
        return general::result::SUCCESS;
    },
            "FindHiddenDoor"
        )
    );
}

void NPCManagerBTDecisionMakingAdministrator::operator()()
{
    Map *myMap = Map::get();

    // Get all the npcs
    const std::vector<Npc *> npcs = NPCManager::get()->getNpcs();
    // Get best goal for each NPCs
    m_goalMap = std::move(myMap->getBestGoalTile(npcs));
    // For every npc we execute the decision tree
    for(auto npc : npcs)
    {
        m_currentNpc = npc;
        m_behaviorTreeRoot();
    }

}