#ifndef GENERALACTION_H
#define GENERALACTION_H


#ifndef BLOCACTION_H
#include "BlocAction.h"
#endif //!BLOCACTION_H



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


    template<class Algoritm>
    class GeneralAction : public BlocAction, public BlocAlgorithm<Algoritm>
    {
    public:
        GeneralAction() = delete;

        GeneralAction(const Algoritm& functor, const std::string& name = "") :
            BlocAlgorithm<Algoritm>(functor),
            BlocAction(name)
        {}


    public:
        virtual general::result operator()()
        {
            return BlocAlgorithm<Algoritm>::m_functor();
        }
    };
}

#endif //!GENERALACTION_H