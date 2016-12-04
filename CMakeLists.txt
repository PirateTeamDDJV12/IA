cmake_minimum_required( VERSION 3.0 )
project( AIBot C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )


SET( AIBOT_BUILDFILES 

	ChangeLog.txt
	CheckInstall.bat
	CleanAll.bat
	CMakeLists.txt
	CreateJob.bat
	Filters.cmake
	GenAIBootCampViewerFiles.bat
	GenAIBootCampViewerFiles.py
	INSTALL.txt
	PlayStandaloneMatch.bat
	rebuild_vis_sln_x64.bat
	TeamInfo.team
)

SET( AIBOT_HEADERS 

	#Framework
	Bot.h	
	BotLogicIF.h
	Globals.h
	LevelInfo.h
	Logger.h
	NPCInfo.h
	ObjectInfo.h
	TileInfo.h
	TurnInfo.h

	#MY_BOT
	MyBotLogic/MyBotLogic.h
	MyBotLogic/Npc.h
	MyBotLogic/Map.h
	MyBotLogic/Node.h
	MyBotLogic/SearchMap.h
	MyBotLogic/SearchNode.h
	MyBotLogic/Singleton.h
	MyBotLogic/NPCManager.h
	MyBotLogic/ObjectManager.h
	MyBotLogic/Object.h
	MyBotLogic/NPCManagerBTNpcUpdateAdministrator.h
	MyBotLogic/NPCManagerBTDecisionMakingAdministrator.h
	MyBotLogic/TimeManager.h
	MyBotLogic/Zone.h
	MyBotLogic/ZoneManager.h
	MyBotLogic/Mission.h
	MyBotLogic/MissionManager.h
	
	#MY_BOT_BT
	MyBotLogic/BehaviourTree/BehaviourTreeModule.h
	MyBotLogic/BehaviourTree/Other/BTUtils.h
	MyBotLogic/BehaviourTree/Other/BTDebugString.h
	MyBotLogic/BehaviourTree/Other/btMethods.h
	MyBotLogic/BehaviourTree/BlocInstance/BaseBloc.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocAction.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocBreakingLoopOnFail.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocBreakingLoopOnSuccess.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocComposite.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocDecorator.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocFail.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocInvert.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocLoop.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocRoot.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocSelect.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocSequence.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocTernarySelector.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocTernary.h
	MyBotLogic/BehaviourTree/BlocInstance/BlocSuccess.h
	MyBotLogic/BehaviourTree/BlocInstance/GeneralAction.h
)

SET( AIBOT_SRC 

	#Framework
	Bot.cpp
	LevelInfo.cpp
	Logger.cpp
	TurnInfo.cpp

	#MY_BOT
	MyBotLogic/MyBotLogic.cpp
	MyBotLogic/Npc.cpp
	MyBotLogic/Map.cpp
	MyBotLogic/Node.cpp
	MyBotLogic/SearchMap.cpp
	MyBotLogic/SearchNode.cpp
	MyBotLogic/NPCManager.cpp
	MyBotLogic/ObjectManager.cpp
	MyBotLogic/Object.cpp
	MyBotLogic/NPCManagerBTNpcUpdateAdministrator.cpp
	MyBotLogic/NPCManagerBTDecisionMakingAdministrator.cpp
	MyBotLogic/TimeManager.cpp
	MyBotLogic/Zone.cpp
	MyBotLogic/ZoneManager.cpp
	MyBotLogic/Mission.cpp
	MyBotLogic/MissionManager.cpp
	
	#Main
	Main.cpp
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_executable(
	aibot

	${AIBOT_BUILDFILES}
	${AIBOT_HEADERS}
	${AIBOT_SRC}
)

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

