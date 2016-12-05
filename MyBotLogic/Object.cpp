#include "Object.h"

#include <sstream>


Object::Object(unsigned int a_id, unsigned int a_tileId, ObjectType a_type, bool is_active) :
    m_id{ a_id },
    m_tileId{ a_tileId },
    m_type{ a_type },
    m_isActive{ is_active }
{}

unsigned int Object::getId() const
{
    return m_id;
}

Object::ObjectType Object::getType() const
{
    return m_type;
}

unsigned int Object::getTileId() const
{
    return m_tileId;
}

bool Object::isActive() const
{
    return m_isActive;
}

std::vector<ObjectRef>& Object::getLinkedObjects()
{
    return m_linkedObjects;
}

void Object::setIsActive(bool isActive)
{
    m_isActive = isActive;
}

void Object::toString(std::stringstream& sstream)
{
    std::string type;
    switch (m_type)
    {
    case DOOR:
        type = "DOOR";
        break;
    case BUTTON:
        type = "BUTTON";
        break;
    case PRESSURE_PLATE:
        type = "PRESSURE_PLATE";
        break;
    }

    sstream << "Object ID = " << m_id << "\n" <<
               "Object Type = " << type << "\n" <<
               "Object TileID = " << m_tileId << "\n" <<
               "Object Activated = " << (m_isActive ? "true" : "false") << "\n" <<
               "Object LinkedObjects = ";

    for (auto curObject : m_linkedObjects)
    {
        sstream << "\t" << curObject->getId() << "\n";
    }
    sstream << "\n";
}
