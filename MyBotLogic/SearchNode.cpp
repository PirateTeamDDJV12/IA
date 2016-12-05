#include "SearchNode.h"


unsigned int SearchNode::getX() const noexcept
{
    return m_xpos;
}

unsigned int SearchNode::getY() const noexcept
{
    return m_ypos;
}

unsigned int SearchNode::getId() const noexcept
{
    return m_id;
}

unsigned int SearchNode::getH() const noexcept
{
    return m_H;
}

unsigned int SearchNode::getG() const noexcept
{
    return m_G;
}

unsigned int SearchNode::getF() const noexcept
{
    return m_G + m_H;
}

SearchNode* SearchNode::getParent() const noexcept
{
    return m_parent;
}

void SearchNode::setX(unsigned int x) noexcept
{
    m_xpos = x;
}

void SearchNode::setY(unsigned int y) noexcept
{
    m_ypos = y;
}

void SearchNode::setH(const  unsigned int H) noexcept
{
    m_H = H;
}

void SearchNode::setG(const  unsigned int G) noexcept
{
    m_G = G;
}

void SearchNode::setParent(SearchNode* parent) noexcept
{
    m_parent = parent;
}
