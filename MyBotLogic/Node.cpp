#include "Node.h"


Node::Node(unsigned int xVal, unsigned int yVal, unsigned int idVal, NodeType typeVal): m_ID(idVal), m_type(typeVal)
{
    m_pos = new Position(xVal, yVal);
}

unsigned int Node::getId() const noexcept
{
    return m_ID;
}

unsigned Node::getNpcIdOnNode() const noexcept
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

void Node::setNpcIdOnNode(unsigned npcId) noexcept
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
    if (std::find(begin(types), end(types), ObjectType_Door) != end(types))
    {
        m_edgesCost[dir] = ObjectType_Door + 1;
    }
    else if (std::find(begin(types), end(types), ObjectType_Window) != end(types))
    {
        m_edgesCost[dir] = ObjectType_Window + 1;
    }
    else if (std::find(begin(types), end(types), ObjectType_HighWall) != end(types))
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
    switch (m_edgesCost[dir])
    {
    case 0: //No wall or anything blocking (default state)
    case ObjectType_Door + 1: // Add +1 because of the default state which is 0 and HighWall is 0 to, so we save the edges with +1
        return false;
    default:
        return true;
    }
}

unsigned int Node::calculateManathan(const Node* goal) const noexcept
{
    int x = goal->getPosition()->x - this->m_pos->x;
    int y = goal->getPosition()->y - this->m_pos->y;
    return (abs(x) + abs(y)) * 10;
}
