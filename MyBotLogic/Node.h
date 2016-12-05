#ifndef NODE_HEADER
#define NODE_HEADER

#include "Globals.h"

#include <set>


struct Position
{
    unsigned int x;
    unsigned int y;
    Position(unsigned int xVal, unsigned int yVal) :
        x{ xVal }, 
        y{ yVal }
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
    unsigned int m_ID;
    unsigned int m_npcId = { 0 };
    unsigned int m_edgesCost[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned int m_zone = { 0 };
    float m_influence = { 0 };

    Position* m_pos;
    NodeType m_type;

    Node* m_neighboors[8] = { nullptr };

    // TODO - Add an area to our nodes to differentiate them and let us know immediately if we can access this node or not

public:
    Node() = delete;
    Node(unsigned int xVal, unsigned int yVal, unsigned int idVal, NodeType typeVal);

public:
    // Getters
    unsigned int getId() const noexcept;
    unsigned getNpcIdOnNode() const noexcept;
    unsigned int getZone() const noexcept;
    float getInfluence() const noexcept;
    Position* getPosition() const noexcept;
    NodeType getType() const noexcept;
    EdgeType getEdge(EDirection dir) const;
    Node* getNeighboor(unsigned int tileId);
    Node* getNeighboor(EDirection dir);

    // Setters
    void setNpcIdOnNode(unsigned npcId) noexcept;
    void setZone(unsigned int zoneId) noexcept;
    void setInfluence(float inf) noexcept;
    void setType(NodeType nType) noexcept;
    void setEdgeCost(EDirection dir, std::set<EObjectType> types);
    void setNeighboor(EDirection dir, Node* p);

    // Verrify if an obstacle blocks the path
    bool isEdgeBlocked(EDirection dir) const;

    // Heuristic for A*
    unsigned int calculateManathan(const Node* goal) const noexcept;
};

#endif // NODE_HEADER
