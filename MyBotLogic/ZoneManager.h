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

    void setLoggerPath(const std::string& a_path);

    unsigned int getZoneCount() const;
    Zone *getZone(unsigned int zoneId) const;
    Zone *addZone();
    void addZone(Zone *zone);
    void removeZone(unsigned int zoneToRemove, unsigned int currentZone);
    bool addJunction(unsigned int firstZone, unsigned int secondZone, Object *object);
    void initZones(std::map<unsigned int, NPCInfo> &npcInfo);
    void updateZones(std::map<unsigned int, NPCInfo> &npcInfo);
    void updateZoneFromTile(Node* currentTile, std::map<unsigned int, Node *> &done, std::map<unsigned int, Node *> &toDo);
    void updateZoneFromNpc(Node* currentTile, NPCInfo &npcInfo);

private:
    static ZoneManager              m_instance;
    std::map<unsigned int, Zone*>   m_zones;
    std::map<unsigned int, Node*>   toDo;
    std::map<unsigned int, Node*>   done;
    std::map<unsigned int, Node *>  m_notSure;
    unsigned int                    m_count;
    // Log stuff
    Logger m_logger; // the logger itself
    std::string m_logPath; // the log path
    ZoneManager();


};

#endif // !_ZONEMANAGER_H_