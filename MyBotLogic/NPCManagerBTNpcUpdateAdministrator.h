#ifndef NPCMANAGERBTNPCUPDATEADMINISTRATOR_H_INCLUDED
#define NPCMANAGERBTNPCUPDATEADMINISTRATOR_H_INCLUDED


#include "BehaviourTree/BehaviourTreeModule.h"

#include <vector>
#include <string>


class Npc;

/*
BT that updates NPCs. 
Just connect, disconnect a Npc bloc to decide if it will be updated. 
Switch a Npc bloc with another to change the update order.
*/
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

    /*Reassign the npc vector to the BT npc administator (in case a new npc appear and the vector resize and chage its address)*/
    void reassignNpcVectorArray(std::vector<Npc*>& npcVectorArray) noexcept;

    BehaviourTree::BlocRoot& getBTRoot() noexcept;

    template<class RootBase>
    RootBase* getBTRootAs() noexcept
    {
        static_assert(
            std::is_base_of<BehaviourTree::BlocComposite, RootBase>::value ||
            std::is_same<BehaviourTree::BlocComposite, RootBase>::value, 
            "Not a valid root casting");

        return m_behaviorTreeRoot.getRoot()->as<RootBase>();
    }


    /* Stop Npc */
    void stopANpcByName(const std::string& npcName);
    void stopANpcByID(unsigned int npcID);
    void stopANpcByIndex(size_t index);

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
    void swapNpcByName(const std::string& npc1Name, const std::string& npc2Name);
    void swapNpcByIndex(size_t indexNpc1, size_t indexNpc2);
    void swapNpcByIDs(unsigned int npc1ID, unsigned int npc2ID);

    /* get Npc Name */
    std::string getNpcNameByID(unsigned int NpcID);
    std::string getNpcNameByIndex(size_t NpcIndex) const noexcept;
    
    /* Get Npc Index */
    size_t getIndexByNpcName(const std::string& npcName);
    size_t getIndexByID(unsigned int npcID);

    /* Utilitary */
    bool npcByNameIsConnected(const std::string& npcName);
    bool npcByIndexIsConnected(size_t npcIndex);
    bool npcByIdIsConnected(unsigned int npcID);


private:
    BehaviourTree::BlocRef createNpcBloc(Npc* npc, const std::string& npcName);
    

public:
    void operator()();
};


#endif //NPCMANAGERBTNPCUPDATEADMINISTRATOR_H_INCLUDED