
/************************************************************************
*    FILE NAME:       stripstop.cpp
*
*    DESCRIPTION:     Math strip stop class
************************************************************************/

// Physical component dependency
#include <slot/stripstop.h>

// Game lib dependencies
#include <slot/mathsymbol.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CStripStop::CStripStop( const CMathSymbol & rMathSymbol, int weight ) :
    m_rMathSymbol(rMathSymbol),
    m_weight(weight)
{
}   // constructor


/************************************************************************
*    desc:  Get the math symbol weight
************************************************************************/
int CStripStop::GetWeight() const
{
    return m_weight;
    
}   // GetWeight


/************************************************************************
*    desc:  Get the math symbol
************************************************************************/
const CMathSymbol & CStripStop::GetMathSymbol() const
{
    return m_rMathSymbol;
}
