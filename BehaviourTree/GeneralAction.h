#ifndef GENERALACTION_H
#define GENERALACTION_H

#include "BlocAction.h"


namespace BehaviourTree
{
    /*
    General action. Really compliant. 
    Just specify the algorithm to use with the template part.
    Be it a lambda (with caption or not), a function pointer, a method pointer or a functor...
    Only 2 obligations :
    -> the algorithm must be :
    - parameterless (a functor must implement its operator()())
    - return a general::result
    */
    template<class Foncteur>
    class GeneralAction : public BlocAction
    {
    private:
        Foncteur functor;

    public:
        GeneralAction() = delete;

        GeneralAction(const Foncteur& functor, const std::string& name = "") :
            BlocAction{ name },
            functor{ functor }
        {}

        general::result operator()()
        {
            return functor();
        }
    };
}

#endif //!GENERALACTION_H