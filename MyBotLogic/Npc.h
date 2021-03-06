#ifndef NPC_H
#define NPC_H

#include "Logger.h"

#include <string>
#include <vector>

#ifdef _DEBUG
#define BOT_LOGIC_DEBUG_NPC
#endif

#ifdef BOT_LOGIC_DEBUG_NPC
#define BOT_LOGIC_NPC_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
#define BOT_LOGIC_NPC_LOG(logger, text, autoEndLine) 0
#endif

struct Action;

class Npc 
{
public:
    enum npcState {
        NONE,
        MOVING,
        WAITING,
        ARRIVED,
        EXPLORING,
        INTERACTING
    };

private:
    npcState m_currentState, m_nextState;
    unsigned int m_id;
    unsigned int m_goal;
    bool m_hasGoal;
    unsigned int m_target;
    unsigned int m_turnCount;
    unsigned int m_zone;
    std::vector<unsigned int> m_path;
    std::vector<unsigned int> m_historyTiles;
    std::vector<Action*> m_nextActions;
    Logger m_logger;

public:
    Npc(unsigned int a_id, unsigned int a_tileId, const std::string& a_path, unsigned int zone);

    unsigned int getId()
    {
        return m_id;
    }

    // Main fonction for our Npc's FSM 
    void update();

    // Stop everything the npc is doing
    bool stopEverything();

    // Remove all move action from next_action List
    void stopMoving();

    // Remove all interact action from next_action List
    void stopInteract();

    // Unstack m_nextAction and act according to action_type
    void unstackActions();

    void calculPath();
    // check path integrity, if pass is corrupted, try to find a new path return true if found
    bool updatePath();

    template<class T>
    void DisplayVector(std::string, const std::vector<T>);

    unsigned int getCurrentTileId()
    {
        return m_path.back();
    }
    int getNextPathTile()const;
    size_t getPathSize() const
    {
        return m_path.size();
    }
    std::vector<unsigned int> getWholePath() const
    {
        return m_path;
    }
    
    void setGoal(unsigned int a_id)
    {
        m_goal = a_id;
        m_hasGoal = true;
    }
    void unsetGoal()
    {
        m_hasGoal = false;
    }
    bool isOnGoalTile() const
    {
        return m_currentState == ARRIVED;
    }
    bool hasGoal() const
    {
        return m_hasGoal;
    }

    unsigned int getZone() const
    {
        return m_zone;
    }
    void setZone(unsigned int zone)
    {
        m_zone = zone;
    }

    void moveForwardOnPath() // remove last tileId from m_path
    {
        m_path.pop_back();
    }

    std::vector<Action*>& getActions()
    {
        return m_nextActions;
    }

    unsigned int getGoal() const
    {
        return m_goal;
    }

    npcState getStatus() const
    {
        return m_currentState;
    }

    void setCurrentTile(unsigned tile_id);  
private:
    void explore();
    void followPath();
    void wait();
    void interact();
};


#endif //NPC_H
