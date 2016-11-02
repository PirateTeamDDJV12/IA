#ifndef BLOCLOOP_H
#define BLOCLOOP_H

#include "BlocDecorator.h"

namespace BehaviourTree
{
    class BlocLoop : public BlocDecorator
    {
    protected:
        size_t iterationCount;


    public:
        BlocLoop(BaseBloc* child, size_t countLoop, const std::string& name = "") noexcept :
            BlocDecorator{ child, name },
            iterationCount{ countLoop }
        {}

        BlocLoop(const BlocRef& child, size_t countLoop, const std::string& name = "") noexcept :
            BlocDecorator{ child, name },
            iterationCount{ countLoop }
        {}

        size_t getIterationCount() const noexcept
        {
            return iterationCount;
        }

        void setIterationCount(size_t newCount) noexcept
        {
            iterationCount = newCount;
        }

        virtual general::result operator()()
        {
            general::result state;

            for (size_t iter = 0; iter < iterationCount; ++iter)
            {
                state = onlyChild->operator()();
            }

            return state;
        }
    };
}


#endif //!BLOCLOOP_H

