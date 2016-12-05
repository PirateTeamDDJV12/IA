#ifndef PLANIFICATEUR_H
#define  PLANIFICATEUR_H
#include "Mission.h"
#include "Map.h"
#include "ZoneManager.h"
#include "Object.h"
#include <vector>
#include <utility>


class Planificateur
{
public:

    std::pair<std::vector<Mission>, std::vector<Mission>> planifyMissions(Mission mission, unsigned npcID);
   
    std::pair<std::vector<Mission>, std::vector<Mission>> createMissionsToReachGoal(unsigned from, unsigned dest);

};

#endif // PLANIFICATEUR_H
