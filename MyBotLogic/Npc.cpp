#include "Npc.h"
#include "Map.h"


#include "Globals.h"

#include <algorithm>
#include "NPCManager.h"
#include "ObjectManager.h"


Npc::Npc(unsigned int a_id, unsigned int a_tileId, const std::string& a_path, unsigned int zone)
    : m_currentState{NONE}, m_nextState{EXPLORING}, m_id{a_id}, m_goal{}, m_hasGoal{false}, m_path{a_tileId}, m_nextActions{}, m_historyTiles{a_tileId}, m_turnCount{0}, m_zone{zone}
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
                if(getCurrentTileId() == m_goal)
                {
                    Map::get()->getNode(getCurrentTileId())->setType(Node::OCCUPIED);
                    BOT_LOGIC_NPC_LOG(m_logger, "\tI am out, i am on my goal !", true);
                    //NPCManager::get()->getNpcUpdateBT().stopANpcByID(m_id);
                }
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
    std::vector<unsigned> path = Map::get()->getNpcPath(getCurrentTileId(), m_goal, {Node::FORBIDDEN, Node::OCCUPIED});
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
    Map* mapManager = Map::get();
    if(m_path.size() > 1)
    {
        unsigned int currentTile = getCurrentTileId();
        unsigned int nextTile = getNextPathTile();
        EDirection dir = mapManager->getNextDirection(currentTile, nextTile);
        EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
        if(mapManager->getNode(currentTile)->isBlockedByDoor(dir) || mapManager->getNode(nextTile)->isBlockedByDoor(invDir))
        {
            m_nextState = INTERACTING;
            return true;
        }
        if(!mapManager->canMoveOnTile(currentTile, nextTile))
        {
            auto newPath = mapManager->getNpcPath(getCurrentTileId(), m_goal, {Node::FORBIDDEN, Node::OCCUPIED});
            if(newPath.size() > 0)
            {
                if(newPath.size() > 30 && m_hasGoal)
                {
                    BOT_LOGIC_NPC_LOG(m_logger, "\t\tTo path to the goal is to long... don't want to go there", true);
                    m_hasGoal = false;
                    m_path = {currentTile};
                    return true;
                }
                m_path = newPath;
                DisplayVector("\t\tPath Updated : ", m_path);
                return true;
            }
        }
    }
    //unsigned int saveCurrentTile{m_path.back()};
    //unsigned int oldTileId{m_path.back()};
    //Map* mapManager = Map::get();
    //for(auto reverseIter = m_path.rbegin(); reverseIter != m_path.rend(); ++reverseIter)
    //{
    //    if(!mapManager->canMoveOnTile(oldTileId, (*reverseIter)))
    //    {
    //        auto newPath = mapManager->getNpcPath(getCurrentTileId(), m_goal, {Node::FORBIDDEN, Node::OCCUPIED});
    //        if(newPath.size() > 0)
    //        {
    //            if(newPath.size() > 30 && m_hasGoal)
    //            {
    //                BOT_LOGIC_NPC_LOG(m_logger, "\t\tTo path to the goal is to long... don't want to go there", true);
    //                m_hasGoal = false;
    //                m_path = {saveCurrentTile};
    //                break;
    //            }
    //            m_path = newPath;
    //            DisplayVector("\t\tPath Updated : ", m_path);
    //            return true;
    //        }
    //        else
    //        {
    //            BOT_LOGIC_NPC_LOG(m_logger, "\t\tCan't access the goal this turn, just wait, can't go from " + std::to_string(oldTileId) + " to " + std::to_string((*reverseIter)), true);
    //            m_nextState = WAITING;
    //            return true;
    //        }
    //    }
    //    oldTileId = (*reverseIter);
    //}
    BOT_LOGIC_NPC_LOG(m_logger, "\t\tNo update needed", true);
    m_nextState = EXPLORING;
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

        if(nonVisitedTiles.size() == 0)
        {
            BOT_LOGIC_NPC_LOG(m_logger, "\tNPC need to explore the door", true);
            m_nextState = INTERACTING;
            return;
        }
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
    if(getCurrentTileId() == m_goal && m_hasGoal)
    {
        m_nextState = ARRIVED;
        return;
    }
    if(m_path.size() <= 1 || getCurrentTileId() == m_target && !hasGoal())
    {
        m_nextState = EXPLORING;
        return;
    }

    BOT_LOGIC_NPC_LOG(m_logger, "-FollowPath", true);
    unsigned int nextTile = getNextPathTile();
    m_nextActions.push_back(new Move{m_id, Map::get()->getNextDirection(getCurrentTileId(), nextTile)});
    BOT_LOGIC_NPC_LOG(m_logger, "\tDeplacement vers " + std::to_string(nextTile), true);
    m_nextState = MOVING;
}

void Npc::wait()
{
    BOT_LOGIC_NPC_LOG(m_logger, "-Wait", true);
    m_nextState = WAITING;
}

void Npc::interact()
{
    BOT_LOGIC_NPC_LOG(m_logger, "-Interact", true);
    auto cObjects = ObjectManager::get()->getAllObjectsOnTile(getCurrentTileId());
    auto nObjects = ObjectManager::get()->getAllObjectsOnTile(getNextPathTile());
    ObjectRef myDoor;
    for(auto object : cObjects)
    {
        if(object->getType() == Object::DOOR)
        {
            myDoor = object;
        }
    }
    for(auto object : nObjects)
    {
        if(object->getType() == Object::DOOR)
        {
            myDoor = object;
        }
    }
    if(myDoor == ObjectRef())
    {
        m_nextActions.push_back(new Interact{m_id, 0, Interaction_SearchHiddenDoor});
        BOT_LOGIC_NPC_LOG(m_logger, "\tTry open hidden door", true);
        return;

    }
    if(myDoor->isActive())
    {
        m_nextState = MOVING;
        return;
    }
    m_nextActions.push_back(new Interact{m_id, myDoor->getId(), Interaction_OpenDoor});
    BOT_LOGIC_NPC_LOG(m_logger, "\tOpen door : " + std::to_string(myDoor->getId()), true);
    myDoor->setIsActive(true);
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