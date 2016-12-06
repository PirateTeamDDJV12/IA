#include "NPCManagerBTNpcUpdateAdministrator.h"
#include "Npc.h"

#include <algorithm>

using namespace BehaviourTree;


void NPCManagerBTNpcUpdateAdministrator::init()
{
    BlocSequence* sequenceRoot = this->getBTRootAs<BlocSequence>();
    
    std::for_each(
        m_npcs->begin(),
        m_npcs->end(),
        [&](Npc* npc) {
            sequenceRoot->connect( this->createNpcBloc(npc, this->getNpcNameByIndex(npc->getId())));
        }
    );
}

void NPCManagerBTNpcUpdateAdministrator::reassignNpcVectorArray(std::vector<Npc*>& npcVectorArray) noexcept
{
    m_npcs = &npcVectorArray;
}

BehaviourTree::BlocRoot& NPCManagerBTNpcUpdateAdministrator::getBTRoot() noexcept
{
    return m_behaviorTreeRoot;
}

BlocRef NPCManagerBTNpcUpdateAdministrator::createNpcBloc(Npc* npc, const std::string& npcName)
{
    return BlocFabric::createGeneralAction(
                [npc]() {
                    npc->update();
                    return general::result::SUCCESS;
                },
                npcName
            );
}

void NPCManagerBTNpcUpdateAdministrator::operator()()
{
    m_behaviorTreeRoot();
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTNpcUpdateAdministrator::stopANpcByIndex(size_t index)
{
    this->stopANpcByName(this->getNpcNameByIndex(index));
}

void NPCManagerBTNpcUpdateAdministrator::stopANpcByID(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));

    if (!npcName.empty())
    {
        this->stopANpcByName(npcName);
    }
}

void NPCManagerBTNpcUpdateAdministrator::stopANpcByName(const std::string& npcName)
{
    this->getBTRootAs<BehaviourTree::BlocComposite>()->disconnectByName(npcName);
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTNpcUpdateAdministrator::restartANpcByID(unsigned int NpcID, const std::string& npcNameBefore)
{
    if (!npcNameBefore.empty())
    {
        std::string npcName = std::move(this->getNpcNameByID(NpcID));

        if (!npcName.empty())
        {
            size_t npcIndex = this->getIndexByNpcName(npcName);

            if (npcIndex < m_npcs->size())
            {
                this->getBTRootAs<BlocComposite>()->connectAfter(this->createNpcBloc((*m_npcs)[npcIndex], npcName), npcNameBefore);
            }
        }
    }
}

void NPCManagerBTNpcUpdateAdministrator::restartANpcByIndex(size_t npcIndex, const std::string& npcNameBefore)
{
    if (npcIndex < m_npcs->size() && this->getIndexByNpcName(npcNameBefore) < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connectAfter(
            this->createNpcBloc((*m_npcs)[npcIndex], this->getNpcNameByIndex(npcIndex)),
            npcNameBefore
        );
    }    
}


//////////////////////////////////////////////////////////////////////////


std::string NPCManagerBTNpcUpdateAdministrator::getNpcNameByID(unsigned int npcID)
{
    size_t npcsCount = m_npcs->size();
    for (size_t iter = 0; iter < npcsCount; ++iter)
    {
        if (m_npcs->operator[](iter)->getId() == npcID)
        {
            return std::move(this->getNpcNameByIndex(iter));
        }
    }

    return "";
}

size_t NPCManagerBTNpcUpdateAdministrator::getIndexByNpcName(const std::string& npcName)
{
    if (npcName.length() > 2)
    {
        size_t unit = static_cast<size_t>(npcName.back() - NPCManagerBTNpcUpdateAdministrator::CHAR_TO_NUMBER_ASCII);
        size_t digit = static_cast<size_t>(npcName[npcName.length() - 2] - NPCManagerBTNpcUpdateAdministrator::CHAR_TO_NUMBER_ASCII);

        return digit * 10 + unit;
    }
    
    return NPCManagerBTNpcUpdateAdministrator::NO_INDEX;
}

std::string NPCManagerBTNpcUpdateAdministrator::getNpcNameByIndex(size_t NpcIndex) const noexcept
{
    return std::move("Npc" + std::to_string(NpcIndex));
}

size_t NPCManagerBTNpcUpdateAdministrator::getIndexByID(unsigned int npcID)
{
    size_t npcCount = m_npcs->size();
    for (size_t iter = 0; iter < npcCount; ++npcCount)
    {
        if (m_npcs->operator[](iter)->getId() == npcID)
        {
            return iter;
        }
    }
    return NPCManagerBTNpcUpdateAdministrator::NO_INDEX;
}


//////////////////////////////////////////////////////////////////////////

void NPCManagerBTNpcUpdateAdministrator::swapNpcByName(const std::string& npc1Name, const std::string& npc2Name)
{
    this->getBTRootAs<BehaviourTree::BlocComposite>()->swapBlocByName(npc1Name, npc2Name);
}

void NPCManagerBTNpcUpdateAdministrator::swapNpcByIndex(size_t indexNpc1, size_t indexNpc2)
{
    if (indexNpc1 < m_npcs->size() && indexNpc2 < m_npcs->size())
    {
        this->swapNpcByName(this->getNpcNameByIndex(indexNpc1), this->getNpcNameByIndex(indexNpc2));
    }
}

void NPCManagerBTNpcUpdateAdministrator::swapNpcByIDs(unsigned int npc1ID, unsigned int npc2ID)
{
    std::string npc1Name = std::move(this->getNpcNameByID(npc1ID));

    if (!npc1Name.empty())
    {
        std::string npc2Name = std::move(this->getNpcNameByID(npc2ID));

        if (!npc2Name.empty())
        {
            this->swapNpcByName(npc1Name, npc2Name);
        }
    }
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTNpcUpdateAdministrator::restartANpcByNameAtBegin(const std::string& npcName)
{
    size_t npcIndex = this->getIndexByNpcName(npcName);

    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connectAtBeginning(createNpcBloc((*m_npcs)[npcIndex], npcName));
    }
}

void NPCManagerBTNpcUpdateAdministrator::restartANpcByIDAtBegin(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));
    if (!npcName.empty())
    {
        this->restartANpcByNameAtBegin(npcName);
    }
}

void NPCManagerBTNpcUpdateAdministrator::restartANpcByIndexAtBegin(size_t npcIndex)
{
    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connectAtBeginning(createNpcBloc((*m_npcs)[npcIndex], this->getNpcNameByIndex(npcIndex)));
    }
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTNpcUpdateAdministrator::restartANpcByNameAtEnd(const std::string& npcName)
{
    size_t npcIndex = this->getIndexByNpcName(npcName);

    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connect(createNpcBloc((*m_npcs)[npcIndex], npcName));
    }
}

void NPCManagerBTNpcUpdateAdministrator::restartANpcByIDAtEnd(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));

    if (!npcName.empty())
    {
        this->restartANpcByNameAtEnd(npcName);
    }
}

void NPCManagerBTNpcUpdateAdministrator::restartANpcByIndexAtEnd(size_t npcIndex)
{
    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connect(createNpcBloc((*m_npcs)[npcIndex], this->getNpcNameByIndex(npcIndex)));
    }
}


//////////////////////////////////////////////////////////////////////////


bool NPCManagerBTNpcUpdateAdministrator::npcByNameIsConnected(const std::string& npcName)
{
    return this->getBTRootAs<BehaviourTree::BlocComposite>()->findByName(npcName) != nullptr;
}

bool NPCManagerBTNpcUpdateAdministrator::npcByIndexIsConnected(size_t npcIndex)
{
    return npcIndex < m_npcs->size() ? npcByNameIsConnected(this->getNpcNameByIndex(npcIndex)) : false;
}

bool NPCManagerBTNpcUpdateAdministrator::npcByIdIsConnected(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));
    return npcName != "" ? npcByNameIsConnected(npcName) : false;
}