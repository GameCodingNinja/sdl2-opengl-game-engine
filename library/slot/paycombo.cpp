
/************************************************************************
*    FILE NAME:       paycombo.cpp
*
*    DESCRIPTION:     Class for holding the pay combo
************************************************************************/

// Physical component dependency
#include <slot/paycombo.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPayCombo::CPayCombo( const std::string & symb, const int count, const uint award, const int bonusCode ) :
    m_symbol(symb),
    m_count(count),
    m_award(award),
    m_bonusCode(bonusCode)
{
}   // constructor


/************************************************************************
*    desc:  Get the symbol ID
************************************************************************/
const std::string & CPayCombo::GetSymbol() const
{
    return m_symbol;
    
}   // GetSymbol


/************************************************************************
*    desc:  Get the number of symbols involved in this pay
************************************************************************/
int CPayCombo::GetCount() const
{
    return m_count;
    
}   // GetCount


/************************************************************************
*    desc:  Get the award
************************************************************************/
uint CPayCombo::GetAward() const
{
    return m_award;
    
}   // GetAward


/************************************************************************
*    desc:  Get the bonus code
************************************************************************/
int CPayCombo::GetBonusCode() const
{
    return m_bonusCode;
    
}   // GetBonusCode
