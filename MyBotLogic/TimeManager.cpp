#include "TimeManager.h"


using namespace std::chrono;


TimeManager TimeManager::m_instance;


void TimeManager::saveStartGameTime()
{
    m_startGameTime = system_clock::now();
}

milliseconds TimeManager::getTimeFromStart() const
{
    return duration_cast<milliseconds>(system_clock::now() - m_startGameTime);
}

time_point<system_clock> TimeManager::savePoint(std::string name)
{
    m_saveTimePoints[name] = system_clock::now();
    return m_saveTimePoints[name];
}

time_point<system_clock> TimeManager::getPoint(std::string name)
{
    if(m_saveTimePoints.find(name) == end(m_saveTimePoints))
    {
        return system_clock::now();
    }
    return m_saveTimePoints[name];
}

void TimeManager::fastSave()
{
    m_fastSave = system_clock::now();
}

milliseconds TimeManager::getFastDifference() const
{
    return duration_cast<milliseconds>(m_fastSave - system_clock::now());
}

milliseconds TimeManager::getRemainingFastTime() const
{
    milliseconds currentDuration = duration_cast<milliseconds>(system_clock::now() - m_fastSave);
    milliseconds remaining = m_turnTimeLimit - currentDuration;
    return remaining <= 0ms ? 0ms : remaining;
}

milliseconds TimeManager::getTimeBetweenTwoPoints(std::string name1, std::string name2)
{
    return duration_cast<milliseconds>(m_saveTimePoints[name1] - m_saveTimePoints[name2]);
}

milliseconds TimeManager::getTimeBetweenTwoPoints(time_point<system_clock> tp1, time_point<system_clock> tp2) const
{
    return duration_cast<milliseconds>(tp1 - tp2);
}

float TimeManager::getElapsedTimeFrame() const
{
    auto now = msNow();
    milliseconds elapsed = duration_cast<milliseconds>(now - m_timePreviousFrame);
    return elapsed.count() * m_timeScale;// <= 0 ? 0.0001f : elapsed.count()
}

void TimeManager::setTurnLimitTime(milliseconds ms)
{
    if(ms <= 0ms)
    {
        return;
    }
    m_turnTimeLimit = ms;
}

void TimeManager::setTurnLimitTime(seconds s)
{
    if(s <= 0ms)
    {
        return;
    }
    m_turnTimeLimit = s;
}

bool TimeManager::isTimeToUpdate() const
{
    return msNow() > m_timeNextFrame;
}

void TimeManager::update()
{
    m_timeCurrent = msNow();

    if(m_timeCurrent > m_timeNextFrame)
    {
        m_timePreviousFrame = m_timeCurrent;
        m_timeNextFrame = m_timeCurrent + m_timeDifference;
    }
}
