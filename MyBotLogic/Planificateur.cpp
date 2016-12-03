#include "Planificateur.h"
#include "NPCManager.h"
#include "ObjectManager.h"
#include "Zone.h"
#include <vector>
#include <map>


std::pair<std::vector<Mission>, std::vector<Mission>> Planificateur::planifyMissions(Mission mission, unsigned npcId)
{
    unsigned npcZone = NPCManager::get()->getNpcs().at(npcId)->getZone();
    
    std::pair<std::vector<Mission>, std::vector<Mission>> subMissions;
    subMissions.first.push_back(mission);
    if (npcZone!=mission.getObjective())
    {
        auto changeZone = createMissionsToReachGoal(npcZone, mission.getObjective());

        subMissions.first.insert(subMissions.first.end(),changeZone.first.begin(), changeZone.first.end());
        subMissions.second.insert(subMissions.second.end(), changeZone.second.begin(), changeZone.second.end());
    } 
    else
    {
    }
    subMissions.first.push_back(mission);
    return subMissions;
}
std::pair<std::vector<Mission>, std::vector<Mission>> Planificateur::createMissionsToReachGoal(unsigned from, unsigned dest)
{
    std::vector<Mission> subMissions;
    std::vector<Mission> subMissionsForOthersNPCs;
    if (from != dest)
    {
        Zone* fromZone = ZoneManager::get().getZone(from);

        auto jonction = fromZone->getZoneJunction(dest); // Crash si zone non adjacentes...

        if (fromZone->m_junctions.count(dest)) //Zones adjacentes
        {
            Object  frontiere = *jonction.m_object;

            //On va à la porte
            Mission goToDoor = Mission(Mission::MissionType::CHANGE_ZONE, frontiere.getTileId());
            subMissions.push_back(goToDoor);

            //Parcourir les objets à activer

            for (int objetCourant = 0; objetCourant < frontiere.getLinkedObjects().size(); ++objetCourant)
            {
                if (frontiere.getLinkedObjects()[objetCourant]->getTileId() != frontiere.getTileId())
                {
                    //L'objet n'est pas collé à la porte -> On demande à un autre npc de l'ouvrir
                    Mission openDoor = Mission(Mission::MissionType::OPEN_DOOR, frontiere.getLinkedObjects()[objetCourant]->getTileId());

                    //récupérer Zone de la frontière
                    unsigned position = frontiere.getLinkedObjects()[objetCourant]->getTileId();
                    unsigned zoneToGo = ZoneManager::get().getZone(position)->getZoneId();
                    std::pair<std::vector<Mission>, std::vector<Mission>> subMissions = planifyMissions(openDoor,0); //TOCHANGE -> 
                    subMissionsForOthersNPCs.insert(subMissionsForOthersNPCs.end(), subMissions.second.begin(), subMissions.second.end());
                    subMissionsForOthersNPCs.push_back(openDoor);
                }
                
            }


        }
        else
        {
            //Zones non adjacentes
            
            //Essayer de trouver la liste des zones à traverser




        }

        return std::make_pair(subMissions, subMissionsForOthersNPCs);
    }
}