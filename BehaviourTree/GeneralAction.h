#ifndef GENERALACTION_H
#define GENERALACTION_H

#include "BlocAction.h"


namespace BehaviourTree
{
	template<class Foncteur>
	class GeneralAction : public BlocAction
	{
	private:
		Foncteur functor;

	public:
        GeneralAction() = delete;

		GeneralAction(const Foncteur& functor, const std::string& name = "") :
			BlocAction{ name },
			functor{functor}
		{}

		general::result operator()()
		{
			return functor();
		}
	};
}

#endif //!GENERALACTION_H