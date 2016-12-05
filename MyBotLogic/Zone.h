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
    struct Junction
    {
        Junction() = delete;
        Junction(unsigned int destZone, ObjectRef object, bool isOpenableAlone, Node* nodeFrom, Node* nodeTo);

        unsigned int    m_destinationZone;
        ObjectRef       m_object;
        Node            *m_nodes[2];
        bool            m_isOpenableAlone;
    };

    // Zone Class
    Zone() = delete;
    Zone(unsigned int id);
    ~Zone();

    const Junction  &getJunction(unsigned int zoneId) const;
    unsigned int    getZoneId() const;
    bool            isExplored() const;
    void            addJunction(unsigned int neighboorZone, ObjectRef object, bool isOpenableAlone, Node* nodeFrom, Node* nodeTo);

private:
    std::map<unsigned int, Junction *>  m_junctions;
    unsigned int                        m_id;
    bool                                m_isExplored;
};

#endif // !_ZONE_H_

