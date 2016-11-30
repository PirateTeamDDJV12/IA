#include "NPCManagerBTDecisionMakingAdministrator.h"
#include "Npc.h"

#include <algorithm>

using namespace BehaviourTree;


void NPCManagerBTDecisionMakingAdministrator::init()
{
    BlocTernarySelector* ternarySelectorRoot = m_behaviorTreeRoot.as<BlocTernarySelector>();


    // First Layer => has a goal? 
    // yes : assign an AStar
    // no : go to next layer : is the area explored ?

    // Has a goal check bloc
    ternarySelectorRoot->connect(
        BlocFabric::createGeneralAction(
            []() {
                // TODO
                return /*Has a goal*/true ? general::result::SUCCESS : general::result::FAIL;
            },
            "HasAGoal"
        )
    );

    // YES : assign AStar
    ternarySelectorRoot->connect(
        BlocFabric::createGeneralAction(
            []() {
                // TODO
            },
            "AStar"
        )
    );


    // Second Layer node creation
    BlocRef exploredAreaTernaryRef = BlocFabric::createCompositeBloc<BlocTernarySelector>("ExploredAreaNode");

    BlocTernarySelector* exploredAreaTernarySelector = exploredAreaTernaryRef->as<BlocTernarySelector>();



    // connection of the first layer with the second layer (NO case)
    ternarySelectorRoot->connect(exploredAreaTernaryRef);



    // Second Layer => is the area explored ? 
    // yes : go to the next layer : a door is there ?
    // no : regular explore assignement

    // a door is there checker bloc
    exploredAreaTernarySelector->connect(
        BlocFabric::createGeneralAction(
            []() {
                // TODO
                return /*is the area explored ?*/true ? general::result::SUCCESS : general::result::FAIL;
            },
            "IsTheAreaExplored"
        )
    );


    // NO : Regular Explore Assignement
    std::string exploredAreaNoCaseOrRegularExploreBlocName{ "RegularExplore" };

    exploredAreaTernarySelector->connect(
        BlocFabric::createGeneralAction(
            []() {
                // TODO
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
}
