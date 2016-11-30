#ifndef NPCMANAGERBTDECISIONMAKINGADMINISTRATOR_H_INCLUDED
#define NPCMANAGERBTDECISIONMAKINGADMINISTRATOR_H_INCLUDED


#include "BehaviourTree/BehaviourTreeModule.h"

#include <vector>
#include <string>


class Npc;

class NPCManagerBTDecisionMakingAdministrator
{
private:
    BehaviourTree::BlocRoot m_behaviorTreeRoot;


public:
    NPCManagerBTDecisionMakingAdministrator() :
        m_behaviorTreeRoot{ BehaviourTree::BlocFabric::initiateRootAsCompositeBloc<BehaviourTree::BlocTernarySelector>("DecisionMakingNpcManagerRoot") }
    {}


public:
    void init();


public:
    void operator()() 
    {
        m_behaviorTreeRoot();
    }
};

#endif //NPCMANAGERBTDECISIONMAKINGADMINISTRATOR_H_INCLUDED