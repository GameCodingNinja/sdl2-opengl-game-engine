
/************************************************************************
*    FILE NAME:       slotgroupview.cpp
*
*    DESCRIPTION:     View base class for the slot group
************************************************************************/

// Physical component dependency
#include <slot/slotgroupview.h>

// Game lib dependencies
#include <slot/slotgroupmodel.h>
#include <slot/pay.h>
#include <slot/icycleresults.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <objectdata/objectdatamanager.h>
#include <2d/sprite2d.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotGroupView::CSlotGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel ) :
    m_spSlotGroupModel( rSlotGroupModel )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSlotGroupView::~CSlotGroupView()
{
}   // destructor


/************************************************************************
*    desc:  Create the view slot strips
************************************************************************/
void CSlotGroupView::Create(
    const XMLNode & node,
    CSymbolSetView & rSymbolSetView,
    std::unique_ptr<iCycleResults> upCycleResults )
{
    // Set the cycle results unique pointer
    m_upCycleResults = std::move(upCycleResults);
    
    // Get the group name
    const std::string group = node.getAttribute( "group" );
    
    // Load the transform data from node
    loadTransFromNode( node.getChildNode( "translation" ) );
    
    // Get the cycle results text node and object data
    const XMLNode cycleResultsTxtNode = node.getChildNode( "cycleResultsText" );
    const std::string objectName = cycleResultsTxtNode.getAttribute( "objectName" );
    
    // Allocate the cycle results text
    m_upCycleResultsTxtSprite.reset( new CSprite2D( CObjectDataMgr::Instance().GetData2D( group, objectName ) ) );
    m_upCycleResultsTxtSprite->setVisible( false );
    m_upCycleResultsTxtSprite->loadTransFromNode( cycleResultsTxtNode );
    
    // Load the font properties from XML node
    m_upCycleResultsTxtSprite->getVisualComponent().loadFontPropFromNode( cycleResultsTxtNode );
    
}   // Create


/************************************************************************
*    desc:  Set the cycle results text
************************************************************************/
void CSlotGroupView::SetCycleResultText( bool visible, const CPay * pPay )
{
    m_upCycleResultsTxtSprite->setVisible( visible );
    
    if( visible && (pPay != nullptr) )
    {
        std::string text;
        
        if( pPay->GetPayType() == NSlotDefs::EP_PAYLINE )
        {
            text = boost::str( boost::format("Line %d Pays %d") % (pPay->GetPayLine()+1) % pPay->GetFinalAward() );
        }
        else
        {
            if( pPay->GetBonusCode() > 0 )
                text = boost::str( boost::format("Bonus Pays %d") % pPay->GetFinalAward() );
            else
                text = boost::str( boost::format("Scatter Pays %d") % pPay->GetFinalAward() );
        }
        
        m_upCycleResultsTxtSprite->getVisualComponent().createFontString(text);
    }
    
}   // SetCycleResultText


/************************************************************************
*    desc:  Load the spin profile from XML node
************************************************************************/
void CSlotGroupView::LoadSpinProfileFromNode( const XMLNode & node )
{
    // Sanity check
    if( !node.isAttributeSet("default") )
        throw NExcept::CCriticalException("Spin profile Creation Error!",
            boost::str( boost::format("Spin profile default attribute not set!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    m_defaultSpinProfile = node.getAttribute( "default" );
    
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        const XMLNode profileLstNode = node.getChildNode(i);
        
        // Get the spin profile id
        const std::string profileId = profileLstNode.getAttribute( "id" );
        
        // Create a new spin profile
        auto iter = m_spinProfileMapVec.emplace( profileId, std::vector<CSpinProfile>() );
        
        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Spin Profile Load Error!",
                boost::str( boost::format("Duplicate id (%s)!\n\n%s\nLine: %s")
                    % profileId % __FUNCTION__ % __LINE__ ));
        }
        
        // Pre-reserve the amount
        iter.first->second.reserve( profileLstNode.nChildNode() );
        
        for( int j = 0; j < profileLstNode.nChildNode(); ++j )
        {
            const XMLNode profileNode = profileLstNode.getChildNode(j);
            
            iter.first->second.emplace_back();
            iter.first->second.back().LoadFromNode( profileNode );
        }
    }
    
}   // LoadSpinProfileFromXML


/************************************************************************
*    desc:  Update the reel group
************************************************************************/
void CSlotGroupView::Update()
{
    if( m_upCycleResults )
        m_upCycleResults->Update();
    
}   // Update


/************************************************************************
*    desc:  Transform the reel group
************************************************************************/
void CSlotGroupView::Transform()
{
    CObject2D::transform();
    
    if( m_upCycleResults )
        m_upCycleResults->Transform( getMatrix(), wasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  Do the render
************************************************************************/
void CSlotGroupView::Render( const CMatrix & matrix )
{
    if( m_upCycleResults )
        m_upCycleResults->Render( matrix );
}


/************************************************************************
*    desc:  Activate the cycle results
************************************************************************/
void CSlotGroupView::ActivateCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->Activate();
    
}   // ActivateCycleResults


/************************************************************************
*    desc:  Deactivate the cycle results
************************************************************************/
void CSlotGroupView::DeactivateCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->Deactivate();
    
}   // DeactivateCycleResults


/************************************************************************
*    desc:  Stop the cycle results animation
************************************************************************/
void CSlotGroupView::StartCycleResultsAnimation()
{
    if( m_upCycleResults )
        m_upCycleResults->StartAnimation();
    
}   // StartCycleResultsAnimation


/************************************************************************
*    desc:  Stop the cycle results animation
************************************************************************/
void CSlotGroupView::StopCycleResultsAnimation()
{
    if( m_upCycleResults )
        m_upCycleResults->StopAnimation();
    
}   // StartCycleResultsAnimation


/************************************************************************
*    desc:  Is the cycle results active
************************************************************************/
bool CSlotGroupView::IsCycleResultsActive()
{
    if( m_upCycleResults )
        return m_upCycleResults->IsCycleResultsActive();
    
    return false;
    
}   // IsCycleResultsActive


/************************************************************************
*    desc:  Is the cycle results animating
************************************************************************/
bool CSlotGroupView::IsCycleResultsAnimating()
{
    if( m_upCycleResults )
        return m_upCycleResults->IsAnimating();
    
    return false;
    
}   // IsCycleResultsAnimating


/************************************************************************
*    desc:  Clear the cycle results symbols
************************************************************************/
void CSlotGroupView::ClearCycleResultSymbs()
{
    // Empty by design. Clearing may not be required
}
