#ifndef BLOCSUCCESS_H
#define BLOCSUCCESS_H

#include "BlocDecorator.h"

namespace BehaviourTree
{
    /*
    Decorator bloc.
    Bloc that executes its child and always returns success. Whatever the result of its child is
    */
    class BlocSuccess : public BlocDecorator
    {
    public:
        BlocSuccess(BaseBloc* child, const std::string& name = "") noexcept :
            BlocDecorator{ child, name }
        {}

        BlocSuccess(const BlocRef& child, const std::string& name = "") noexcept :
            BlocDecorator{ child, name }
        {}

        virtual general::result operator()()
        {
            onlyChild->operator()();
            return general::result::SUCCESS;
        }
    };
}


#endif //!BLOCSUCCESS_H
