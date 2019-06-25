#include "timer.h"

Timer::Timer()
{
    hasStarted = false;
    totalTime = 0;
}

void Timer::start()
{
    hasStarted = true;
    startTime = time(nullptr);
}

void Timer::stop()
{
    if(hasStarted) {
        hasStarted = false;
        time_t endTime = time(nullptr);
        double difference = difftime(endTime, startTime);
        totalTime += difference;
    }
}

void Timer::reset()
{
    hasStarted = false;
    startTime = NULL;
    totalTime = 0;
}

int Timer::getRunningTime()
{
    if(hasStarted) {
        return static_cast<int>(difftime(time(nullptr), startTime)) + totalTime;
    } else {
        return -1;
    }
}

int Timer::getTotalTime() const
{
    return totalTime;
}




