
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
}   // constructor


/************************************************************************
*    desc:  Create a new play results entry into the vector
************************************************************************/
CPlayResult & CSlotResults::Create()
{
    m_playResultVec.emplace_back();
    
    return m_playResultVec.back();
    
}   // Create


/************************************************************************
*    desc:  Clear the pays
************************************************************************/
void CSlotResults::Clear()
{
    m_totalWinAmount = 0;
    
    for( auto & iter : m_playResultVec )
        iter.Clear();
    
}   // Clear


/************************************************************************
*    desc:  Sort the pays
************************************************************************/
void CSlotResults::SortPays()
{
    for( auto & iter : m_playResultVec )
        iter.SortPays();
    
}   // SortPays


/************************************************************************
*    desc:  Add up the win
************************************************************************/
void CSlotResults::AddUpWin()
{
    m_totalWinAmount = 0;
    
    for( auto & iter : m_playResultVec )
        m_totalWinAmount += iter.AddUpWin();

}   // AddUpWin


/************************************************************************
*    desc:  Get the total win
************************************************************************/
uint CSlotResults::GetTotalWin()
{
    return m_totalWinAmount;
    
}   // GetTotalWin


/************************************************************************
*    desc:  Do we have a win
************************************************************************/
bool CSlotResults::IsWin()
{
    return (m_totalWinAmount > 0);
    
}   // IsWin
