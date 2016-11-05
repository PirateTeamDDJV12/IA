#ifndef BTUTILS_H_INCLUDED
#define BTUTILS_H_INCLUDED

#include "../BehaviourTreeModule.h"

#include <type_traits>
#include <cassert>

namespace BehaviourTree
{
    /*
    Deprecated. Replaced as sometimes ago but doesn't works wery well... 
    I'll see later if I can fix it because I was planning to use it with the CRTP idiom
    */
    template<class BlocClass>
    class Instanciator
    {
    public:
        template<class Base>
        static BlocClass* instance(Base* base) { return base; }
    };

    /*
    General fabric structure. 
    Use it to create blocs...
    */
    struct BlocFabric
    {
        /************************************************************************/
        /*                                 Root bloc                            */
        /************************************************************************/

        /*
        Create a root as a composite bloc.
        Pass the type you want this root to be.
         
        ex : 
        BlocRoot root = BlocFabric::initiateRootAsCompositeBloc<BlocSequence>("my root name");
        */
        template<class BlocType>
        static BlocRoot initiateRootAsCompositeBloc(const std::string& name = "")
        {
            return BlocRoot(createCompositeBloc<BlocType>(name), name);
        }


        /*
        Create a root as a decorator bloc. 
        Pass the type you want this root to be.

        ex :
        BlocRoot root = BlocFabric::initiateRootAsDecoratorBloc<BlocInvert>("my root name");
        */
        template<class BlocType>
        static BlocRoot initiateRootAsDecoratorBloc(const BlocRef& child, const std::string& name = "")
        {
            return BlocRoot(createDecoratorBloc<BlocType>(child, name), name);
        }



        /************************************************************************/
        /*                                 Composite bloc                       */
        /************************************************************************/

        /*
        Create a composite bloc of BlocType. 
        Must be a child from composite bloc
        */
        template<class BlocType>
        static BlocRef createCompositeBloc(const std::string& blocName = "")
        {
            static_assert(std::is_convertible<BlocType*, BlocComposite*>::value, "Not a composite bloc");

            return BlocRef(new BlocType(blocName));
        }

        /*
        Create a general action bloc. Just specify its algorithm.

        ex (one from many others. You can do what you want with it):

        auto aGeneralcompositeBloc = BlocFabric::createGeneralAction(
        [](){
        //... Do something with children
        return general::result::SUCCESS;
        },
        "smart composite bloc name"
        );
        */
        template<class LambdaAction>
        static BlocRef createGeneralComposite(LambdaAction& compositeBehavior, const std::string& name = "")
        {
            return BlocRef(new GeneralBloc<LambdaAction, BlocComposite>(compositeBehavior, name));
        }



        /************************************************************************/
        /*                                 General action bloc                  */
        /************************************************************************/

        /*
        Create a general action bloc. Just specify its algorithm.

        ex (one from many others. You can do what you want with it):

        BlackBoard someBlackBoard;
        //...

        auto actionBloc = BlocFabric::createGeneralAction(
            [&someBlackBoard](){
                //...
                return general::result::SUCCESS;
            },
            "smart action bloc name"
        );
        */
        template<class LambdaAction>
        static BlocRef createGeneralAction(LambdaAction& actionBehavior, const std::string& name = "")
        {
            return BlocRef(new GeneralBloc<LambdaAction, BlocAction>(actionBehavior, name));
        }



        /************************************************************************/
        /*                                 Decorator bloc                       */
        /************************************************************************/
        
        /*
        Create a decorator bloc. 
        NB : a decorator bloc must have a child and it is MANDATORY...

        ex : 
        auto invertBloc = BlocFabric::createDecoratorBloc<BlocInvert>(
            BlocFabric::createGeneralAction([&someBlackBoard](){ return general::result::SUCCESS; },
            "smart decorator bloc name"
        );
        */
        template<class BlocType>
        static BlocRef createDecoratorBloc(const BlocRef& childBloc, const std::string& blocName = "")
        {
            static_assert(std::is_convertible<BlocType*, BlocDecorator*>::value, "Not a decorator bloc");

            return BlocRef(new BlocType(childBloc, blocName));
        }


        /*
        Create a general decorator bloc. Just specify its algorithm.

        ex (one from many others. You can do what you want with it):

        BlackBoard someBlackBoard;
        //...

        auto generalDecoratorBloc = BlocFabric::createGeneralDecorator(
        [](){
        //... do something with onlyChild;
        return general::result::SUCCESS;
        },
        BlocFabric::createGeneralAction([&someBlackBoard](){ return general::result::SUCCESS; },
        "smart decorator bloc name"
        );
        */
        template<class LambdaAction>
        static BlocRef createGeneralDecorator(LambdaAction& decoratorBehavior, const BlocRef& childBloc, const std::string& name = "")
        {
            return BlocRef(new GeneralBloc<LambdaAction, BlocDecorator>(decoratorBehavior, childBloc, name));
        }


        /*
        Create a general decorator bloc. Just specify its algorithm.

        See the other specification. 
        */
        template<class LambdaAction>
        static BlocRef createGeneralDecorator(LambdaAction& decoratorBehavior, BaseBloc* child, const std::string& name = "")
        {
            return BlocRef(new GeneralBloc<LambdaAction, BlocDecorator>(decoratorBehavior, child, name));
        }


        /************************************************************************/
        /*                                 Loop bloc                            */
        /************************************************************************/

        /*
        Create a loop bloc. Same as the createDecoratorBloc but it's specify the loop count
        */
        template<class BlocType>
        static BlocRef createLoopBloc(size_t loopCount, const BlocRef& childBloc, const std::string& blocName = "")
        {
            static_assert(std::is_convertible<BlocType, BlocLoop>::value, "It's not a child of BlocLoop or a BlocLoop");

            return BlocRef(new BlocType(childBloc, loopCount, blocName));
        }

        
        /*
        Create a loop bloc. Same as the createDecoratorBloc but it's specify the loop count
        */
        template<class BlocType>
        static BlocRef createLoopBloc(size_t loopCount, const BlocRef& childBloc, general::result resultOnBreak, const std::string& blocName = "")
        {
            static_assert(std::is_convertible<BlocType, BlocLoop>::value, "It's not a child of BlocLoop or a BlocLoop");

            return BlocRef(new BlocType(childBloc, loopCount, resultOnBreak, blocName));
        }
    };
}

#endif //!BTUTILS_H_INCLUDED