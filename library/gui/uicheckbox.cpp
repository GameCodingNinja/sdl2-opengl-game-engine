
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
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CUICheckBox::~CUICheckBox()
{
}


/************************************************************************
*    desc:  Set the Select state of this control
*  
*    param: EControlState value - control state
************************************************************************/
void CUICheckBox::setToggleState( bool value )
{
    m_toggleState = value;
}


/************************************************************************
*    desc:  Get the Select state of this control
*  
*    ret: EControlState - control state
************************************************************************/
bool CUICheckBox::getToggleState()
{
    return m_toggleState;
}

/************************************************************************
*    desc:  Handle OnSelectExecute message
************************************************************************/
void CUICheckBox::onSelectExecute( const SDL_Event & rEvent )
{
    if( getState() == NUIControl::ECS_SELECTED )
        m_toggleState = !m_toggleState;

    CUIControl::onSelectExecute( rEvent );
}


/************************************************************************
*    desc:  Render the control
************************************************************************/
void CUICheckBox::render( const CMatrix & matrix )
{
    for( size_t i = 0; i < m_spriteDeq.size()-1; ++i )
        m_spriteDeq[i].render( matrix );

    if( m_toggleState == NUIControl::TOGGLE_STATE_ON )
        m_spriteDeq.back().render( matrix );
}
