#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"
#include "Mission.h"

#include <utility>
#include <vector>
#include <algorithm>

// Type de Retour du planificateur. Il suffira de remplacer ici une fois que
//le vrai planificateur sera fait
typedef std::vector<Mission> retourPlanificateur;


// A enlever quand le vrai Planif sera fait
class Planificateur
{
public:


    static retourPlanificateur planifiedMissions(Mission mission, unsigned int npcID)
    {
        std::vector<Mission> subMissions;
        subMissions.push_back(mission);
        return subMissions;
    }
    static retourPlanificateur createMissionsToReachGoal(unsigned from, unsigned destination)
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
    retourPlanificateur m_missions;
    std::vector<std::pair<unsigned, unsigned>> m_objectives;
    enum { MissionMax = 200 };
    unsigned int m_missionsCount;
    unsigned int m_objectivesCount;
    MissionManager() :m_missionsCount{ 0 }, m_objectivesCount{0}
    {
        m_missions.reserve(MissionMax);
        m_objectives.reserve(MissionMax);
    }
public:
    static MissionManager* get()
    {
        return &m_instance;
    }
    retourPlanificateur getMissions()
    {
        return m_missions;
    }

    //ajouter un Objectif (Changer de Zone)
    void subscribeGoal(unsigned fromZone, unsigned destinationZone);
    
    
    //Ajouter une mission (Se déplacer/ intéragir)
    void subscribeMission(Mission mission, unsigned int npcId);
    
    //Supprimer une mission
    void deleteMission(unsigned int missionId)
    {
        //Une seule mission par npc, on la place à la fin
        std::remove_if(m_missions.begin(), m_missions.end(), [missionId](Mission mission) {return mission.getId() == missionId; });
        //et on l'efface
        m_missions.erase(m_missions.end());
    }


};


#endif //!_MISSIONMANAGER_H_