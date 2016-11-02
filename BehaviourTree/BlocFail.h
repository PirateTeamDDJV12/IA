#ifndef BLOCFAIL_H
#define BLOCFAIL_H

#include "BlocDecorator.h"

namespace BehaviourTree
{
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
