
/************************************************************************
*    FILE NAME:       icycleresult.cpp
*
*    DESCRIPTION:     Interface for cycle results animations
************************************************************************/

// Physical component dependency
#include <slot/icycleresults.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
iCycleResults::iCycleResults() :
    m_pPlayResult(nullptr),
    m_cyclePayCounter(0),
    m_firstCycleComplete(false),
    m_cycleResultsActive(false)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
iCycleResults::~iCycleResults()
{
}   // destructor


/***************************************************************************
*    desc:  Start the cycle results
****************************************************************************/
void iCycleResults::Start()
{
    m_cycleResultsActive = true;
    m_firstCycleComplete = false;
    m_cyclePayCounter = 0;
    
}   // Start


/***************************************************************************
*    desc:  Stop the cycle results
****************************************************************************/
void iCycleResults::Stop()
{
    m_cycleResultsActive = false;
    
}   // Stop


/***************************************************************************
*    desc:  Did we complete one animation cycle of all the pays?
****************************************************************************/
bool iCycleResults::IsFirstCycleComplete()
{
    return m_firstCycleComplete;
    
}   // IsFirstCycleComplete


/***************************************************************************
*    desc:  Is the cycle results active?
****************************************************************************/
bool iCycleResults::IsCycleResultsActive()
{
    return m_cycleResultsActive;
    
}   // IsCycleResultsActive
