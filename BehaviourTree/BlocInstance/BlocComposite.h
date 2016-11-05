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
        /* return the child at specified index. */
        BlocRef child(size_t index) noexcept
        {
            return children[index];
        }

        /* Return the number of child (connection) this bloc currently has */
        size_t nbChild() const noexcept
        {
            return children.size();
        }

        virtual general::type type() const noexcept
        {
            return general::type::COMPOSITE;
        }

        /*Connect to a BlocRef.*/
        virtual void connect(BlocRef toConnect)
        {
            this->children.push_back(toConnect);
            //this->children[this->children.size() - 1] = toConnect;
            toConnect->learnParent(this);
        }

        /*
        Disconnect one of the bloc connected to this bloc instance.
        Use the index (same order as the execution order).
        Even if its works well, please, use disconnectByName method instead
        if you don't want to be lost in a huge tree...
        NB : It won't disconnect a child of a child...
        */
        virtual void disconnect(size_t iter)
        {
            children.erase(children.begin() + iter);
        }

        /*
        Disconnect one of the bloc connected to this bloc instance.
        Use the name of the bloc child.
        Disconnect the first found so do not connect twins.
        NB : It won't disconnect a child of a child...
        */
        virtual void disconnectByName(const std::string& name)
        {
            for (size_t iter = 0; iter < children.size(); ++iter)
            {
                if (children[iter]->m_name.size() == name.size() && children[iter]->m_name == name)
                {
                    this->disconnect(iter);
                }
            }
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

        virtual std::string toStdString() const noexcept 
        { 
            return typeToStdString<general::type::COMPOSITE>() + m_name + "\n"; 
        }


    public:
        /*
        Specific to composite bloc.
        Connect the specified toConnect bloc before the bloc (is one of the bloc DIRECTLY connected to this instance) specified
        by its name. If there is no bloc with the specified name. It connects the bloc at the end of the fray.
        */
        virtual void connectBefore(BlocRef& toConnect, const std::string& blocNameToConnectAfter)
        {
            for (auto iter = children.begin(); iter != children.end(); ++iter)
            {
                if ((*iter)->m_name.size() == blocNameToConnectAfter.size() && (*iter)->m_name == blocNameToConnectAfter)
                {
                    toConnect->learnParent(this);
                    this->children.insert(iter, toConnect);
                    return;
                }
            }

            this->connect(toConnect);
        }

        /*
        Specific to composite bloc.
        Connect the specified toConnect bloc after the bloc (is one of the bloc DIRECTLY connected to this instance) specified
        by its name. If there is no bloc with the specified name. It connects the bloc at the end of the fray.
        */
        virtual void connectAfter(BlocRef& toConnect, const std::string& blocNameToConnectAfter)
        {
            auto lastElement = children.end() - 1;

            for (auto iter = children.begin(); iter != lastElement; ++iter)
            {
                if ((*iter)->m_name.size() == blocNameToConnectAfter.size() && (*iter)->m_name == blocNameToConnectAfter)
                {
                    toConnect->learnParent(this);
                    this->children.insert(iter + 1, toConnect);
                    
                    return;
                }
            }

            this->connect(toConnect);
        }

        /* swap connected bloc with index. */
        virtual void swapBlocByIndex(size_t index1, size_t index2)
        {
            std::swap(children[index1], children[index2]);
        }

        /* 
        Swap connected bloc with their name
        Return true if the swap succeeded.
        False otherwise (if no bloc with the specified name are connected).
        */
        virtual bool swapBlocByName(const std::string& blocName1, const std::string& blocName2)
        {
            size_t index1; 
            size_t index2;

            if (findBothIndex2DirectChildByName(blocName1, index1, blocName2, index2))
            {
                std::swap(children[index1], children[index2]);
                return true;
            }

            return false;
        }

        /*
        Move the specified bloc given by bloc name to the last bloc position
        ->This bloc becomes the last to be executed since it will be moved to the end of the array
        */
        virtual void moveBlocAtEnd(const std::string& blocName)
        {
            size_t index1;

            if (findIndexDirectChildByName(blocName, index1))
            {
                this->connect(children[index1]);
                this->disconnect(index1);
            }
        }

        /*
        Move the specified bloc given by bloc name to the first bloc position
        ->This bloc becomes the first to be executed since it will be moved to the beginning of the array
        */
        virtual void moveBlocAtBeginning(const std::string& blocName)
        {
            size_t index1;

            if (findIndexDirectChildByName(blocName, index1))
            {
                this->children.insert(children.begin(), children[index1]);
                this->disconnect(index1 + 1);
            }
        }

        /*
        Move the specified bloc given by bloc name to the specified index position
        ->This bloc becomes the indexth to be executed since it will be moved to the indexth position of the array
        NB : The relative order of the other bloc won't change.
        */
        virtual void moveBlocAtIndex(const std::string& blocName, size_t index)
        {
            size_t index1;

            if (findIndexDirectChildByName(blocName, index1))
            {
                this->children.insert(children.begin() + index, children[index1]);
                this->disconnect((index1 < index) ? index1 : index1 + 1);
            }
        }

        /*
        Move the specified bloc given by bloc name just before the second bloc position
        NB : The relative order of the other bloc won't change.
        It return true if the move was successfully executed (both blocs were found)
        false otherwise
        */
        virtual bool moveBlocBefore(const std::string& blocNameToRearrange, const std::string& blocNameToBeAfter)
        {
            size_t index1;
            size_t index2;

            if (findBothIndex2DirectChildByName(blocNameToRearrange, index1, blocNameToBeAfter, index2))
            {
                this->connectBefore(children[index1], blocNameToBeAfter);
                this->disconnect((index1 < index2) ? index1 : index1 + 1);
                return true;
            }

            return false;
        }

        /*
        Move the specified bloc given by bloc name just after the second bloc position
        NB : The relative order of the other bloc won't change.
        It return true if the move was successfully executed (both blocs were found)
        false otherwise
        */
        virtual bool moveBlocAfter(const std::string& blocNameToRearrange, const std::string& blocNameToBeBefore)
        {
            size_t index1;
            size_t index2;

            if (findBothIndex2DirectChildByName(blocNameToRearrange, index1, blocNameToBeBefore, index2))
            {
                this->connectAfter(children[index1], blocNameToBeBefore);
                this->disconnect((index1 < index2) ? index1 : index1 + 1);
                return true;
            }

            return false;
        }

    public:
        virtual general::result operator()() = 0;


    protected:
        bool findIndexDirectChildByNameFrom(const std::string& blocName, size_t& result)
        {
            for (; result < children.size(); ++result)
            {
                if (children[result]->m_name.size() == blocName.size() && children[result]->m_name == blocName)
                {
                    return true;
                }
            }

            return false;
        }

        bool findIndexDirectChildByName(const std::string& blocName, size_t& result)
        {
            result = 0;
            return findIndexDirectChildByNameFrom(blocName, result);
        }

        bool findBothIndex2DirectChildByName(const std::string& blocName1, size_t& result1, const std::string& blocName2, size_t& result2)
        {
            result1 = 0;

            for (; result1 < children.size(); ++result1)
            {
                if (children[result1]->m_name.size() == blocName1.size() && children[result1]->m_name == blocName1)
                {
                    result2 = result1;
                    return findIndexDirectChildByNameFrom(blocName2, result2);
                }
                if (children[result1]->m_name.size() == blocName2.size() && children[result1]->m_name == blocName2)
                {
                    result2 = result1;
                    return findIndexDirectChildByNameFrom(blocName1, result1);
                }
            }

            return false;
        }
    };
}


#endif//!BLOCCOMPOSITE_H