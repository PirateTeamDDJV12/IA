#include "NPCManager.h"
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

void NPCManager::initNpc(std::pair<unsigned, NPCInfo> curNpcs)
{
    Map *map = Map::get();
    map->setZoneCount(map->getZoneCount() + 1);
    unsigned int zone = map->getZoneCount();
    m_npcs.push_back(new Npc(curNpcs.second.npcID, curNpcs.second.tileID, m_logPath, zone));
    map->getNode(curNpcs.second.tileID)->setNpcIdOnNode(curNpcs.second.npcID);
}

void NPCManager::initNpcs(std::map<unsigned, NPCInfo> npcs)
{
    for (std::pair<unsigned int, NPCInfo> curNpcs : npcs)
    {
        initNpc(curNpcs);
    }

    initBT();
}

void NPCManager::initBT()
{
    m_BTAdministrator.init();
}

void NPCManager::updateNPCs(std::vector<Action*> &_actionList)
{
    Map *myMap = Map::get();

    // Get best goal for each NPCs
    std::map<unsigned, unsigned> goalMap = myMap->getBestGoalTile(m_npcs);

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
    m_BTAdministrator();

    for each(auto myNpc in m_npcs)
    {
        // Get next npc tile
        int nextNpcTile = myNpc->getNextPathTile();

        if (nextNpcTile >= 0)
        {
            // check if npc can move on nextTile
            for (Npc* curP : m_npcs)
            {

                if (curP->getId() != myNpc->getId()
                    && curP->getNextPathTile() == nextNpcTile)
                {
                    // Handle
                    if (myNpc->getPathSize() < curP->getPathSize())
                    {
                        myNpc->stopMoving();
                        break;
                    }
                    // else prioritize by npcs id
                    if (curP->getId() < myNpc->getId()
                        && myNpc->getPathSize() == curP->getPathSize())
                    {
                        myNpc->stopMoving();
                        break;
                    }
                }
            }
            // copy npc's action list into the action list
            for (Action* curAction : myNpc->getActions())
            {
                _actionList.push_back(curAction->Clone());

                // Update NPC position on node
                // TODO - be careful about the action type, atm it's move but it can be different !!
                myMap->getNode(myNpc->getCurrentTileId())->setNpcIdOnNode(0);
                myMap->getNode(nextNpcTile)->setNpcIdOnNode(myNpc->getId());
            }
        }
    }

    // Empty all NPCs list
    std::for_each(begin(m_npcs),
        end(m_npcs),
        [](Npc* myNpc) {myNpc->unstackActions(); });
}
void NPCManager::createMission(Mission::MissionType type, unsigned objectif)
{
    Mission mission = Mission(type, objectif);
    MissionManager::get()->subscribeMission(mission);
}