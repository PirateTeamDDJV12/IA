#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"
#include "Mission.h"
#include "NPCManager.h"
#include <utility>
#include <vector>

typedef std::vector<Mission> retourPlanificateur;
class Planificateur
{
public:

    //A changer quand le vrai Planif sera fait
    static retourPlanificateur planifiedMissions(Mission mission)
    {
        std::vector<Mission> subMissions;
        subMissions.push_back(mission);
    }
    static retourPlanificateur createMissionsToReachGoal(unsigned from, unsigned destination);

};
class MissionManager : public Singleton
{
private:

    static MissionManager m_instance;
    retourPlanificateur m_missions;
    std::vector<std::pair<unsigned, unsigned>> m_objectifs;

    MissionManager()
    {
        m_missions.reserve(10*(NPCManager::get()->getNpcs().size()));
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


    void subscribeGoal(unsigned from, unsigned destination)
    {
        retourPlanificateur stepsToReachGoal = Planificateur::createMissionsToReachGoal(from, destination);
        m_missions.insert(m_missions.end(), stepsToReachGoal.begin(), stepsToReachGoal.end());
    }

    void subscribeMission(Mission mission)
    {
        //Consulter Planificateur pour obtenir les sous missions
        retourPlanificateur subMissions = Planificateur::planifiedMissions(mission);
        m_missions.insert(m_missions.end(), subMissions.begin(), subMissions.end());
    }


};


#endif //!_MISSIONMANAGER_H_