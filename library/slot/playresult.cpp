
/************************************************************************
*    FILE NAME:       playresult.cpp
*
*    DESCRIPTION:     Class for holding pays
************************************************************************/

// Physical component dependency
#include <slot/playresult.h>
#include <slot/paycombo.h>

// Standard lib dependencies
#include <algorithm>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPlayResult::CPlayResult() :
    m_totalWinAmount(0)
{
}   // constructor


/************************************************************************
*    desc:  Add a slot pay
************************************************************************/
void CPlayResult::AddPay(
    const NSlotDefs::EPayType payType,
    const CPayCombo & rCombo,
    const int multiplier,
    const int winLine,
    const std::vector<CSymbPos> & symbPos )
{
    m_payVec.emplace_back( payType, rCombo.GetAward(), rCombo.GetBonusCode(), multiplier, winLine, symbPos );
    
    //m_payVec.back().Debug();
    
}   // AddPay


/************************************************************************
*    desc:  Sort the pays
************************************************************************/
void CPlayResult::SortPays()
{
    // Sort via lamda expression
    std::sort(m_payVec.begin(), m_payVec.end(),
        [](const CPay & a, const CPay & b){ return (a.GetBaseAward() >= b.GetBaseAward()); });
    
}   // SortPays


/************************************************************************
*    desc:  Add up the win
************************************************************************/
uint CPlayResult::AddUpWin()
{
    m_totalWinAmount = 0;
    
    for( auto & iter : m_payVec )
        m_totalWinAmount += iter.GetFinalAward();
                
    return m_totalWinAmount;
    
}   // AddUpWin


/************************************************************************
*    desc:  Clear the pays
************************************************************************/
void CPlayResult::Clear()
{
    m_totalWinAmount = 0;
    m_payVec.clear();
    
}   // Clear


/************************************************************************
*    desc:  Get the pay
************************************************************************/
const CPay & CPlayResult::GetPay( int index ) const
{
    return m_payVec.at( index );
    
}   // GetPay


/************************************************************************
*    desc:  Get the number of pays
************************************************************************/
uint CPlayResult::GetPayCount() const
{
    return m_payVec.size();
    
}   // GetPayCount
