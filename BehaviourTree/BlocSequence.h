#ifndef BLOCSEQUENCE_H
#define BLOCSEQUENCE_H

#include "BlocComposite.h"

namespace BehaviourTree
{
    class BlocSequence : public BlocComposite
    {
    public:
        BlocSequence(const std::string& name = "") :
            BlocComposite{ name }
        {}


    public:
        virtual general::result operator()()
        {
            for (auto iter = children.begin(); iter != children.end(); ++iter)
            {
                if ((*iter)->operator()() == general::result::FAIL)
                {
                    return general::result::FAIL;
                }
            }
            return general::result::SUCCESS;
        }
    };
}


#endif //!BLOCSEQUENCE_H
