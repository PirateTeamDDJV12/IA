#ifndef SEARCHMAP_HEADER
#define SEARCHMAP_HEADER
#include "Node.h"
#include <vector>
#include <set>

class Map;
class SearchNode;

class SearchMap
{
    SearchNode* m_start;
    SearchNode* m_goal;
    std::vector<SearchNode*> openList;
    std::vector<SearchNode*> closedList;
    std::vector<unsigned int> m_pathToGoal;
    std::set<Node::NodeType> m_forbiddenType;
    bool m_isGoalFound = false;
    bool m_isInitialized = false;
public:
    SearchMap() = delete;
    SearchMap(SearchNode* start, SearchNode* goal) : m_start(start), m_goal(goal)
    {}
    SearchMap(Node* start, Node* goal);
    SearchMap(Node* start, Node* goal, std::set<Node::NodeType> forbiddenType);

    void initSearchMap(Node*, Node*, std::set<Node::NodeType> forbiddenType = {Node::NodeType::FORBIDDEN, Node::NodeType::OCCUPIED});
    std::vector<unsigned int> search();
    void prepareNode(Node*, unsigned int, SearchNode*);
    SearchNode* getNextNodeToSearch();
    unsigned int calculateManathan(const SearchNode* start, const SearchNode* goal) const;
    void setStartNode(SearchNode* n)
    {
        m_start = n;
    }
    void setGoalNode(SearchNode* n)
    {
        m_goal = n;
    }
};

#endif // SEARCHMAP_HEADER
