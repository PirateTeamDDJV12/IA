#include "Planificateur.h"


std::vector<Mission> Planificateur::planifyMissions(Mission mission, unsigned int npcID)
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

            //On va � la porte
            Mission goToDoor = Mission(Mission::MissionType::CHANGE_ZONE, frontiere.getTileId());
            subMissions.push_back(goToDoor);

            //Parcourir les objets � activer

            for (int objetCourant = 0; objetCourant < frontiere.getLinkedObjects().size(); ++objetCourant)
            {
                if (frontiere.getLinkedObjects()[objetCourant]->getTileId() != frontiere.getTileId())
                {
                    //L'objet n'est pas coll� � la porte -> On demande � un autre npc de l'ouvrir
                    Mission openDoor = Mission(Mission::MissionType::OPEN_DOOR, frontiere.getLinkedObjects()[objetCourant]->getTileId());

                    //r�cup�rer Zone de la fronti�re
                    unsigned position = frontiere.getLinkedObjects()[objetCourant]->getTileId();
                    unsigned zoneToGo = ZoneManager::get().getZone(position)->getZoneId();

                    subMissionsForOthersNPCs.push_back(openDoor);
                }


                //Regarder si l'objet qui active la porte est coll� � la porte
                bool porteAdjacente = false;


            }


        }

        return std::make_pair(subMissions, subMissionsForOthersNPCs);
    }
}