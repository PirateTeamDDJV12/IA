#include "ZoneManager.h"
#include "NPCManager.h"
#include "Map.h"

ZoneManager ZoneManager::m_instance;

ZoneManager::ZoneManager()
{}

ZoneManager::~ZoneManager()
{}

size_t ZoneManager::getZoneCount() const
{
    return m_zones.size();
}

const Zone::Junction &ZoneManager::getJunctionBetween(unsigned from, unsigned to)
{
    return m_zones.at(from)->getJunction(to);
}

void ZoneManager::setLoggerPath(const std::string& a_path)
{
#ifdef BOT_LOGIC_DEBUG_MAP
    m_logger.Init(a_path, "ZoneManager.log");
#endif

    BOT_LOGIC_MAP_LOG(m_logger, "Configure", true);
}

Zone *ZoneManager::getZone(unsigned int zoneId) const
{
    return m_zones.at(zoneId);
}

Zone *ZoneManager::addZone()
{
    ++m_zoneCount;

    unsigned int zoneId = static_cast<unsigned int>(m_zoneCount);
    m_zones[zoneId] = new Zone(zoneId);

    return m_zones[zoneId];
}

void ZoneManager::addZone(Zone *zone)
{
    // Add the new Zone
    m_zones[zone->getZoneId()] = zone;
    // Increment the zone count
    ++m_zoneCount;
}

bool ZoneManager::addJunction(unsigned int firstZone, unsigned int secondZone, ObjectRef object)
{
    BOT_LOGIC_MAP_LOG(m_logger, "New junction to link zones " + std::to_string(firstZone) + " and " + std::to_string(secondZone), true);

    // Adds a junction between firstZone and secondZone through objet
    Zone *first = m_zones.at(firstZone);
    Zone *second = m_zones.at(secondZone);


    if(!(first && second))
    {
        return false;
    }

    Map* myMap = Map::get();
    Node* currentNode = myMap->getNode(object->getTileId());
    for(int i = NE; i <= NW; ++i)
    {
        if(currentNode->getEdge(static_cast<EDirection>(i)) == ObjectType_Door + 1)
        {
            Node* neighbour = currentNode->getNeighboor(static_cast<EDirection>(i));
            bool canMove = myMap->canMoveOnTile(object->getTileId(), neighbour->getId());
            bool canMoveInv = myMap->canMoveOnTile(neighbour->getId(), object->getTileId());

            first->addJunction(secondZone, object, canMove, currentNode, neighbour);
            second->addJunction(firstZone, object, canMoveInv, neighbour, currentNode);
        }
    }
    return true;
}

void ZoneManager::updateZones()
{
    std::vector<Npc *> npcs = NPCManager::get()->getNpcs();

    // For every NPC
    for(auto npc : npcs)
    {
        Node *npcNode = Map::get()->getNode(npc->getCurrentTileId());
        m_done.insert(npcNode);
        updateFromTile(npcNode);
        m_done.clear();
    }
    // For all the toDo tiles
    while(!m_toDo.empty())
    {
        m_done.insert(m_toDo[0]);
        updateFromTile(m_toDo[0]);
        m_toDo.erase(std::find(m_toDo.begin(), m_toDo.end(), m_toDo[0]));
        m_done.clear();
    }
}

void ZoneManager::updateFromTile(Node* currentTile)
{
    // Look for all neighbours
    // Begin by NE because N is always nullptr
    for(unsigned int dir = EDirection::NE; dir <= EDirection::NW; ++dir)
    {
        // Get the neighbour
        Node *neighbour = currentTile->getNeighboor(static_cast<EDirection>(dir));
        if(neighbour != nullptr && neighbour->getType() != Node::NONE)
        {
            // If the neighbour has not been done yet
            if(std::find(m_done.begin(), m_done.end(), neighbour) == m_done.end())
            {
                EDirection direction = static_cast<EDirection>(dir);
                EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
                // If there is no obstacle between the neighbour and the current tile
                if((!currentTile->isEdgeBlocked(direction) && !neighbour->isEdgeBlocked(invDir)))
                {
                    // If the neighbour was in the toDo list we erase it
                    auto itNeighbour = std::find(begin(m_toDo), end(m_toDo), neighbour);
                    if(itNeighbour != end(m_toDo))
                    {
                        m_toDo.erase(itNeighbour);
                    }
                    m_done.insert(neighbour);
                    // We set the neighbour to the new zone
                    neighbour->setZone(currentTile->getZone());
                    updateFromTile(neighbour);
                }
                // If there was a wall and the zoneId is 0
                else if(neighbour->getZone() == 0)
                {
                    Map *map = Map::get();
                    // We create a new zone
                    addZone();
                    // We set the neighbour to the new zone and we add him to the toDo list
                    neighbour->setZone(m_zoneCount);
                    m_toDo.push_back(neighbour);
                }
            }
        }
    }
}


