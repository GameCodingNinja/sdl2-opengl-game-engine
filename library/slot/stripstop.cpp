
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
}


/************************************************************************
*    desc:  Get the math symbol weight
************************************************************************/
int CStripStop::getWeight() const
{
    return m_weight;
}


/************************************************************************
*    desc:  Get the math symbol
************************************************************************/
const CMathSymbol & CStripStop::getMathSymbol() const
{
    return m_rMathSymbol;
}
