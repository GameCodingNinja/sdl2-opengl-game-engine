
/************************************************************************
*    FILE NAME:       uicheckbox.cpp
*
*    DESCRIPTION:     Class for user interface check box buttons
************************************************************************/

// Physical component dependency
#include <gui/uicheckbox.h>

// Game lib dependencies
#include <2d/sprite2d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CUICheckBox::CUICheckBox( const std::string & group ) :
    CUIControl( group ),
    m_toggleState(false)
{
    m_type = NUIControl::ECT_CHECK_BOX;

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CUICheckBox::~CUICheckBox()
{
}	// destructor


/************************************************************************
*    desc:  Set the Select state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUICheckBox::SetToggleState( bool value )
{
    m_toggleState = value;

}	// SetSelectState


/************************************************************************
*    desc:  Get the Select state of this control
*  
*    ret: EControlState - control state
************************************************************************/
bool CUICheckBox::GetToggleState()
{
    return m_toggleState;

}	// GetSelectState

/************************************************************************
*    desc:  Handle OnSelectExecute message
************************************************************************/
void CUICheckBox::OnSelectExecute( const SDL_Event & rEvent )
{
    if( GetState() == NUIControl::ECS_SELECTED )
        m_toggleState = !m_toggleState;

    CUIControl::OnSelectExecute( rEvent );

}   // OnSelectExecute


/************************************************************************
*    desc:  Render the control
************************************************************************/
void CUICheckBox::Render( const CMatrix & matrix )
{
    for( size_t i = 0; i < m_spriteDeq.size()-1; ++i )
        m_spriteDeq[i].Render( matrix );

    if( m_toggleState == NUIControl::TOGGLE_STATE_ON )
        m_spriteDeq.back().Render( matrix );

}   // Render



