#pragma once

#include "BotLogicIF.h"
#include "Logger.h"
#include "Map.h"
#include "Npc.h"

#ifdef _DEBUG
   #define BOT_LOGIC_DEBUG
#endif

#ifdef BOT_LOGIC_DEBUG
   #define BOT_LOGIC_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
   #define BOT_LOGIC_LOG(logger, text, autoEndLine) 0
#endif

//Custom BotLogic where the AIBot decision making algorithms should be implemented.
//This class must be instantiated in main.cpp.
class MyBotLogic : public virtual BotLogicIF
{
    // debug stuff
    // TODO - set the turn counter in a BlackBoard or anything else more globals to have acces everywhere
    unsigned m_turnCount;
    std::string m_logPath;
public:
	MyBotLogic();
	virtual ~MyBotLogic();

	virtual void Configure(int argc, char *argv[], const std::string& _logpath);
	virtual void Start();
	virtual void Init(LevelInfo& _levelInfo);
	virtual void OnGameStarted();
	virtual void FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList);   //calculate moves for a single turn
	virtual void Exit();

protected:
	Logger mLogger;
};

