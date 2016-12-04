#ifndef BLOCTERNARY_H
#define BLOCTERNARY_H

#include "BlocComposite.h"


namespace BehaviourTree
{
    /*
    Composite bloc
    Executes its second child if the first child returns success, the third child otherwise
    If the second children returns false then the third is called

    Don't support InProgress (due to time rushing)
    */
    class BlocTernary : public BlocComposite
    {
    public:
        BlocTernary(const std::string& name = "") :
            BlocComposite{ name }
        {
            children.reserve(3);
        }


    public:
        virtual general::result operator()()
        {
            if ((*children[0])() == general::result::SUCCESS)
            {
                if ((*children[1])() == general::result::FAIL)
                    return (*children[2])();
                return general::result::SUCCESS;
            }
            else
            {
                return (*children[2])();
            }
        }
    };
}


#endif //!BLOCTERNARY_H