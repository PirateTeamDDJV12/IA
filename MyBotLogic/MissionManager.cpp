#include "MissionManager.h"

MissionManager MissionManager::m_instance;

//ajout objectif et d�coupage en mission
void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{

    //Consulter Planificateur pour obtenir les sous missions
    Objectif newObj = Objectif(m_objectivesCount, fromZone, destinationZone);
    ++m_objectivesCount;
    std::pair<std::vector<Mission>, std::vector<Mission>> stepsToReachGoal = Planificateur::createMissionsToReachGoal(m_objectivesCount,fromZone,destinationZone);

    m_objectives.insert(std::make_pair(newObj, stepsToReachGoal.first));
    m_missions.insert(m_missions.end(),stepsToReachGoal.second.begin(), stepsToReachGoal.second.end());
}


//Ajouter mission sans objectifs
void MissionManager::subscribeMission(Mission mission, unsigned int npcId)
{

    //Consulter Planificateur pour obtenir les sous missions et les enregistrer
    std::vector<Mission> subMissions = Planificateur::planifyMissions(mission, npcId);
    for (Mission submis : subMissions)
    {
        submis.setId(m_missionsCount);
        ++m_missionsCount;
    }
    m_missions.insert(m_missions.end(), subMissions.begin(), subMissions.end());
}