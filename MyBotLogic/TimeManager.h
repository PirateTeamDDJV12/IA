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
    void saveStartGameTime()
    {
        m_startGameTime = std::chrono::system_clock::now();
    }

    // Lets you know how much time has elapsed since the start of the game
    std::chrono::milliseconds getTimeFromStart() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - m_startGameTime);
    }

    // Saves a specfic time point in the game and store it in an history 
    std::chrono::time_point<std::chrono::system_clock> savePoint(std::string name);

    // Return the time point of the saved name. If no name, return now()
    std::chrono::time_point<std::chrono::system_clock> getPoint(std::string name);

    // Save the time right now
    void fastSave()
    {
        m_fastSave = std::chrono::system_clock::now();
    }

    // Get the diffrence between a specific time saved previously and the current time
    std::chrono::milliseconds getFastDifference() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_fastSave - std::chrono::system_clock::now());
    }

    // Get the remaining time between the fast save and now
    std::chrono::milliseconds getRemainingFastTime() const;

    // Time between two saved time point (first - second)
    std::chrono::milliseconds getTimeBetweenTwoPoints(std::string name1, std::string name2)
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_saveTimePoints[name1] - m_saveTimePoints[name2]);
    }

    // Time between two time point (first - second)
    std::chrono::milliseconds getTimeBetweenTwoPoints(std::chrono::time_point<std::chrono::system_clock> tp1, 
        std::chrono::time_point<std::chrono::system_clock> tp2) const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(tp1 - tp2);
    }

    // Recovers the time between 2 frames
    float getElapsedTimeFrame() const
    {
        auto now = msNow();
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_timePreviousFrame);
        return elapsed.count() * m_timeScale;// <= 0 ? 0.0001f : elapsed.count()
    }

    // Changes the time limit for all turns
    void setTurnLimitTime(std::chrono::milliseconds ms);
    void setTurnLimitTime(std::chrono::seconds s);

    // Check if there needs to be an update
    bool isTimeToUpdate() const
    {
        return msNow() > m_timeNextFrame;
    }

    // Update the state of the TimeManager
    void update();
};

#endif //TIME_MANAGER_HEADER
