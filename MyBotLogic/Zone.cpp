#include "Zone.h"

// Junction class Implementation
Zone::Junction::Junction(unsigned int destZone, ObjectRef object, bool isOpenableAlone, Node* nodeFrom, Node* nodeTo)
    : m_destinationZone{destZone},
    m_object{object},
    m_nodes{nodeFrom,nodeTo},
    m_isOpenableAlone{isOpenableAlone}
{}

// Zone class Implementation
Zone::Zone(unsigned int id) : m_id{id}
{}

Zone::~Zone()
{
    for(auto junction : m_junctions)
    {
        delete junction.second;
    }
}

unsigned int Zone::getZoneId() const
{
    return m_id;
}

const Zone::Junction &Zone::getJunction(unsigned int zoneId) const
{
    return *m_junctions.at(zoneId);
}

bool Zone::isExplored() const
{
    return m_isExplored;
}

void Zone::addJunction(unsigned int neighboorZone, ObjectRef object, bool isOpenableAlone, Node* nodeFrom, Node* nodeTo)
{
    m_junctions[neighboorZone] = new Junction(neighboorZone, object, isOpenableAlone, nodeFrom, nodeTo);
}
