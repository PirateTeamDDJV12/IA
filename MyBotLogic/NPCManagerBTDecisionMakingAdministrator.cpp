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
}
