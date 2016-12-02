#include "Node.h"

Node::Node(int xVal, int yVal, unsigned int idVal, NodeType typeVal) : m_ID(idVal), m_type(typeVal), m_sure{false}
{
    m_pos = new Position(xVal, yVal);
}
