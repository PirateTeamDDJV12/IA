#include "Npc.h"
#include "Map.h"

/* INCLUDE FROM CARLE */
#include "Globals.h"

#include <algorithm>


Npc::Npc(unsigned int a_id, unsigned int a_tileId, std::string a_path, unsigned int zone) : 
    m_currentState{ NONE }, 
    m_nextState{ EXPLORING }, 
    m_id{ a_id }, m_goal{}, 
    m_hasGoal{ false }, 
    m_path{ a_tileId }, 
    m_nextActions{}, 
    m_historyTiles{ a_tileId }, 
    m_turnCount{ 0 }, 
    m_zone{ zone },
    m_exploreBT{ BehaviourTree::BlocFabric::initiateRootAsCompositeBloc<BehaviourTree::BlocSelect>("NpcExploreRoot") }
{
#ifdef BOT_LOGIC_DEBUG_NPC
    m_logger.Init(a_path, "Npc_" + std::to_string(m_id) + ".log");
#endif

    BOT_LOGIC_NPC_LOG(m_logger, "Configure", true);

    this->initExploreBT();
};

void Npc::update()
{
    ++m_turnCount;
    BOT_LOGIC_NPC_LOG(m_logger, "\nTurn #" + std::to_string(m_turnCount) + "\n\tCurrent Tile : " + std::to_string(getCurrentTileId()), true);
    updatePath();
    BOT_LOGIC_NPC_LOG(m_logger, "\tEntering State Machine : ", true);
    do
    {
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
                m_exploreBT();
                break;
            case(INTERACTING):
                interact();
                break;
            case(ARRIVED):
                m_nextState = ARRIVED; // May be useless atm
                m_currentState = ARRIVED;
                break;
        }
    } while(m_currentState != m_nextState);
    int test = 0;
}

bool Npc::stopEverything()
{
    // deleting item
    for(std::vector< Action* >::iterator it = m_nextActions.begin(); it != m_nextActions.end(); ++it)
    {
        delete (*it);
    }
    m_nextActions.clear();
    return true;
}

void Npc::stopMoving()
{
    auto it = std::partition(std::begin(m_nextActions),
                             std::end(m_nextActions),
                             [](const Action* curAction) { return curAction->actionType != Action_Move; });

    for(std::vector< Action* >::iterator itDelete = it; itDelete != m_nextActions.end(); ++itDelete)
    {
        delete (*itDelete);
    }
    m_nextActions.erase(it, std::end(m_nextActions));
}

void Npc::stopInteract()
{
    auto it = std::partition(std::begin(m_nextActions),
                             std::end(m_nextActions),
                             [](const Action* curAction) { return curAction->actionType == Action_Interact; });

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
    m_path = Map::get()->getNpcPath(getCurrentTileId(), m_goal);
}

bool Npc::updatePath()
{
    BOT_LOGIC_NPC_LOG(m_logger, "\tUpdating Path ", true);
    displayVector("\t\tOld path: ", m_path);
    std::vector<unsigned> reversePath;
    reversePath.resize(m_path.size());
    std::reverse_copy(begin(m_path), end(m_path), begin(reversePath));
    unsigned int oldTileId{reversePath.front()};
    for(unsigned int tileId : reversePath)
    {
        if(!Map::get()->canMoveOnTile(oldTileId, tileId))
        {
            m_path = Map::get()->getNpcPath(getCurrentTileId(), m_goal);
            displayVector("\t\tPath Updated : ", m_path);
            return true;
        }
        oldTileId = tileId;
    }
    BOT_LOGIC_NPC_LOG(m_logger, "\t\tNo update needed", true);
    return false;
}

int Npc::getNextPathTile() const
{
    if(m_path.size() == 1)
    {
        return -1;
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
        displayVector("\tNpc base path :", m_path);
        m_nextState = MOVING;
        return;
    }
    m_exploreBT();

    // Get the most influenced tile near the NPC
    int bestTile = Map::get()->getNearInfluencedTile(getCurrentTileId());

    // If we have not a best choice around us, let see a little bit futher
    if(bestTile < 0)
    {
        // TODO - Try to get the most influent tile around us in range of 2 instead of looking for a non visited tile
        // Get all non visited tiles
        std::vector<unsigned> nonVisitedTiles = Map::get()->getNonVisitedTile();
        displayVector("\t-Looking for the non visited tiles : ", nonVisitedTiles);
        for(unsigned index : nonVisitedTiles)
        {
            // Test if we can have a good path to this tile
            std::vector<unsigned> temp = Map::get()->getNpcPath(getCurrentTileId(), index, 
                {Node::NodeType::FORBIDDEN, Node::NodeType::NONE, Node::NodeType::OCCUPIED});

            // If we got a good path, let's configure this
            if(!temp.empty())
            {
                m_path = temp;
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

        m_nextActions.push_back(new Move{m_id, Map::get()->getNextDirection(getCurrentTileId(), getNextPathTile())});

        BOT_LOGIC_NPC_LOG(m_logger, "Deplacement vers " + std::to_string(bestTile), true);

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
void Npc::displayVector(std::string info, const std::vector<T> v)
{
    std::string s{""};
    for(T u : v)
    {
        s += std::to_string(u) + " ";
    }
    BOT_LOGIC_NPC_LOG(m_logger, info + s, true);
}

///////////////////////////////////////////////

void Npc::initExploreBT()
{
    /************************************************************************/
    /* First layer                                                          */
    /************************************************************************/

    // Root of the Exploration's BT
    BehaviourTree::BlocComposite* rootSelect = m_exploreBT.getRoot()->as<BehaviourTree::BlocComposite>();


    /************************************************************************/
    /* Second layer                                                         */
    /************************************************************************/

    // rootSelect's first child (sequence)
    BehaviourTree::BlocRef tileExplorationSequenceRef = BehaviourTree::BlocFabric::createCompositeBloc<BehaviourTree::BlocSequence>("TileExplorationSequence");
    BehaviourTree::BlocSequence* tileExplorationSequence = tileExplorationSequenceRef->as<BehaviourTree::BlocSequence>();

    // rootSelect's second child (sequence)
    BehaviourTree::BlocRef wallExplorationSequenceRef = BehaviourTree::BlocFabric::createCompositeBloc<BehaviourTree::BlocSequence>("WallExplorationSequence");
    BehaviourTree::BlocSequence* wallExplorationSequence = wallExplorationSequenceRef->as<BehaviourTree::BlocSequence>();

    // Connecting rootSelect's childs to itself
    rootSelect->connect(tileExplorationSequenceRef);
    rootSelect->connect(wallExplorationSequenceRef);


    /************************************************************************/
    /* Third layer                                                          */
    /************************************************************************/

    // tileExplorationSequence's first child (loop) - The loop will iterate for each directions (6x max)
    BehaviourTree::BlocRef checkSequenceRef = BehaviourTree::BlocFabric::createCompositeBloc<BehaviourTree::BlocSequence>("CheckLoop");
    BehaviourTree::BlocSequence* checkSequence = checkSequenceRef->as<BehaviourTree::BlocSequence>();

    BehaviourTree::BlocRef checkLoopRef =  BehaviourTree::BlocFabric::createLoopBloc<BehaviourTree::BlocBreakingLoopOnSuccess>(6, checkSequenceRef, "Loop");
    BehaviourTree::BlocBreakingLoopOnSuccess* checkLoop = checkLoopRef->as<BehaviourTree::BlocBreakingLoopOnSuccess>();

    // tileExplorationSequence's second child (action)
    BehaviourTree::BlocRef moveToTileActionRef = createMoveToTileAction();

    // Connecting tileExplorationSequence's childs to itself
    tileExplorationSequence->connect(checkLoopRef);
    tileExplorationSequence->connect(moveToTileActionRef);


    /************************************************************************/
    /* Fourth layer                                                         */
    /************************************************************************/

    // checkSequence's first child (action)
    BehaviourTree::BlocRef moveToNextTileActionRef = createCheckTileAction();

    // checkSequence's second child (action)
    BehaviourTree::BlocRef moveToNextTileActionRef = createModifyMoveDirectionAction();
}

BehaviourTree::BlocRef Npc::createCheckTileAction()
{
    return BehaviourTree::BlocFabric::createGeneralAction(
        [this]()
    {



        return BehaviourTree::general::result::FAIL;
    },
        "CheckTile"
        );
}

BehaviourTree::BlocRef Npc::createModifyMoveDirectionAction()
{
    return BehaviourTree::BlocFabric::createGeneralAction(
        [this]()
    {



        return BehaviourTree::general::result::FAIL;
    },
        "ChangeDirection"
        );
}

BehaviourTree::BlocRef Npc::createMoveToTileAction()
{
    return BehaviourTree::BlocFabric::createGeneralAction(
        [this]()
    {



        return BehaviourTree::general::result::FAIL;
    },
        "CheckTile"
        );
}

//void Npc::swapToExplore()
//{
//    getBTRootAs<BehaviourTree::BlocComposite>()->disconnect(0);
//    getBTRootAs<BehaviourTree::BlocComposite>()->connect(createExploreAction());
//}
//
//void Npc::swapToExploreWall()
//{
//    getBTRootAs<BehaviourTree::BlocComposite>()->disconnect(0);
//    getBTRootAs<BehaviourTree::BlocComposite>()->connect(createExploreWallAction());
//}
