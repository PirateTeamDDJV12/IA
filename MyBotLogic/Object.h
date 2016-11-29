#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "ObjectInfo.h"
#include "TileInfo.h"

#include <string>
#include <memory>


class Object
{
    friend class ObjectManager;

public:
    enum ObjectType
    {
        PORTE,
        BOUTON,
        PRESSURE_PLATE
    };

private:
    unsigned int m_id;
    ObjectType m_type;
    unsigned int m_tileId;
    bool m_isActive;
    std::vector<std::shared_ptr<Object>> m_linkedObjects;

    //unsigned int m_activeTurnInfo; // 3;        // TODO WHEN INFORMATION WILL BE DELIVERED
    //unsigned int m_counter; // 3--;             // TODO WHEN INFORMATION WILL BE DELIVERED

public:
    Object() = delete;
    Object(unsigned int a_id, unsigned int a_tileId, ObjectType a_type, bool is_active) :
        m_id{ a_id },
        m_tileId{ a_tileId },
        m_type{ a_type },
        m_isActive{ is_active }
    {}

public:
    // Getters
    unsigned int getId() const { return m_id; }
    ObjectType getType() const { return m_type; }
    unsigned int getTileId() const { return m_tileId; }
    bool isActive() const { return m_isActive; }
    std::vector<std::shared_ptr<Object>>& getLinkedObjects() { return m_linkedObjects; }

    // Setter
    void setIsActive(bool isActive) { m_isActive = isActive; }

    // For logger
    void toString(std::stringstream& sstream);
};

using ObjectRef = std::shared_ptr<Object>;

#endif // OBJECT_HEADER
