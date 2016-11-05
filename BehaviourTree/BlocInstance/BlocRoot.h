#ifndef BLOCROOT_H
#define BLOCROOT_H


#include "BaseBloc.h"


#include <algorithm>


namespace BehaviourTree
{
    /*
    A huge behavior tree object.
    */
    class BlocRoot : public BaseBloc
    {
    private:
        BlocRef mRoot;


    public:
        BlocRoot(const BlocRef& root, const std::string& name = "") noexcept :
            BaseBloc{ name },
            mRoot{ root }
        {}


    public:
        /*Get the real root bloc implemented below. Not really useful*/
        BaseBloc* getRoot() noexcept
        {
            return &*mRoot;
        }


        general::type type() const noexcept
        {
            return mRoot->type();
        }

        virtual BaseBloc* findByName(const std::string& name)
        {
            return mRoot->findByName(name);
        }

        virtual std::string toStdString() const noexcept { return "rootBloc: " + m_name + "\n"; }


    public:
        general::result operator()()
        {
            return mRoot->operator()();
        }

        /*
        Allows to navigate from the current bloc in the behaviour Tree by calling the indexes (from child to child...).
        Hard to use because you must know very well your BT.
        */
        template<class ... IntegerIndex>
        BaseBloc* operator()(IntegerIndex ... indexes)
        {
            BaseBloc* iteratorMod = mRoot.get();

            size_t argument[] = { indexes... };

            std::for_each(std::begin(argument),
                          std::end(argument),
                          [&iteratorMod](size_t index) {
                              iteratorMod = iteratorMod->child(index).get();
                          }
            );

            return iteratorMod;
        }
    };
}


#endif //!BLOCROOT_H