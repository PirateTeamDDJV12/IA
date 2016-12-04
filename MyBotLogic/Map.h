#ifndef MAP_HEADER
#define MAP_HEADER

#include "Node.h"
#include "Singleton.h"
#include "Logger.h"
#include "ObjectInfo.h"
#include "Npc.h"
#include "TileInfo.h"
#include "NPCInfo.h"
#include <map>
#include <vector>

#ifdef _DEBUG
#define BOT_LOGIC_DEBUG_MAP
#endif

#ifdef BOT_LOGIC_DEBUG_MAP
#define BOT_LOGIC_MAP_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
#define BOT_LOGIC_MAP_LOG(logger, text, autoEndLine) 0
#endif

class Map : Singleton
{

    static Map m_instance;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_influenceRange;
    std::map<unsigned int, Node*> m_nodeMap;
    std::vector<unsigned int> m_goalTiles;
    std::map<unsigned, bool> m_seenTiles;
    std::vector<Node*> m_interestingNodes;
    //std::map<unsigned int, SearchMap*> m_searchMap;

    // Log stuff
    Logger m_logger;
    Logger m_loggerInfluence;
    Logger m_loggerZone;
    Logger m_loggerEdges;

private:
    Map() : m_width(0), m_height(0)
    {}
    int calculateDistance(int start, int end);
    std::string getStringDirection(unsigned int, unsigned int);
    void testAddTile(std::vector<unsigned>& v, unsigned int, unsigned int tileId);
public:
    // Set the logger path and initialise all map logger
    void setLoggerPath(const std::string &a_path);

    // Change the node type
    void setNodeType(unsigned int, Node::NodeType);

    // Change the node zone
    void setNodeZone(unsigned int tileId, unsigned int zoneId);

    // Create a new node
    void createNode(Node*);

    // Connect all the nodes together (neighboor) to create the graph
    void connectNodes();

    Node* getNode(unsigned int, unsigned int);
    Node* getNode(unsigned int);

    // Return for each NPCs the best goal
    std::map<unsigned, unsigned> getBestGoalTile(const std::vector<Npc*>& npcInfo);

    // return the direction between two points
    EDirection getNextDirection(unsigned int a_start, unsigned int a_end);

    static Map *get() noexcept
    {
        return &m_instance;
    }

    unsigned int getWidth() const
    {
        return m_width;
    }
    void setWidth(unsigned int w)
    {
        m_width = w;
    }
    unsigned int getHeight() const
    {
        return m_height;
    }
    void setHeight(unsigned int h)
    {
        m_height = h;
    }
    unsigned int getInfluenceRange() const
    {
        return m_influenceRange;
    }
    void setInfluenceRange(unsigned int range)
    {
        m_influenceRange = range;
    }

    void addSeenTile(unsigned tileId)
    {
        if (m_seenTiles[tileId])
        {
            return;
        }
        m_seenTiles[tileId] = false;
    }

    void visitTile(unsigned tileId)
    {
        m_seenTiles[tileId] = true;
    }

    bool isVisited(unsigned tileId)
    {
        return m_seenTiles[tileId];
    }

    std::vector<unsigned> getNonVisitedTile()
    {
        std::vector<unsigned> v;
        for (auto seenTile : m_seenTiles)
        {
            if (!seenTile.second)
            {
                v.push_back(seenTile.first);
            }
        }
        return std::move(v);
    }

    //std::vector<unsigned> getMostInfluencedTile();
    //std::vector<unsigned int> getNearUnVisitedTile(unsigned int a_currentId);

    void addGoalTile(unsigned int number);
    void createInfluenceMap();
    void propagateInfluence();
    void propage(Node* myNode, unsigned curDist, unsigned maxDist, float initialInfluence) const;

    std::vector<unsigned int> getNpcPath(unsigned int a_start, unsigned int a_end, std::set<Node::NodeType> forbiddenType = { Node::FORBIDDEN });

    bool canMoveOnTile(unsigned int a_fromTileId, unsigned int a_toTileId);

    int getNearInfluencedTile(unsigned int a_currentId);
    bool isAllNeighboorHaveSameInfluence(Node* node);
    void logMap(unsigned);
    void logInfluenceMap(unsigned nbTurn);
    void logZoneMap(unsigned nbTurn);

    void initMap(unsigned int row, unsigned int col, unsigned int range);
    void updateEdges(const std::map<unsigned int, ObjectInfo>& objects, unsigned nbTurn);
    void updateTiles(const std::map<unsigned int, TileInfo>& tiles);
};

#endif // MAP_HEADER
