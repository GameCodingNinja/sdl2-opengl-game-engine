/************************************************************************
*    FILE NAME:       uilabel.cpp
*
*    DESCRIPTION:     Class for user interface labels
************************************************************************/

// Physical component dependency
#include <gui/uilabel.h>

/************************************************************************
*    DESC:  Constructor                                                             
************************************************************************/
CUILabel::CUILabel( const std::string & group ) :
    CUIControl( group )
{
    m_type = NUIControl::ECT_LABEL;

}   // Constructor


/************************************************************************
*    DESC:  Destructor                                                             
************************************************************************/
CUILabel::~CUILabel()
{
}   // Destructor

