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

    unsigned int getZoneCount() const;
    Zone *getZone(unsigned int zoneId) const;
    Zone *addZone();
    void addZone(Zone *zone);
    bool addJunction(unsigned int firstZone, unsigned int secondZone, Object *object);
    void updateZones();
    void updateTileZone(Node* currentTile, std::set<Node *> &done, std::vector<Node *> &toDo);

private:
    static ZoneManager              m_instance;
    std::map<unsigned int, Zone*>   m_zones;

    ZoneManager();


};

#endif // !_ZONEMANAGER_H_