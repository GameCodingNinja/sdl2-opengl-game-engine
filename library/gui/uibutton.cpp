/************************************************************************
*    FILE NAME:       uibutton.cpp
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

// Physical component dependency
#include <gui/uibutton.h>

/************************************************************************
*    DESC:  Constructor                                                             
************************************************************************/
CUIButton::CUIButton( const std::string & group ) :
    CUIControl( group )
{
    m_type = NUIControl::ECT_BUTTON;

}   // Constructor


/************************************************************************
*    DESC:  Destructor                                                             
************************************************************************/
CUIButton::~CUIButton()
{
}   // Destructor

