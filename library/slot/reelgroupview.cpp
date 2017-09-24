
/************************************************************************
*    FILE NAME:       reelgroupview.cpp
*
*    DESCRIPTION:     View class for the reel group
************************************************************************/

// Physical component dependency
#include <slot/reelgroupview.h>

// Game lib dependencies
#include <slot/symbolsetview.h>
#include <slot/slotgroupmodel.h>
#include <slot/slotstripmodel.h>
#include <slot/pay.h>
#include <slot/reelstripview.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <utilities/matrix.h>
#include <objectdata/objectdatamanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CReelGroupView::CReelGroupView( const CSlotGroupModel & rSlotGroupModel ) :
    CSlotGroupView( rSlotGroupModel )
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CReelGroupView::~CReelGroupView()
{
    DeleteCycleResultSymbs();
    
}   // destructor


/************************************************************************
*    desc:  Create the view slot strips
************************************************************************/
void CReelGroupView::Create( const XMLNode & node, CSymbolSetView & rSymbolSetView )
{
    CSlotGroupView::Create( node, rSymbolSetView );
    
    // Get the group name
    const std::string group = node.getAttribute( "group" );
    
    // Sanity check
    const XMLNode reelstripLstNode = node.getChildNode( "reelstripList" );
    if( reelstripLstNode.isEmpty() )
        throw NExcept::CCriticalException("Reel Strip Creation Error!",
            boost::str( boost::format("Reel strip list node is empty!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    // Check that the model and view reel strip count is the same
    if( static_cast<int>(m_rSlotGroupModel.GetCount()) != reelstripLstNode.nChildNode() )
    {
        throw NExcept::CCriticalException("Reel Strip Creation Error!",
            boost::str( boost::format("Reelstrip model count does not match view count.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }
    
    // Build the visible symbol set
    rSymbolSetView.BuildSymbolSetView();
    
    // Create the view reel strips
    for( int reel = 0; reel < reelstripLstNode.nChildNode(); ++reel )
    {
        const XMLNode reelNode = reelstripLstNode.getChildNode(reel);
        
        // Add the model reel strip to the view reel strip
        m_reelStripViewDeq.emplace_back( m_rSlotGroupModel.GetStrip(reel), rSymbolSetView, reel );
        m_reelStripViewDeq.back().Create( reelNode, group );
    }

    // Setup the cycle result symbol vectors
    m_cycleResultSymbVec.resize( m_reelStripViewDeq.size() );
    for( size_t i = 0; i < m_reelStripViewDeq.size(); ++i )
        m_cycleResultSymbVec.at(i).resize( m_reelStripViewDeq.at(i).GetVisibleSymbolCount() );
    
}   // CreateReelStrips


/************************************************************************
*    desc:  Generate the cycle results symbols
************************************************************************/
void CReelGroupView::GenerateCycleResultSymbs()
{
    for( size_t reel = 0; reel < m_cycleResultSymbVec.size(); ++reel )
    {
        auto & evalSymbIndexVec = m_rSlotGroupModel.GetStrip(reel).GetEvalIndexVec();
        
        // This is an allocated pointer and will need to be freed
        for( size_t symb = 0; symb < evalSymbIndexVec.size(); ++symb )
            m_cycleResultSymbVec.at(reel).at(symb) = m_reelStripViewDeq.at(reel).GetCycleResultSymbol( evalSymbIndexVec.at(symb) );
    }
    
}   // GenerateCycleResultSymbs


/************************************************************************
*    desc:  Get the cycle results symbols
************************************************************************/
std::vector<std::vector<CSymbol2d *>> & CReelGroupView::GetCycleResultSymbs()
{
    return m_cycleResultSymbVec;
    
}   // GetCycleResultSymbs


/************************************************************************
*    desc:  Clear the cycle results symbols
************************************************************************/
void CReelGroupView::ClearCycleResultSymbs()
{
    // This replaces the temporary symbols with the ones used for spinning
    for( auto & iter : m_reelStripViewDeq )
        iter.ClearCycleResultSymbs();
        
    // Free the memory allocated for the cycle result symbols
    DeleteCycleResultSymbs();
    
}   // ClearCycleResultSymbs


/************************************************************************
*    desc:  Free the memory allocated for the cycle result symbols
************************************************************************/
void CReelGroupView::DeleteCycleResultSymbs()
{
    for( size_t reel = 0; reel < m_cycleResultSymbVec.size(); ++reel )
        for( size_t symb = 0; symb < m_cycleResultSymbVec.at(reel).size(); ++symb )
            NDelFunc::Delete( m_cycleResultSymbVec.at(reel).at(symb) );
    
}   // DeleteCycleResultSymbs


/************************************************************************
*    desc:  Update the reel group
************************************************************************/
void CReelGroupView::Update()
{
    if( IsVisible() )
    {
        for( auto & iter : m_reelStripViewDeq )
            iter.Update();
    }
    
}   // Update


/************************************************************************
*    desc:  Transform the reel group
************************************************************************/
void CReelGroupView::Transform()
{
    if( IsVisible() )
    {
        CObject2D::Transform();
        
        for( auto & iter : m_reelStripViewDeq )
            iter.Transform( GetMatrix(), WasWorldPosTranformed() );
        
        m_upCycleResultsTxtSprite->Transform( GetMatrix(), WasWorldPosTranformed() );
    }
    
}   // Transform


/************************************************************************
*    desc:  do the render
************************************************************************/
void CReelGroupView::Render( const CMatrix & matrix )
{
    if( IsVisible() )
    {
        for( auto & iter : m_reelStripViewDeq )
            iter.Render( matrix );
        
        m_upCycleResultsTxtSprite->Render( matrix );
    }
    
}   // Render


/************************************************************************
*    desc:  Start the reels spinning
************************************************************************/
void CReelGroupView::StartSpin()
{
    if( IsSpinState( NSlotDefs::ESS_STOPPED ) )
    {
        auto iter = m_spinProfileMapVec.find( m_defaultSpinProfile );
        if( iter == m_spinProfileMapVec.end() )
            throw NExcept::CCriticalException("Spin Profile Set Error!",
                boost::str( boost::format("Spin profile does not exist! (%s).\n\n%s\nLine: %s")
                    % m_defaultSpinProfile % __FUNCTION__ % __LINE__ ));

        // Init the reels with the spin profile
        for( size_t i = 0; i < m_reelStripViewDeq.size(); ++i )
            m_reelStripViewDeq.at(i).SetSpinProfile( iter->second.at(i) );

        // Start the spin
        for( auto & iter : m_reelStripViewDeq )
            iter.StartSpin();
    }
    
}   // StartSpin


/************************************************************************
*    desc:  Stop the reels spinning
************************************************************************/
void CReelGroupView::StopSpin()
{
    for( auto & iter : m_reelStripViewDeq )
        iter.StopSpin();
        
}   // StopSpin


/************************************************************************
*    desc:  Is the spin state
************************************************************************/
bool CReelGroupView::IsSpinState( NSlotDefs::ESpinState state ) const
{
    bool result(true);
    
    for( auto & iter : m_reelStripViewDeq )
    {
        if( iter.GetSpinState() != state )
        {
            result = false;
            break;
        }
    }
    
    return result;
    
}   // IsSpinState


/************************************************************************
*    desc:  Do we allow the stop sounds?
************************************************************************/
void CReelGroupView::AllowStopSounds( bool allow )
{
    for( auto & iter : m_reelStripViewDeq )
        iter.AllowStopSounds( allow );
    
}   // AllowStopSounds