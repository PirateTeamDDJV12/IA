#include "MyBotLogic.h"
#include "TurnInfo.h"
#include "LevelInfo.h"
#include "SearchMap.h"
#include <windows.h>
#include <algorithm>
#include "Map.h"
#include "NPCManager.h"
#include "ObjectManager.h"
#include "ZoneManager.h"
#include "TimeManager.h"


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
    mLogger.Init(_logpath, "MyBotLogic.log");
#ifdef BOT_LOGIC_DEBUG
#endif

    BOT_LOGIC_LOG(mLogger, "Configure", true);
    Map::get()->setLoggerPath(_logpath);
    NPCManager::get()->setLoggerPath(_logpath);
    ObjectManager::get()->setLoggerPath(_logpath);
    ZoneManager::get().setLoggerPath(_logpath);

    m_turnCount = 0;

    //Write Code Here
}

/*virtual*/ void MyBotLogic::Start()
{
    //Write Code Here
#ifdef BOT_LOGIC_DEBUG
    Sleep(2000);
#endif
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
    Map* myMap = Map::get();
    // Init MAP
    BOT_LOGIC_LOG(mLogger, "Map Initialisation", true);
    myMap->initMap(_levelInfo.rowCount, _levelInfo.colCount, _levelInfo.visionRange);
    myMap->updateEdges(_levelInfo.objects, 0);
    myMap->updateTiles(_levelInfo.tiles);
    myMap->logMap(0);

    // Init npcs
    BOT_LOGIC_LOG(mLogger, "NPCs Initialisation", true);
    NPCManager::get()->initNpcs(_levelInfo.npcs);

    // Init zones
    BOT_LOGIC_LOG(mLogger, "Zones Initialisation", true);
    ZoneManager::get().updateZones();
    myMap->logZoneMap(0);

    // Init objects
    BOT_LOGIC_LOG(mLogger, "Objects Initialisation", true);
    ObjectManager::get()->initObjects(_levelInfo.objects, _levelInfo.tiles);

}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
    Map *myMap = Map::get();

    // Save time before update to know to duration every turn
    TimeManager::getInstance().fastSave();

    // Update all the map (tiles, edges, objects, everything)
    myMap->update(_turnInfo);

    // Update all NPCs and fill the action list
    NPCManager::get()->updateNPCs(_turnInfo.npcs, _actionList);

    // Log the time
    mLogger.Log("elapsed time in turn : " + std::to_string(TimeManager::getInstance().getFastDifference<std::chrono::microseconds>().count()) + "us");
}

/*virtual*/ void MyBotLogic::Exit()
{
    //Write Code Here
}