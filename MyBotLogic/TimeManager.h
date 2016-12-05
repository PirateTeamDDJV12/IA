#ifndef TIME_MANAGER_HEADER
#define TIME_MANAGER_HEADER

#include "Singleton.h"

#include <chrono>
#include <map>


using namespace std::chrono_literals;


class TimeManager : Singleton
{
private:
    // Singleton instance
    static TimeManager m_instance;

private:
    float m_timeScale = 0.00001f;

    std::chrono::milliseconds m_timeDifference = 1000ms / 60;
    
    std::chrono::milliseconds m_timeCurrent;
    std::chrono::milliseconds m_timeNextFrame;
    std::chrono::milliseconds m_timePreviousFrame;
    
    std::chrono::milliseconds m_turnTimeLimit = 0ms;

    std::chrono::time_point<std::chrono::system_clock> m_fastSave = std::chrono::system_clock::now();

    std::chrono::time_point<std::chrono::system_clock> m_startGameTime = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> m_startExecutionTime = std::chrono::system_clock::now();

    // Saved time points history
    std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> m_saveTimePoints;

public:
    // Get the instance of the manager
    static TimeManager& getInstance()
    {
        return m_instance;
    }

    // Get the current time
    static std::chrono::time_point<std::chrono::system_clock> now()
    {
        return std::chrono::system_clock::now();
    }

    // Get the current time in milliseconds
    static std::chrono::milliseconds msNow()
    {
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    }

public:
    // Saves the time of the beginning of the game
    void saveStartGameTime();

    // Lets you know how much time has elapsed since the start of the game
    std::chrono::milliseconds getTimeFromStart() const;

    // Saves a specfic time point in the game and store it in an history 
    std::chrono::time_point<std::chrono::system_clock> savePoint(std::string name);

    // Return the time point of the saved name. If no name, return now()
    std::chrono::time_point<std::chrono::system_clock> getPoint(std::string name);

    // Save the time right now
    void fastSave();

    // Get the diffrence between a specific time saved previously and the current time
    std::chrono::milliseconds getFastDifference() const;

    // Get the remaining time between the fast save and now
    std::chrono::milliseconds getRemainingFastTime() const;

    // Time between two saved time point (first - second)
    std::chrono::milliseconds getTimeBetweenTwoPoints(std::string name1, std::string name2);

    // Time between two time point (first - second)
    std::chrono::milliseconds getTimeBetweenTwoPoints(std::chrono::time_point<std::chrono::system_clock> tp1,
                                                      std::chrono::time_point<std::chrono::system_clock> tp2) const;

    // Recovers the time between 2 frames
    float getElapsedTimeFrame() const;

    // Changes the time limit for all turns
    void setTurnLimitTime(std::chrono::milliseconds ms);
    void setTurnLimitTime(std::chrono::seconds s);

    // Check if there needs to be an update
    bool isTimeToUpdate() const;

    // Update the state of the TimeManager
    void update();
};

#endif //TIME_MANAGER_HEADER
