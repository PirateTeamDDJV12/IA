#include "ZoneManager.h"
#include "NPCManager.h"
#include "Map.h"

ZoneManager ZoneManager::m_instance;

ZoneManager::ZoneManager() : m_zones{}, m_count{0}
{
}

ZoneManager::~ZoneManager()
{
}

size_t ZoneManager::getZoneCount() const
{
    return m_count;
}


Zone *ZoneManager::getZone(unsigned int zoneId) const
{
    return m_zones.at(zoneId);
}

Zone *ZoneManager::addZone()
{
    ++m_count;
    unsigned int zoneId = static_cast<unsigned int>(m_zones.size() + 1);
    m_zones[zoneId] = new Zone(zoneId);
    return m_zones[zoneId];
}

void ZoneManager::addZone(Zone *zone)
{
    m_zones[zone->getZoneId()] = zone;
    ++m_count;
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
        if (npcNode->getZone() != npc->getZone())
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
                    addZone();
                    neighbour->setZone(m_count);
                    toDo.push_back(neighbour);
                }
            }
        }
    }
}


