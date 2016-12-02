#ifndef PLANIFICATEUR_H
#define  PLANIFICATEUR_H
#include "Mission.h"

#include "Map.h"
#include "ZoneManager.h"
#include <vector>
#include "Object.h"
#include <utility>
class Planificateur
{
public:




    static std::vector<Mission> planifyMissions(Mission mission, unsigned int npcID)
    {
        std::vector<Mission> subMissions;
        subMissions.push_back(mission);
        return subMissions;
    }
    static std::pair<std::vector<Mission>, std::vector<Mission>> createMissionsToReachGoal(unsigned id, unsigned from, unsigned dest)
    {
        std::vector<Mission> subMissions;
        std::vector<Mission> subMissionsForOthersNPCs;
        if (from != dest)
        {
           Zone* fromZone = ZoneManager::get().getZone(from);
           
           auto jonction=fromZone->getZoneJunction(dest); // Crash si zone non adjacentes...

           //if (jonction) //Zones adjacentes
           {
           Object  frontiere = *jonction.m_object;

           //On va à la porte
           Mission goToDoor = Mission(Mission::MissionType::CHANGE_ZONE, frontiere.getTileId());
           subMissions.push_back(goToDoor);
           if (frontiere.getLinkedObjects().size()==1)
           {
               if (frontiere.getLinkedObjects() [0]->getTileId() != frontiere.getTileId())
               {
                   //L'objet n'est pas collé à la porte -> On demande à un autre npc de l'ouvrir
                       Mission openDoor = Mission(Mission::MissionType::OPEN_DOOR, frontiere.getLinkedObjects()[0]->getTileId());
                       subMissionsForOthersNPCs.push_back(openDoor);

               }
           }

           //Regarder si l'objet qui active la porte est collé à la porte
           bool porteAdjacente = false;
                   
           }
        }
        
        return std::make_pair(subMissions,subMissionsForOthersNPCs);
    }

};

#endif // ifndef
