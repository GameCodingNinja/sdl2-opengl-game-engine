
/************************************************************************
*    FILE NAME:       uibuttonlist.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibuttonlist.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/uicontroldefs.h>
#include <2d/sprite2d.h>
#include <utilities/genfunc.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectvisualdata2d.h>
#include <utilities/exceptionhandling.h>
#include <common/defs.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CUIButtonList::CUIButtonList( const std::string & group ) :
    CUISubControl( group ),
    m_activeIndex(0),
    m_imageLstIndex(-1)
{
    m_type = NUIControl::ECT_BUTTON_LIST;

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CUIButtonList::~CUIButtonList()
{
}	// destructor


/************************************************************************
*    desc:  Load the control info from XML node
************************************************************************/
void CUIButtonList::LoadFromNode( const XMLNode & node )
{
    // Call the parent
    CUISubControl::LoadFromNode( node );
    
    // See what the control will respond to
    const XMLNode actionResponse = node.getChildNode( "actionResponse" );
    if( actionResponse.isAttributeSet( "up" ) &&
        (std::strcmp( actionResponse.getAttribute("up"), "true" ) == 0) )
    {
        m_ActionMask.Add( EAR_UP );
    }
    
    if( actionResponse.isAttributeSet( "down" ) &&
        (std::strcmp( actionResponse.getAttribute("down"), "true" ) == 0) )
    {
        m_ActionMask.Add( EAR_DOWN );
    }
    
    if( actionResponse.isAttributeSet( "left" ) &&
        (std::strcmp( actionResponse.getAttribute("left"), "true" ) == 0) )
    {
        m_ActionMask.Add( EAR_LEFT );
    }
    
    if( actionResponse.isAttributeSet( "right" ) &&
        (std::strcmp( actionResponse.getAttribute("right"), "true" ) == 0) )
    {
        m_ActionMask.Add( EAR_RIGHT );
    }
}   // LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
************************************************************************/
void CUIButtonList::LoadControlFromNode( const XMLNode & node )
{
    // Call the parent
    CUISubControl::LoadControlFromNode( node );

    // See if there is an image list
    for( size_t i = 0; i < m_spriteDeq.size(); ++i )
    {
	if( m_spriteDeq[i].GetObjectData().GetVisualData().GetFrameCount() > 1 )
        {
            m_imageLstIndex = i;
            break;
        }
    }

}   // LoadControlFromNode


/************************************************************************
*    desc:  Inc/Dec control
************************************************************************/
void CUIButtonList::Inc()
{
    NGenFunc::DispatchEvent( 
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_SELECTED,
            (void *)m_pSubControlVec[NUIControl::BTN_INC] );
    
}   // Inc

void CUIButtonList::Dec()
{
    NGenFunc::DispatchEvent( 
        NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
        NUIControl::ECS_SELECTED,
        (void *)m_pSubControlVec[NUIControl::BTN_DEC] );
    
}   // Inc


/************************************************************************
*    desc:  Handle Onmessage
************************************************************************/
void CUIButtonList::OnDownAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.IsSet( EAR_DOWN ) )
        Dec();

}   // OnDownAction

void CUIButtonList::OnUpAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.IsSet( EAR_UP ) )
        Inc();

}   // OnUpAction

void CUIButtonList::OnLeftAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.IsSet( EAR_LEFT ) )
        Dec();

}   // OnLeftAction

void CUIButtonList::OnRightAction( const SDL_Event & rEvent )
{
    if( (rEvent.user.code == NDefs::EAP_DOWN) && m_ActionMask.IsSet( EAR_RIGHT ) )
        Inc();

}   // OnRightAction


/************************************************************************
*    desc:  Handle OnLeftScroll message
************************************************************************/
void CUIButtonList::OnDownScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.IsSet( EAR_DOWN ) )
        Dec();

}   // OnDownScroll

void CUIButtonList::OnUpScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.IsSet( EAR_UP ) )
        Inc();

}   // OnUpScroll

void CUIButtonList::OnLeftScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.IsSet( EAR_LEFT ) )
        Dec();

}   // OnLeftScrol

void CUIButtonList::OnRightScroll( const SDL_Event & rEvent )
{
    if( m_ActionMask.IsSet( EAR_RIGHT ) )
        Inc();

}   // OnRightScroll


/************************************************************************
*    desc:  Handle OnStateChange message
************************************************************************/
void CUIButtonList::OnStateChange( const SDL_Event & rEvent )
{
    CUISubControl::OnStateChange( rEvent );

    NUIControl::EControlState state = NUIControl::EControlState(rEvent.user.code);
    
    if( state == NUIControl::ECS_SELECTED )
    {
        if( (void *)m_pSubControlVec[NUIControl::BTN_DEC] == rEvent.user.data1 )
        {
            // Dec the list
            DecList();

            // Update the display
            UpdateDisplay( m_activeIndex );

            // Execute smart gui
            SmartExecuteAction();
        }
        else if( (void *)m_pSubControlVec[NUIControl::BTN_INC] == rEvent.user.data1 )
        {
            // Inc the list
            IncList();

            // Update the display
            UpdateDisplay( m_activeIndex );

            // Execute smart gui
            SmartExecuteAction();
        }
    }

}   // OnStateChange


/************************************************************************
*    desc:  Inc the list
************************************************************************/
void CUIButtonList::IncList()
{
    if( !m_stringVec.empty() )
        m_activeIndex = (m_activeIndex + 1) % m_stringVec.size();

}   // IncList

void CUIButtonList::DecList()
{
    if( !m_stringVec.empty() )
    {
        if( m_activeIndex > 0 )
            m_activeIndex = (m_activeIndex - 1) % m_stringVec.size();
        else
            m_activeIndex = static_cast<int>(m_stringVec.size()-1);
    }

}   // DeList


/************************************************************************
*    desc:  Update the display
************************************************************************/
void CUIButtonList::UpdateDisplay( int index )
{
    m_activeIndex = index;

    CreateFontString( m_activeIndex );

    if( m_imageLstIndex > -1 )
        m_spriteDeq.at(m_imageLstIndex).GetVisualComponent().SetFrame( m_activeIndex );

}  // UpdateDisplay


/************************************************************************
*    desc: Set the first inactive control to be active
*    NOTE: Don't want this functuality for the button list
************************************************************************/
bool CUIButtonList::ActivateFirstInactiveControl()
{
    return CUIControl::ActivateFirstInactiveControl();

}   // ActivateFirstInactiveControl


/************************************************************************
*    desc:  Get the active index
************************************************************************/
int CUIButtonList::GetActiveIndex() const
{
    return m_activeIndex;

}   // GetActiveIndex
