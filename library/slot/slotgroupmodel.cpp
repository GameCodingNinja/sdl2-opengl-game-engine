
/************************************************************************
*    FILE NAME:       slotgroupmodel.cpp
*
*    DESCRIPTION:     Model class for the slot group
************************************************************************/

// Physical component dependency
#include <slot/slotgroupmodel.h>

// Game lib dependencies
#include <slot/slotmath.h>
#include <slot/playresult.h>
#include <slot/symbolposition.h>
#include <slot/slotmathmanager.h>
#include <slot/betmanager.h>
#include <slot/paylineset.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <chrono>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotGroupModel::CSlotGroupModel( const CSlotMath & rSlotMath, CPlayResult & rPlayResult ) :
    m_rSlotMath( rSlotMath ),
    m_rPlayResult( rPlayResult ),
    m_rPaylineSet( CSlotMathMgr::Instance().GetPaylineSet( rSlotMath.GetPaylineSetID() ) )
{
    // Seed the random number generator
    auto time = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto seed = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    m_rng.seed( seed );
    
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotGroupModel::~CSlotGroupModel()
{
}   // destructor


/************************************************************************
*    desc:  Create the model strips
************************************************************************/
void CSlotGroupModel::Create(
    const std::string & rStripSetId, const std::string & paytableSetId )
{
    m_paytableSetId = paytableSetId;
    
    // Get the reel strip set
    auto & rStripSetVec = m_rSlotMath.GetStripSet( rStripSetId );
    
    // Create the model strips
    for( auto & iter : rStripSetVec )
        m_slotStripModelDeq.emplace_back( m_rSlotMath.GetStrip( iter.GetId() ), m_rng, iter.GetEvalIndexVec() );
    
    // Setup the evaluation vectors
    m_evalMathSymbs.resize( rStripSetVec.size() );
    for( size_t i = 0; i < rStripSetVec.size(); ++i )
        m_evalMathSymbs.at(i).resize( rStripSetVec.at(i).GetEvalIndexVec().size() );
    
}   // CreateReelStrips


/************************************************************************
*    desc:  Generate the strip stops
************************************************************************/
void CSlotGroupModel::GenerateStops()
{
    for( auto & iter : m_slotStripModelDeq )
        iter.GenerateStop();
    
}   // GenerateStops


/************************************************************************
*    desc:  Evaluate the strips
************************************************************************/
void CSlotGroupModel::Evaluate()
{
    GenerateEvalSymbs();
    
    // Get the paytable set and evaluate as line pay or scatter
    auto & rPaytableSetVec = m_rSlotMath.GetPaytableSet( m_paytableSetId );
    for( auto & iter: rPaytableSetVec )
    {
        if( iter.GetType() == NSlotDefs::EP_PAYLINE )
            EvaluateLinePays( iter.GetId() );
        
        else if( iter.GetType() == NSlotDefs::EP_SCATTER )
            EvaluateScatters( iter.GetId() );
    }
    
}   // Evaluate


/************************************************************************
*    desc:  Generate the evaluation symbols
************************************************************************/
void CSlotGroupModel::GenerateEvalSymbs()
{
    for( size_t strip = 0; strip < m_slotStripModelDeq.size(); ++strip )
    {
        const int stop = m_slotStripModelDeq.at(strip).GetStop();
        const auto & rEvalSymbIndexVec = m_slotStripModelDeq.at(strip).GetEvalIndexVec();
        
        for( size_t symb = 0; symb < rEvalSymbIndexVec.size(); ++symb )
            m_evalMathSymbs.at(strip).at(symb) = &m_slotStripModelDeq.at(strip).GetSymbol( stop + rEvalSymbIndexVec.at(symb) );
    }
    
}   // GenerateEvalSymbs


/************************************************************************
*    desc:  Evaluate the line pays
************************************************************************/
void CSlotGroupModel::EvaluateLinePays( const std::string & paytable )
{
    auto & rPayComboVec = m_rSlotMath.GetPayComboSet( paytable );
    
    const auto & rPaylineSetVecVec = m_rPaylineSet.GetLineData();
    
    // An vector of flags to indicate a payline has been awarded and is no longer checked
    std::vector<bool> awarded(rPaylineSetVecVec.size());
    
    for( auto & cboIter : rPayComboVec )
    {
        for( size_t payline = 0; payline < rPaylineSetVecVec.size(); ++payline )
        {
            // Continue if this payline has already been awarded
            if( awarded.at(payline) )
                continue;
            
            for( size_t strip = 0; strip < rPaylineSetVecVec.at(payline).size(); ++strip )
            {
                const int pos = rPaylineSetVecVec.at(payline).at(strip);

                auto pMathSymb = m_evalMathSymbs.at(strip).at(pos);

                // Break here if not a match to start checking the next payline
                if( !pMathSymb->IsMatch( cboIter.GetSymbol() ) )
                    break;
                
                // If we made it this far and the below condition is true, then it's a match
                if( strip == static_cast<size_t>(cboIter.GetCount()-1) )
                {
                    awarded.at(payline) = true;

                    AddLinePay( cboIter, payline, rPaylineSetVecVec );

                    break;
                }
            }
        }
    }
    
}   // EvaluateLinePays


/************************************************************************
*    desc:  Add line pay to slot result
************************************************************************/
void CSlotGroupModel::AddLinePay(
    const CPayCombo & rPayCombo,
    const int payline,
    const std::vector<std::vector<int8_t>> & rPaylineSetVecVec )
{
    std::vector<CSymbPos> symbPos;
    symbPos.reserve(rPayCombo.GetCount());
    
    // Copy over the symbol offsets for the number of strips effected by the win
    for( int i = 0; i < rPayCombo.GetCount(); ++i )
        symbPos.emplace_back( i, rPaylineSetVecVec.at(payline).at(i) );
    
    // Add the win to the play result
    m_rPlayResult.AddPay( NSlotDefs::EP_PAYLINE, rPayCombo, CBetMgr::Instance().GetLineBet(), payline, symbPos );
    
}   // AddLinePay


/************************************************************************
*    desc:  Evaluate the scatter pays
************************************************************************/
void CSlotGroupModel::EvaluateScatters( const std::string & paytable )
{
    const std::vector<CPayCombo> & rPayComboVec = m_rSlotMath.GetPayComboSet( paytable );
    
    // An array of each unique scatter symbol name in the paytable
    std::vector<std::string> symbVec;
    // A multi-dementional array to record the position of scatter symbols
    std::vector<std::vector<CSymbPos>> posVecVec;

	 // Record each unique scatter symbol and setup a multi-dementional array
    // to hold the list of positions of each scatter symbol found on the strips.
    for( auto & cboIter : rPayComboVec )
    {
        if( std::find( symbVec.begin(), symbVec.end(), cboIter.GetSymbol() ) == symbVec.end() )
        {
            symbVec.push_back( cboIter.GetSymbol() );
            posVecVec.emplace_back();
        }
    }
    
    // Go through the eval symbols to find these symbols
    for( size_t strip = 0; strip < m_evalMathSymbs.size(); ++strip )
    {
        for( size_t pos = 0; pos < m_evalMathSymbs.at(strip).size(); ++pos )
        {
            // Check if these scatter positions are allowed
            if( m_rPaylineSet.IndexOfScaterData( strip, pos ) )
            {
                auto pMathSymb = m_evalMathSymbs.at(strip).at(pos);

                for( size_t symb = 0; symb < symbVec.size(); ++symb )
                {
                    // If the symbol is a match, record it's position
                    if( pMathSymb->IsMatch( symbVec.at(symb) )  )
                    {
                        posVecVec.at(symb).emplace_back( strip, pos );
                    }
                }
            }
        }
    }
    
    // Go throught the combo, check for the symbol and see if any of the counts match
    for( auto & cboIter : rPayComboVec )
    {
        for( size_t symb = 0; symb < symbVec.size(); ++symb )
        {
            if( (cboIter.GetSymbol() == symbVec.at(symb)) &&
                (posVecVec.at(symb).size() == static_cast<size_t>(cboIter.GetCount())) )
            {
                // Add the win to the play result
                m_rPlayResult.AddPay( NSlotDefs::EP_SCATTER, cboIter, CBetMgr::Instance().GetTotalBet(), -1, posVecVec.at(symb) );
            }
        }
    }
    
}   // EvaluateScatters


/************************************************************************
*    desc:  Get the math strip
************************************************************************/
const CSlotStripModel & CSlotGroupModel::GetStrip( int index ) const
{
    return m_slotStripModelDeq.at(index);
    
}   // GetMathStrips


/************************************************************************
*    desc:  Get the number of math strips
************************************************************************/
size_t CSlotGroupModel::GetCount() const
{
    return m_slotStripModelDeq.size();
    
}   // GetCount

