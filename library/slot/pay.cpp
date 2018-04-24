
/************************************************************************
*    FILE NAME:       pay.cpp
*
*    DESCRIPTION:     Slot pay
************************************************************************/

// Physical component dependency
#include <slot/pay.h>

// Standard lib dependencies
#include <iostream>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPay::CPay(
    const NSlotDefs::EPayType payType,
    const uint award,
    const int bonusCode,
    const uint multiplier,
    const int payLine,
    const std::vector<CSymbPos> & symbPos ) :
        m_payType(payType),
        m_award(award),
        m_bonusCode(bonusCode),
        m_multiplier(multiplier),
        m_payLine(payLine),
        m_symbPosVec(symbPos)
{
}


/************************************************************************
*    desc:  Get the pay type
************************************************************************/
NSlotDefs::EPayType CPay::getPayType() const
{
    return m_payType;
}


/************************************************************************
*    desc:  Get the award
************************************************************************/
uint CPay::getBaseAward() const
{
    return m_award;
}


/************************************************************************
*    desc:  Get the final award
************************************************************************/
uint CPay::getFinalAward() const
{
    return m_award * m_multiplier;
}


/************************************************************************
*    desc:  Get the multiplier
************************************************************************/
uint CPay::getMultiplier() const
{
    return m_multiplier;
}


/************************************************************************
*    desc:  Get the pay line
************************************************************************/
int CPay::getPayLine() const
{
    return m_payLine;
}


/************************************************************************
*    desc:  Get the bonus code
************************************************************************/
int CPay::getBonusCode() const
{
    return m_bonusCode;
}


/************************************************************************
*    desc:  Get the symbol positions
************************************************************************/
const std::vector<CSymbPos> & CPay::getSymbPos() const
{
    return m_symbPosVec;
}


/************************************************************************
*    desc:  Debug output
************************************************************************/
void CPay::debug() const
{
    std::cout << "Pay Type: ";

    if( m_payType == NSlotDefs::EP_PAYLINE )
        std::cout << "Pay Line - " << (m_payLine + 1) << std::endl;
    else
        std::cout << "Scatter" << std::endl;

    std::cout << "Bonus Code: " << m_bonusCode << std::endl;
    std::cout << "Award: " << m_award << std::endl;
    std::cout << "Multiplier: " << m_multiplier << std::endl;
    std::cout << "Symbol Pos:";
    for( auto & iter: m_symbPosVec )
        std::cout << " (reel: " << iter.getReel() << ", pos: " << iter.getPos() << ")";

    std::cout << std::endl << "__________________________________" << std::endl << std::endl;
}
