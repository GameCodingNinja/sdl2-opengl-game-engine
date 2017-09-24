
/************************************************************************
*    FILE NAME:       timer.cpp
*
*    DESCRIPTION:     Timer Class
************************************************************************/

// Physical component dependency
#include <utilities/timer.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CTimer::CTimer(double interval, bool startExpired)
    : m_timeInterval(interval),
      m_disabled(false),
      m_disableValue(false)
{
    if( startExpired )
        SetExpired();
    else
        Reset();

}   // constructor

CTimer::CTimer()
    : m_expiredTime(CHighResTimer::Instance().GetTime()),
      m_disabled(false)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CTimer::~CTimer()
{
}	// destructor


/***************************************************************************
*   desc:  Reset the timer to start over
****************************************************************************/
void CTimer::Reset()
{
    m_expiredTime = m_timeInterval + CHighResTimer::Instance().GetTime();
    m_disabled = false;
    
}   // Reset


/***************************************************************************
*   desc:  Set the time to have expired
****************************************************************************/
void CTimer::SetExpired()
{
    m_expiredTime = CHighResTimer::Instance().GetTime();

}   // Reset


/***************************************************************************
*   desc:  Set the timer interval
****************************************************************************/
void CTimer::Set( double interval )
{
    m_timeInterval = interval;
    Reset();

}   // Set


/***************************************************************************
*   desc:  Has the timer expired
*
*   param:  resetOnExpire - Reset the timer if it has expired
*
****************************************************************************/
bool CTimer::Expired( bool resetOnExpire )
{
    // Has the timer been disabled
    if( m_disabled )
        return m_disableValue;

    bool result(false);

    if( CHighResTimer::Instance().GetTime() > m_expiredTime )
    {
        result = true;

        if( resetOnExpire )
            Reset();
    }

    return result;

}   // Expired


/***************************************************************************
*   desc:  Disable this timer
****************************************************************************/
void CTimer::Disable( bool disabled )
{
    m_disabled = disabled;

}   // Disable


/***************************************************************************
*   desc:  Set the value returned by Expired when the timer is disabled
****************************************************************************/
void CTimer::SetDisableValue( bool disableValue )
{
    m_disableValue = disableValue;

}   // Disable
