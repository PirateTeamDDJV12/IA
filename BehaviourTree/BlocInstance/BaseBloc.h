#ifndef BASEBLOC_H
#define BASEBLOC_H

#include <string>
#include <memory>


namespace BehaviourTree
{
    /*Here are the main enums we find in the BT system*/
    namespace general
    {
        /*The result of the bloc execution...*/
        enum result
        {
            FAIL = 0,
            SUCCESS = 1,
            INPROGRESS = 2
        };

        /*The type of the bloc*/
        enum type
        {
            COMPOSITE,
            DECORATOR,
            ACTION
        };
    }


    class BaseBloc;

    /*
    A shared pointer that contains a bloc. 
    Blocs are dynamicaly created by new operator thus it is important to wipe our asses (delete) after playing with them.
    This mode of instanciation relieves us from this fact (because it does it automaticaly for us).
    */
    using BlocRef = std::shared_ptr<BaseBloc>;

    /*
    Weak pointer to a Base bloc.
    Could be useful for the incoming time.
    */
    using BlocWeakRef = std::weak_ptr<BaseBloc>;




    /*
    The basic behaviour tree bloc and the base of everything.
    The very heart of the behaviour tree
    ABSTRACT
    */
    class BaseBloc
    {
    public:
        size_t m_lvlId;
        size_t m_idNode;

        std::string m_name;


    protected:
        BaseBloc* mParent;


    public:
        BaseBloc(const std::string& blocName = "") :
            m_lvlId{},
            m_idNode{},
            m_name{ blocName }
        {}

        BaseBloc(size_t lvl, size_t id, const std::string& blocName = "") :
            m_lvlId{ lvl },
            m_idNode{ id },
            m_name{ blocName }
        {}


    public:
        /*
        return the child at index. 
        Works only for composite bloc.
        For decorator bloc, it returns its only child whatever index value is.
        For an action bloc, it return an empty BlocRef.
        TODO : some refactor using "as" methode. Maybe.
        */
        virtual BlocRef child(size_t index) noexcept = 0;

        /*
        Return the number of child this bloc currently has.
        Really interesting for a composite bloc since a decorator always returns 1 and an action returns 0
        */
        virtual size_t nbChild() const noexcept = 0;

        /*Returns the type of this bloc*/
        virtual general::type type() const noexcept = 0;

        /*
        Connect to a BlocRef. Do nothing with an action bloc.
        TODO : some refactor using "as" methode. Maybe.
        */
        virtual void connect(BlocRef& toConnect) = 0;

        /*
        Disconnect one of the bloc connected to this bloc instance.
        Does nothing in the case of an action or a decorator bloc.
        Use the index (same order as the execution order).
        Even if its works well, please, use disconnectByName method instead
        if you don't want to be lost in a huge tree...
        NB : It won't disconnect a child of a child...
        TODO : some refactor using "as" methode. Maybe.
        */
        virtual void disconnect(size_t iter) = 0;

        /*
        Disconnect one of the bloc connected to this bloc instance.
        Does nothing in the case of an action or a decorator bloc.
        Use the name of the bloc child. 
        Disconnect the first found so do not connect twins.
        NB : It won't disconnect a child of a child...
        TODO : some refactor using "as" methode. Maybe.
        */
        virtual void disconnectByName(const std::string& name) = 0;

        /*
        Returns the base bloc at the specified level and id.
        nullptr if not found.
        Works well with little BT, useless with huge one. 
        Please use findByName instead.

        NB : The find does not go backward in the BT.
        It will begin to search from the instance you called this method
        and search in every childs and childs of childs... but not parent.
        For maximum efficacity (comes with less performance), use it from a root
        */
        virtual BaseBloc* find(size_t lvl, size_t id) = 0;

        /*
        Returns the first base bloc with the specified name.
        nullptr if not found.
       
        NB : The find does not go backward in the BT.
        It will begin to search from the instance you called this method
        and search in every childs and childs of childs... but not parent.
        For maximum efficacity (comes with less performance), use it from a root
        */
        virtual BaseBloc* findByName(const std::string& name) = 0;

        /*
        Output parsing of the bloc... into a std string. 
        Use it for debug purposes for example
        */
        virtual std::string toStdString() const noexcept = 0;


    public:
        /*Get the parent. Not really useful but, well, if you find some uses...*/
        BlocRef parent() const noexcept { return BlocRef(mParent); }

        /*Get the name but because it is a public variable. Not really useful...*/
        const std::string& name() const noexcept { return m_name; }

        /*Set the name but because it is a public variable. Not really useful...*/
        void rename(const std::string& newName) noexcept { m_name = newName; }


        /*
        Do not use it. 
        Not really useful...
        It's for updating id and level purpose. 
        Buggy and since you use findByName instead... Don't mind it.
        */
        void learnParent(BaseBloc* toConn) noexcept
        {
            mParent = toConn;
            m_lvlId = toConn->m_lvlId + 1;
        }


    public:
        virtual general::result operator()() = 0;


    public:
        /*
        The method to cast this instance into the specified bloc type pointer.
        Useful to access specific method of specifics blocs types but do not use 
        it if you do not know its real type. Be afraid to cast something you shouldn't.

        TODO : compile time security check for this method (if convertible...)
        */
        template<class DerivatedBloc>
        DerivatedBloc* as() { return static_cast<DerivatedBloc*>(this); }
    };
}


#include "Other\BTDebugString.h"


#endif//!BASEBLOC_H