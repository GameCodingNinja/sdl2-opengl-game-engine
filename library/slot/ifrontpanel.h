
/************************************************************************
*    FILE NAME:       ifrontpanel.h
*
*    DESCRIPTION:     Interface container for buttons and meters
************************************************************************/

#ifndef __i_front_panel_h__
#define __i_front_panel_h__

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class CUIButton;
class CUIMeter;
class CUIControl;

class iFrontPanel
{
public:
    
    // Init a new game
    virtual void InitGame( uint credits ) = 0;
    
    // Start the bang up
    virtual void StartBangUp( uint credits, uint win ) = 0;
    
    // Are the meters banging
    virtual bool IsBanging() = 0;
    
    // Start the fast bang
    virtual void FastBang() = 0;
    
    // Enable the buttons
    virtual void EnableButtons( bool allowPlay ) = 0;

};

#endif  // __i_front_panel_h__


