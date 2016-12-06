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
#ifdef BOT_LOGIC_DEBUG
    mLogger.Init(_logpath, "MyBotLogic.log");
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

    // Update graph
    myMap->updateEdges(_turnInfo.objects, _turnInfo.turnNb);
    myMap->updateTiles(_turnInfo.tiles);
    //ZoneManager::get().updateZones();

    // Update ObjectManager by adding all new discovered objects
    ObjectManager::get()->updateObjects(_turnInfo.objects, _turnInfo.tiles);

    // Create Influence map
    myMap->createInfluenceMap();

    // Update loggers
    myMap->logZoneMap(_turnInfo.turnNb);
    myMap->logInfluenceMap(_turnInfo.turnNb);
    myMap->logMap(_turnInfo.turnNb);
    ObjectManager::get()->updateLogger(_turnInfo);

    // Update all NPCs and fill the action list
    NPCManager::get()->updateNPCs(_turnInfo.npcs, _actionList);
}

/*virtual*/ void MyBotLogic::Exit()
{
    //Write Code Here
}