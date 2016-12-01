#include "ZoneManager.h"
#include "NPCManager.h"
#include "Map.h"

ZoneManager ZoneManager::m_instance;

ZoneManager::ZoneManager()
{
}

ZoneManager::~ZoneManager()
{
}

Zone *ZoneManager::getZone(unsigned int zoneId) const
{
    return m_zones.at(zoneId);
}

void ZoneManager::addZone(Zone *zone)
{
    m_zones[zone->getZoneId()] = zone;
}

bool ZoneManager::addJunction(unsigned int firstZone, unsigned int secondZone, Object *object)
{
    Zone *first = m_zones.at(firstZone);
    Zone *second = m_zones.at(secondZone);

    if (first && second)
    {
        first->addJunction(secondZone, object);
        second->addJunction(firstZone, object);
        return true;
    }
    return false;
}

void ZoneManager::updateZones()
{
    std::vector<Node*> toDo;
    std::set<Node*> done;

    // Pour chaque npc
    std::vector<Npc *> npcs = NPCManager::get()->getNpcs();
    for (auto npc : npcs)
    {
        Node *npcNode = Map::get()->getNode(npc->getCurrentTileId());
        if (npcNode->getZone() == 0)
        {
            npcNode->setZone(npc->getZone());
        }
        else
        {
            npc->setZone(npcNode->getZone());
        }
        done.insert(npcNode);
        updateTileZone(npcNode, done, toDo);
        done.clear();
    }
    while (!toDo.empty())
    {
        done.insert(toDo[0]);
        updateTileZone(toDo[0], done, toDo);
        toDo.erase(std::find(toDo.begin(), toDo.end(), toDo[0]));
        done.clear();
    }
}

void ZoneManager::updateTileZone(Node* currentTile, std::set<Node *> &done, std::vector<Node *> &toDo)
{
    for (unsigned int dir = EDirection::N; dir <= EDirection::NW; ++dir)
    {
        // Get the neighbour
        Node *neighbour = currentTile->getNeighboor(static_cast<EDirection>(dir));
        if (neighbour != nullptr && neighbour->getType() != Node::NONE)
        {
            EDirection direction = static_cast<EDirection>(dir);
            EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
            if (std::find(done.begin(), done.end(), neighbour) == done.end())
            {
                // If there is no obstacle between the neighbour and the current tile
                if ((!currentTile->isEdgeBlocked(direction) && !neighbour->isEdgeBlocked(invDir)))
                {
                    auto itNeighbour = std::find(begin(toDo), end(toDo), neighbour);
                    if (itNeighbour != end(toDo)) {
                        toDo.erase(itNeighbour);
                    }
                    done.insert(neighbour);
                    neighbour->setZone(currentTile->getZone());
                    updateTileZone(neighbour, done, toDo);
                }
                else if (neighbour->getZone() == 0)
                {
                    Map *map = Map::get();
                    map->setZoneCount(map->getZoneCount() + 1);
                    neighbour->setZone(map->getZoneCount());
                    toDo.push_back(neighbour);
                }
            }
        }
    }
}


