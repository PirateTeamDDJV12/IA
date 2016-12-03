#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"
#include "Mission.h"
#include "Planificateur.h"
#include <utility>
#include <vector>
#include <algorithm>
#include <map>
#include <cstring>


struct Objectif
{
    unsigned int m_id;
    unsigned int m_from;
    unsigned int m_destinaton;

    bool operator==(Objectif other) const
    {
        return (m_from == other.m_from && m_destinaton == other.m_destinaton);
    }
    bool operator>(Objectif other) const
    {
        return false;
    }
    bool  operator<(Objectif other) const
    {
        return true;
    }
    Objectif(unsigned int id, unsigned int from, unsigned int dest) : m_id{ id }, m_from{ from }, m_destinaton{ dest }
    {}
};







//MissionManager
class MissionManager : public Singleton
{
private:
    Planificateur planificateur;
    static MissionManager m_instance;
    //Missions sans objectifs
    std::vector<Mission> m_missions;
    //objectifs et missions associées
    std::map<Objectif, std::vector<Mission>> m_objectives;
    enum { MissionMax = 200 };

    unsigned int m_missionsCount;
    unsigned int m_objectivesCount;
    MissionManager() :m_missionsCount{ 0 }, m_objectivesCount{ 0 }
    {
        m_missions.reserve(MissionMax);
    }
public:
    static MissionManager* get()
    {
        return &m_instance;
    }

    std::map<Objectif, std::vector<Mission>>* getObjectives()
    {
        return &m_objectives;
    }


    std::vector<Mission>* getMissions()
    {
        return &m_missions;
    }

    //ajouter un Objectif (Changer de Zone)
    void subscribeGoal(unsigned fromZone, unsigned destinationZone);


    //Ajouter une mission (Se déplacer/ intéragir)
    std::pair<std::vector<Mission>, std::vector<Mission>> subscribeMission(Mission mission, unsigned npcId, unsigned zoneId);

    //Supprimer un Objectif
    void deleteObjectif(unsigned int idToDelete, unsigned int from, unsigned int dest )
    {
        m_objectives.erase(Objectif(idToDelete, from, dest));
    }

    //Supprimer une mission d'un objectif

    void deleteMissionFromObjective(unsigned int idObjective);
   

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