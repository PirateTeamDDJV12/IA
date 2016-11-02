#ifndef BLOCSELECT_H
#define BLOCSELECT_H

#include "BlocComposite.h"

namespace BehaviourTree
{
    class BlocSelect : public BlocComposite
    {
	public:
		BlocSelect(const std::string& name = "") :
			BlocComposite{ name }
		{}


    public:
        virtual general::result operator()()
        {
            for(auto iter = children.begin(); iter != children.end(); ++iter)
            {
                if((*iter)->operator()() == general::result::SUCCESS)
                {
                    return general::result::SUCCESS;
                }
            }

            return general::result::FAIL;
        }

    };
}


#endif //!BLOCSELECT_H