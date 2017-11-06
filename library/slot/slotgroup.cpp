
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
#include <slot/playresult.h>
#include <slot/slotgroupmodel.h>
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
CSlotGroup::CSlotGroup(
    std::shared_ptr<CSlotGroupModel> spSlotGroupModel,
    std::shared_ptr<CSlotGroupView> spSlotGroupView,
    std::unique_ptr<iCycleResults> upCycleResults ) :
        m_spSlotGroupModel( spSlotGroupModel ),
        m_spSlotGroupView( spSlotGroupView ),
        m_upCycleResults( std::move(upCycleResults) )
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotGroup::~CSlotGroup()
{
}   // destructor


/************************************************************************
*    desc:  Activate the cycle results
************************************************************************/
void CSlotGroup::ActivateCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->Activate();
    
}   // ActivateCycleResults


/************************************************************************
*    desc:  Deactivate the cycle results
************************************************************************/
void CSlotGroup::DeactivateCycleResults()
{
    if( m_upCycleResults )
        m_upCycleResults->Deactivate();
    
}   // DeactivateCycleResults


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
*    desc:  Is the cycle results active
************************************************************************/
bool CSlotGroup::IsCycleResultsActive()
{
    if( m_upCycleResults )
        return m_upCycleResults->IsCycleResultsActive();
    
    return false;
    
}   // IsCycleResultsActive


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
    m_spSlotGroupView->Update();
    
    if( m_upCycleResults )
        m_upCycleResults->Update();
    
}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CSlotGroup::Transform()
{
    m_spSlotGroupView->Transform();
}


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CSlotGroup::Render( const CMatrix & matrix )
{
    m_spSlotGroupView->Render( matrix );
    
    if( m_upCycleResults )
        m_upCycleResults->Render( matrix );
    
}   // Render


/************************************************************************
*    desc:  Get the slot group model
************************************************************************/
CSlotGroupModel * CSlotGroup::GetModel()
{
    return m_spSlotGroupModel.get();
    
}   // GetModel


/************************************************************************
*    desc:  Get the slot group view
************************************************************************/
CSlotGroupView * CSlotGroup::GetView()
{
    return m_spSlotGroupView.get();
    
}   // GetView
