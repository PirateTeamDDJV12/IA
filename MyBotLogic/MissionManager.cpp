#include "MissionManager.h"

MissionManager MissionManager::m_instance;


void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{
    m_objectives.push_back(std::make_pair(fromZone, destinationZone));
    //Consulter Planificateur pour obtenir les sous missions
    retourPlanificateur stepsToReachGoal = Planificateur::createMissionsToReachGoal(fromZone, destinationZone);
    m_missions.insert(m_missions.end(), stepsToReachGoal.begin(), stepsToReachGoal.end());
}

void MissionManager::subscribeMission(Mission mission, unsigned int npcId)
{
    
    mission.setId(m_missionsCount);
    ++m_missionsCount;
    //Consulter Planificateur pour obtenir les sous missions
    retourPlanificateur subMissions = Planificateur::planifiedMissions(mission, npcId);
    for (Mission submis : subMissions)
    {
        mission.setId(m_missionsCount);
        ++m_missionsCount;
    }
    m_missions.insert(m_missions.end(), subMissions.begin(), subMissions.end());
}