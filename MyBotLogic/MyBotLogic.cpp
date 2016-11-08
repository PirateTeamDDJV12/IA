#include "MyBotLogic.h"
#include "TurnInfo.h"
#include "LevelInfo.h"
#include "SearchMap.h"
#include <windows.h>
#include <algorithm>
#include "Map.h"

MyBotLogic::MyBotLogic()
{
    //Write Code Here
}

/*virtual*/ MyBotLogic::~MyBotLogic()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::Configure(int argc, char *argv[], const std::string& _logpath)
{
    m_logPath = _logpath;
#ifdef BOT_LOGIC_DEBUG
    mLogger.Init(_logpath, "MyBotLogic.log");
#endif

    BOT_LOGIC_LOG(mLogger, "Configure", true);
    Map::get()->setLoggerPath(_logpath);
    m_turnCount = 0;

    //Write Code Here
}

/*virtual*/ void MyBotLogic::Load()
{
    //Write Code Here
#ifdef BOT_LOGIC_DEBUG
    Sleep(5000);
#endif
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
    // Init MAP
    BOT_LOGIC_LOG(mLogger, "Map Initialisation", true);
    Map::get()->initMap(_levelInfo.rowCount, _levelInfo.colCount, _levelInfo.visionRange);

    // Init npcs
    BOT_LOGIC_LOG(mLogger, "NPCs Initialisation", true);
    for(std::pair<unsigned int, NPCInfo> curNpcs : _levelInfo.npcs)
    {
        m_npcs[curNpcs.second.npcID] = new Npc(curNpcs.second.npcID, curNpcs.second.tileID, m_logPath);
        Map::get()->getNode(curNpcs.second.tileID)->setNpcIdOnNode(curNpcs.second.npcID);
    }
}

/*virtual*/ void MyBotLogic::OnBotInitialized()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
    BOT_LOGIC_LOG(mLogger, "\nTURN #" + std::to_string(++m_turnCount), true);

    Map *myMap = Map::get();

    // Update graph
    BOT_LOGIC_LOG(mLogger, "Update Edges and Tiles", true);
    myMap->updateEdges(_turnInfo.objects, m_turnCount);
    myMap->updateTiles(_turnInfo.tiles);

    std::map<unsigned, unsigned> goalMap = myMap->getBestGoalTile(_turnInfo.npcs);

    // If any NPC are exploring, create an influence map
    if(goalMap.size() < _turnInfo.npcs.size())
    {
        myMap->createInfluenceMap();
        myMap->logInfluenceMap(m_turnCount);
    }

    myMap->logMap(m_turnCount);

    // Calcul path for npc and set goal tile
    for(std::pair<unsigned int, NPCInfo> curNpc : _turnInfo.npcs)
    {
        Npc* myNpc = m_npcs[curNpc.first];
        myMap->visitTile(myNpc->getCurrentTileId());
        if(!myNpc->hasGoal() && goalMap.find(curNpc.second.npcID) != end(goalMap))
        {
            unsigned int goalTile = goalMap[curNpc.second.npcID];
            myNpc->setGoal(goalTile);
            myNpc->calculPath();
        }
    }

    // Move Npcs
    for each(auto info in _turnInfo.npcs)
    {
        // Get my search map
        Npc* myNpc = m_npcs[info.second.npcID];

        // If my npc path is not correct anymore, we try to find another path
        myNpc->update();

        // Get next npc tile
        int nextNpcTile = myNpc->getNextPathTile();

        if(nextNpcTile >= 0)
        {
            // check if npc can move on nextTile
            for(std::pair<unsigned int, Npc*> curP : m_npcs)
            {

                if(curP.first != myNpc->getId()
                   && curP.second->getNextPathTile() == nextNpcTile)
                {
                    // Handle
                    if(myNpc->getPathSize() < curP.second->getPathSize())
                    {
                        myNpc->stopMoving();
                        break;
                    }
                    // else prioritize by npcs id
                    if(curP.first < myNpc->getId()
                       && myNpc->getPathSize() == curP.second->getPathSize())
                    {
                        myNpc->stopMoving();
                        break;
                    }
                }
            }
            // copy npc's action list into the action list
            for(Action* curAction : myNpc->getActions())
            {
                _actionList.push_back(curAction->Clone());

                // Update NPC position on node
                // TODO - be careful about the action type, atm it's move but it can be different !!
                myMap->getNode(myNpc->getCurrentTileId())->setNpcIdOnNode(0);
                myMap->getNode(nextNpcTile)->setNpcIdOnNode(info.second.npcID);
            }
        }
    }
    std::for_each(begin(m_npcs),
                  end(m_npcs),
                  [](std::pair<unsigned int, Npc*> myNpc) {myNpc.second->unstackActions(); });
}

/*virtual*/ void MyBotLogic::Exit()
{
    //Write Code Here
}