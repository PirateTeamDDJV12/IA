#ifndef BLOCSELECT_H
#define BLOCSELECT_H

#include "BlocComposite.h"

namespace BehaviourTree
{
    /*
    Composite bloc
    Executes each one its childs in the order of its connections
    - Stops the loop if one of those execution returns success -> return success
    - If it has succeeded to run all its childs and no success -> return false

    TODO : No InProgress taken into account. So must be implemented.... If needed.
    */
    class BlocSelect : public BlocComposite
    {
    public:
        BlocSelect(const std::string& name = "") :
            BlocComposite{ name }
        {}


    public:
        virtual general::result operator()()
        {
            for (auto iter = children.begin(); iter != children.end(); ++iter)
            {
                if ((*iter)->operator()() == general::result::SUCCESS)
                {
                    return general::result::SUCCESS;
                }
            }

            return general::result::FAIL;
        }

    };
}


#endif //!BLOCSELECT_H