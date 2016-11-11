#ifndef OBJECT_MANAGER_HEADER
#define OBJECT_MANAGER_HEADER
#include "Singleton.h"
#include <vector>
#include <map>

enum ObjectType
{
    PORTE,
    BOUTON,
    SENTINELLE,
};

struct Object
{
    unsigned int m_tileId;
    ObjectType m_type;
    bool m_isActive;
    Object* m_linkObject;
    unsigned int m_activeTurnInfo; // 3;
    unsigned int m_counter; // 3--;
};

class ObjectManager : public Singleton
{
private:
    std::map<ObjectType, std::vector<Object>> m_allObjects;

    static ObjectManager m_instance;
    ObjectManager()
    {}
public:
    static ObjectManager* get()
    {
        return &m_instance;
    }
    std::vector<Object> getObjetsOnTile(unsigned tileId);
    void setObjectOnTile(Object obj);
    std::vector<Object> getAllObjects(ObjectType type);
};

#endif // OBJECT_MANAGER_HEADER