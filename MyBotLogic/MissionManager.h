#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"
#include <vector>
#include "Mission.h"
#include "NPCManager.h"

class Planificateur
{
public:
    //A changer quand le vrai Planif sera fait
    static std::vector<Mission> planifiedMissions(Mission mission)
    {
        std::vector<Mission> subMissions;
        subMissions.push_back(mission);
    }
};
class MissionManager : public Singleton
{
private:

    static MissionManager m_instance;
    std::vector<Mission> m_missions;
    MissionManager()
    {
        m_missions.reserve(10*NPCManager::get()->getNpcs().size());
    }
public:
    static MissionManager* get()
    {
        return &m_instance;
    }
    std::vector<Mission> getMissions()
    {
        return m_missions;
    }
    void subscribeMission(Mission mission)
    {
        //Consulter Planificateur pour obtenir les sous missions
        std::vector<Mission> subMissions = Planificateur::planifiedMissions(mission);
        m_missions.insert(m_missions.end(), subMissions.begin(), subMissions.end());
    }

};


#endif //!_MISSIONMANAGER_H_