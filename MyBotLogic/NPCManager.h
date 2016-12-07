#ifndef NPC_MANAGER_HEADER
#define NPC_MANAGER_HEADER
#include "Singleton.h"
#include "Npc.h"
#include "NPCInfo.h"
#include "NPCManagerBTNpcUpdateAdministrator.h"
#include "NPCManagerBTDecisionMakingAdministrator.h"
#include "Mission.h"
#include "NPCManagerBTNpcUpdateAdministrator.h"
#include "NPCManagerBTDecisionMakingAdministrator.h"
#include <vector>
#include <map>
#include <string>
#include "Map.h"

class NPCManager : Singleton
{
private:
    enum
    {
        MAX_NPCS = 50
    };

    // Log stuff
    Logger m_logger; // the logger itself
    std::string m_logPath; // the log path

    //std::map<unsigned int, Npc*> m_npcs;
    std::vector<Npc*> m_npcs;

    //BT administrator to manage NPCs via BT. Has plenty of useful methods...
    NPCManagerBTNpcUpdateAdministrator m_BTNpcUpdateAdministrator;

    //BT that stores the decision making algorithm
    NPCManagerBTDecisionMakingAdministrator m_BTDecisionMakingAdministrator;

    // Singleton instance
    static NPCManager m_instance;

    NPCManager() :
        m_BTNpcUpdateAdministrator{}
    {
        m_npcs.reserve(MAX_NPCS);
        m_BTNpcUpdateAdministrator.reassignNpcVectorArray(m_npcs);
    }


public:
    // Define the log path for the NPCManager logger and save the path to give it for each npc
    void setLoggerPath(const std::string &a_path);
    static NPCManager* get()
    {
        return &m_instance;
    }
    // Instantiate an NPC and save it in the vector
    void initNpc(const std::pair<unsigned, NPCInfo>& curNpcs);
    // Instantiate all NPCs
    void initNpcs(const std::map<unsigned, NPCInfo>& npcs);
    // Create BT and attach all NPCs to it
    void initBT();
    // Get a specific npc
    const Npc *getNpcById(int npc_id_on_tile);
    // Update all NPCs
    void updateNPCs(const std::map<unsigned int, NPCInfo> &_npcs, std::vector<Action*> &_actionList);
    bool UpdateNpcActions(Map * myMap);

    // Npc Getter
    const std::vector<Npc *> &getNpcs() const
    {
        return m_npcs;
    }

    NPCManagerBTNpcUpdateAdministrator& getNpcUpdateBT()
    {
        return m_BTNpcUpdateAdministrator;
    }

    bool isGoalAlreadyAssign(unsigned int goalId);
    
};

#endif //NPC_MANAGER_HEADER