#ifndef GENERALBLOC_H
#define GENERALBLOC_H


#ifndef BLOCACTION_H
#include "BlocAction.h"
#endif //!BLOCACTION_H

#ifndef BLOCCOMPOSITE_H
#include "BlocComposite.h"
#endif //!BLOCCOMPOSITE_H

#ifndef BLOCDECORATOR_H
#include "BlocDecorator.h"
#endif //!BLOCDECORATOR_H




namespace BehaviourTree
{
    template <class Algorithm>
    class BlocAlgorithm
    {
    protected:
        Algorithm m_functor;


    public:
        BlocAlgorithm() = delete;

        BlocAlgorithm(const Algorithm& functor) :
            m_functor{ functor }
        {}


    public:
        Algorithm& getAlgorithm() noexcept
        {
            return m_functor;
        }


    public:
        general::result operator()()
        {
            return m_functor();
        }
    };



    /*
    General bloc. Really really compliant. 
    Just specify the algorithm to use with the template part.
    Be it a lambda (with caption or not), a function pointer, a method pointer or a functor...
    Only 2 obligations :
    -> the algorithm must be :
    - parameterless (a functor must implement its operator()())
    - return a general::result

    THIS IS FOR HERITAGE PURPOSES. ABSTRACT CLASS
    */
    template<class Algoritm, class Base>
    class GeneralBloc : public Base, public BlocAlgorithm<Algoritm> {};


    template<class Algoritm>
    class GeneralBloc<Algoritm, BlocAction> : public BlocAction, public BlocAlgorithm<Algoritm>
    {
    public:
        GeneralBloc() = delete;

        GeneralBloc(const Algoritm& functor, const std::string& name = "") :
            BlocAlgorithm<Algoritm>(functor),
            BlocAction(name)
        {}


    public:
        virtual general::result operator()()
        {
            return BlocAlgorithm<Algoritm>::m_functor();
        }
    };

    template<class Algoritm>
    class GeneralBloc<Algoritm, BlocComposite> : public BlocComposite, public BlocAlgorithm<Algoritm>
    {
    public:
        GeneralBloc() = delete;

        GeneralBloc(const Algoritm& functor, const std::string& name = "") :
            BlocAlgorithm<Algoritm>(functor),
            BlocComposite(name)
        {}


    public:
        virtual general::result operator()()
        {
            return BlocAlgorithm<Algoritm>::m_functor();
        }
    };

    template<class Algoritm>
    class GeneralBloc<Algoritm, BlocDecorator> : public BlocDecorator, public BlocAlgorithm<Algoritm>
    {
    public:
        GeneralBloc() = delete;

        GeneralBloc(const Algoritm& functor, BaseBloc* child, const std::string& name = "") :
            BlocAlgorithm<Algoritm>(functor),
            BlocDecorator(child, name)
        {}

        GeneralBloc(const Algoritm& functor, const BlocRef& child, const std::string& name = "") :
            BlocAlgorithm<Algoritm>(functor),
            BlocDecorator(child, name)
        {}


    public:
        virtual general::result operator()()
        {
            return BlocAlgorithm<Algoritm>::m_functor();
        }
    };
}

#endif //!GENERALBLOC_H