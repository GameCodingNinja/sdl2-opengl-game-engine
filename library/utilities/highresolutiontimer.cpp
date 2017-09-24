/************************************************************************
*    FILE NAME:       highresolutiontimer.h
*
*    DESCRIPTION:     high resolution timer class
************************************************************************/

// Physical component dependency
#include <utilities/highresolutiontimer.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CHighResTimer::CHighResTimer()
    : m_inverseTimerFrequency(0.0),
      m_lastTime(uint64_t(0.0)),
      m_elapsedTime(0.0),
      m_fps(0.0f)
{
    // inverse it so that we can do a simple multiplication instead of division
    m_inverseTimerFrequency = 1000.0 / static_cast<double>(SDL_GetPerformanceFrequency());

    // Init the lastTime variable for the first runthrough
    m_lastTime = SDL_GetPerformanceCounter();

}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CHighResTimer::~CHighResTimer()
{
}   // Destructor


/***************************************************************************
*    desc:  Simple timer start
****************************************************************************/
void CHighResTimer::TimerStart()
{
    // Get the current performance time
    m_timer = SDL_GetPerformanceCounter();
        
}  // TimerStart


/***************************************************************************
*    desc:  Simple timer duration
****************************************************************************/
float CHighResTimer::TimerStop()
{
    // Get the current performance time
    uint64_t time = SDL_GetPerformanceCounter();

    // Get the elapsed time
    return (float)((time - m_timer) * m_inverseTimerFrequency);
        
}  // TimerStop


/***************************************************************************
*    desc:  Calc the elapsed time
****************************************************************************/
void CHighResTimer::CalcElapsedTime()
{
    // Get the current performance time
    uint64_t time = SDL_GetPerformanceCounter();

    // Set the elapsed time
    m_elapsedTime = static_cast<double>(time - m_lastTime) * m_inverseTimerFrequency;

    // Set the fps
    m_fps = static_cast<float>(1000.0f / m_elapsedTime);

    // Catch any hickups - cap to about 10 fps
    // Elapsed time is not expected to get this 
    // high in  a game which is why it's capped
    if( m_elapsedTime > 100.0f )
        m_elapsedTime = 100.0f;

    // Reset the last time
    m_lastTime = time;
    
}  // GetPerformanceTime


/***************************************************************************
*    desc:  Get the elapsed time
****************************************************************************/
double CHighResTimer::GetElapsedTime()
{
    return m_elapsedTime;
}


/***************************************************************************
*    desc:  get the elapsed time
****************************************************************************/
float CHighResTimer::GetFPS()
{
    return m_fps;
}


/***************************************************************************
*    desc:  get the raw time
****************************************************************************/
double CHighResTimer::GetTime()
{
    // Get the current performance time
    uint64_t time = SDL_GetPerformanceCounter();

    return (double)(time * m_inverseTimerFrequency);

}   // GetTime
