#include "MissionManager.h"

MissionManager MissionManager::m_instance;


void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{
    m_objectifs.push_back(std::make_pair(fromZone, destinationZone));
    //Consulter Planificateur pour obtenir les sous missions
    retourPlanificateur stepsToReachGoal = Planificateur::createMissionsToReachGoal(fromZone, destinationZone);
    m_missions.insert(m_missions.end(), stepsToReachGoal.begin(), stepsToReachGoal.end());
}

void MissionManager::subscribeMission(Mission mission, unsigned int npcId)
{
    //Consulter Planificateur pour obtenir les sous missions
    retourPlanificateur subMissions = Planificateur::planifiedMissions(mission, npcId);

    //enregistrer missions rendues par le planificateur
    m_missions.insert(m_missions.end(), subMissions.begin(), subMissions.end());
}