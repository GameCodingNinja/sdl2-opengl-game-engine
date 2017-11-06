
/************************************************************************
*    FILE NAME:       simplecycleresults.cpp
*
*    DESCRIPTION:     Simple cycle results implementation
************************************************************************/

// Physical component dependency
#include <slot/simplecycleresults.h>

// Game lib dependencies
#include <slot/slotgroupview.h>
#include <slot/playresult.h>
#include <slot/symbol2d.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSimpleCycleresults::CSimpleCycleresults()
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSimpleCycleresults::~CSimpleCycleresults()
{
}   // destructor


/***************************************************************************
*    desc:  Do some inits
****************************************************************************/
void CSimpleCycleresults::Init( std::shared_ptr<CSlotGroupView> & spSlotGroupView, CPlayResult * pPlayResult )
{
    m_spSlotGroupView = spSlotGroupView;
    m_pPlayResult = pPlayResult;
    
}   // Init


/***************************************************************************
*    desc:  Update the cycle results
****************************************************************************/
void CSimpleCycleresults::Update()
{

}   // Update


/***************************************************************************
*    desc:  Start the cycle results
****************************************************************************/
void CSimpleCycleresults::Activate()
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
void CSimpleCycleresults::Deactivate()
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
void CSimpleCycleresults::StartAnimation()
{
    if( m_cycleResultsActive )
    {
        m_cycleResultsTimer.Set( 1000 );
        
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        
        auto & rPay = m_pPlayResult->GetPay( m_cyclePayCounter );
        auto & rSymbPos = rPay.GetSymbPos();
        m_cyclePayCounter = (m_cyclePayCounter + 1) % m_pPlayResult->GetPayCount();

        // Set them all to a low alphs
        for( auto & iter : rCycleResultSymb )
            for( auto sympIter : iter )
                sympIter->GetSprite().SetAlpha( 0.20f );

        // Set only the winners to default color
        for( auto & iter : rSymbPos )
            rCycleResultSymb.at(iter.GetReel()).at(iter.GetPos())->GetSprite().SetDefaultColor();

        m_spSlotGroupView->SetCycleResultText( true, &rPay );
    }
    
}   // StartAnimation


/***************************************************************************
*    desc:  Stop the cycle results animation
****************************************************************************/
void CSimpleCycleresults::StopAnimation()
{
    if( m_cycleResultsActive )
    {
        m_cycleResultsTimer.SetExpired();
        
        auto & rCycleResultSymb = m_spSlotGroupView->GetCycleResultSymbs();
        
        // Set it all back to normal
        for( auto & iter : rCycleResultSymb )
            for( auto sympIter : iter )
                sympIter->GetSprite().SetDefaultColor();
        
        m_spSlotGroupView->SetCycleResultText( false );
    }
    
}   // StopAnimation


/***************************************************************************
*    desc:  Are we still animating
****************************************************************************/
bool CSimpleCycleresults::IsAnimating()
{
    return !m_cycleResultsTimer.Expired();
    
}   // Animate
