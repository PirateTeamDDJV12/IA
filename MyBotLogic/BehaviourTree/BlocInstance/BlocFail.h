#ifndef BLOCFAIL_H
#define BLOCFAIL_H

#include "BlocDecorator.h"

namespace BehaviourTree
{
    /*
    Decorator bloc.
    Bloc that executes its child and always returns false. Whatever the result of its child is
    */
    class BlocFail : public BlocDecorator
    {
    public:
        BlocFail(BaseBloc* child, const std::string& name = "") noexcept :
            BlocDecorator{ child, name }
        {}

        BlocFail(const BlocRef& child, const std::string& name = "") noexcept :
            BlocDecorator{ child, name }
        {}

        virtual general::result operator()()
        {
            onlyChild->operator()();
            return general::result::FAIL;
        }
    };
}


#endif //!BLOCFAIL_H
