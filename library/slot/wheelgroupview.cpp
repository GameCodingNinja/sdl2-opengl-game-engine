
/************************************************************************
*    FILE NAME:       wheelgroupview.cpp
*
*    DESCRIPTION:     Class for wheel group view
************************************************************************/

// Physical component dependency
#include <slot/wheelgroupview.h>

// Game lib dependencies
#include <slot/symbolsetview.h>
#include <slot/slotgroupmodel.h>
#include <slot/slotstripmodel.h>
#include <2d/sprite2d.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CWheelGroupView::CWheelGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel ) :
    CSlotGroupView( rSlotGroupModel )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CWheelGroupView::~CWheelGroupView()
{
}   // destructor


/************************************************************************
*    desc:  Create the view slot strips
************************************************************************/
void CWheelGroupView::Create( const XMLNode & node, CSymbolSetView & rSymbolSetView )
{
    CSlotGroupView::Create( node, rSymbolSetView );
    
    // Get the group name
    const std::string group = node.getAttribute( "group" );
    
    // Sanity check
    const XMLNode wheelLstNode = node.getChildNode( "wheelList" );
    if( wheelLstNode.isEmpty() )
        throw NExcept::CCriticalException("Wheel View Creation Error!",
            boost::str( boost::format("Wheel view node is empty!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    // Create the view wheel
    for( int wheel = 0; wheel < wheelLstNode.nChildNode(); ++wheel )
    {
        const XMLNode wheelNode = wheelLstNode.getChildNode(wheel);
        
        // Add the model reel strip to the view reel strip
        m_wheelViewDeq.emplace_back( m_spSlotGroupModel->GetStrip(wheel), rSymbolSetView, wheel );
        m_wheelViewDeq.back().Create( wheelNode, group );
    }
    
    // Setup the cycle result symbol vectors
    m_cycleResultSymbVec.resize( m_wheelViewDeq.size() );
    for( size_t i = 0; i < m_wheelViewDeq.size(); ++i )
        m_cycleResultSymbVec.at(i).resize( m_wheelViewDeq.at(i).GetVisibleSymbolCount() );
    
    // One time transform to set the non-spinning wheel sprites
    Transform();
    for( auto & iter : m_wheelViewDeq )
        iter.PreTransform();
    
}   // Create


/************************************************************************
*    desc:  Generate the cycle results symbols
************************************************************************/
void CWheelGroupView::GenerateCycleResultSymbs()
{
    for( size_t wheel = 0; wheel < m_cycleResultSymbVec.size(); ++wheel )
    {
        auto & evalSymbIndexVec = m_spSlotGroupModel->GetStrip(wheel).GetEvalIndexVec();
        
        // Get the first symbol index array as the starting point
        // NOTE: It is assumed these values as listed in the math file are in order from lowest to highest
        int evalStartPoint = evalSymbIndexVec.at(0);
        
        // These pointers don't need to be freed
        for( size_t symb = 0; symb < m_wheelViewDeq.at(wheel).GetVisibleSymbolCount(); ++symb )
            m_cycleResultSymbVec.at(wheel).at(symb) = &m_wheelViewDeq.at(wheel).GetSymbol( evalStartPoint + symb );
    }
    
}   // GenerateCycleResultSymbs


/************************************************************************
*    desc:  Get the cycle results symbols
************************************************************************/
std::vector<std::vector<CSymbol2d *>> & CWheelGroupView::GetCycleResultSymbs()
{
    return m_cycleResultSymbVec;
    
}   // GetCycleResultSymbs


/************************************************************************
*    desc:  Update the reel group
************************************************************************/
void CWheelGroupView::Update()
{
    if( IsVisible() )
    {
        for( auto & iter : m_wheelViewDeq )
            iter.Update();
    }
    
}   // Update


/************************************************************************
*    desc:  Transform the reel group
************************************************************************/
void CWheelGroupView::Transform()
{
    if( IsVisible() )
    {
        CObject2D::Transform();
        
        for( auto & iter : m_wheelViewDeq )
            iter.Transform( GetMatrix(), WasWorldPosTranformed() );
        
        m_upCycleResultsTxtSprite->Transform( GetMatrix(), WasWorldPosTranformed() );
    }
    
}   // Transform


/************************************************************************
*    desc:  do the render
************************************************************************/
void CWheelGroupView::Render( const CMatrix & matrix )
{
    if( IsVisible() )
    {
        for( auto & iter : m_wheelViewDeq )
            iter.Render( matrix );
        
        m_upCycleResultsTxtSprite->Render( matrix );
    }
    
}   // Render


/************************************************************************
*    desc:  Start the reels spinning
************************************************************************/
void CWheelGroupView::StartSpin()
{
    if( IsSpinState( NSlotDefs::ESS_STOPPED ) )
    {
        auto iter = m_spinProfileMapVec.find( m_defaultSpinProfile );
        if( iter == m_spinProfileMapVec.end() )
            throw NExcept::CCriticalException("Spin Profile Set Error!",
                boost::str( boost::format("Spin profile does not exist! (%s).\n\n%s\nLine: %s")
                    % m_defaultSpinProfile % __FUNCTION__ % __LINE__ ));

        // Init the wheels with the spin profile
        for( size_t i = 0; i < m_wheelViewDeq.size(); ++i )
            m_wheelViewDeq.at(i).SetSpinProfile( iter->second.at(i) );

        // Start the spin
        for( auto & iter : m_wheelViewDeq )
            iter.StartSpin();
    }
    
}   // StartSpin


/************************************************************************
*    desc:  Stop the reels spinning
************************************************************************/
void CWheelGroupView::StopSpin()
{
    for( auto & iter : m_wheelViewDeq )
        iter.StopSpin();
        
}   // StopSpin


/************************************************************************
*    desc:  Is the spin state
************************************************************************/
bool CWheelGroupView::IsSpinState( NSlotDefs::ESpinState state ) const
{
    bool result(true);
    
    for( auto & iter : m_wheelViewDeq )
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
void CWheelGroupView::AllowStopSounds( bool allow )
{
    for( auto & iter : m_wheelViewDeq )
        iter.AllowStopSounds( allow );
    
}   // AllowStopSounds
