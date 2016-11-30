#ifndef BLOCDECORATOR_H
#define BLOCDECORATOR_H


#include "BaseBloc.h"
#include "../Other/btMethods.h"

#include <algorithm>

namespace BehaviourTree
{
    /*
    Base for a decorator bloc.
    Has only one and only child (not many, not 0 -> cannot be disconnected).
    */
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
        /*Return the only child of this bloc*/
        BlocRef child() const noexcept
        {
            return onlyChild;
        }

        virtual general::type type() const noexcept
        {
            return general::type::DECORATOR;
        }

        /* Replace its only child */
        void connect(BlocRef& toConnect)
        {
            onlyChild = toConnect;
            toConnect->learnParent(this);
        }

        virtual BaseBloc* findByName(const std::string& name)
        {
            if (m_name.size() == name.size() && m_name == name)
            {
                return this;
            }

            return onlyChild->findByName(name);
        }

        virtual std::string toStdString() const noexcept 
        { 
            return DebugBT::typeToStdString<general::type::DECORATOR>() + m_name + "\n"; 
        }


    public:
        virtual general::result operator()() = 0;
    };
}


#endif//!BLOCDECORATOR_H
