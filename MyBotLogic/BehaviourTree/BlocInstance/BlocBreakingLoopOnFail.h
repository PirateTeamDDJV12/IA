#ifndef BLOCBREAKINGLOOPONFAIL_H
#define BLOCBREAKINGLOOPONFAIL_H

#include "BlocLoop.h"


namespace BehaviourTree
{
    /*
    Decorator bloc.
    A loop bloc for executing a predefined number of time its connected childs
    Return the result of THE LAST execution of its child.
    */
    class BlocBreakingLoopOnFail : public BlocLoop
    {
    protected:
        general::result m_resultOnBreak;
        general::result m_resultOnExecutionEnd;


    public:
        BlocBreakingLoopOnFail(BaseBloc* child, size_t countLoop, general::result resultOnBreak, const std::string& name = "") noexcept :
            BlocLoop{ child, countLoop, name },
            m_resultOnBreak{ resultOnBreak },
            m_resultOnExecutionEnd(BlocMethods::invertResult(resultOnBreak))
        {}

        BlocBreakingLoopOnFail(const BlocRef& child, size_t countLoop, general::result resultOnBreak, const std::string& name = "") noexcept :
            BlocLoop{ child, countLoop, name },
            m_resultOnBreak{ resultOnBreak },
            m_resultOnExecutionEnd(BlocMethods::invertResult(resultOnBreak))
        {}

        virtual general::result operator()()
        {
            for (size_t iter = 0; iter < iterationCount; ++iter)
            {
                if (onlyChild->operator()() == general::result::FAIL)
                {
                    return m_resultOnBreak;
                }
            }

            return m_resultOnExecutionEnd;
        }
    };
}


#endif //!BLOCBREAKINGLOOPONFAIL_H

