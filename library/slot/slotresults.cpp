
/************************************************************************
*    FILE NAME:       slotresults.cpp
*
*    DESCRIPTION:     Class of slot result's
************************************************************************/

// Physical component dependency
#include <slot/slotresults.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotResults::CSlotResults() :
    m_totalWinAmount(0)
{
}


/************************************************************************
*    desc:  Create a new play results entry into the vector
************************************************************************/
CPlayResult & CSlotResults::create()
{
    m_playResultVec.emplace_back();

    return m_playResultVec.back();
}


/************************************************************************
*    desc:  Clear the pays
************************************************************************/
void CSlotResults::clear()
{
    m_totalWinAmount = 0;

    for( auto & iter : m_playResultVec )
        iter.clear();
}


/************************************************************************
*    desc:  Sort the pays
************************************************************************/
void CSlotResults::sortPays()
{
    for( auto & iter : m_playResultVec )
        iter.sortPays();
}


/************************************************************************
*    desc:  Add up the win
************************************************************************/
void CSlotResults::addUpWin()
{
    m_totalWinAmount = 0;

    for( auto & iter : m_playResultVec )
        m_totalWinAmount += iter.addUpWin();
}


/************************************************************************
*    desc:  Get the total win
************************************************************************/
uint CSlotResults::getTotalWin()
{
    return m_totalWinAmount;
}


/************************************************************************
*    desc:  Do we have a win
************************************************************************/
bool CSlotResults::isWin()
{
    return (m_totalWinAmount > 0);
}
