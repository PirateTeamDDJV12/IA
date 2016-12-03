#include "MissionManager.h"
#include "NPCManager.h"

MissionManager MissionManager::m_instance;

//ajout objectif et découpage en mission
void MissionManager::subscribeGoal(unsigned fromZone, unsigned destinationZone)
{

    //Consulter Planificateur pour obtenir les sous missions
    Objectif newObj = Objectif(m_objectivesCount, fromZone, destinationZone);
    ++m_objectivesCount;
    std::pair<std::vector<Mission>, std::vector<Mission>> stepsToReachGoal = planificateur.createMissionsToReachGoal(fromZone,destinationZone);

    m_objectives.insert(std::make_pair(newObj, stepsToReachGoal.first));
    m_missions.insert(m_missions.end(),stepsToReachGoal.second.begin(), stepsToReachGoal.second.end());
}


//Ajouter mission sans objectifs
std::pair<std::vector<Mission>, std::vector<Mission>> MissionManager::subscribeMission(Mission mission, unsigned npcId,unsigned zoneId)
{

    //Consulter Planificateur pour obtenir les sous missions et les enregistrer
    std::pair<std::vector<Mission>, std::vector<Mission>> subMissions = planificateur.planifyMissions(mission, npcId);
    for (Mission submis : subMissions.second)
    {
        submis.setId(m_missionsCount);
        ++m_missionsCount;
    }
    m_missions.insert(m_missions.end(), subMissions.second.begin(), subMissions.second.end());
    return(subMissions);
}

void MissionManager::deleteMissionFromObjective(unsigned int idObjective)
{
    auto itObjectiveToCheck = std::find_if(m_objectives.begin(), m_objectives.end(), [idObjective](std::pair<Objectif, std::vector<Mission>> obj)
    {return obj.first.m_id == idObjective; });
    //on met la mission à la fin du vector
    std::remove_if(itObjectiveToCheck->second.begin(), itObjectiveToCheck->second.end(), [itObjectiveToCheck](Mission mission)
    {return (mission.getId() == itObjectiveToCheck->second.front().getId()); });
    //et on l'efface
    itObjectiveToCheck->second.erase(itObjectiveToCheck->second.end());
}