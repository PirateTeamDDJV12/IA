#include "Object.h"

#include <sstream>


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
