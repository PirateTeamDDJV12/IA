#ifndef SEARCHMAP_HEADER
#define SEARCHMAP_HEADER
#include "Node.h"
#include <vector>
#include <set>

class Map;
class SearchNode;

class SearchMap
{
    // The start and goal node for the A*
    SearchNode* m_start;
    SearchNode* m_goal;

    // List of all the open node to search
    std::vector<SearchNode*> openList;

    // List of all the node the algorithm already search
    std::vector<SearchNode*> closedList;

    // All the node to go through the start to the goal
    std::vector<unsigned int> m_pathToGoal;

    // List of the type we can access
    std::set<Node::NodeType> m_forbiddenType;

    bool m_isGoalFound = false;
    bool m_isInitialized = false;
public:
    SearchMap() = delete;
    SearchMap(SearchNode* start, SearchNode* goal) : m_start(start), m_goal(goal)
    {}
    SearchMap(Node* start, Node* goal);
    SearchMap(Node* start, Node* goal, std::set<Node::NodeType> forbiddenType);

    // Initialise the map between two nodes
    void initSearchMap(Node*, Node*, std::set<Node::NodeType> forbiddenType = {Node::NodeType::FORBIDDEN, Node::NodeType::OCCUPIED});

    // Execute the search function to get a path
    std::vector<unsigned int> search();

    // Prepare the node to add it in the open list
    void prepareNode(Node*, unsigned int, SearchNode*);

    // Return the next node to be in the closed list (lowest cost)
    SearchNode* getNextNodeToSearch();

    // Calcule the manhattan distance
    unsigned int calculateManhattan(const SearchNode* start, const SearchNode* goal) const;
};

#endif // SEARCHMAP_HEADER
