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
    // Log stuff
    Logger m_logger; // the logger itself
    std::string m_logPath; // the log path

    //std::map<unsigned int, Npc*> m_npcs;
    std::vector<Npc*> m_npcs;

    // Singleton instance
    static NPCManager m_instance;

    NPCManager()
    {
        m_npcs.reserve(50);
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
    void initNpcs(std::map<unsigned int, NPCInfo> npcs);

    void updateNPCs(std::vector<Action*> &_actionList);
};

#endif //NPC_MANAGER_HEADER