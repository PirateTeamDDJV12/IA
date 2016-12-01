#ifndef NPCMANAGERBTNPCUPDATEADMINISTRATOR_H_INCLUDED
#define NPCMANAGERBTNPCUPDATEADMINISTRATOR_H_INCLUDED


#include "BehaviourTree/BehaviourTreeModule.h"

#include <vector>
#include <string>


class Npc;

class NPCManagerBTNpcUpdateAdministrator
{
public:
    enum
    {
        NO_INDEX = 0xFFFFFFFF
    };

private:
    enum
    {
        CHAR_TO_NUMBER_ASCII = 48
    };


private:
    BehaviourTree::BlocRoot m_behaviorTreeRoot;

    std::vector<Npc*>* m_npcs;


public:
    NPCManagerBTNpcUpdateAdministrator() :
        m_behaviorTreeRoot{ BehaviourTree::BlocFabric::initiateRootAsCompositeBloc<BehaviourTree::BlocSequence>("UpdateNpcManagerRoot") }
    {}


public:
    void init();

    void reassignNpcVectorArray(std::vector<Npc*>& npcVectorArray)
    {
        m_npcs = &npcVectorArray;
    }

    BehaviourTree::BlocRoot& getBTRoot() noexcept
    {
        return m_behaviorTreeRoot;
    }

    template<class RootBase>
    RootBase* getBTRootAs() noexcept
    {
        static_assert(
            std::is_base_of<BehaviourTree::BlocComposite, RootBase>::value ||
            std::is_same<BehaviourTree::BlocComposite, RootBase>::value
            , "Not a valid root casting");

        return m_behaviorTreeRoot.getRoot()->as<RootBase>();
    }


    /* Stop Npc */
    void stopANpcByName(const std::string& npcName)
    {
        this->getBTRootAs<BehaviourTree::BlocComposite>()->disconnectByName(npcName);
    }

    void stopANpcByID(unsigned int npcID);
    void stopANpcByIndex(size_t index)
    {
        this->stopANpcByName(this->getNpcNameByIndex(index));
    }

    /* Restart Npc */
    void restartANpcByID(unsigned int npcIndex, const std::string& npcNameBefore);
    void restartANpcByIndex(size_t NpcIndex, const std::string& npcNameBefore);

    /* Restart Npc at beginning */
    void restartANpcByNameAtBegin(const std::string& npcName);
    void restartANpcByIDAtBegin(unsigned int npcID);
    void restartANpcByIndexAtBegin(size_t index);

    /* Restart Npc at ending */
    void restartANpcByNameAtEnd(const std::string& npcName);
    void restartANpcByIDAtEnd(unsigned int npcID);
    void restartANpcByIndexAtEnd(size_t index);

    /* Swap Npc */
    void swapNpcByName(const std::string& npc1Name, const std::string& npc2Name)
    {
        this->getBTRootAs<BehaviourTree::BlocComposite>()->swapBlocByName(npc1Name, npc2Name);
    }

    void swapNpcByIndex(size_t indexNpc1, size_t indexNpc2)
    {
        if (indexNpc1 < m_npcs->size() && indexNpc2 < m_npcs->size())
        {
            this->swapNpcByName(this->getNpcNameByIndex(indexNpc1), this->getNpcNameByIndex(indexNpc2));
        }
    }

    void swapNpcByIDs(unsigned int npc1ID, unsigned int npc2ID);

    /* get Npc Name */
    std::string getNpcNameByID(unsigned int NpcID);
    std::string getNpcNameByIndex(size_t NpcIndex) const noexcept
    {
        return "Npc" + std::to_string(NpcIndex);
    }
    
    /* Get Npc Index (from its name) */
    size_t getIndexByNpcName(const std::string& npcName);


    /* Utilitary */
    bool npcByNameIsConnected(const std::string& npcName)
    {
        return this->getBTRootAs<BehaviourTree::BlocComposite>()->findByName(npcName) != nullptr;
    }

    bool npcByIndexIsConnected(size_t npcIndex)
    {
        return npcIndex < m_npcs->size() ? npcByNameIsConnected(this->getNpcNameByIndex(npcIndex)) : false;
    }

    bool npcByIdIsConnected(unsigned int npcID)
    {
        std::string npcName = std::move(this->getNpcNameByID(npcID));
        return npcName != "" ? npcByNameIsConnected(npcName) : false;
    }

private:
    BehaviourTree::BlocRef createNpcBloc(Npc* npc, const std::string& npcName);
    

public:
    void operator()() 
    {
        m_behaviorTreeRoot();
    }
};

#endif //NPCMANAGERBTNPCUPDATEADMINISTRATOR_H_INCLUDED