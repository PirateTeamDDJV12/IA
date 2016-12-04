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
    Sleep(5000);
#endif
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
    Map *myMap = Map::get();
    m_omniscient = _levelInfo.bOmnicientMode;
    // Init MAP
    BOT_LOGIC_LOG(mLogger, "Map Initialisation", true);
    myMap->initMap(_levelInfo.rowCount, _levelInfo.colCount, _levelInfo.visionRange);
    // Update graph
    myMap->updateEdges(_levelInfo.objects, 0);
    myMap->updateTiles(_levelInfo.tiles);

    // Init objects
    BOT_LOGIC_LOG(mLogger, "Objects Initialisation", true);
    ObjectManager::get()->initObjects(_levelInfo.objects, _levelInfo.tiles);

    // Init npcs
    BOT_LOGIC_LOG(mLogger, "NPCs Initialisation", true);
    NPCManager::get()->initNpcs(_levelInfo.npcs);

    // Init zones
    BOT_LOGIC_LOG(mLogger, "NPCs Initialisation", true);
    ZoneManager::get().initZones();
}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
    //Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
    Map *myMap = Map::get();
   
    // Update graph
    myMap->updateEdges(_turnInfo.objects, m_turnCount);
    myMap->updateTiles(_turnInfo.tiles);
    
    if (!m_omniscient)
        ZoneManager::get().updateZones();

    // Create Influence map
    myMap->createInfluenceMap();

    // Log this
    myMap->logZoneMap(m_turnCount);
    myMap->logInfluenceMap(m_turnCount);
    myMap->logMap(m_turnCount);

    // Update ObjectManager by adding all new discovered objects
    ObjectManager::get()->updateObjects(_turnInfo.objects, _turnInfo.tiles);

    // Update all NPCs and fill the action list
    NPCManager::get()->updateNPCs(_actionList);

    // Update loggers
    ObjectManager::get()->updateLogger(_turnInfo);
}

/*virtual*/ void MyBotLogic::Exit()
{
    //Write Code Here
}