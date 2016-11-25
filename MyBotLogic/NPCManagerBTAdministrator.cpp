#include "NPCManagerBTAdministrator.h"
#include "Npc.h"

#include <algorithm>

using namespace BehaviourTree;


void NPCManagerBTAdministrator::init()
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

BlocRef NPCManagerBTAdministrator::createNpcBloc(Npc* npc, const std::string& npcName)
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


void NPCManagerBTAdministrator::stopANpcByID(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));

    if (!npcName.empty())
    {
        this->stopANpcByName(npcName);
    }
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTAdministrator::restartANpcByID(unsigned int NpcID, const std::string& npcNameBefore)
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

void NPCManagerBTAdministrator::restartANpcByIndex(size_t npcIndex, const std::string& npcNameBefore)
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


std::string NPCManagerBTAdministrator::getNpcNameByID(unsigned int npcID)
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

size_t NPCManagerBTAdministrator::getIndexByNpcName(const std::string& npcName)
{
    if (npcName.length() > 2)
    {
        size_t unit = static_cast<size_t>(npcName.back() - NPCManagerBTAdministrator::CHAR_TO_NUMBER_ASCII);
        size_t digit = static_cast<size_t>(npcName[npcName.length() - 2] - NPCManagerBTAdministrator::CHAR_TO_NUMBER_ASCII);

        return digit * 10 + unit;
    }
    
    return NPCManagerBTAdministrator::NO_INDEX;
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTAdministrator::swapNpcByIDs(unsigned int npc1ID, unsigned int npc2ID)
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


void NPCManagerBTAdministrator::restartANpcByNameAtBegin(const std::string& npcName)
{
    size_t npcIndex = this->getIndexByNpcName(npcName);

    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connectAtBeginning(createNpcBloc((*m_npcs)[npcIndex], npcName));
    }
}

void NPCManagerBTAdministrator::restartANpcByIDAtBegin(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));
    if (!npcName.empty())
    {
        this->restartANpcByNameAtBegin(npcName);
    }
}

void NPCManagerBTAdministrator::restartANpcByIndexAtBegin(size_t npcIndex)
{
    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connectAtBeginning(createNpcBloc((*m_npcs)[npcIndex], this->getNpcNameByIndex(npcIndex)));
    }
}


//////////////////////////////////////////////////////////////////////////


void NPCManagerBTAdministrator::restartANpcByNameAtEnd(const std::string& npcName)
{
    size_t npcIndex = this->getIndexByNpcName(npcName);

    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connect(createNpcBloc((*m_npcs)[npcIndex], npcName));
    }
}

void NPCManagerBTAdministrator::restartANpcByIDAtEnd(unsigned int npcID)
{
    std::string npcName = std::move(this->getNpcNameByID(npcID));

    if (!npcName.empty())
    {
        this->restartANpcByNameAtEnd(npcName);
    }
}

void NPCManagerBTAdministrator::restartANpcByIndexAtEnd(size_t npcIndex)
{
    if (npcIndex < m_npcs->size())
    {
        this->getBTRootAs<BlocComposite>()->connect(createNpcBloc((*m_npcs)[npcIndex], this->getNpcNameByIndex(npcIndex)));
    }
}
