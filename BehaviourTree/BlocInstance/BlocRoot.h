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
        {
            mRoot->m_idNode = 0;
            mRoot->m_lvlId = 0;
        }


    public:
        /*Get the real root bloc implemented below. Not really useful*/
        BaseBloc& getRoot() noexcept
        {
            return *mRoot;
        }


        virtual BlocRef child(size_t index)   noexcept
        {
            return mRoot->child(index);
        }

        virtual size_t nbChild()       const noexcept
        {
            return mRoot->nbChild();
        }

        virtual void connect(BlocRef& otherModule)
        {
            mRoot->connect(otherModule);
        }

        virtual void disconnect(size_t index)
        {
            mRoot->disconnect(index);
        }

        void disconnectByName(const std::string& name)
        {
            mRoot->disconnectByName(name);
        }


        general::type type() const noexcept
        {
            return mRoot->type();
        }

        virtual BaseBloc* find(size_t lvl, size_t id)
        {
            return mRoot->find(lvl, id);
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
        Deprecated. DO NOT USE IT FOR NOW
        A remaining method that remains from my own BT implementation.
        I haven't updated it in this BT refactoring situation, 
        but will be interested in doing so in the near future if time doesn't get in my way.
        It's the main reason this piece of code stays here until I update it correctly.

        Allows to navigate from the current bloc in the behaviour Tree by calling the indexes (from child to child...).
        */
        template<class ... IntegerIndex>
        BaseBloc* operator()(IntegerIndex ... indexes)
        {
            BaseBloc* iteratorMod = *mRoot;

            size_t argument[] = { indexes... };

            std::for_each(std::begin(argument),
                          std::end(argument),
                          [&iteratorMod](size_t index) {
                              iteratorMod = iteratorMod->child(index);
                          }
            );

            return iteratorMod;
        }
    };
}


#endif //!BLOCROOT_H