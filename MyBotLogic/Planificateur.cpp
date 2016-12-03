#include "Planificateur.h"


std::vector<Mission> Planificateur::planifyMissions(Mission mission, unsigned zoneId,unsigned npcId)
{
    std::vector<Mission> subMissions;
    subMissions.push_back(mission);
    return subMissions;
}
std::pair<std::vector<Mission>, std::vector<Mission>> Planificateur::createMissionsToReachGoal(unsigned id, unsigned from, unsigned dest)
{
    std::vector<Mission> subMissions;
    std::vector<Mission> subMissionsForOthersNPCs;
    if (from != dest)
    {
        Zone* fromZone = ZoneManager::get().getZone(from);

        auto jonction = fromZone->getZoneJunction(dest); // Crash si zone non adjacentes...

        if (true) //Zones adjacentes
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
                    std::vector<Mission> subMissions = planifyMissions(openDoor,zoneToGo,41); //TOCHANGE -> 
                    subMissionsForOthersNPCs.insert(subMissionsForOthersNPCs.end(), subMissions.begin(), subMissions.end());
                    subMissionsForOthersNPCs.push_back(openDoor);
                }
                
            }


        }

        return std::make_pair(subMissions, subMissionsForOthersNPCs);
    }
}