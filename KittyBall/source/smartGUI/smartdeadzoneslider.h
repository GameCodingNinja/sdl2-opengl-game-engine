
/************************************************************************
*    FILE NAME:       smartdeadzoneslider.h
*
*    DESCRIPTION:     Class CSmartVSyncCheckBox
************************************************************************/

#ifndef __smart_dead_zone_slider_h__
#define __smart_dead_zone_slider_h__

// Physical component dependency
#include "smartsettingsmenubtn.h"

class CSmartDeadZoneSlider : public CSmartSettingsMenuBtn
{
public:

    // Constructor
    CSmartDeadZoneSlider( CUIControl * pUIControl );
    
    // Handle events
    void HandleEvent( const SDL_Event & rEvent );

    // Called when the control is executed
    void Execute();

};

#endif  // __smart_dead_zone_slider_h__


