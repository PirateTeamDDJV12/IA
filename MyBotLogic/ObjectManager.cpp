#include "ObjectManager.h"

#include <sstream>
#include <algorithm>
#include "ZoneManager.h"
#include "Map.h"


ObjectManager ObjectManager::m_instance;


ObjectManager::ObjectManager()
{
    m_allObjects[Object::ObjectType::DOOR].reserve(MAX_OBJECTS);
    m_allObjects[Object::ObjectType::BUTTON].reserve(MAX_OBJECTS);
    m_allObjects[Object::ObjectType::PRESSURE_PLATE].reserve(MAX_OBJECTS);
}

ObjectManager* ObjectManager::get() noexcept
{
    return &m_instance;
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
    updateObjects(objects, tiles);
}

void ObjectManager::updateObjects(const std::map<unsigned int, ObjectInfo>& objects, const std::map<unsigned int, TileInfo>& tiles)
{
    for (auto curObject : objects)
    {
        for (auto curObjectType : curObject.second.objectTypes)
        {
            if (curObjectType == ObjectType_Door)
            {
                updateDoorObject(curObject.second);
            }
        }
    }

    for (auto curTile : tiles)
    {
        updatePressurePlateObject(curTile.second);
    }
}

const ObjectRef ObjectManager::getObjectById(Object::ObjectType type, size_t index) const
{
    for (auto iterPair : m_allObjects)
    {
        if (iterPair.first == type)
        {
            return iterPair.second[index];
        }
    }
    return ObjectRef();
}

std::vector<ObjectRef> ObjectManager::getAllObjectsOnTile(unsigned int tileId) const
{
    std::vector<ObjectRef> objectsOnTile;
    for (auto iterPair : m_allObjects)
    {
        for (auto curObject : iterPair.second)
        {
            if (curObject->getTileId() == tileId)
            {
                objectsOnTile.push_back(curObject);
            }
        }
    }
    return std::move(objectsOnTile);
}

const std::vector<ObjectRef>& ObjectManager::getAllObjectsByType(Object::ObjectType type) const
{
    return m_allObjects.at(type);
}

std::vector<ObjectRef> ObjectManager::getAllUnactivatedObjects() const
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
    return std::move(unactivatedObjects);
}

std::vector<ObjectRef> ObjectManager::getAllActivatedObjects() const
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
    return std::move(activatedObjects);
}

const std::map<Object::ObjectType, std::vector<ObjectRef>>& ObjectManager::getAllObjects() const noexcept
{
    return m_allObjects;
}

void ObjectManager::updateDoorObject(const ObjectInfo& objectInfo)
{
    // Check if the objet is already found
    for (auto curObject : m_allObjects[Object::ObjectType::DOOR])
    {
        if (curObject->getId() == objectInfo.objectID)
        {
            for(auto curState : objectInfo.objectStates)
            {
                switch(curState)
                {
                    case ObjectState_Opened:
                        curObject->setIsActive(true);
                        break;
                    case ObjectState_Closed:
                        curObject->setIsActive(false);
                        break;
                    default:
                        break;
                }
            }
            return;
        }
    }

    // Check if the door is activated (open)
    bool isActive = false;
    for (auto curState : objectInfo.objectStates)
    {
        switch (curState)
        {
        case ObjectState_Opened:
            isActive = true;
            break;
        default:
            isActive = false;
            break;
        }
    }

    ObjectRef obj{ new Object(objectInfo.objectID, objectInfo.tileID, Object::ObjectType::DOOR, isActive) };
    m_allObjects[obj->getType()].push_back(obj);

    Map* myMap = Map::get();
    unsigned int currentObjectZone = myMap->getNode(objectInfo.tileID)->getZone();
    unsigned int targetZone = 0;
    for(int i = NE; i <= NW; ++i)
    {
        if(objectInfo.edgesCost[i] == 0)
        {
            Node* neighbour = myMap->getNode(objectInfo.tileID)->getNeighboor(static_cast<EDirection>(i));
            targetZone = neighbour->getZone();
        }
    }
    if(currentObjectZone == targetZone || targetZone == 0)
    {
        return;
    }
    ZoneManager::get().addJunction(currentObjectZone, targetZone, obj);
}

void ObjectManager::updatePressurePlateObject(const TileInfo& tileInfo)
{
    // Check if the objet is already found
    for (auto curObject : m_allObjects[Object::ObjectType::PRESSURE_PLATE])
    {
        if (curObject->getId() == tileInfo.tileID)
        {
            std::vector<ObjectRef> &objects = curObject->getLinkedObjects();
            if(Map::get()->getNode(curObject->getTileId())->isTileOccupied())
            {
                for(auto object : objects)
                {
                    object->setIsActive(true);
                }
            }
            else
            {
                for(auto object : objects)
                {
                    object->setIsActive(true);
                }
            }
            return;
        }
    }

    for (auto curTileAttribute : tileInfo.tileAttributes)
    {
        if (curTileAttribute == TileAttribute_PressurePlate)
        {
            ObjectRef obj{ new Object(tileInfo.tileID, tileInfo.tileID, Object::ObjectType::PRESSURE_PLATE, true) };

            linkObjects(tileInfo, obj);
        }
    }
}

void ObjectManager::linkObjects(const TileInfo& tileInfo, ObjectRef obj)
{
    // Get all object that is a door
    auto doors = m_allObjects[Object::ObjectType::DOOR];

    // Finds the specific door linked to the pressure plate
    auto it = std::find_if(doors.begin(), doors.end(),
        [&tileInfo](ObjectRef it)->bool
        {
            return it->m_id == tileInfo.controlledDeviceID;
        }
    );

    // Link the pressure plate to the door
    obj->getLinkedObjects().push_back(*it._Ptr);

    // Link the door to the pressure plate
    (*it._Ptr)->getLinkedObjects().push_back(obj);

    // Add the new object to the manager's data structure
    m_allObjects[obj->getType()].push_back(obj);
}
