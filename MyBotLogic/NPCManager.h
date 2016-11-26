#ifndef NPC_MANAGER_HEADER
#define NPC_MANAGER_HEADER
#include "Singleton.h"
#include "Npc.h"
#include "NPCInfo.h"
#include <vector>
#include <map>

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

    // Singleton instance
    static NPCManager m_instance;

    NPCManager()
    {
        m_npcs.reserve(MAX_NPCS);
    }
public:
    // Define the log path for the NPCManager logger and save the path to give it for each npc
    void setLoggerPath(const std::string &a_path);
    static NPCManager* get()
    {
        return &m_instance;
    }
    // Instantiate an NPC and save it in the vector
    void initNpc(std::pair<unsigned int, NPCInfo> curNpcs);
    // Instantiate all NPCs
    void initNpcs(std::map<unsigned int, NPCInfo> npcs);
    // Update all NPCs
    void updateNPCs(std::vector<Action*> &_actionList);
    // Npc Getter
    const std::vector<Npc *> &getNpcs() const
    {
        return m_npcs;
    }
};

#endif //NPC_MANAGER_HEADER