#ifndef _ZONE_H_
#define _ZONE_H_

#include <map>
#include <vector>

#include "Node.h"
#include "ObjectManager.h"

class Zone
{
public:
    // Junction class
    class Junction
    {
        friend class Planificateur;
    public:
        Junction() = delete;
        Junction(unsigned int destZone, Object *object);

    private:
        unsigned int    m_destinationZone;
        Node            *m_nodes[2];
        Object          *m_object;
    };

    // Zone Class
    Zone() = delete;
    Zone(unsigned int id);
    ~Zone();

    const Junction  &getZoneJunction(unsigned int zoneId) const;
    unsigned int    getZoneId() const;
    bool            isExplored() const;
    void            addJunction(unsigned int neighboorZone, Object *object);

private:
    std::map<unsigned int, Junction *>  m_junctions;
    unsigned int                        m_id;
    bool                                m_isExplored;
};

#endif // !_ZONE_H_

