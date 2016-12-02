#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"
#include "Mission.h"

#include <utility>
#include <vector>
#include <algorithm>
#include <map>

// Type de Retour du planificateur. Il suffira de remplacer ici une fois que
//le vrai planificateur sera fait
typedef std::vector<Mission> retourPlanificateur;

struct Objectif
{
    unsigned int m_id;
    unsigned int m_from;
    unsigned int m_destinaton;
    Objectif(unsigned int id, unsigned int from, unsigned int dest) : m_id{ id }, m_from{ from }, m_destinaton{ dest }
    {}
    std::vector<Mission> m_missionsObjectif;
};


// A enlever quand le vrai Planif sera fait
class Planificateur
{
public:




    static retourPlanificateur planifyMissions(Mission mission, unsigned int npcID)
    {
        std::vector<Mission> subMissions;
        subMissions.push_back(mission);
        return subMissions;
    }
    static retourPlanificateur createMissionsToReachGoal(Objectif newObj)
    {
        std::vector<Mission> subMissions;
        return subMissions;
    }

};





//MissionManager
class MissionManager : public Singleton
{
private:

    static MissionManager m_instance;
    //Missions sans objectifs (il yen aura ?)
    retourPlanificateur m_missions;
    //objectifs et missions associées
    std::vector<Objectif> m_objectives;
    enum { MissionMax = 200 };

    unsigned int m_missionsCount;
    unsigned int m_objectivesCount;
    MissionManager() :m_missionsCount{ 0 }, m_objectivesCount{ 0 }
    {
        m_missions.reserve(MissionMax);
        m_objectives.reserve(MissionMax);
    }
public:
    static MissionManager* get()
    {
        return &m_instance;
    }

    std::vector<Objectif>* getObjectives()
    {
        return &m_objectives;
    }


    retourPlanificateur* getMissions()
    {
        return &m_missions;
    }

    //ajouter un Objectif (Changer de Zone)
    void subscribeGoal(unsigned fromZone, unsigned destinationZone);


    //Ajouter une mission (Se déplacer/ intéragir)
    void subscribeMission(Mission mission, unsigned int npcId);

    //Supprimer un Objectif
    void deleteObjectif(unsigned int idToDelete)
    {
        m_objectives.erase(std::remove_if(m_objectives.begin(), m_objectives.end(), [idToDelete](Objectif obj) {return obj.m_id == idToDelete; }));
    }

    //Supprimer une mission d'un objectif

    void deleteMissionFromObjective(unsigned int idObjective)
    {
        auto itObjectiveToCheck = std::find_if(m_objectives.begin(), m_objectives.end(), [idObjective](Objectif obj)
        {return obj.m_id == idObjective; });
        //on met la mission à la fin du vector
        std::remove_if(itObjectiveToCheck->m_missionsObjectif.begin(), itObjectiveToCheck->m_missionsObjectif.end(), [itObjectiveToCheck](Mission mission)
        {return (mission.getId() == itObjectiveToCheck->m_missionsObjectif.front().getId()); });
        //et on l'efface
        itObjectiveToCheck->m_missionsObjectif.erase(itObjectiveToCheck->m_missionsObjectif.end());
    }

    //Supprimer une mission indépendante
        void deleteMission(unsigned int missionId)
    {
        //on met la mission à la fin du vector
        std::remove_if(m_missions.begin(), m_missions.end(), [missionId](Mission mission) {return mission.getId() == missionId; });
        //et on l'efface
        m_missions.erase(m_missions.end());
    }




};


#endif //!_MISSIONMANAGER_H_