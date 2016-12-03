#include <Math.h>
#include <windows.h>
#include <algorithm>

#include "Map.h"
#include "TurnInfo.h"
#include "SearchMap.h"
#include "NPCManager.h"

Map Map::m_instance;

void Map::setLoggerPath(const std::string &a_path)
{
#ifdef BOT_LOGIC_DEBUG_MAP
    m_logger.Init(a_path, "Map.log");
    m_loggerInfluence.Init(a_path, "Map_Influence.log");
    m_loggerZone.Init(a_path, "Map_Zone.log");
    m_loggerEdges.Init(a_path, "Map_Edges.log");
#endif

    BOT_LOGIC_MAP_LOG(m_logger, "Configure", true);
    BOT_LOGIC_MAP_LOG(m_loggerInfluence, "Configure", true);
    BOT_LOGIC_MAP_LOG(m_loggerZone, "Configure", true);
    BOT_LOGIC_MAP_LOG(m_loggerEdges, "Configure", true);
}

void Map::setNodeType(unsigned int tileId, Node::NodeType tileType)
{
    // TODO - Attention si jamais on veut refaire bouger un NPC une fois qu'il est sur une tuile
    if(m_nodeMap[tileId]->getType() != Node::OCCUPIED)
    {
        m_nodeMap[tileId]->setType(tileType);        
    }
}

void Map::setNodeZone(unsigned int tileId, unsigned int zoneId)
{
    m_nodeMap[tileId]->setZone(zoneId);
}

void Map::createNode(Node* node)
{
    m_nodeMap[node->getId()] = node;
}

void Map::connectNodes()
{
    for(std::pair<unsigned int, Node*> curNode : m_nodeMap)
    {
        // connecting
        Node* nw;
        Node* ne;
        Node* e;
        Node* se;
        Node* sw;
        Node* w;
        if(curNode.second->getPosition()->y % 2 == 0)
        {
            nw = getNode(curNode.second->getPosition()->x - 1, curNode.second->getPosition()->y - 1);
            ne = getNode(curNode.second->getPosition()->x, curNode.second->getPosition()->y - 1);
            e = getNode(curNode.second->getPosition()->x + 1, curNode.second->getPosition()->y);
            se = getNode(curNode.second->getPosition()->x, curNode.second->getPosition()->y + 1);
            sw = getNode(curNode.second->getPosition()->x - 1, curNode.second->getPosition()->y + 1);
            w = getNode(curNode.second->getPosition()->x - 1, curNode.second->getPosition()->y);
        }
        else
        {
            nw = getNode(curNode.second->getPosition()->x, curNode.second->getPosition()->y - 1);
            ne = getNode(curNode.second->getPosition()->x + 1, curNode.second->getPosition()->y - 1);
            e = getNode(curNode.second->getPosition()->x + 1, curNode.second->getPosition()->y);
            se = getNode(curNode.second->getPosition()->x + 1, curNode.second->getPosition()->y + 1);
            sw = getNode(curNode.second->getPosition()->x, curNode.second->getPosition()->y + 1);
            w = getNode(curNode.second->getPosition()->x - 1, curNode.second->getPosition()->y);
        }
        curNode.second->setNeighboor(NW, nw);
        curNode.second->setNeighboor(NE, ne);
        curNode.second->setNeighboor(E, e);
        curNode.second->setNeighboor(SE, se);
        curNode.second->setNeighboor(SW, sw);
        curNode.second->setNeighboor(W, w);
    }
}

Node* Map::getNode(unsigned int x, unsigned int y)
{
    if(x < 0 || x > getWidth() - 1 || y < 0 || y > getHeight() - 1)
    {
        return nullptr;
    }
    unsigned int index = x + y * m_width;
    return m_nodeMap[index];
}

Node* Map::getNode(unsigned int index)
{
    return m_nodeMap[index];
}

int Map::calculateDistance(int indexStart, int indexEnd)
{
    Node* start = m_nodeMap[indexStart];
    Node* end = m_nodeMap[indexEnd];
    int x = start->getPosition()->x - end->getPosition()->x;
    int y = start->getPosition()->y - end->getPosition()->y;
    return abs(x) + abs(y);
}

std::map<unsigned, unsigned> Map::getBestGoalTile(const std::vector<Npc*>& npcInfo)
{
    std::map<unsigned, unsigned> goalMap;
    NPCManager *NpcMan = NPCManager::get();
    if(m_goalTiles.size() > npcInfo.size())
    {
        auto copyMapGoal = m_goalTiles;
        for(Npc* npc : npcInfo)
        {
            int bestDist = 666;
            unsigned goalId = -1;
            std::map<unsigned int, bool>::iterator goalIt = begin(copyMapGoal);
            std::map<unsigned int, bool>::iterator saveGoalIt = end(copyMapGoal);
            for(; goalIt != end(copyMapGoal); ++goalIt)
            {
                unsigned int currentGoalId = (*goalIt).first;
                if(NpcMan->isGoalAlreadyAssign(currentGoalId))
                {
                    continue;
                }
                int distance = calculateDistance(npc->getCurrentTileId(), currentGoalId);
                if(distance < bestDist)
                {
                    goalId = currentGoalId;
                    bestDist = distance;
                    saveGoalIt = goalIt;
                }
            }
            goalMap[npc->getId()] = goalId;
            copyMapGoal.erase(saveGoalIt);
        }
    }
    else
    {
        for(std::pair<unsigned int, bool> goal : m_goalTiles)
        {
            if(NpcMan->isGoalAlreadyAssign(goal.first))
            {
                continue;
            }
            if(npcInfo.empty())
            {
                break;
            }
            int bestDist = 666;
            int npcId = -1;
            for(Npc* npc : npcInfo)
            {
                // Don't want a npc to have many goal
                if(goalMap.find(npc->getId()) != end(goalMap) || npc->hasGoal())
                {
                    continue;
                }
                int distance = calculateDistance(npc->getCurrentTileId(), goal.first);
                if(distance < bestDist)
                {
                    npcId = npc->getId();
                    bestDist = distance;
                }
            }
            goalMap[npcId] = goal.first;
        }
    }

    BOT_LOGIC_MAP_LOG(m_logger, "Attribution des goals : ", true);
    for(auto goal : goalMap)
    {
        BOT_LOGIC_MAP_LOG(m_logger, "\tNPC " + std::to_string(goal.first) + " a pour goal : " + std::to_string(goal.second), true);
    }
    return std::move(goalMap);
}

// TODO - See if we can implement this
//std::vector<unsigned> Map::getMostInfluencedTile()
//{
//    std::vector<unsigned> v;
//    for(auto node : m_seenTiles)
//    {
//        if(!node.second)
//        {
//            
//        }
//    }
//    for(auto node : m_interestingNodes)
//    {
//        v.push_back(node->getId());
//    }
//    return v;
//}

void Map::addGoalTile(unsigned int number)
{
    if(m_goalTiles.find(number) == end(m_goalTiles))
    {
        bool canAccesTile = false;
        Node* currentNode = getNode(number);

        for(int i = N; i <= NW; ++i)
        {
            EDirection dir = static_cast<EDirection>(i);
            EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
            Node* tempNode = currentNode->getNeighboor(dir);
            if(tempNode != nullptr && (!currentNode->isEdgeBlocked(dir) && !tempNode->isEdgeBlocked(invDir)))
            {
                canAccesTile = true;
            }
        }

        if(canAccesTile)
        {
            m_goalTiles[number] = false;
        }
    }
}

void Map::createInfluenceMap()
{
    m_interestingNodes.clear();
    for(auto node : m_seenTiles)
    {
        Node* myNode = getNode(node.first);
        myNode->setInfluence(0.0f);
        if(!node.second)
        {
            float tempInflu = 0.0f;
            for(int i = N; i <= NW; ++i)
            {
                EDirection dir = static_cast<EDirection>(i);
                EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
                auto edgeType = myNode->getEdge(dir);
                Node* tempNode = myNode->getNeighboor(dir);
                auto edgeNeibType = Node::FREE;
                if(tempNode != nullptr)
                {
                    edgeNeibType = tempNode->getEdge(invDir);
                }
                if(edgeType == Node::WINDOW || edgeNeibType == Node::WINDOW)
                {
                    tempInflu += 1.0f;
                }
            }

            for(int i = N; i <= NW; ++i)
            {
                EDirection dir = static_cast<EDirection>(i);
                EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
                Node* tempNode = myNode->getNeighboor(dir);
                if(tempNode != nullptr && (!myNode->isEdgeBlocked(dir) && !tempNode->isEdgeBlocked(invDir)))
                {
                    if(tempNode->getType() == Node::NONE)
                    {
                        tempNode->setInfluence(1.0f);
                        m_interestingNodes.push_back(tempNode);
                    }
                }
            }
            if(tempInflu > 0.0f)
            {
                myNode->setInfluence(tempInflu);
                m_interestingNodes.push_back(myNode);
            }
        }
    }
    std::sort(begin(m_interestingNodes), end(m_interestingNodes), [](const Node* a, const Node* b) {
        return a->getInfluence() > b->getInfluence();
    });
    propagateInfluence();
}

void Map::propagateInfluence()
{
    unsigned maxDist = m_influenceRange;
    for(auto node : m_interestingNodes)
    {
        propage(node, 0, maxDist, node->getInfluence());
    }
}

void Map::propage(Node* myNode, unsigned curDist, unsigned maxDist, float initialInfluence) const
{
    if(curDist > maxDist)
    {
        return;
    }
    for(int i = N; i <= NW; ++i)
    {
        EDirection dir = static_cast<EDirection>(i);
        EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
        Node* tempNode = myNode->getNeighboor(dir);
        if(tempNode != nullptr && (!myNode->isEdgeBlocked(dir) && !tempNode->isEdgeBlocked(invDir)))
        {
            if(tempNode->getType() == Node::PATH)
            {
                auto newInfluence = myNode->getInfluence() - (initialInfluence / m_influenceRange);
                if(newInfluence > tempNode->getInfluence())
                {
                    tempNode->setInfluence(newInfluence);
                    propage(tempNode, ++curDist, maxDist, initialInfluence);
                }
            }
        }
    }
}

EDirection Map::getNextDirection(unsigned int a_start, unsigned int a_end)
{
    std::string direction = std::move(getStringDirection(a_start, a_end));

    if(direction == "N")
    {
        return N;
    }
    if(direction == "E")
    {
        return E;
    }
    if(direction == "W")
    {
        return W;
    }
    if(direction == "S")
    {
        return S;
    }
    if(direction == "NW")
    {
        return NW;
    }
    if(direction == "NE")
    {
        return NE;
    }
    if(direction == "SW")
    {
        return SW;
    }
    if(direction == "SE")
    {
        return SE;
    }
    return NE;
}

std::string Map::getStringDirection(unsigned int start, unsigned int end)
{
    Node* nStart = m_nodeMap[start];
    Node* nEnd = m_nodeMap[end];
    int x = nEnd->getPosition()->x - nStart->getPosition()->x;
    int y = nEnd->getPosition()->y - nStart->getPosition()->y;

    std::string direction{""};

    // Set the North/South direction
    if(y < 0)
    {
        direction += "N";
    }
    else if(y > 0)
    {
        direction += "S";
    }

    // Depending on the row, we need to adapt the West.East direction
    if(nStart->getPosition()->y % 2 == 0)
    {
        if(x < 0)
        {
            direction += "W";
        }
        else
        {
            direction += "E";
        }
    }
    else
    {
        if(x > 0)
        {
            direction += "E";
        }
        else
        {
            direction += "W";
        }
    }

    return std::move(direction);
}

std::vector<unsigned int> Map::getNpcPath(unsigned int a_start, unsigned int a_end, std::set<Node::NodeType> forbiddenType)
{
    SearchMap mySearch{getNode(a_start), getNode(a_end), forbiddenType};
    return std::move(mySearch.search());
}

bool Map::canMoveOnTile(unsigned int a_fromTileId, unsigned int a_toTileId)
{
    if(a_fromTileId == a_toTileId)
    {
        return true;
    }

    bool isStateOk = !(getNode(a_toTileId)->getType() == Node::FORBIDDEN || getNode(a_toTileId)->isTileOccupied());
    EDirection dir = getNextDirection(a_fromTileId, a_toTileId);
    EDirection invDir = static_cast<EDirection>((dir + 4) % 8);
    return isStateOk && !getNode(a_fromTileId)->isEdgeBlocked(dir) && !getNode(a_toTileId)->isEdgeBlocked(invDir);
}

int Map::getNearInfluencedTile(unsigned int a_currentId)
{
    Node* current = getNode(a_currentId);
    int bestTile = -1;

    if(isAllNeighboorHaveSameInfluence(current))
    {
        return bestTile;
    }

    float bestinf = 0.0f;
    for(int i = N; i <= NW; ++i)
    {
        Node* neighboor = current->getNeighboor(static_cast<EDirection>(i));
        if(neighboor && canMoveOnTile(a_currentId, neighboor->getId()))
        {
            float nodeinf = neighboor->getInfluence();
            if(nodeinf > 0.0f)
            {
                if(bestinf < nodeinf)
                {
                    bestinf = nodeinf;
                    bestTile = neighboor->getId();
                }
            }
        }
    }
    return bestTile;
}

bool Map::isAllNeighboorHaveSameInfluence(Node* node)
{
    float startInf = 0.0f;
    int counterTile = 0;
    for(int i = N; i <= NW; ++i)
    {
        Node* neighboor = node->getNeighboor(static_cast<EDirection>(i));
        if(neighboor && canMoveOnTile(node->getId(), neighboor->getId()))
        {
            counterTile++;
            if(startInf == 0.0f)
            {
                startInf = neighboor->getInfluence();
            }

            if(startInf != neighboor->getInfluence())
            {
                return false;
            }
        }
    }
    // Ensure moving on the only available tile !
    if(counterTile == 1)
    {
        return false;
    }
    return true;
}

void Map::testAddTile(std::vector<unsigned int>& v, unsigned int fromTileId, unsigned int toTileId)
{
    if(canMoveOnTile(fromTileId, toTileId) && !isVisited(toTileId))
    {
        v.push_back(toTileId);
    }
}




//----------------------------------------------------------------------
// LOGGER
//----------------------------------------------------------------------
void Map::logMap(unsigned nbTurn)
{
#ifdef BOT_LOGIC_DEBUG_MAP
    std::string myLog = std::move("\nTurn #" + std::to_string(nbTurn) + "\n");

    // Printing some infos
    myLog += "\tAccessible Goal Number : " + std::to_string(m_goalTiles.size()) + "\n";

    // Printing the map
    myLog += "Map : \n";
    unsigned int currentTileId{};
    for(unsigned int row = 0; row < m_height; ++row)
    {
        if(row % 2)
        {
            myLog += "   ";
        }
        for(unsigned int col = 0; col < m_width; ++col)
        {
            Node* tempNode = getNode(currentTileId++);
            if(tempNode->getNpcIdOnNode() > 0)
            {
                myLog += std::to_string(tempNode->getNpcIdOnNode()) + "-" + getTileStringToLog(tempNode) + "--";
            }
            else
            {
                myLog += getTileStringToLog(tempNode) + "----";
            }
            myLog += "  ";
        }
        myLog += "\n";
    }
    BOT_LOGIC_MAP_LOG(m_logger, myLog, false);
#endif

}

std::string Map::getTileStringToLog(Node* node) const
{
#ifdef BOT_LOGIC_DEBUG_MAP
    switch(node->getType())
    {       
        case Node::NodeType::FORBIDDEN:
            return"F";
        case Node::NodeType::GOAL:
            return"G";
        case Node::NodeType::OCCUPIED:
            return"X";
        case Node::NodeType::PATH:
            return"P";
        default:
            return"-";
    }
#endif
    return "";
}

void Map::logInfluenceMap(unsigned nbTurn)
{
#ifdef BOT_LOGIC_DEBUG_MAP
    std::string myLog = std::move("\nTurn #" + std::to_string(nbTurn) + "\n");

    // Printing the map
    myLog += "Map : \n";
    unsigned int currentTileId{};
    for(unsigned int row = 0; row < m_height; ++row)
    {
        if(row % 2)
        {
            myLog += "   ";
        }
        for(unsigned int col = 0; col < m_width; ++col)
        {
            Node* tempNode = getNode(currentTileId++);
            float influ = std::trunc(100 * tempNode->getInfluence()) / 100;
            if(tempNode->getNpcIdOnNode() > 0)
            {
                myLog += std::to_string(tempNode->getNpcIdOnNode()) + "-" + std::to_string(influ);
            }
            else
            {
                myLog += std::to_string(influ);
            }
            myLog += "  ";
        }
        myLog += "\n";
    }
    BOT_LOGIC_MAP_LOG(m_loggerInfluence, myLog, false);
#endif

}

void Map::logZoneMap(unsigned nbTurn)
{
#ifdef BOT_LOGIC_DEBUG_MAP
    std::string myLog = std::move("\nTurn #" + std::to_string(nbTurn) + "\n");

    // Printing the map
    myLog += "Map : \n";
    unsigned int currentTileId{};
    for(unsigned int row = 0; row < m_height; ++row)
    {
        if(row % 2)
        {
            myLog += "  ";
        }
        for(unsigned int col = 0; col < m_width; ++col)
        {
            Node* tempNode = getNode(currentTileId++);
            unsigned int zone = tempNode->getZone();
            if(tempNode->getNpcIdOnNode() > 0)
            {
                myLog += std::to_string(zone);
            }
            else
            {
                myLog += std::to_string(zone);
            }
            myLog += "  ";
        }
        myLog += "\n";
    }
    BOT_LOGIC_MAP_LOG(m_loggerZone, myLog, false);
#endif

}

void Map::initMap(unsigned int row, unsigned int col, unsigned int range)
{
    // Init parameters
    m_height = row;
    m_width = col;
    setInfluenceRange(max(range + 2, 4));

    // Create all Nodes
    unsigned int countIndex = 0;
    for(unsigned int i = 0; i < row; ++i)
    {
        for(unsigned int j = 0; j < col; ++j)
        {
            createNode(new Node{j, i, countIndex, Node::NONE});
            countIndex++;
        }
    }

    // Connect all nodes together (neighboor)
    connectNodes();
}

void Map::updateEdges(const std::map<unsigned int, ObjectInfo>& objects, unsigned int nbTurn)
{
    BOT_LOGIC_MAP_LOG(m_loggerEdges, std::move("\nTURN #" + std::to_string(nbTurn)), true);

    for(std::pair<unsigned, ObjectInfo> info : objects)
    {
        Node* node = getNode(info.second.tileID);
        for(int i = N; i <= NW; ++i)
        {
            if(info.second.edgesCost[i] == 0)
            {
                node->setEdgeCost(static_cast<EDirection>(i), info.second.objectTypes);
                BOT_LOGIC_MAP_LOG(m_loggerEdges, std::move("\tTileID : " + std::to_string(info.second.tileID) + " - Dir : " + std::to_string(i) + " - Type : " + std::to_string(node->getEdge(static_cast<EDirection>(i)))), true);
            }
        }
    }
}

void Map::updateTiles(const std::map<unsigned int, TileInfo>& tiles)
{
    for each(auto info in tiles)
    {
        auto tileInfo = info.second;

        auto ITisForbidden = find(begin(tileInfo.tileAttributes), end(tileInfo.tileAttributes), TileAttribute_Forbidden);
        if(ITisForbidden != tileInfo.tileAttributes.end())
        {
            setNodeType(tileInfo.tileID, Node::FORBIDDEN);
        }
        else
        {
            auto ITisTarget = find(begin(tileInfo.tileAttributes), end(tileInfo.tileAttributes), TileAttribute_Target);
            if(ITisTarget != tileInfo.tileAttributes.end())
            {
                setNodeType(tileInfo.tileID, Node::GOAL);
                addGoalTile(tileInfo.tileID);
                addSeenTile(tileInfo.tileID);
            }
            else
            {
                auto ITisDescriptor = find(begin(tileInfo.tileAttributes), end(tileInfo.tileAttributes), TileAttribute_Descriptor);
                if(ITisDescriptor != tileInfo.tileAttributes.end())
                {
                    setNodeType(tileInfo.tileID, Node::PATH);
                    addSeenTile(tileInfo.tileID);
                }
            }
        }
    }
}