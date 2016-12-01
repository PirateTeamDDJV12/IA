
#ifndef _MISSION_H_
#define _MISSION_H_

class Mission
{
public:
    enum MissionType
    {
        CHANGE_ZONE,
        OPEN_DOOR,
    };
private:
    MissionType m_mType;
    unsigned int m_id; //attribué par MissionManager lors de l'assignation
    unsigned m_objective;
    Mission* m_prev = nullptr;
    Mission* m_next = nullptr;
    bool m_status = false;
public:

    Mission(MissionType t, unsigned obj)
        : m_mType{ t }, m_objective{ obj }
    {
    }

    unsigned getObjective() const
    {
        return m_objective;
    }
    MissionType GetMissionType()
    {
        return m_mType;
    }

    //Ne pasn appeler, passer par setDependanceMission
    Mission setPrevMission(Mission* m)
    {
        m_prev = m;
    }

   

    void setDependanceMission(Mission *m)
    {
        m_next = m;
        m->setPrevMission(this);
    }

    bool isSuccess() const
    {
        return m_status;
    }

    void success()
    {
        m_status = true;
        delete m_next;
        m_next = nullptr;
    }

private:
    void setSuccesDependanceMission() const
    {
        m_next->success();
    }
public:
    void setId(unsigned id)
    {
        m_id = id;
    }
    unsigned int getId()
    {
        return m_id;
    }
};

#endif //!_MISSION_H_