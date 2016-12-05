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
        std::string m_name;


    protected:
        BaseBloc* mParent;


    public:
        BaseBloc(const std::string& blocName = "") :
            m_name{ blocName }
        {}


    public:
        /*Returns the inherent type of this bloc*/
        virtual general::type type() const noexcept = 0;


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
        }


    public:
        virtual general::result operator()() = 0;


    public:
        /*
        The method to cast this instance into the specified bloc type pointer.
        Useful to access specific method of specifics blocs types but do not use 
        it if you do not know its real type. Be afraid to cast something you shouldn't.
        */
        template<class DerivatedBloc>
        DerivatedBloc* as() 
        {
            static_assert(std::is_base_of<BaseBloc, DerivatedBloc>::value, "the specified type is not a base from BaseBloc");
            return static_cast<DerivatedBloc*>(this); 
        }

        /*
        The method to cast this instance into the specified bloc type pointer.
        Useful to access specific method of specifics blocs types.

        RETURN nullptr if you want to cast into the wrong type.
        Done at Runtime. If you know what it is, use simple as<T> method instead.
        */
        template<class DerivatedBloc>
        DerivatedBloc* getAs()
        {
            return dynamic_cast<DerivatedBloc*>(this);
        }

        /*
        The method that do findByName(name) and as<DerivatedBloc>() methods into one expression.
        */
        template<class DerivatedBloc>
        DerivatedBloc* findByName(const std::string& name)
        {
            return findByName(name)->as<DerivatedBloc>();
        }

        /*
        The method to cast this instance into the specified bloc type pointer.
        Useful to access specific method of specifics blocs types.

        RETURN nullptr if you want to cast into the wrong type.
        Done at Runtime. If you know what it is, use simple as<T> method instead.
        */
        template<class DerivatedBloc>
        DerivatedBloc* getByName(const std::string& name)
        {
            return findByName(name)->getAs<DerivatedBloc>();
        }
    };
}


#include "..\Other\BTDebugString.h"


#endif//!BASEBLOC_H