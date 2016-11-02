#ifndef BTUTILS_H_INCLUDED
#define BTUTILS_H_INCLUDED

#include "../BehaviourTreeModule.h"

#include <type_traits>
#include <cassert>

namespace BehaviourTree
{
    template<class BlocClass>
    class Instanciator
    {
    public:
        template<class Base>
        static BlocClass* instance(Base* base) { return base; }
    };


    struct BlocFabric
    {
        /************************************************************************/
        /*                                 Root bloc                            */
        /************************************************************************/
        template<class BlocType>
        static BlocRoot initiateRootAsCompositeBloc(const std::string& name = "")
        {
            return BlocRoot(createCompositeBloc<BlocType>(name), name);
        }

        template<class BlocType>
        static BlocRoot initiateRootAsDecoratorBloc(const BlocRef& child, const std::string& name = "")
        {
            return BlocRoot(createDecoratorBloc<BlocType>(child, name), name);
        }



        /************************************************************************/
        /*                                 Composite bloc                       */
        /************************************************************************/
        template<class BlocType>
        static BlocRef createCompositeBloc(const std::string& blocName = "")
        {
            static_assert(std::is_convertible<BlocType*, BlocComposite*>::value, "Not a composite bloc");

            return BlocRef(new BlocType(blocName));
        }



        /************************************************************************/
        /*                                 General action bloc                  */
        /************************************************************************/
        template<class LambdaAction>
        static BlocRef createGeneralAction(LambdaAction& actionBehavior, const std::string& name = "")
        {
            return BlocRef(new GeneralAction<LambdaAction>(actionBehavior, name));
        }



        /************************************************************************/
        /*                                 Decorator bloc                       */
        /************************************************************************/
        template<class BlocType>
        static BlocRef createDecoratorBloc(const BlocRef& childBloc, const std::string& blocName = "")
        {
            static_assert(std::is_convertible<BlocType*, BlocDecorator*>::value, "Not a decorator bloc");

            return BlocRef(new BlocType(childBloc, blocName));
        }



        /************************************************************************/
        /*                                 Loop bloc                            */
        /************************************************************************/
        template<class BlocType>
        static BlocRef createLoopBloc(size_t loopCount, const BlocRef& childBloc, const std::string& blocName = "");

        template<>
        static BlocRef createLoopBloc<BlocLoop>(size_t loopCount, const BlocRef& childBloc, const std::string& blocName)
        {
            return BlocRef(new BlocLoop(childBloc, loopCount, blocName));
        }
    };
}

#endif //!BTUTILS_H_INCLUDED