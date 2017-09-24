
/************************************************************************
*    FILE NAME:       slotgroup.cpp
*
*    DESCRIPTION:     Controller class for the slot group
************************************************************************/

// Physical component dependency
#include <slot/slotgroup.h>

// Game lib dependencies
#include <slot/slotmathmanager.h>
#include <slot/playresult.h>
#include <slot/symbol2d.h>
#include <slot/reelgroupview.h>
#include <slot/wheelgroupview.h>
#include <slot/icycleresults.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <iostream>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotGroup::CSlotGroup( const CSlotMath & rSlotMath, CPlayResult & rPlayResult ) :
    m_slotGroupModel( rSlotMath, rPlayResult ),
    m_rPlayResult( rPlayResult )
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotGroup::~CSlotGroup()
{
}   // destructor


/************************************************************************
*    desc:  Create the slot group. Math and video slot strips
************************************************************************/
void CSlotGroup::Create(
    const NSlotDefs::ESlotDevice slotDevice,
    const std::string & slotStripSetId,
    const std::string & paytableSetId,
    const XMLNode & viewSlotCfgNode,
    const XMLNode & viewSpinProfileCfgNode,
    CSymbolSetView & rSymbolSetView,
    std::unique_ptr<iCycleResults> upCycleResults )
{
    // Create the model slot strips
    m_slotGroupModel.Create( slotStripSetId, paytableSetId );
    
    // Allocate the slot device
    if( slotDevice == NSlotDefs::ED_REEL )
        m_upSlotGroupView.reset( new CReelGroupView(m_slotGroupModel) );
    
    else if( slotDevice == NSlotDefs::ED_WHEEL )
        m_upSlotGroupView.reset( new CWheelGroupView(m_slotGroupModel) );
    
    else
        throw NExcept::CCriticalException("Slot Group Create Error!",
            boost::str( boost::format("Undefined slot device!\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    // Create the view slot strips
    m_upSlotGroupView->Create( viewSlotCfgNode, rSymbolSetView );
    
    // Load the spin profile from XML node
    m_upSlotGroupView->LoadSpinProfileFromNode( viewSpinProfileCfgNode );
    
    if( upCycleResults )
    {
        m_upCycleResults = std::move(upCycleResults);
        m_upCycleResults->Init( m_upSlotGroupView.get(), &m_rPlayResult );
    }
    
}   // Create


/************************************************************************
*    desc:  Start the cycle results
************************************************************************/
void CSlotGroup::StartCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->Start();
    
}   // StartCycleResults


/************************************************************************
*    desc:  Stop the cycle results
************************************************************************/
void CSlotGroup::StopCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->Stop();
    
}   // StopCycleResults


/************************************************************************
*    desc:  Stop the cycle results animation
************************************************************************/
void CSlotGroup::StartCycleResultsAnimation()
{
    if( m_upCycleResults )
        m_upCycleResults->StartAnimation();
    
}   // StartCycleResultsAnimation


/************************************************************************
*    desc:  Stop the cycle results animation
************************************************************************/
void CSlotGroup::StopCycleResultsAnimation()
{
    if( m_upCycleResults )
        m_upCycleResults->StopAnimation();
    
}   // StartCycleResultsAnimation


/************************************************************************
*    desc:  Is the cycle results animating
************************************************************************/
bool CSlotGroup::IsCycleResultsAnimating()
{
    if( m_upCycleResults )
        return m_upCycleResults->IsAnimating();
    
    return false;
    
}   // IsCycleResultsAnimating


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CSlotGroup::Update()
{
    m_upSlotGroupView->Update();
    
    if( m_upCycleResults )
        m_upCycleResults->Update();
    
}   // Update


/************************************************************************
*    desc:  Get the slot group model
************************************************************************/
CSlotGroupModel & CSlotGroup::GetModel()
{
    return m_slotGroupModel;
    
}   // GetModel


/************************************************************************
*    desc:  Get the slot group view
************************************************************************/
CSlotGroupView * CSlotGroup::GetView()
{
    return m_upSlotGroupView.get();
    
}   // GetView
