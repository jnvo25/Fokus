#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <ctime>

class Timer
{
public:
    bool hasStarted;
    Timer();
    void start();
    void pause();
    void stop();
    int getRunningTime();
    int getTotalTime() const;
    void reset();

private:
    time_t startTime;
    bool isPaused;
    int totalTime;
};

#endif // TIMER_H
