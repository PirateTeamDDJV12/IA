#ifndef NODE_HEADER
#define NODE_HEADER
#include <cmath>
#include "Globals.h"
#include <set>

struct Position
{
    unsigned int x;
    unsigned int y;
    Position(unsigned int xVal, unsigned int yVal)
        :x{ xVal }, y{ yVal }
    {}
};

class Node
{
public:
    enum NodeType
    {
        NONE,
        FORBIDDEN,
        GOAL,
        OCCUPIED,
        PATH,
    };
    enum EdgeType
    {
        FREE,
        WALL,
        WINDOW,
    };
private:
    Position* m_pos;
    NodeType m_type;
    Node* m_neighboors[8] = { nullptr };
    unsigned int m_ID;
    unsigned int m_edgesCost[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned int m_npcId = { 0 };
    unsigned int m_zone = { 0 };
    float m_influence = { 0 };
    // TODO - Faire en sorte de definir si on sait tout du node ou pas pour eviter d'aller dessus, pour optimiser la recherche de chemin
    bool m_knowEverythingAboutIt;

    // TODO - Ajouter une zone a nos nodes pour les differencier et permettre de tout de suite savoir si on peut acceder a ce node ou pas
public:
    Node() = delete;
    Node(unsigned int xVal, unsigned int yVal, unsigned int idVal, NodeType typeVal);

    NodeType getType() const noexcept
    {
        return m_type;
    }

    Position* getPosition() const noexcept
    {
        return m_pos;
    }

    unsigned int getId() const noexcept
    {
        return m_ID;
    }

    void setType(NodeType nType)
    {
        m_type = nType;
    }

    void setZone(unsigned int zoneId)
    {
        m_zone = zoneId;
    }

    unsigned int getZone() const noexcept
    {
        return m_zone;
    }

    void setEdgeCost(EDirection dir, std::set<EObjectType> types)
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

    bool isEdgeBlocked(EDirection dir) const
    {
        switch(m_edgesCost[dir])
        {
            case 0: //No wall or anything blocking (default state)
            case ObjectType_Door + 1: // Add +1 because of the default state which is 0 and HighWall is 0 to, so we save the edges with +1
                return false;
            default:
                return true;

        }
    }

    void setNeighboor(EDirection dir, Node* p)
    {
        m_neighboors[dir] = p;
    }

    Node* getNeighboor(EDirection dir)
    {
        return m_neighboors[dir];
    }

    Node* getNeighboor(unsigned int tileId)
    {
        // Peut etre optimisé en gardant un set des id des neighbours direct
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

    void setNpcIdOnNode(unsigned npcId)
    {
        m_npcId = npcId;
    }

    unsigned getNpcIdOnNode() const
    {
        return m_npcId;
    }

    EdgeType getEdge(EDirection dir) const
    {
        return static_cast<EdgeType>(m_edgesCost[dir]);
    }

    float getInfluence() const
    {
        return m_influence;
    }

    void setInfluence(float inf)
    {
        m_influence = inf;
    }

    bool knowEverythingAboutThis() const
    {
        return m_knowEverythingAboutIt;
    }

    void setKnowEverything(bool know)
    {
        m_knowEverythingAboutIt = know;
    }

    unsigned int calculateManathan(const Node* goal) const
    {
        int x = goal->getPosition()->x - this->m_pos->x;
        int y = goal->getPosition()->y - this->m_pos->y;
        return (abs(x) + abs(y)) * 10;
    }
};

#endif // NODE_HEADER
