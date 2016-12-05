#include "Planificateur.h"
#include "NPCManager.h"
#include "ObjectManager.h"
#include "Zone.h"
#include "BehaviourTree/BehaviourTreeModule.h"
#include <vector>
#include <map>


std::pair<std::vector<Mission>, std::vector<Mission>> Planificateur::planifyMissions(Mission mission, unsigned npcId)
{
    unsigned npcZone = NPCManager::get()->getNpcs().at(npcId)->getZone();

    std::pair<std::vector<Mission>, std::vector<Mission>> subMissions;
    subMissions.first.push_back(mission);
    if (npcZone != mission.getObjective())
    {
        auto changeZone = createMissionsToReachGoal(npcZone, mission.getObjective());

        subMissions.first.insert(subMissions.first.end(), changeZone.first.begin(), changeZone.first.end());
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

    Zone* fromZone = ZoneManager::get().getZone(from);
    Zone* destZone = ZoneManager::get().getZone(dest);


    auto jonction = fromZone->getZoneJunction(dest); // Crash si zones non adjacentes...
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


            //Créer les missions
            std::pair<std::vector<Mission>, std::vector<Mission>> subMissions = planifyMissions(openDoor,-1); //-1 car non attribuée à un NPC 
            subMissionsForOthersNPCs.insert(subMissionsForOthersNPCs.end(), subMissions.second.begin(), subMissions.second.end());
            subMissionsForOthersNPCs.push_back(openDoor);
        }

    }

    return std::make_pair(subMissions, subMissionsForOthersNPCs);

}



