
/************************************************************************
*    FILE NAME:       scrollparam.h
*
*    DESCRIPTION:     Class for handling scroll parameter data
************************************************************************/

#ifndef __scroll_param_h__
#define __scroll_param_h__

// Standard lib dependencies
#include <map>

// Forward declaration(s)
struct XMLNode;

class CScrollParam
{
public:

    // Constructor
    CScrollParam();
    
    // Load the scroll data from node
    void LoadFromNode( const XMLNode & node );

    // Does this menu or control support scrolling this message?
    bool CanScroll( int msg );

    // Get/Set Start delay
    int GetStartDelay();

    // Get/Set scroll delay
    int GetScrollDelay();

    // Get the message
    int GetMsg();

private:

    // Map that holds the scroll messages allows by this menu or control
    std::map<int, int> m_scrollTypesMap;

    // The delay of the first scroll message
    int m_startDelay;

    // The delay of the rest of the scroll messages
    int m_scrollDelay;

    // The scroll message to send from the timer
    int m_msg;
};

#endif  // __scroll_param_h__


