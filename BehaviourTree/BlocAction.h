#ifndef BLOCACTION_H
#define BLOCACTION_H


#include "BaseBloc.h"

#include <algorithm>

namespace BehaviourTree
{
    class BlocAction : public BaseBloc
    {
    public:
        BlocAction(const std::string& name = "") :
            BaseBloc{ name }
        {}


    public:
        virtual BlocRef child(size_t index) noexcept
        {
            return BlocRef();
        }

        virtual size_t nbChild() const noexcept
        {
            return 0;
        }

        virtual general::type type() const noexcept
        {
            return general::type::ACTION;
        }

        void connect(BlocRef& toConnect) {}

        void disconnect(size_t iter) {}

        void disconnectByName(const std::string& name) {}

        virtual BaseBloc* find(size_t lvl, size_t id)
        {
            if (lvl == BaseBloc::m_lvlId && id == BaseBloc::m_idNode)
            {
                return this;
            }

            return nullptr;
        }

        virtual BaseBloc* findByName(const std::string& name)
        {
            if (m_name.size() == name.size() && m_name == name)
            {
                return this;
            }

            return nullptr;
        }

        virtual std::string toStdString() const noexcept { return typeToStdString<general::type::ACTION>() + m_name + "\n"; }

        virtual general::result operator()() = 0;
    };
}


#endif//!BLOCACTION_H

