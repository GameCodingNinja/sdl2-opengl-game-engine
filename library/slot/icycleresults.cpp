
/************************************************************************
*    FILE NAME:       icycleresult.cpp
*
*    DESCRIPTION:     Interface for cycle results animations
************************************************************************/

// Physical component dependency
#include <slot/icycleresults.h>

// Game lib dependencies
#include <slot/playresult.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
iCycleResults::iCycleResults( CPlayResult * pPlayResult ) :
    m_pPlayResult( pPlayResult ),
    m_cyclePayCounter(0),
    m_curPayIndex(0),
    m_firstCycleComplete(false),
    m_cycleResultsActive(false)
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
iCycleResults::~iCycleResults()
{
}


/***************************************************************************
*    desc:  Activate the cycle results
****************************************************************************/
void iCycleResults::activate()
{
    m_cycleResultsActive = true;
    m_firstCycleComplete = false;
    m_cyclePayCounter = 0;
    m_curPayIndex = 0;
}


/***************************************************************************
*    desc:  Deactivate the cycle results
****************************************************************************/
void iCycleResults::deactivate()
{
    m_cycleResultsActive = false;
}


/***************************************************************************
*    desc:  Did we complete one animation cycle of all the pays?
****************************************************************************/
bool iCycleResults::isFirstCycleComplete()
{
    return m_firstCycleComplete;
}


/***************************************************************************
*    desc:  Is the cycle results active?
****************************************************************************/
bool iCycleResults::isCycleResultsActive()
{
    return m_cycleResultsActive;
}
