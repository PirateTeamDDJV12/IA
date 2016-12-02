#include "ZoneManager.h"
#include "NPCManager.h"
#include "Map.h"

ZoneManager ZoneManager::m_instance;

ZoneManager::ZoneManager()
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

unsigned int ZoneManager::getZoneCount() const
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
    unsigned int zoneId = m_count;
    m_zones[zoneId] = new Zone(zoneId);
    return m_zones[zoneId];
}

void ZoneManager::addZone(Zone *zone)
{
    ++m_count;
    m_zones[zone->getZoneId()] = zone;
}

void ZoneManager::removeZone(unsigned int zoneToRemove, unsigned int currentZone)
{
    // Need to change the zone of the tile that were in that zone
    Zone *tmp = m_zones.at(zoneToRemove);
    delete m_zones.at(zoneToRemove);
    m_zones.erase(zoneToRemove);
    // Update all tiles with zoneId
    // Can be optimized if we keep the tiles of a zone in the zone structure
    Map::get()->changeTileZone(zoneToRemove, currentZone);
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

void ZoneManager::initZones(std::map<unsigned int, NPCInfo> &npcsInfo)
{
    std::vector<Npc *> npcs = NPCManager::get()->getNpcs();

    // For each npc
    for(auto npc : npcs)
    {
        Node *npcNode = Map::get()->getNode(npc->getCurrentTileId());
        unsigned int npcZone = npc->getZone();

        done[npcNode->getId()] = npcNode;
        updateZoneFromNpc(npcNode, npcsInfo.at(npc->getId()));
        done.clear();
    }
    for(auto npc : npcs)
    {
        while(!toDo.empty())
        {
            Node *node = (*toDo.begin()).second;
            unsigned int id = node->getId();
            done[id] = node;
            updateZoneFromNpc(node, npcsInfo.at(npc->getId()));
            toDo.erase(id);
            done.clear();
        }
    }
}

void ZoneManager::updateZones(std::map<unsigned int, NPCInfo> &npcsInfo)
{
    std::vector<Npc *> npcs = NPCManager::get()->getNpcs();

    for(auto npc : npcs)
    {
        Node *npcNode = Map::get()->getNode(npc->getCurrentTileId());
        NPCInfo &npcInfo = npcsInfo.at(npc->getId());

        unsigned int npcTileId = npc->getCurrentTileId();
        std::vector<int> remove;
        for(auto node : m_notSure)
        {
            done.clear();
            unsigned int nodeId = node.second->getId();
            std::set<unsigned int> &visibleTiles = npcInfo.visibleTiles;
            if(std::find(visibleTiles.begin(), visibleTiles.end(), nodeId) != visibleTiles.end())
            {
                remove.push_back(nodeId);
                updateZoneFromNpc(npcNode, npcInfo);
            }
        }
        for(int i = 0; i < remove.size(); ++i)
        {
            m_notSure.erase(remove[i]);
        }
    }
}


void ZoneManager::updateZoneFromNpc(Node* currentTile, NPCInfo &npcInfo)
{
    std::stringstream ss;


    ss << "CURRENT TILE : " << currentTile->getId() << std::endl;
    m_logger.Log(ss.str(), true);
    ss.str("");
    ss.clear();
    for(unsigned int dir = EDirection::NE; dir <= EDirection::NW; ++dir)
    {
        // Get the neighbour
        Node *neighbour = currentTile->getNeighboor(static_cast<EDirection>(dir));
        if(neighbour != nullptr)
        {
            unsigned int neighbourId = neighbour->getId();
            // If the node has not been done
            if(done.find(neighbourId) == done.end())
            {
                std::set<unsigned int> &visibleTiles = npcInfo.visibleTiles;
                // If the node is visible
                if(std::find(visibleTiles.begin(), visibleTiles.end(), neighbourId) != visibleTiles.end())
                {
                    EDirection direction = static_cast<EDirection>(dir);
                    EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
                    // If there is no wall
                    if((!currentTile->isEdgeBlocked(direction) && !neighbour->isEdgeBlocked(invDir)))
                    {
                        if(toDo.find(neighbourId) != toDo.end())
                        {
                            toDo.erase(neighbourId);
                        }
                        // If the neighbour was in a zone this zone needs to be deleted
                        if(neighbour->getZone() != currentTile->getZone() && neighbour->getZone() != 0)
                        {
                            ss << "Remove Zone :" << neighbour->getZone() << std::endl;
                            m_logger.Log(ss.str(), true);
                            ss.str("");
                            ss.clear();
                            removeZone(neighbour->getZone(), currentTile->getZone());
                        }
                        // we then change his zone to the currentTile zone
                        done[neighbourId] = neighbour;
                        neighbour->setZone(currentTile->getZone());
                        neighbour->sure();
                        ss << "(Visible tile nowall) set neighbour :" << neighbourId << " to zone " << currentTile->getZone() << std::endl;
                        m_logger.Log(ss.str(), true);
                        ss.str("");
                        ss.clear();
                        updateZoneFromNpc(neighbour, npcInfo);
                        ss << "BACK TO TILE :" << currentTile->getId() << std::endl;
                        m_logger.Log(ss.str(), true);
                        ss.str("");
                        ss.clear();
                    }
                    // Else if there is a wall (Omniscient or window)
                    else if(neighbour->getZone() == 0)
                    {
                        ZoneManager::get().addZone();
                        neighbour->setZone(ZoneManager::get().getZoneCount());
                        toDo[neighbourId] = neighbour;
                        neighbour->sure();
                        ss << "(Visible tile with wall) Tile " << neighbourId << " added to toDo list. New Zone = " << neighbour->getZone() << std::endl;
                        m_logger.Log(ss.str());
                        ss.str("");
                        ss.clear();
                    }
                }
                // Else if i dont't see it
                else if(neighbour->getZone() == 0)
                {
                    ZoneManager::get().addZone();
                    neighbour->setZone(ZoneManager::get().getZoneCount());
                    done[neighbourId] = neighbour;
                    m_notSure[neighbourId] = neighbour;
                    ss << "(Not visible) Tile " << neighbourId << " added to notSure list. New Zone = " << neighbour->getZone() << std::endl;
                    m_logger.Log(ss.str());
                    ss.str("");
                    ss.clear();
                }
            }
        }
    }
}