#ifndef BLOCCOMPOSITE_H
#define BLOCCOMPOSITE_H


#include "BaseBloc.h"

#include <algorithm>
#include <vector>

namespace BehaviourTree
{
    /*
    Base of composite bloc.
    Can have a lot of child, or no child.
    */
    class BlocComposite : public BaseBloc
    {
    protected:
        std::vector<BlocRef> children;


    public:
        BlocComposite(const std::string& name = "") :
            BaseBloc{ name }
        {}


    public:
        virtual BlocRef child(size_t index) noexcept
        {
            return children[index];
        }

        virtual size_t nbChild() const noexcept
        {
            return children.size();
        }

        virtual general::type type() const noexcept
        {
            return general::type::COMPOSITE;
        }

        virtual void connect(BlocRef& toConnect)
        {
            toConnect->m_idNode = this->children.size();
            this->children.push_back(BlocRef());
            this->children[toConnect->m_idNode] = toConnect;
            toConnect->learnParent(this);
        }

        virtual void disconnect(size_t iter)
        {
            children.erase(children.begin() + iter);

            iter = 0;

            std::for_each(
                children.begin(),
                children.end(),
                [&iter](BlocRef& bloc) {
                bloc->m_idNode = iter;
                ++iter;
            }
            );
        }

        virtual void disconnectByName(const std::string& name)
        {
            for (size_t iter = 0; iter < children.size(); ++iter)
            {
                if (children[iter]->m_name.size() == name.size() && children[iter]->m_name == name)
                {
                    children.erase(children.begin() + iter);
                }
            }
        }

        virtual BaseBloc* find(size_t lvl, size_t id)
        {
            if (lvl == m_lvlId && id == m_idNode)
            {
                return this;
            }

            for (auto iter = children.begin(); iter != children.end(); ++iter)
            {
                BaseBloc* result = iter->get()->find(lvl, id);

                if (result != nullptr)
                {
                    return result;
                }
            }

            return nullptr;
        }

        virtual BaseBloc* findByName(const std::string& name)
        {
            if (m_name.size() == name.size() && m_name == name)
            {
                return this;
            }

            for (auto iter = children.begin(); iter != children.end(); ++iter)
            {
                BaseBloc* result = iter->get()->findByName(name);

                if (result != nullptr)
                {
                    return result;
                }
            }

            return nullptr;
        }

        virtual std::string toStdString() const noexcept { return typeToStdString<general::type::COMPOSITE>() + m_name + "\n"; }


    public:
        /*
        Specific to composite bloc.
        Connect the specified toConnect bloc after the bloc (is one of the bloc DIRECTLY connected to this instance) specified
        by its name. If there is no bloc with the specified name. It connects the bloc at the end of the fray.
        */
        virtual void connectAfter(BlocRef& toConnect, const std::string& blocNameToConnectAfter)
        {
            for (auto iter = children.begin(); iter != children.end(); ++iter)
            {
                if ((*iter)->m_name.size() == blocNameToConnectAfter.size() && (*iter)->m_name == blocNameToConnectAfter)
                {
                    toConnect->m_idNode = this->children.size();
                    toConnect->learnParent(this);
                    this->children.insert(iter, toConnect);
                    return;
                }
            }

            this->connect(toConnect);
        }

    public:
        virtual general::result operator()() = 0;
    };
}


#endif//!BLOCCOMPOSITE_H