#ifndef BLOCSEQUENCE_H
#define BLOCSEQUENCE_H

#include "BlocComposite.h"

namespace BehaviourTree
{
    /*
    Composite bloc
    Executes each one its childs in the order of its connections
    - Stops the loop if one of those execution returns fail -> return fail
    - If it has succeeded to run all its childs and no fail -> return success

    TODO : No InProgress taken into account. So must be implemented.... If needed.
    */
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
