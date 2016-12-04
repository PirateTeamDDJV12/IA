#include "ZoneManager.h"
#include "NPCManager.h"
#include "Map.h"

ZoneManager ZoneManager::m_instance;

ZoneManager::ZoneManager() : m_zones{}, m_count{0}
{}

ZoneManager::~ZoneManager()
{}

void ZoneManager::setLoggerPath(const std::string& a_path)
{
    m_logPath = a_path;
#ifdef BOT_LOGIC_DEBUG_OBJECTMANAGER
    m_logger.Init(a_path, "ZoneManager.log");
#endif
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

    if(first && second)
    {
        first->addJunction(secondZone, object);
        second->addJunction(firstZone, object);
        return true;
    }
    return false;
}

void ZoneManager::initZones()
{
    std::vector<Node*> toDo;
    std::set<Node*> done;

    // Pour chaque npc
    std::vector<Npc *> npcs = NPCManager::get()->getNpcs();
    for(auto npc : npcs)
    {
        Node *npcNode = Map::get()->getNode(npc->getCurrentTileId());
        if(npcNode->getZone() != npc->getZone())
        {
            npc->setZone(npcNode->getZone());
        }
        done.insert(npcNode);
        updateTileZone(npcNode);
        done.clear();
    }
    while(!toDo.empty())
    {
        done.insert(toDo[0]);
        updateTileZone(toDo[0]);
        toDo.erase(std::find(toDo.begin(), toDo.end(), toDo[0]));
        done.clear();
    }
}

void ZoneManager::updateZones()
{
    std::stringstream ss;
    std::vector<Node*> toDo;
    std::set<Node*> done;

    // Pour chaque npc
    std::vector<Npc *> npcs = NPCManager::get()->getNpcs();
    for(auto npc : npcs)
    {
        ss << "NPC " << npc->getId() << std::endl;
        m_logger.Log(ss.str(), true);
        ss.str("");
        ss.clear();
        Node *npcNode = Map::get()->getNode(npc->getCurrentTileId());
        if(npcNode->getZone() != npc->getZone())
        {
            npc->setZone(npcNode->getZone());
        }
        done.insert(npcNode);
        updateTileZone(npcNode);
        done.clear();
    }
    while(!toDo.empty())
    {
        done.insert(toDo[0]);
        updateTileZone(toDo[0]);
        toDo.erase(std::find(toDo.begin(), toDo.end(), toDo[0]));
        done.clear();
    }
}

void ZoneManager::updateTileZone(Node* currentTile)
{
    std::stringstream ss;

    ss << "Current tile" << currentTile->getId() << std::endl;
    m_logger.Log(ss.str(), true);
    ss.str("");
    ss.clear();
    for(unsigned int dir = EDirection::NE; dir <= EDirection::NW; ++dir)
    {
        // Get the neighbour
        Node *neighbour = currentTile->getNeighboor(static_cast<EDirection>(dir));
        if(neighbour != nullptr)
        {
            if(neighbour->getType() != Node::NONE)
            {
                if(std::find(done.begin(), done.end(), neighbour) == done.end())
                {
                    EDirection direction = static_cast<EDirection>(dir);
                    EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
                    // If there is no obstacle between the neighbour and the current tile
                    if((!currentTile->isEdgeBlocked(direction) && !neighbour->isEdgeBlocked(invDir)))
                    {
                        auto it = m_notSure.find(neighbour->getId());
                        if (it != m_notSure.end())
                            m_notSure.erase(it);
                        auto itNeighbour = std::find(begin(toDo), end(toDo), neighbour);
                        if(itNeighbour != end(toDo))
                        {
                            toDo.erase(itNeighbour);
                        }
                        done.insert(neighbour);
                        neighbour->setZone(currentTile->getZone());
                        ss << "(No wall) Set tile " << neighbour->getId() << " zone to " << currentTile->getZone() << std::endl;
                        m_logger.Log(ss.str(), true);
                        ss.str("");
                        ss.clear();
                        updateTileZone(neighbour);
                        ss << "BACK TO " << currentTile->getId() << std::endl;
                        m_logger.Log(ss.str(), true);
                        ss.str("");
                        ss.clear();
                    }
                    else if(neighbour->getZone() == 0)
                    {
                        Map *map = Map::get();
                        addZone();
                        neighbour->setZone(m_count);
                        toDo.push_back(neighbour);
                        ss << "(Wall) Add tile " << neighbour->getId() << " to toDO list " << std::endl;
                        m_logger.Log(ss.str(), true);
                        ss.str("");
                        ss.clear();
                    }
                }
            }
            else
                m_notSure[neighbour->getId()] = neighbour;
        }
    }
}


