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
#include "TurnInfo.h"

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
    std::map<unsigned int, bool> m_goalTiles;
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

    static Map *get() noexcept
    {
        return &m_instance;
    }

    // Set the logger path and initialise all map logger
    void setLoggerPath(const std::string &a_path);

    // Update all the map (tiles, edges, objects, everything)
    void update(const TurnInfo& turn_info);

    // Initialise the map with row and col number.
    void initMap(unsigned int row, unsigned int col, unsigned int range);

    // Update map edges
    void updateEdges(const std::map<unsigned int, ObjectInfo>& objects, unsigned int nbTurn);

    // Update map tiles
    void updateTiles(const std::map<unsigned int, TileInfo>& tiles);

    // Logger
    void logMap(unsigned);
    void logInfluenceMap(unsigned nbTurn);
    void logZoneMap(unsigned nbTurn);

    // Return node
    Node* getNode(unsigned int x, unsigned int y);
    Node* getNode(unsigned int index);

    // Get a path between two points with an A* algorithm
    std::vector<unsigned int> getNpcPath(unsigned int a_start, unsigned int a_end, std::set<Node::NodeType> forbiddenType = {Node::FORBIDDEN});

    // Return the direction between two points
    EDirection getNextDirection(unsigned int a_start, unsigned int a_end);

    // Return if we can move on a tile or not
    bool canMoveOnTile(unsigned int a_fromTileId, unsigned int a_toTileId);

    // Return for each NPCs the best goal
    std::map<unsigned, unsigned> getBestGoalTile(const std::vector<Npc*>& npcInfo);

    // Return all influence tiles near a position
    int getNearInfluencedTile(unsigned int a_currentId);

    // Return all non visited tiles
    std::vector<unsigned> getNonVisitedTile();

    // Set a tile as visited
    void visitTile(unsigned tileId)
    {
        m_seenTiles[tileId] = true;
    }
private:
    // Change the node type
    void setNodeType(unsigned int, Node::NodeType);

    // Create a new node
    void createNode(Node*);

    // Connect all the nodes together (neighboor) to create the graph
    void connectNodes();

    // Set the influence range
    void setInfluenceRange(unsigned int range)
    {
        m_influenceRange = range;
    }

    // Add a seen tile (we see it but we don't visit it)
    void addSeenTile(unsigned tileId)
    {
        if(m_seenTiles[tileId])
        {
            return;
        }
        m_seenTiles[tileId] = false;
    }

    // Return if the selected tile is visited
    bool isVisited(unsigned tileId)
    {
        return m_seenTiles[tileId];
    }

    // Add a goal to the goal list to define wich NPC go to this goal
    void addGoalTile(unsigned int number);

    // Create the influence map
    void createInfluenceMap();

    // Propagate the influence from the highest spots
    void propagateInfluence();
    void propage(Node* myNode, unsigned curDist, unsigned maxDist, float initialInfluence) const;
    
    // Return if all neighbour of a tile have the same influence
    bool isAllNeighboorHaveSameInfluence(Node* node);

    // Log string function
    std::string getTileStringToLog(Node* node) const;
};

#endif // MAP_HEADER
