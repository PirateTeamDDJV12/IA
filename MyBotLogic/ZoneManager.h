#ifndef _ZONEMANAGER_H_
#define _ZONEMANAGER_H_

#include <map>

#include "Zone.h"

class ZoneManager
{
public:
    ZoneManager(const ZoneManager &) = delete;
    ZoneManager &operator=(const ZoneManager&) = delete;
    ~ZoneManager();

    static ZoneManager &get()
    {
        return m_instance;
    }

    Zone    *getZone(unsigned int zoneId) const;
    Zone    *addZone();
    void    addZone(Zone *zone);
    void    updateZones();
    void    updateFromTile(Node* currentTile);
    bool    addJunction(unsigned int firstZone, unsigned int secondZone, Object *object);
    size_t  getZoneCount() const;

private:
    static ZoneManager              m_instance;
    unsigned int                    m_zoneCount;
    std::set<Node*>                 m_done;
    std::vector<Node*>              m_toDo;
    std::map<unsigned int, Zone*>   m_zones;

    ZoneManager();


};

#endif // !_ZONEMANAGER_H_