#include "MissionManager.h"

MissionManager MissionManager::m_instance;


void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{
    
    //m_objectives.push_back(std::make_pair(fromZone, destinationZone));
    //Consulter Planificateur pour obtenir les sous missions
    Objectif newObj = Objectif(m_objectivesCount, fromZone, destinationZone);
    retourPlanificateur stepsToReachGoal = Planificateur::createMissionsToReachGoal(newObj);
    for (Mission submis : stepsToReachGoal)
    {
        submis.setId(m_missionsCount);
        ++m_missionsCount;
    }
    m_missions.insert(m_missions.end(), stepsToReachGoal.begin(), stepsToReachGoal.end());
    m_objectives.insert(std::make_pair(newObj, stepsToReachGoal));
}

void MissionManager::subscribeMission(Mission mission, unsigned int npcId)
{
    
    //Consulter Planificateur pour obtenir les sous missions et les enregistrer
    retourPlanificateur subMissions = Planificateur::planifyMissions(mission, npcId);
    for (Mission submis : subMissions)
    {
        submis.setId(m_missionsCount);
        ++m_missionsCount;
    }
    m_missions.insert(m_missions.end(), subMissions.begin(), subMissions.end());
}