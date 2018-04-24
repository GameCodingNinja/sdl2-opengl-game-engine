
/************************************************************************
*    FILE NAME:       animatedcycleresults.cpp
*
*    DESCRIPTION:     Animated cycle results implementation
************************************************************************/

// Physical component dependency
#include <slot/animatedcycleresults.h>

// Game lib dependencies
#include <slot/slotgroupview.h>
#include <slot/playresult.h>
#include <slot/symbol2d.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CAnimatedCycleResults::CAnimatedCycleResults( CPlayResult * pPlayResult ) :
    iCycleResults( pPlayResult )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CAnimatedCycleResults::~CAnimatedCycleResults()
{
}   // destructor


/***************************************************************************
*    desc:  Do some inits
****************************************************************************/
void CAnimatedCycleResults::Init( std::shared_ptr<CSlotGroupView> & spSlotGroupView )
{
    m_spSlotGroupView = spSlotGroupView;
    
}   // Init


/***************************************************************************
*    desc:  Update the cycle results
****************************************************************************/
void CAnimatedCycleResults::Update()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        auto & rPay = m_pPlayResult->GetPay( m_curPayIndex );
        auto & rSymbPos = rPay.GetSymbPos();

        for( auto & iter : rSymbPos )
            rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos())->GetSprite().getScriptComponent().update();
    }
    
}   // Update


/************************************************************************
*    desc:  Transform
************************************************************************/
void CAnimatedCycleResults::Transform( const CMatrix & matrix, bool tranformWorldPos )
{
    
}   // Transform


/***************************************************************************
*    desc:  Activate the cycle results
****************************************************************************/
void CAnimatedCycleResults::Activate()
{
    if( m_pPlayResult->GetPayCount() > 0 )
    {
        iCycleResults::Activate();

        m_spSlotGroupView->GenerateCycleResultSymbs();
    }
    
}   // Activate


/***************************************************************************
*    desc:  Deactivate the cycle results
****************************************************************************/
void CAnimatedCycleResults::Deactivate()
{
    if( m_cycleResultsActive )
    {
        iCycleResults::Deactivate();
        
        m_spSlotGroupView->ClearCycleResultSymbs();
    }
    
}   // Deactivate


/***************************************************************************
*    desc:  Start the cycle results animation
****************************************************************************/
void CAnimatedCycleResults::StartAnimation()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        
        m_curPayIndex = m_cyclePayCounter;
        m_cyclePayCounter = (m_cyclePayCounter + 1) % m_pPlayResult->GetPayCount();
        
        auto & rPay = m_pPlayResult->GetPay( m_curPayIndex );
        auto & rSymbPos = rPay.GetSymbPos();
        

        // Set them all the alphs low
        for( auto & iter : rCycleResultSymb )
        {
            for( auto sympIter : iter )
            {
                sympIter->GetSprite().setAlpha( 0.20f );
                sympIter->GetSprite().getScriptComponent().resetAndRecycle();
                sympIter->SetDeferredRender( false );
            }
        }

        // Set only the winners to default color and set the script function
        for( auto & iter : rSymbPos )
        {
            auto symbol = rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos());
            symbol->GetSprite().setDefaultColor();
            symbol->GetSprite().prepareFuncId( "animate" );
            symbol->SetDeferredRender( true );
        }

        m_spSlotGroupView->SetCycleResultText( true, &rPay );
    }
    
}   // StartAnimation


/***************************************************************************
*    desc:  Stop the cycle results animation
****************************************************************************/
void CAnimatedCycleResults::StopAnimation()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        
        // Set it all back to normal
        for( auto & iter : rCycleResultSymb )
        {
            for( auto sympIter : iter )
            {
                sympIter->GetSprite().setDefaultColor();
                sympIter->GetSprite().getScriptComponent().resetAndRecycle();
                sympIter->SetDeferredRender( false );
            }
        }
        
        m_spSlotGroupView->SetCycleResultText( false );
    }
    
}   // StopAnimation


/***************************************************************************
*    desc:  Are we still animating
****************************************************************************/
bool CAnimatedCycleResults::IsAnimating()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        auto & rPay = m_pPlayResult->GetPay( m_curPayIndex );
        auto & rSymbPos = rPay.GetSymbPos();

        for( auto & iter : rSymbPos )
        {
            auto symbol = rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos());
            if( symbol->GetSprite().getScriptComponent().isActive() )
                return true;
        }
    }
    
    return false;
    
}   // IsAnimating


/************************************************************************
*    desc:  Do the render
************************************************************************/
void CAnimatedCycleResults::Render( const CMatrix & matrix )
{
    if( m_cycleResultsActive )
        m_spSlotGroupView->DeferredRender( matrix );
        
}   // Render
