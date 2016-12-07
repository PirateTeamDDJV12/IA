#include "Node.h"
#include <vector>
#include "Npc.h"
#include "NPCManager.h"

Node::Node(unsigned int xVal, unsigned int yVal, unsigned int idVal, NodeType typeVal)
    : m_ID(idVal), m_type(typeVal), m_npcId(-1)
{
    m_pos = new Position(xVal, yVal);
}

unsigned int Node::getId() const noexcept
{
    return m_ID;
}

int Node::getNpcIdOnNode() const noexcept
{
    return m_npcId;
}

unsigned int Node::getZone() const noexcept
{
    return m_zone;
}

float Node::getInfluence() const noexcept
{
    return m_influence;
}

Position* Node::getPosition() const noexcept
{
    return m_pos;
}

Node::NodeType Node::getType() const noexcept
{
    return m_type;
}

Node::EdgeType Node::getEdge(EDirection dir) const
{
    return static_cast<EdgeType>(m_edgesCost[dir]);
}

Node* Node::getNeighboor(unsigned int tileId)
{
    // Can be optimized by keeping a set of ids of direct neighbors
    for (auto nodes : m_neighboors)
    {
        if (nodes != nullptr)
        {
            if (nodes->m_ID == tileId)
                return nodes;
        }
    }
    return nullptr;
}

Node* Node::getNeighboor(EDirection dir)
{
    return m_neighboors[dir];
}

void Node::setNpcIdOnNode(int npcId) noexcept
{
    m_npcId = npcId;
}

void Node::setZone(unsigned int zoneId) noexcept
{
    m_zone = zoneId;
}

void Node::setInfluence(float inf) noexcept
{
    m_influence = inf;
}

void Node::setType(Node::NodeType nType) noexcept
{
    m_type = nType;
}

void Node::setEdgeCost(EDirection dir, std::set<EObjectType> types)
{
    if(std::find(begin(types), end(types), ObjectType_Door) != end(types))
    {
        m_edgesCost[dir] = ObjectType_Door + 1;
    }
    else if(std::find(begin(types), end(types), ObjectType_Window) != end(types))
    {
        m_edgesCost[dir] = ObjectType_Window + 1;
    }
    else if(std::find(begin(types), end(types), ObjectType_HighWall) != end(types))
    {
        m_edgesCost[dir] = ObjectType_HighWall + 1;
    }
}

void Node::setNeighboor(EDirection dir, Node* p)
{
    m_neighboors[dir] = p;
}

bool Node::isEdgeBlocked(EDirection dir) const
{
    return m_edgesCost[dir] == 0 ? false : true;
}

bool Node::isTileOccupied() const
{
    return m_npcId >= 0;
}

bool Node::isTileHasNpcArrived() const
{
    if(m_npcId >= 0)
    {
        const std::vector<Npc*> &allNpcs = NPCManager::get()->getNpcs();
        for(Npc* npc : allNpcs)
        {
            if(npc->getId() == static_cast<unsigned int>(m_npcId))
            {
                return npc->getStatus() == Npc::ARRIVED;
            }
        }
    }
    return false;
}

bool Node::isBlockedByDoor(EDirection dir) const
{
    return m_edgesCost[dir] == (ObjectType_Door + 1);
}

unsigned int Node::calculateManathan(const Node* goal) const noexcept
{
    int x = goal->getPosition()->x - this->m_pos->x;
    int y = goal->getPosition()->y - this->m_pos->y;
    return (abs(x) + abs(y)) * 10;
}
