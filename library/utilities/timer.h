
/************************************************************************
*    FILE NAME:       timer.h
*
*    DESCRIPTION:     Timer Class
************************************************************************/

#ifndef __timer_h__
#define __timer_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CTimer : public boost::noncopyable
{
public:

    enum
    { START_EXPIRED = 1,
      RESTART_ON_EXPIRE = 1, };

    // Constructor
    CTimer();
    CTimer( double interval, bool startExpired = false );

    // Destructor
    ~CTimer();

    // Set the time to have expired
    void SetExpired();

    // Has the timer expired
    bool Expired( bool resetOnExpire = false );

    // Reset the timer to start over
    void Reset();

    // Set the timer interval
    void Set(double interval);

    // Set the timer interval
    void Disable( bool disabled = true );
    
    // Set the value returned by Expired when the timer is disabled
    void SetDisableValue( bool disableValue );

private:

    // Expired time
    double m_expiredTime;

    // time interval
    double m_timeInterval;

    // Disabled flag
    bool m_disabled;
    
    // Disabled return value.
    // This value allows a disabled timer to act as expired or not
    bool m_disableValue;

};

#endif  // __timer_h__
