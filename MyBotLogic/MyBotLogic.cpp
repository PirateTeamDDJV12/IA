#include "MyBotLogic.h"
#include "TurnInfo.h"
#include "LevelInfo.h"
#include "SearchMap.h"
#include <windows.h>
#include <algorithm>
#include "Map.h"
#include "NPCManager.h"

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
    NPCManager::get()->setLoggerPath(_logpath);
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
    NPCManager::get()->initNpcs(_levelInfo.npcs);
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
    myMap->updateEdges(_turnInfo.objects, m_turnCount);
    myMap->updateTiles(_turnInfo.tiles);

    // Create Influence map
    myMap->createInfluenceMap();

    // Log this
    myMap->logInfluenceMap(m_turnCount);
    myMap->logMap(m_turnCount);

    // Update all NPCs and fill the action list
    NPCManager::get()->updateNPCs(_actionList);
}

/*virtual*/ void MyBotLogic::Exit()
{
    //Write Code Here
}