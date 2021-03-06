#ifndef OBJECT_MANAGER_HEADER
#define OBJECT_MANAGER_HEADER

#include "Singleton.h"

#include "Logger.h"
#include "TurnInfo.h"
#include "Object.h"

#include <vector>
#include <map>


#ifdef _DEBUG
#define BOT_LOGIC_DEBUG_OBJECTMANAGER
#endif

#ifdef BOT_LOGIC_DEBUG_OBJECTMANAGER
#define BOT_LOGIC_OBJECTMANAGER_LOG(logger, text, autoEndLine) logger.Log(text, autoEndLine)
#else
#define BOT_LOGIC_OBJECTMANAGER_LOG(logger, text, autoEndLine) 0
#endif


class ObjectManager : public Singleton
{
private:
    enum
    {
        MAX_OBJECTS = 100
    };

private:
    // Singleton instance
    static ObjectManager m_instance;

private:
    // Log stuff
    Logger m_logger;
    std::string m_logPath;

    // Contains all the objects in the map
    std::map<Object::ObjectType, std::vector<ObjectRef>> m_allObjects;

private:
    ObjectManager();

public:
    // Get the instance of the manager
    static ObjectManager* get() noexcept;

public:
    // Define the log path for the ObjectManager logger and save the path to give it for each object
    void setLoggerPath(const std::string& a_path);

    // Update ObjectManager's logger for each turn
    void updateLogger(const TurnInfo& turnInfo);

    // Instantiate all Objects
    void initObjects(const std::map<unsigned int, ObjectInfo>& objects, const std::map<unsigned int, TileInfo>& tiles);

    // Update all objects
    void updateObjects(const std::map<unsigned int, ObjectInfo>& objects, const std::map<unsigned int, TileInfo>& tiles);

    // Get an object by its Id
    const ObjectRef getObjectById(Object::ObjectType type, size_t index) const;

    // Get all objects on a specific tile
    std::vector<ObjectRef> getAllObjectsOnTile(unsigned int tileId) const;

    // Get all objects of a specific type
    const std::vector<ObjectRef>& getAllObjectsByType(Object::ObjectType type) const;

    // Get all objects that is not activated
    std::vector<ObjectRef> getAllUnactivatedObjects() const;

    // Get all objects that is activated
    std::vector<ObjectRef> getAllActivatedObjects() const;

    // Get all objects
    const std::map<Object::ObjectType, std::vector<ObjectRef>>& getAllObjects() const noexcept;

private:
    // Instantiate an object and save it in the vector
    void updateDoorObject(const ObjectInfo& objectInfo);

    // Update pressure plates if the npc found a new one
    void updatePressurePlateObject(const TileInfo& tile);

    // Link all devices to its object
    void linkObjects(const TileInfo& tileInfo, ObjectRef obj);
};

#endif // OBJECT_MANAGER_HEADER
