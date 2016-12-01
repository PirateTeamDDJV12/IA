#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"
#include "Mission.h"

#include <utility>
#include <vector>

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
    std::vector<std::pair<unsigned, unsigned>> m_objectifs;
    enum { MissionMax = 200 };
    MissionManager()
    {
        m_missions.reserve(MissionMax);
        m_objectifs.reserve(MissionMax);
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
    


};


#endif //!_MISSIONMANAGER_H_