
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
    ResetCounters();

}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CStatCounter::~CStatCounter()
{

}   // Destructor


/************************************************************************
*    desc:  Connect to the signal
************************************************************************/
void CStatCounter::Connect( const StatCounterSignal::slot_type & slot )
{
    m_statCounterSignal.connect(slot);

}   // Connect

/************************************************************************
*    desc:  Disconnect all slots
************************************************************************/
void CStatCounter::Disconnect()
{
    m_statCounterSignal.disconnect_all_slots();;

}   // Disconnect


/************************************************************************
*    desc:  Have we run through one cycle.
*
*	 ret: bool - true if one cycle has been completed
************************************************************************/
void CStatCounter::IncCycle()
{
    // These counters are incremeented each game loop cycle so they can
    // be placed here in this function because this function is also called
    // each game loop cycle
    m_elapsedFPSCounter += CHighResTimer::Instance().GetFPS();

    ++m_cycleCounter;

    // update the stats every 500 miliseconds
    if( m_statsDisplayTimer.Expired(CTimer::RESTART_ON_EXPIRE) )
    {
        FormatStatString();
        m_statCounterSignal( m_statStr );

        // Now that the stats are displayed, we can reset out counters.
        ResetCounters();
    }

}   // HasCycleCompleted


/************************************************************************
*    desc:  Reset the counter                                                             
************************************************************************/
void CStatCounter::ResetCounters()
{
    m_vObjCounter = 0;
    m_physicsObjCounter = 0;
    m_elapsedFPSCounter = 0.0;
    m_cycleCounter = 0;
    m_activeContexCounter = 0;

}   // ResetCounters


/************************************************************************
*    desc:  Format the stat string                                                             
************************************************************************/
void CStatCounter::FormatStatString()
{
    m_statStr = boost::str( boost::format("fps: %d - scx: %d of %d - vis: %d - phy: %d - res: %d x %d")
        % ((int)(m_elapsedFPSCounter / (double)m_cycleCounter))
        % (m_activeContexCounter / m_cycleCounter)
        % m_scriptContexCounter
        % (m_vObjCounter / m_cycleCounter)
        % (m_physicsObjCounter / m_cycleCounter)
        % CSettings::Instance().GetSize().w
        % CSettings::Instance().GetSize().h
        //% (playerPos.x)
        //% (playerPos.y) 
        );

}   // GetStatString


/************************************************************************
*    desc:  Inc the display counter                                                             
************************************************************************/
void CStatCounter::IncDisplayCounter( size_t value )
{
    m_vObjCounter += value;

}   // IncDisplayCounter


/************************************************************************
*    desc:  Inc the physics objects counter                                                             
************************************************************************/
void CStatCounter::IncPhysicsObjectsCounter()
{
    ++m_physicsObjCounter;

}   // IncPhysicsObjectsCounter


/************************************************************************
*    desc:  Inc the script contex counter                                                             
************************************************************************/
void CStatCounter::IncScriptContexCounter()
{
    ++m_scriptContexCounter;

}   // IncScriptContexCounter


/************************************************************************
*    desc:  Inc the active script contex counter                                                             
************************************************************************/
void CStatCounter::IncActiveScriptContexCounter()
{
    ++m_activeContexCounter;

}   // IncScriptContexCounter
