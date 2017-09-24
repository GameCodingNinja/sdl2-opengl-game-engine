/************************************************************************
*    FILE NAME:       highresolutiontimer.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

#ifndef __highresolutiontimer_h__
#define __highresolutiontimer_h__

// SDL/OpenGL lib dependencies
#include <SDL.h>

class CHighResTimer
{
public:

    // Get the instance of the singleton class
    static CHighResTimer & Instance()
    {
        static CHighResTimer highResTimer;
        return highResTimer;
    }

    // Calc the elapsed time
    void CalcElapsedTime();

    // Get the elapsed time
    double GetElapsedTime();
    
    // Simple timer start
    void TimerStart();
    
    // Simple timer stop
    float TimerStop();

    // Get the FPS
    float GetFPS();

    // Get the time
    double GetTime();

private:

    // Constructor
    CHighResTimer();

    // Destructor
    ~CHighResTimer();
    
    // the inverse of the timer frenquency - multipling is faster
    double m_inverseTimerFrequency;

    // Last recorded time
    uint64_t m_lastTime;
    
    // Timer time
    uint64_t m_timer;

    // The amount of time that has elapsed between frames
    double m_elapsedTime;

    // The frames per second
    float m_fps;

};

#endif  // __highresolutiontimer_h__
