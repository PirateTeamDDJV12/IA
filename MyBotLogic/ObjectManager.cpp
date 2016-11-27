#include "ObjectManager.h"

#include "Map.h"

#include <algorithm>


ObjectManager ObjectManager::m_instance;


ObjectManager::ObjectManager()
{
    m_allObjects[Object::ObjectType::PORTE].reserve(MAX_OBJECTS);
    m_allObjects[Object::ObjectType::BOUTON].reserve(MAX_OBJECTS);
    m_allObjects[Object::ObjectType::SENTINELLE].reserve(MAX_OBJECTS);
    m_allObjects[Object::ObjectType::PRESSURE_PLATE].reserve(MAX_OBJECTS);
}

void ObjectManager::setLoggerPath(const std::string& a_path)
{
    m_logPath = a_path;
#ifdef BOT_LOGIC_DEBUG_OBJECTMANAGER
    m_logger.Init(a_path, "ObjectManager.log");
#endif
}

void ObjectManager::updateLogger(const TurnInfo& turnInfo)
{
    std::stringstream sstream;

    for (auto iterPair : m_allObjects)
    {
        for (auto curObject : iterPair.second)
        {
            curObject->toString(sstream);
        }
    }

    BOT_LOGIC_OBJECTMANAGER_LOG(m_logger, "\nTURN #" + std::to_string(turnInfo.turnNb) + "\n" + sstream.str(), true);
}

void ObjectManager::initObjects(const std::map<unsigned int, ObjectInfo>& objects, const std::map<unsigned int, TileInfo>& tiles)
{
    for (auto curObject : objects)
    {
        std::shared_ptr<Object> obj = initObject(curObject.second);
        m_allObjects[obj->getType()].push_back(obj);
    }
    linkObjects(tiles);
}

const Object& ObjectManager::getObjectById(Object::ObjectType type, size_t index) const
{
    for (auto iterPair : m_allObjects)
    {
        if (iterPair.first == type)
        {
            return *iterPair.second[index];
        }
    }
}

const std::vector<ObjectRef>& ObjectManager::getAllObjectsOnTile(unsigned int tileId) const
{
    std::vector<ObjectRef> objectsOnTile;
    for (auto iterPair : m_allObjects)
    {
        for (auto curObject : iterPair.second)
        {
            if (curObject->getType() == tileId)
            {
                objectsOnTile.push_back(curObject);
            }
        }
    }
    return objectsOnTile;
}

const std::vector<ObjectRef>& ObjectManager::getAllUnactivatedObjects() const
{
    std::vector<ObjectRef> unactivatedObjects;
    for (auto iterPair : m_allObjects)
    {
        for (auto curObject : iterPair.second)
        {
            if (!curObject->isActive())
            {
                unactivatedObjects.push_back(curObject);
            }
        }
    }
    return unactivatedObjects;
}

const std::vector<ObjectRef>& ObjectManager::getAllActivatedObjects() const
{
    std::vector<ObjectRef> activatedObjects;
    for (auto iterPair : m_allObjects)
    {
        for (auto curObject : iterPair.second)
        {
            if (curObject->isActive())
            {
                activatedObjects.push_back(curObject);
            }
        }
    }
    return activatedObjects;
}

void ObjectManager::updateObjects(TurnInfo& turnInfo)
{

}

std::shared_ptr<Object> ObjectManager::initObject(const ObjectInfo& objectInfo)
{
    std::shared_ptr<Object> obj{ new Object(objectInfo.objectID, objectInfo.tileID) };

    obj->setObjectType(objectInfo.objectTypes);
    obj->setObjectState(objectInfo.objectStates);

    return obj;
}

void ObjectManager::linkObjects(const std::map<unsigned int, TileInfo>& tiles)
{
    for (auto curTile : tiles)
    {
        for (auto curAttribute : curTile.second.tileAttributes)
        {
            if (curAttribute == TileAttribute_PressurePlate)
            {
                ObjectRef obj{ new Object(curTile.second.tileID, curTile.second.tileID) };
                obj->m_type = Object::ObjectType::PRESSURE_PLATE;
                obj->m_isActive = true;

                // Get all object that is a door
                auto doors = m_allObjects[Object::ObjectType::PORTE];

                // Finds the specific door linked to the pressure plate
                auto it = std::find_if(doors.begin(), doors.end(),
                    [&curTile](ObjectRef it)->bool
                    {
                        return it->m_id == curTile.second.controlledDeviceID;
                    }
                );

                // Link the pressure plate to the door
                obj->getLinkedObjects().push_back(*it._Ptr);

                // Link the door to the pressure plate
                (*it._Ptr)->getLinkedObjects().push_back(obj);

                // Add the new object to the manager's data structure
                m_allObjects[obj->getType()].push_back(obj);
            }
        }
    }
}
