
/************************************************************************
*    FILE NAME:       scrollparam.cpp
*
*    DESCRIPTION:     Class for handling scroll parameter data
************************************************************************/

// Physical component dependency
#include <gui/scrollparam.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <utilities/xmlParser.h>

// Standard lib dependencies
#include <cstring>
#include <cstdlib>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CScrollParam::CScrollParam() : 
    m_startDelay(-1),
    m_scrollDelay(-1), 
    m_msg(-1)
{
}   // constructor


/************************************************************************
*    DESC:  Load the scroll data from node
************************************************************************/
void CScrollParam::loadFromNode( const XMLNode & node )
{
    if( !node.isEmpty() )
    {
        m_startDelay = std::atoi(node.getAttribute( "startDelay" ));
        m_scrollDelay = std::atoi(node.getAttribute( "scrollDelay" ));

        if( node.isAttributeSet( "up" ) && (std::strcmp( node.getAttribute( "up" ), "true" ) == 0) )
            m_scrollTypesMap.emplace( NMenu::EGE_MENU_UP_ACTION, NMenu::EGE_MENU_SCROLL_UP );

        if( node.isAttributeSet( "down" ) && (std::strcmp( node.getAttribute( "down" ), "true" ) == 0) )
            m_scrollTypesMap.emplace( NMenu::EGE_MENU_DOWN_ACTION, NMenu::EGE_MENU_SCROLL_DOWN );

        if( node.isAttributeSet( "left" ) && (std::strcmp( node.getAttribute( "left" ), "true" ) == 0) )
            m_scrollTypesMap.emplace( NMenu::EGE_MENU_LEFT_ACTION, NMenu::EGE_MENU_SCROLL_LEFT );

        if( node.isAttributeSet( "right" ) && (std::strcmp( node.getAttribute( "right" ), "true" ) == 0) )
            m_scrollTypesMap.emplace( NMenu::EGE_MENU_RIGHT_ACTION, NMenu::EGE_MENU_SCROLL_RIGHT );
    }
    
}   // LoadFromNode


/************************************************************************
*    DESC:  Clear out the data
************************************************************************/
bool CScrollParam::canScroll( int msg )
{
    m_msg = -1;

    auto iter = m_scrollTypesMap.find(msg);
    if( iter != m_scrollTypesMap.end() )
    {
        m_msg = iter->second;
        return true;
    }

    return false;
    
}   // CanScroll


/************************************************************************
*    DESC:  Clear out the data
************************************************************************/
int CScrollParam::getStartDelay()
{
    return m_startDelay;
}


/************************************************************************
*    DESC:  Clear out the data
************************************************************************/
int CScrollParam::getScrollDelay()
{
    return m_scrollDelay;
}


/************************************************************************
*    DESC:  Get the message
************************************************************************/
int CScrollParam::getMsg()
{
    return m_msg;
}
