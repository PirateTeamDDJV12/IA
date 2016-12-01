#include "NPCManagerBTNpcUpdateAdministrator.h"
#include "Npc.h"

#include <algorithm>

using namespace BehaviourTree;


void NPCManagerBTNpcUpdateAdministrator::init()
{
    BlocSequence* sequenceRoot = this->getBTRootAs<BlocSequence>();

    size_t iter = 0;

    std::for_each(
        m_npcs->begin(),
        m_npcs->end(),
        [&](Npc* npc) {
            sequenceRoot->connect( this->createNpcBloc(npc, this->getNpcNameByIndex(iter)));
            ++iter;
        }
    );
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


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTNpcUpdateAdministrator::stopANpcByID(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));

    if (!npcName.empty())
    {
        this->stopANpcByName(npcName);
    }
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
    for (size_t iter = 0; iter < m_npcs->size(); ++iter)
    {
        if ((*m_npcs)[iter]->getId() == npcID)
        {
            return this->getNpcNameByIndex(iter);
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


//////////////////////////////////////////////////////////////////////////


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
