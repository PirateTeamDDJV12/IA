#ifndef BTMETHODS_H_INCLUDED
#define BTMETHODS_H_INCLUDED

#include "../BlocInstance/BaseBloc.h"

namespace BehaviourTree
{
    struct BlocMethods
    {
        /*invert the result of bloc execution*/
        static general::result invertResult(general::result input) noexcept
        {
            switch (input)
            {
            case BehaviourTree::general::FAIL:
                return BehaviourTree::general::SUCCESS;

            case BehaviourTree::general::SUCCESS:
                return BehaviourTree::general::FAIL;

            default:
                return input;
            }
        }
    };
}

#endif //!BTMETHODS_H_INCLUDED