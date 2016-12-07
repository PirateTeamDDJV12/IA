#ifndef _ZONEMANAGER_H_
#define _ZONEMANAGER_H_

#include <map>

#include "Zone.h"

#ifdef BOT_LOGIC_DEBUG_MAP
#define BOT_LOGIC_ZONE_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
#define BOT_LOGIC_ZONE_LOG(logger, text, autoEndLine) 0
#endif

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
    bool    addJunction(unsigned firstZone, unsigned secondZone, ObjectRef object);
    size_t  getZoneCount() const;
    const Zone::Junction &getJunctionBetween(unsigned int from, unsigned int to);

    // Set the logger path and initialise all map logger
    void setLoggerPath(const std::string &a_path);

private:
    static ZoneManager              m_instance;
    unsigned int                    m_zoneCount;
    std::set<Node*>                 m_done;
    std::vector<Node*>              m_toDo;
    std::map<unsigned int, Zone*>   m_zones;
    Logger                          m_logger;

    ZoneManager();


};

#endif // !_ZONEMANAGER_H_