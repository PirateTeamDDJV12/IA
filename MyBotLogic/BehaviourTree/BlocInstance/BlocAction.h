#ifndef BLOCACTION_H
#define BLOCACTION_H


#include "BaseBloc.h"

#include <algorithm>

namespace BehaviourTree
{
    /*
    Base of action bloc.
    Specify an action bloc. 
    Thus, it doesn't have childs and is the only one allowed to know of a blackboard and 
    to play with the environnement outside of the BT.


    2 choices :

    - You don't want to storms your brain too much and only considers the executed algorithm to be 
    the only changes from an action to another -> use GeneralAction object

    - You want to make your own action, with custom members you want to control and change outside 
    of the algorithm execution... And certainly many other reasons (justified or not)...
    -> Create your own custom action bloc -> derivate it from this action bloc...
    */
    class BlocAction : public BaseBloc
    {
    public:
        BlocAction(const std::string& name = "") :
            BaseBloc{ name }
        {}


    public:
        virtual general::type type() const noexcept
        {
            return general::type::ACTION;
        }

        virtual BaseBloc* findByName(const std::string& name)
        {
            if (m_name.size() == name.size() && m_name == name)
            {
                return this;
            }

            return nullptr;
        }

        virtual std::string toStdString() const noexcept 
        { 
            return DebugBT::typeToStdString<general::type::ACTION>() + m_name + "\n"; 
        }

        virtual general::result operator()() = 0;
    };
}


#endif//!BLOCACTION_H

