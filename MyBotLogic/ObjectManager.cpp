#include "ObjectManager.h"

ObjectManager ObjectManager::m_instance;

std::vector<Object> ObjectManager::getObjetsOnTile(unsigned tileId)
{
    std::vector<Object> v;
    for(std::pair<ObjectType, std::vector<Object>> objects : m_allObjects)
    {
        for(Object obj : objects.second)
        {
            if(obj.m_tileId == tileId)
            {
                v.push_back(obj);
            }
        }
    }

    return v;
}

void ObjectManager::setObjectOnTile(Object obj)
{
    m_allObjects[obj.m_type].push_back(obj);
}

std::vector<Object> ObjectManager::getAllObjects(ObjectType type)
{
    return m_allObjects[type];
}
