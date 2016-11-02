#ifndef BLOCDECORATOR_H
#define BLOCDECORATOR_H



#include "BaseBloc.h"

#include <algorithm>

namespace BehaviourTree
{
    class BlocDecorator : public BaseBloc
    {
    protected:
        BlocRef onlyChild;


    public:
        BlocDecorator() = delete;

        BlocDecorator(BaseBloc* child, const std::string& name = "") noexcept :
            BaseBloc{ name },
            onlyChild{ child }
        {}

        BlocDecorator(const BlocRef& child, const std::string& name = "") noexcept :
            BaseBloc{ name },
            onlyChild{ child }
        {}


    public:
        virtual BlocRef child(size_t index) noexcept
        {
            return onlyChild;
        }

        virtual size_t nbChild() const noexcept
        {
            return 1;
        }

        virtual general::type type() const noexcept
        {
            return general::type::DECORATOR;
        }

        void connect(BlocRef& toConnect)
        {
            onlyChild = toConnect;
            toConnect->learnParent(this);
            toConnect->m_idNode = 0;
        }

        void disconnect(size_t iter) {}

        void disconnectByName(const std::string& name) {}

        virtual BaseBloc* find(size_t lvl, size_t id)
        {
            if (lvl == BaseBloc::m_lvlId && id == BaseBloc::m_idNode)
            {
                return this;
            }

            return onlyChild->find(lvl, id);
        }

        virtual BaseBloc* findByName(const std::string& name)
        {
            if (m_name.size() == name.size() && m_name == name)
            {
                return this;
            }

            return onlyChild->findByName(name);
        }

        virtual std::string toStdString() const noexcept { return typeToStdString<general::type::DECORATOR>() + m_name + "\n"; }


    public:
        virtual general::result operator()() = 0;
    };
}


#endif//!BLOCDECORATOR_H
