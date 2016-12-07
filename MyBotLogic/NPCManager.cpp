#include "NPCManager.h"
#include "ZoneManager.h"
#include "Map.h"
#include "BehaviourTree/BehaviourTreeModule.h"
#include <algorithm>

// Init singleton
NPCManager NPCManager::m_instance;

void NPCManager::setLoggerPath(const std::string &a_path)
{
    m_logPath = a_path;
#ifdef BOT_LOGIC_DEBUG_MAP
    m_logger.Init(a_path, "NPCManager.log");
#endif

    BOT_LOGIC_MAP_LOG(m_logger, "Configure", true);
}

void NPCManager::initNpc(const std::pair<unsigned, NPCInfo>& curNpcs)
{
    Map *map = Map::get();
    ZoneManager &zoneManager = ZoneManager::get();

    // Create a new zone for the npc
    Zone *newZone = zoneManager.addZone();
    unsigned int zoneId = newZone->getZoneId();

    // Create the npc and give him the new zoneId
    m_npcs.push_back(new Npc(curNpcs.second.npcID, curNpcs.second.tileID, m_logPath, zoneId));
    // Get the npc node and set the node zone to the new zone id
    Node *npcNode = map->getNode(curNpcs.second.tileID);
    npcNode->setNpcIdOnNode(curNpcs.second.npcID);
    npcNode->setZone(zoneId);
}

void NPCManager::initNpcs(const std::map<unsigned, NPCInfo>& npcs)
{
    for (std::pair<unsigned int, NPCInfo> curNpcs : npcs)
    {
        initNpc(curNpcs);
    }

    initBT();
}

void NPCManager::initBT()
{
    m_BTDecisionMakingAdministrator.init();
    m_BTNpcUpdateAdministrator.init();
}

const Npc* NPCManager::getNpcById(int npc_id_on_tile)
{
    for(auto npc : m_npcs)
    {
        if(npc->getId() == static_cast<unsigned int>(npc_id_on_tile))
        {
            return npc;
        }
    }
    return nullptr;
}

void NPCManager::updateNPCs(const std::map<unsigned int, NPCInfo> &_npcs, std::vector<Action*> &_actionList)
{
    Map *myMap = Map::get();

    for each(auto myNpc in m_npcs)
    {
        NPCInfo currentNpcInfo = _npcs.at(myNpc->getId());
        myNpc->setCurrentTile(currentNpcInfo.tileID);
    }

    // Get best goal for each NPCs
    std::map<unsigned, unsigned> goalMap = std::move(myMap->getBestGoalTile(m_npcs));

    // Calcul path for npc and set goal tile
    for (Npc* curNpc : m_npcs)
    {
        myMap->visitTile(curNpc->getCurrentTileId());
        if (!curNpc->hasGoal() && goalMap.find(curNpc->getId()) != end(goalMap))
        {
            unsigned int goalTile = goalMap[curNpc->getId()];
            curNpc->setGoal(goalTile);
            curNpc->calculPath();
        }
    }
    
    // Move Npcs
    m_BTNpcUpdateAdministrator();
    UpdateNpcActions(myMap);

    for each(auto myNpc in m_npcs)
    {
        // copy npc's action list into the action list
        for(Action* curAction : myNpc->getActions())
        {
            _actionList.push_back(curAction->Clone());

            if(curAction->actionType == Action_Move)
            {
                // Update NPC position on node
                myMap->getNode(myNpc->getCurrentTileId())->setNpcIdOnNode(-1);
                myMap->getNode(myNpc->getNextPathTile())->setNpcIdOnNode(static_cast<int>(myNpc->getId()));
            }
        }
    }

    // Empty all NPCs list
    std::for_each(begin(m_npcs),
        end(m_npcs),
        [](Npc* myNpc) {myNpc->unstackActions(); });
}

bool NPCManager::UpdateNpcActions(Map * myMap)
{
    bool isNpcUpdated = false;
    for each(auto myNpc in m_npcs)
    {
        // Get next npc tile
        int nextNpcTile = myNpc->getNextPathTile();
        if(myNpc->getActions().size() == 0)
        {
            continue;
        }

        if(nextNpcTile >= 0)
        {
            // Don't want to move on a tile where an npc is already
            if(myMap->getNode(nextNpcTile)->isTileOccupied())
            {
                int npcIdOnTile = myMap->getNode(nextNpcTile)->getNpcIdOnNode();
                const Npc* npcOnTile = getNpcById(npcIdOnTile);
                if(npcOnTile->getNextPathTile() < 0)
                {
                    myNpc->stopMoving();
                    isNpcUpdated = true;
                    break;
                }
            }

            // check if npc can move on nextTile
            for(Npc* curP : m_npcs)
            {
                if(curP->getPathSize() > 1)
                {
                    if(curP->getId() != myNpc->getId()
                       && curP->getNextPathTile() == nextNpcTile)
                    {
                        // Handle
                        if(myNpc->getPathSize() < curP->getPathSize())
                        {
                            myNpc->stopMoving();
                            isNpcUpdated = true;
                            break;
                        }
                        // else prioritize by npcs id
                        if(curP->getId() < myNpc->getId()
                           && myNpc->getPathSize() == curP->getPathSize())
                        {
                            myNpc->stopMoving();
                            isNpcUpdated = true;
                            break;
                        }
                    }
                }
                else
                {
                    // Don't want to move on a tile where an npc is already and don't want to move
                    if(curP->getId() != myNpc->getId()
                       && curP->getCurrentTileId() == static_cast<unsigned int>(nextNpcTile))
                    {
                        myNpc->stopMoving();
                        isNpcUpdated = true;
                    }
                }
            }
        }
    }
    return isNpcUpdated;
}

bool NPCManager::isGoalAlreadyAssign(unsigned goalId)
{
    for(auto npc : m_npcs)
    {
        if(!npc->hasGoal())
        {
            continue;
        }

        if(npc->getGoal() == goalId)
        {
            return true;
        }
    }
    return false;
}