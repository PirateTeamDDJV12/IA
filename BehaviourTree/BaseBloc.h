#ifndef BASEBLOC_H
#define BASEBLOC_H

#include <string>
#include <memory>


namespace BehaviourTree
{
    namespace general
    {
        enum result
        {
            FAIL = 0,
            SUCCESS = 1,
            INPROGRESS = 2
        };

        enum type
        {
            COMPOSITE,
            DECORATOR,
            ACTION
        };
    }
    

	class BaseBloc;
	using BlocRef = std::shared_ptr<BaseBloc>;
	using BlocWeakRef = std::weak_ptr<BaseBloc>;

    class BaseBloc
    {
	public:
        size_t m_lvlId;
        size_t m_idNode;
		
		std::string m_name;


	protected:
		BaseBloc* mParent;


    public :
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
        virtual BlocRef child(size_t index) noexcept = 0;
        virtual size_t nbChild() const noexcept = 0;
        
        virtual general::type type() const noexcept = 0;
        virtual void connect(BlocRef& toConnect) = 0;
        virtual void disconnect(size_t iter) = 0;
		virtual void disconnectByName(const std::string& name) = 0;

		virtual BaseBloc* find(size_t lvl, size_t id) = 0;
		virtual BaseBloc* findByName(const std::string& name) = 0;

		virtual std::string toStdString() const noexcept = 0;


	public:
		BlocRef parent() const noexcept { return BlocRef(mParent); }

		const std::string& name() const noexcept { return m_name; }
		void rename(const std::string& newName) noexcept { m_name = newName; }

		void learnParent(BaseBloc* toConn) noexcept
		{
			mParent = toConn;
			m_lvlId = toConn->m_lvlId + 1;
		}


	public:
		virtual general::result operator()() = 0;


	public:
		template<class DerivatedBloc>
		DerivatedBloc* as() { return static_cast<DerivatedBloc*>(this); }
    };
}


#include "Other\BTDebugString.h"


#endif//!BASEBLOC_H