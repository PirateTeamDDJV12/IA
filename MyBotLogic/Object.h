#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "ObjectInfo.h"
#include "TileInfo.h"

#include <string>
#include <memory>


class Object;

using ObjectRef = std::shared_ptr<Object>;


class Object
{
    friend class ObjectManager;

public:
    enum ObjectType
    {
        DOOR,
        BUTTON,
        PRESSURE_PLATE
    };

private:
    unsigned int m_id;
    ObjectType m_type;
    unsigned int m_tileId;
    bool m_isActive;
    std::vector<ObjectRef> m_linkedObjects;

    //unsigned int m_activeTurnInfo; // 3;        // TODO WHEN INFORMATION WILL BE DELIVERED
    //unsigned int m_counter; // 3--;             // TODO WHEN INFORMATION WILL BE DELIVERED

public:
    Object() = delete;
    Object(unsigned int a_id, unsigned int a_tileId, ObjectType a_type, bool is_active);

public:
    // Getters
    unsigned int getId() const noexcept;
    ObjectType getType() const noexcept;
    unsigned int getTileId() const noexcept;
    bool isActive() const noexcept;
    std::vector<ObjectRef>& getLinkedObjects() noexcept;

    // Setter
    void setIsActive(bool isActive) noexcept;

    // For logger
    void toString(std::stringstream& sstream);
};

#endif // OBJECT_HEADER
