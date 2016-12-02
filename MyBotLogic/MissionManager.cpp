#include "MissionManager.h"

MissionManager MissionManager::m_instance;

//ajout objectif et découpage en mission
void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{

    //Consulter Planificateur pour obtenir les sous missions
    Objectif newObj = Objectif(m_objectivesCount, fromZone, destinationZone);
    ++m_objectivesCount;
    retourPlanificateur stepsToReachGoal = Planificateur::createMissionsToReachGoal(newObj);

    m_objectives.insert(std::make_pair(newObj, stepsToReachGoal));
}


//Ajouter mission sans objectifs
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