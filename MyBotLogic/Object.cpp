#include "Object.h"

#include <sstream>


void Object::setObjectType(const std::set<EObjectType>& objectTypes)
{

    for (auto type : objectTypes)
    {
        switch (type)
        {
        case ObjectType_Door:
            m_type = PORTE;
            break;

            //TODO : ADD OTHER CASES FOR OBJECT TYPES WHEN THE INFORMATION WILL BE DELIVERED
        }
    }
}

void Object::setObjectState(const std::set<EObjectState>& objectStates)
{
    for (auto state : objectStates)
    {
        switch (state)
        {
        case ObjectState_Opened:
            m_isActive = true;
            break;

        case ObjectState_Closed:
            m_isActive = false;
            break;
        }
    }
}

void Object::toString(std::stringstream& sstream)
{
    sstream << "Object ID = " << m_id << "\n" <<
               "Object Type = " << m_type << "\n" <<
               "Object TileID = " << m_tileId << "\n" <<
               "Object Activated = " << m_isActive << "\n" <<
               "Object LinkedObjects = ";

    for (auto curObject : m_linkedObjects)
    {
        sstream << "\t" << curObject->getId() << "\n";
    }
    sstream << "\n";
}
