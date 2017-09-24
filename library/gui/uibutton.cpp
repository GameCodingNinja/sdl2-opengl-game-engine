/************************************************************************
*    FILE NAME:       uibutton.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibutton.h>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CUIButton::CUIButton( const std::string & group ) :
    CUIControl( group )
{
    m_type = NUIControl::ECT_BUTTON;

}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CUIButton::~CUIButton()
{
}   // Destructor

