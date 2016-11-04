#ifndef BLOCINVERT_H
#define BLOCINVERT_H

#include "BlocDecorator.h"

namespace BehaviourTree
{
    /*
    Decorator bloc.
    Bloc that executes its child and returns the inverse of the result of its child 
    (in case of success or fail only. The result as is if not).
    */
    class BlocInvert : public BlocDecorator
    {
    public:
        BlocInvert(BaseBloc* child, const std::string& name = "") noexcept :
            BlocDecorator{ child, name }
        {}

        BlocInvert(const BlocRef& child, const std::string& name = "") noexcept :
            BlocDecorator{ child, name }
        {}


        virtual general::result operator()()
        {
            general::result res = onlyChild->operator()();

            switch (res)
            {
            case BehaviourTree::general::FAIL:
                return BehaviourTree::general::SUCCESS;

            case BehaviourTree::general::SUCCESS:
                return BehaviourTree::general::FAIL;

            default:
                return res;
            }
        }
    };
}


#endif //!BLOCINVERT_H
