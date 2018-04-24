
/************************************************************************
*    FILE NAME:       statcounter.cpp
*
*    DESCRIPTION:     3D stat counter class singleton
************************************************************************/

// Physical component dependency
#include <utilities/statcounter.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <common/build_defs.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CStatCounter::CStatCounter() :
    m_vObjCounter(0),
    m_physicsObjCounter(0),
    m_elapsedFPSCounter(0),
    m_cycleCounter(0),
    m_scriptContexCounter(0),
    m_activeContexCounter(0),
    m_statsDisplayTimer(1000)
{
    resetCounters();
}


/************************************************************************
*    desc:  Destructor
************************************************************************/
CStatCounter::~CStatCounter()
{
}


/************************************************************************
*    desc:  Connect to the signal
************************************************************************/
void CStatCounter::connect( const StatCounterSignal::slot_type & slot )
{
    m_statCounterSignal.connect(slot);
}

/************************************************************************
*    desc:  Disconnect all slots
************************************************************************/
void CStatCounter::disconnect()
{
    m_statCounterSignal.disconnect_all_slots();;
}


/************************************************************************
*    desc:  Have we run through one cycle.
*
*	 ret: bool - true if one cycle has been completed
************************************************************************/
void CStatCounter::incCycle()
{
    // These counters are incremeented each game loop cycle so they can
    // be placed here in this function because this function is also called
    // each game loop cycle
    m_elapsedFPSCounter += CHighResTimer::Instance().getFPS();

    ++m_cycleCounter;

    // update the stats every 500 miliseconds
    if( m_statsDisplayTimer.expired(CTimer::RESTART_ON_EXPIRE) )
    {
        formatStatString();
        m_statCounterSignal( m_statStr );

        // Now that the stats are displayed, we can reset out counters.
        resetCounters();
    }
}


/************************************************************************
*    desc:  Reset the counter
************************************************************************/
void CStatCounter::resetCounters()
{
    m_vObjCounter = 0;
    m_physicsObjCounter = 0;
    m_elapsedFPSCounter = 0.0;
    m_cycleCounter = 0;
    m_activeContexCounter = 0;
}


/************************************************************************
*    desc:  Format the stat string
************************************************************************/
void CStatCounter::formatStatString()
{
    m_statStr = boost::str( boost::format("fps: %d - scx: %d of %d - vis: %d - phy: %d - res: %d x %d")
        % ((int)(m_elapsedFPSCounter / (double)m_cycleCounter))
        % (m_activeContexCounter / m_cycleCounter)
        % m_scriptContexCounter
        % (m_vObjCounter / m_cycleCounter)
        % (m_physicsObjCounter / m_cycleCounter)
        % CSettings::Instance().getSize().w
        % CSettings::Instance().getSize().h
        //% (playerPos.x)
        //% (playerPos.y)
        );
}


/************************************************************************
*    desc:  Inc the display counter
************************************************************************/
void CStatCounter::incDisplayCounter( size_t value )
{
    m_vObjCounter += value;
}


/************************************************************************
*    desc:  Inc the physics objects counter
************************************************************************/
void CStatCounter::incPhysicsObjectsCounter()
{
    ++m_physicsObjCounter;
}


/************************************************************************
*    desc:  Inc the script contex counter
************************************************************************/
void CStatCounter::incScriptContexCounter()
{
    ++m_scriptContexCounter;
}


/************************************************************************
*    desc:  Inc the active script contex counter
************************************************************************/
void CStatCounter::incActiveScriptContexCounter()
{
    ++m_activeContexCounter;
}
