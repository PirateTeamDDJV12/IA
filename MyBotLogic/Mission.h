
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

    bool isSuccess() const
    {
        return m_status;
    }

    void success()
    {
        m_status = true;
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