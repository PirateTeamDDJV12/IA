#ifndef NPCMANAGERBTDECISIONMAKINGADMINISTRATOR_H_INCLUDED
#define NPCMANAGERBTDECISIONMAKINGADMINISTRATOR_H_INCLUDED


#include "BehaviourTree/BehaviourTreeModule.h"
#include "Npc.h"

#include <map>

class NPCManagerBTDecisionMakingAdministrator
{
private:
    BehaviourTree::BlocRoot     m_behaviorTreeRoot;
    Npc                         *m_currentNpc;
    std::map<unsigned, unsigned> m_goalMap;

public:
    NPCManagerBTDecisionMakingAdministrator();


public:
    void init();


public:
    void operator()();
};

#endif //NPCMANAGERBTDECISIONMAKINGADMINISTRATOR_H_INCLUDED