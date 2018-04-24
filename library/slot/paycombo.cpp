
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
}


/************************************************************************
*    desc:  Get the symbol ID
************************************************************************/
const std::string & CPayCombo::getSymbol() const
{
    return m_symbol;
}


/************************************************************************
*    desc:  Get the number of symbols involved in this pay
************************************************************************/
int CPayCombo::getCount() const
{
    return m_count;
}


/************************************************************************
*    desc:  Get the award
************************************************************************/
uint CPayCombo::getAward() const
{
    return m_award;
}


/************************************************************************
*    desc:  Get the bonus code
************************************************************************/
int CPayCombo::getBonusCode() const
{
    return m_bonusCode;
}
