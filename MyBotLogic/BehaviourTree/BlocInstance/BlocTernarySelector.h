#ifndef BLOCTERNARYSELECTOR_H
#define BLOCTERNARYSELECTOR_H

#include "BlocComposite.h"
#include <assert.h>

namespace BehaviourTree
{
    /*
    Composite bloc
    Executes its second child if the first child returns success, the third child otherwise

    Don't support InProgress (due to time rushing)
    */
    class BlocTernarySelector : public BlocComposite
    {
    public:
        BlocTernarySelector(const std::string& name = "") :
            BlocComposite{ name }
        {
            children.reserve(3);
        }


    public:
        virtual general::result operator()()
        {
            if ((*children[0])() == general::result::SUCCESS)
            {
                return (*children[1])();
            }
            else
            {
                return (*children[2])();
            }
        }
    };
}


#endif //!BLOCTERNARYSELECTOR_H