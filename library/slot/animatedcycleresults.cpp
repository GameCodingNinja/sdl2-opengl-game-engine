
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
CAnimatedCycleresults::CAnimatedCycleresults()
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CAnimatedCycleresults::~CAnimatedCycleresults()
{
}   // destructor


/***************************************************************************
*    desc:  Do some inits
****************************************************************************/
void CAnimatedCycleresults::Init( std::shared_ptr<CSlotGroupView> & spSlotGroupView, CPlayResult * pPlayResult )
{
    m_spSlotGroupView = spSlotGroupView;
    m_pPlayResult = pPlayResult;
    
}   // Init


/***************************************************************************
*    desc:  Update the cycle results
****************************************************************************/
void CAnimatedCycleresults::Update()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        auto & rPay = m_pPlayResult->GetPay( m_curPayIndex );
        auto & rSymbPos = rPay.GetSymbPos();

        for( auto & iter : rSymbPos )
            rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos())->GetSprite().GetScriptComponent().Update();
    }
    
}   // Update


/***************************************************************************
*    desc:  Activate the cycle results
****************************************************************************/
void CAnimatedCycleresults::Activate()
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
void CAnimatedCycleresults::Deactivate()
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
void CAnimatedCycleresults::StartAnimation()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        
        m_curPayIndex = m_cyclePayCounter;
        auto & rPay = m_pPlayResult->GetPay( m_curPayIndex );
        auto & rSymbPos = rPay.GetSymbPos();
        m_cyclePayCounter = (m_cyclePayCounter + 1) % m_pPlayResult->GetPayCount();

        // Set them all the alphs low
        for( auto & iter : rCycleResultSymb )
        {
            for( auto sympIter : iter )
            {
                sympIter->GetSprite().SetAlpha( 0.20f );
                sympIter->SetDeferredRender( false );
                sympIter->GetSprite().GetScriptComponent().ResetAndRecycle();
            }
        }

        // Set only the winners to default color
        for( auto & iter : rSymbPos )
        {
            auto symbol = rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos());
            symbol->GetSprite().SetDefaultColor();
            symbol->SetDeferredRender( true );
            symbol->GetSprite().PrepareFuncId( "animate" );
        }

        m_spSlotGroupView->SetCycleResultText( true, &rPay );
    }
    
}   // StartAnimation


/***************************************************************************
*    desc:  Stop the cycle results animation
****************************************************************************/
void CAnimatedCycleresults::StopAnimation()
{
    if( m_cycleResultsActive )
    {
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        
        // Set it all back to normal
        for( auto & iter : rCycleResultSymb )
        {
            for( auto sympIter : iter )
            {
                sympIter->GetSprite().SetDefaultColor();
                sympIter->SetDeferredRender( false );
                sympIter->GetSprite().GetScriptComponent().ResetAndRecycle();
            }
        }
        
        m_spSlotGroupView->SetCycleResultText( false );
    }
    
}   // StopAnimation


/***************************************************************************
*    desc:  Are we still animating
****************************************************************************/
bool CAnimatedCycleresults::IsAnimating()
{
    auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
    auto & rPay = m_pPlayResult->GetPay( m_curPayIndex );
    auto & rSymbPos = rPay.GetSymbPos();
    
    for( auto & iter : rSymbPos )
    {
        auto symbol = rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos());
        if( symbol->GetSprite().GetScriptComponent().IsActive() )
            return true;
    }
    
    return false;
    
}   // IsAnimating


/************************************************************************
*    desc:  Do the render
************************************************************************/
void CAnimatedCycleresults::Render( const CMatrix & matrix )
{
    if( m_cycleResultsActive )
        m_spSlotGroupView->DeferredRender( matrix );
        
}   // Render
