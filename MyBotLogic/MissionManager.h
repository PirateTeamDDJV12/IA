#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include "Singleton.h"

class MissionManager : public Singleton
{
private:

    static MissionManager m_instance;
    MissionManager() = default;
public:
    static MissionManager* get()
    {
        return &m_instance;
    }
};

#endif //!_MISSIONMANAGER_H_