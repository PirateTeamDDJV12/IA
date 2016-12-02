#include "MissionManager.h"

MissionManager MissionManager::m_instance;

//ajout objectif et découpage en mission
void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{
    
    //Consulter Planificateur pour obtenir les sous missions
    Objectif newObj = Objectif(m_objectivesCount, fromZone, destinationZone);
    ++m_objectivesCount;
    retourPlanificateur stepsToReachGoal = Planificateur::createMissionsToReachGoal(newObj);
    //enregistrer missions dans Objectif et register Objectif
    newObj.m_missionsObjectif.insert(m_missions.end(), stepsToReachGoal.begin(), stepsToReachGoal.end());
    m_objectives.push_back(newObj);
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