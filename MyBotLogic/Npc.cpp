#include "Npc.h"
#include "Map.h"


#include "Globals.h"

#include <algorithm>


Npc::Npc(unsigned int a_id, unsigned int a_tileId, const std::string& a_path, unsigned int zone)
    : m_currentState{NONE}, m_nextState{EXPLORING}, m_id{a_id}, m_goal{}, m_hasGoal{false}, m_path{a_tileId}, m_nextActions{}, m_historyTiles{a_tileId}, m_turnCount{0}, m_zone {zone}
{
#ifdef BOT_LOGIC_DEBUG_NPC
    m_logger.Init(a_path, "Npc_" + std::to_string(m_id) + ".log");
#endif

    BOT_LOGIC_NPC_LOG(m_logger, "Configure", true);

};

void Npc::update()
{
    ++m_turnCount;
    BOT_LOGIC_NPC_LOG(m_logger, "\nTurn #" + std::to_string(m_turnCount) + "\n\tCurrent Tile : " + std::to_string(getCurrentTileId()), true);
    updatePath();
    BOT_LOGIC_NPC_LOG(m_logger, "\tEntering State Machine : ", true);
    do
    {
        //Set npc behaviour according to their state
        m_currentState = m_nextState;
        switch(m_currentState)
        {
            case(MOVING):
                followPath();
                break;
            case(WAITING):
                wait();
                break;
            case(EXPLORING):
                explore();
                break;
            case(INTERACTING):
                interact();
                break;
            case(ARRIVED):
                m_nextState = ARRIVED; 
                m_currentState = ARRIVED;
                break;
        }
    } while(m_currentState != m_nextState);
}

bool Npc::stopEverything()
{
    // deleting actions
    for(std::vector< Action* >::iterator it = m_nextActions.begin(); it != m_nextActions.end(); ++it)
    {
        delete (*it);
    }
    m_nextActions.clear();
    return true;
}

void Npc::stopMoving()
{
    // separate move and interact actions
    auto it = std::partition(std::begin(m_nextActions),
                             std::end(m_nextActions),
                             [](const Action* curAction) { return curAction->actionType != Action_Move; });

    // deleting move actions
    for(std::vector< Action* >::iterator itDelete = it; itDelete != m_nextActions.end(); ++itDelete)
    {
        delete (*itDelete);
    }
    m_nextActions.erase(it, std::end(m_nextActions));
}

void Npc::stopInteract()
{
    // separate move and interact actions
    auto it = std::partition(std::begin(m_nextActions),
                             std::end(m_nextActions),
                             [](const Action* curAction) { return curAction->actionType != Action_Interact; });
    // deleting interact actions
    for(std::vector< Action* >::iterator itDelete = it; itDelete != m_nextActions.end(); ++itDelete)
    {
        delete (*itDelete);
    }
    m_nextActions.erase(it, std::end(m_nextActions));
}

void Npc::unstackActions()
{
    while(m_nextActions.size())
    {
        Action* curAction{m_nextActions.back()};
        switch(curAction->actionType)
        {
            case Action_None:
                // Do nothing
                break;
            case Action_Move:
                //Go on
                moveForwardOnPath();
                break;
            case Action_Interact:
                // TODO - do nothing atm
                break;
        }
        m_nextActions.pop_back();
    }
}

void Npc::calculPath()
{
    if(!hasGoal())
    {
        return;
    }
    std::vector<unsigned> path = Map::get()->getNpcPath(getCurrentTileId(), m_goal);
    if(path.size() <= 0)
    {
        m_hasGoal = false;
        return;
    }
    m_path = path;
}

bool Npc::updatePath()
{
    BOT_LOGIC_NPC_LOG(m_logger, "\tUpdating Path ", true);
    DisplayVector("\t\tOld path: ", m_path);
    unsigned int oldTileId{ m_path.back()};
    Map* mapManager = Map::get();
    for(auto reverseIter = m_path.rbegin(); reverseIter != m_path.rend(); ++reverseIter)
    {
        if(!mapManager->canMoveOnTile(oldTileId, (*reverseIter)))
        {
            m_path = mapManager->getNpcPath(getCurrentTileId(), m_goal);
            DisplayVector("\t\tPath Updated : ", m_path);
            return true;
        }
        oldTileId = (*reverseIter);
    }
    BOT_LOGIC_NPC_LOG(m_logger, "\t\tNo update needed", true);
    return false;
}

int Npc::getNextPathTile() const
{
    if(m_path.size() == 1)
    {
        return -1; // empty path, only contains current tile
    }
    unsigned int index = m_path[m_path.size() - 2];
    return index;
}

void Npc::explore()
{
    BOT_LOGIC_NPC_LOG(m_logger, "-Explore", true);
    if(hasGoal())
    {
        BOT_LOGIC_NPC_LOG(m_logger, "\tNPC have a goal : " + std::to_string(m_goal), true);
        DisplayVector("\tNpc base path :", m_path);
        m_nextState = MOVING;
        return;
    }

    Map* mapPtr = Map::get();

    // Get the most influenced tile near the NPC
    int bestTile = mapPtr->getNearInfluencedTile(getCurrentTileId());

    // If we have not a best choice around us, let see a little bit futher
    if(bestTile < 0)
    {
        // TODO - Try to get the most influent tile around us in range of 2 instead of looking for a non visited tile
        // Get all non visited tiles
        std::vector<unsigned> nonVisitedTiles = std::move(mapPtr->getNonVisitedTile());
        DisplayVector("\t-Looking for the non visited tiles : ", nonVisitedTiles);
        for(unsigned index : nonVisitedTiles)
        {
            // Test if we can have a good path to this tile
            std::vector<unsigned> temp = std::move(mapPtr->getNpcPath(getCurrentTileId(), index,
                {Node::NodeType::FORBIDDEN, Node::NodeType::NONE, Node::NodeType::OCCUPIED}));

            // If we got a good path, let's configure this
            if(!temp.empty())
            {
                m_path = std::move(temp);
                m_target = index;
                m_nextState = MOVING;
                break;
            }
        }
    }
    else
    {
        // Set the best tile in path and push the action
        unsigned int bestTileUnsigned = bestTile;
        m_path = {bestTileUnsigned, getCurrentTileId()};
        m_historyTiles.push_back(bestTile);

        m_nextActions.push_back(new Move{m_id, mapPtr->getNextDirection(getCurrentTileId(), getNextPathTile())});

        BOT_LOGIC_NPC_LOG(m_logger, std::move("Deplacement vers " + std::to_string(bestTile)), true);

        m_nextState = EXPLORING;
    }
}

void Npc::followPath()
{
    BOT_LOGIC_NPC_LOG(m_logger, "-FollowPath", true);
    // Get the direction between the two last nodes of m_path
    if(getCurrentTileId() == m_goal)
    {
        m_nextState = ARRIVED;
        return;
    }
    if(getCurrentTileId() == m_target && !hasGoal())
    {
        m_nextState = EXPLORING;
        return;
    }
    m_nextActions.push_back(new Move{m_id, Map::get()->getNextDirection(getCurrentTileId(), getNextPathTile())});
    BOT_LOGIC_NPC_LOG(m_logger, "\tDeplacement vers " + std::to_string(getNextPathTile()), true);
    m_nextState = MOVING;
}

void Npc::wait()
{
    BOT_LOGIC_NPC_LOG(m_logger, "-Wait", true);
    // TODO - Test why we are blocked ?
}

void Npc::interact()
{
    BOT_LOGIC_NPC_LOG(m_logger, "-Interact", true);
    // TODO - interact with some fancy stuff
}

template<class T>
void Npc::DisplayVector(std::string info, const std::vector<T> v)
{
    std::string s{""};
    for(T u : v)
    {
        s += std::to_string(u) + " ";
    }
    BOT_LOGIC_NPC_LOG(m_logger, info + s, true);
}