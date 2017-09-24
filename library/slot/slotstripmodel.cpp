
/************************************************************************
*    FILE NAME:       slotstripmodel.cpp
*
*    DESCRIPTION:     Slot strip model
************************************************************************/

// Physical component dependency
#include <slot/slotstripmodel.h>

// Game lib dependencies
#include <slot/stripstop.h>

// Standard lib dependencies
#include <iostream>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotStripModel::CSlotStripModel( 
    const std::vector<CStripStop> & rMathStripVec,
    std::mt19937 & rRng,
    const std::vector<int8_t> & evalSymbIndexVec ) :
        m_rMathStripVec(rMathStripVec),
        m_rRng(rRng),
        m_evalSymbIndexVec(evalSymbIndexVec),
        m_totalWeight(0),
        m_lastStop(0),
        m_stop(0)
{
    // Get the total weight of the slot strip
    for( auto & iter : rMathStripVec )
        m_totalWeight += iter.GetWeight();
        
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotStripModel::~CSlotStripModel()
{
}   // destructor


/************************************************************************
*    desc:  Generate the strip stop
************************************************************************/
void CSlotStripModel::GenerateStop()
{
    m_lastStop = m_stop;
    m_stop = 0;
    int weightCount = 0;
    
    int awardedWeight = m_rRng() % (m_totalWeight + 1);
    
    for( auto & iter : m_rMathStripVec )
    {
        weightCount += iter.GetWeight();

        if( awardedWeight <= weightCount )
            break;

        ++m_stop;
    }
    
}   // GenerateStop


/************************************************************************
*    desc:  Get the last strip stop
************************************************************************/
int CSlotStripModel::GetLastStop() const
{
    return m_lastStop;
    
}   // GetCurrentStop


/************************************************************************
*    desc:  Get the strip stop
************************************************************************/
int CSlotStripModel::GetStop() const
{
    return m_stop;
    
}   // GetStop


/************************************************************************
*    desc:  Get the math symbol
************************************************************************/
const CMathSymbol & CSlotStripModel::GetSymbol( const int stop ) const
{
    const int index = GetSymbolIndex( stop );
    
    return m_rMathStripVec.at(index).GetMathSymbol();
    
}   // GetSymbol


/************************************************************************
*    desc:  Get the math symbol index
************************************************************************/
int CSlotStripModel::GetSymbolIndex( const int stop ) const
{
    int index = std::abs(stop) % m_rMathStripVec.size();
    
    if( (stop < 0) && (index != 0) )
        index = m_rMathStripVec.size() - index;
    
    return index;
    
}   // GetSymbolIndex


/************************************************************************
*    desc:  Get the evaluation symbol index count
************************************************************************/
const std::vector<int8_t> & CSlotStripModel::GetEvalIndexVec() const
{
    return m_evalSymbIndexVec;
}


/************************************************************************
*    desc:  Get the math symbol strip vector
************************************************************************/
const std::vector<CStripStop> & CSlotStripModel::GetStripVec() const
{
    return m_rMathStripVec;
    
}   // GetEvalSymbolCount

