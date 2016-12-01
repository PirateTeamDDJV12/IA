#include "Zone.h"

// Junction class Implementation
Zone::Junction::Junction(unsigned int destZone, Object *object) : m_destinationZone{ destZone }, m_nodes{}, m_object{ object }
{
}

// Zone class Implementation
Zone::Zone()
{
}

Zone::~Zone()
{
    for (auto junction : m_junctions)
    {
        delete junction.second;
    }
}

unsigned int Zone::getZoneId() const
{
    return m_id;
}

const Zone::Junction &Zone::getZoneJunction(unsigned int zoneId) const
{
    return *m_junctions.at(zoneId);
}

bool Zone::isExplored() const
{
    return m_isExplored;
}

void Zone::addJunction(unsigned int neighboorZone, Object *object)
{
    m_junctions[neighboorZone] = new Junction(neighboorZone, object);
}
