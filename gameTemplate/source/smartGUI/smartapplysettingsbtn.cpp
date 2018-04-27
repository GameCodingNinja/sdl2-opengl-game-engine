
/************************************************************************
*    FILE NAME:       smartapplysettingsbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include "smartapplysettingsbtn.h"

// Game dependencies local
#include "smartresolutionbtn.h"

// Game dependencies
#include "../game/game.h"

// Game lib dependencies
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/uicheckbox.h>
#include <gui/uibuttonlist.h>
#include <gui/menu.h>
#include <gui/uislider.h>
#include <common/defs.h>
//#include <managers/hudmanager.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSmartApplySettingsBtn::CSmartApplySettingsBtn( CUIControl * pUIControl ) :
    CSmartSettingsMenuBtn( pUIControl )
{
}


/***************************************************************************
*    decs:  Handle events
****************************************************************************/
void CSmartApplySettingsBtn::handleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
        m_pUIControl->changeState(NUIControl::ECS_DISABLED);
}


/***************************************************************************
*    decs:  Called when the control is executed - Applies the settings
****************************************************************************/
void CSmartApplySettingsBtn::execute()
{
    bool resChanged(false), fullScreenChanged(false);
    bool settingsChangeMade(false);

    // Get the settings menu
    CMenu & rMenu = CMenuManager::Instance().getActiveMenu();

    // Check for the resolution button list
    CSmartResolutionBtn & rSmartRes = (*dynamic_cast<CSmartResolutionBtn *>(rMenu.getPtrToControl( "resolution_btn_lst" )->getSmartGuiPtr()));
    if( rSmartRes.wasResolutionChanged() )
    {
        rSmartRes.setResolutionChange();
        resChanged = true;
        settingsChangeMade = true;
    }
    
    // Get the control
    CUIControl * pControl;

    // Check for the full screen check box
    pControl = rMenu.getPtrToControl( "full_screen_check_box" );
    const bool fullScreenState = (*dynamic_cast<CUICheckBox *>(pControl)).getToggleState();
    const bool oldFullScreenState = CSettings::Instance().getFullScreen();
    if( CSettings::Instance().getFullScreen() != fullScreenState )
    {
        CSettings::Instance().setFullScreen( fullScreenState );
        fullScreenChanged = true;
        settingsChangeMade = true;
    }

    // Check for the v-sync check box
    pControl = rMenu.getPtrToControl( "v-sync_check_box" );
    const bool vsyncState = (*dynamic_cast<CUICheckBox *>(pControl)).getToggleState();
    if( CSettings::Instance().getVSync() != vsyncState )
    {
        CSettings::Instance().setVSync( vsyncState );
        CDevice::Instance().enableVSync( vsyncState );
        settingsChangeMade = true;
    }

    // Check for dead zone slider
    pControl = rMenu.getPtrToControl( "settings_dead_zone_slider" );
    int sliderValue = (*dynamic_cast<CUISlider *>(pControl)).getValue();
    if( CSettings::Instance().getGamePadStickDeadZone() != sliderValue )
    {
        CSettings::Instance().setGamePadStickDeadZone( sliderValue );
        settingsChangeMade = true;
    }

    if( resChanged )
    {
        // SDL2 doesn't allow res change in full screen
        // so take us out of full screen mode for res changes
        if( oldFullScreenState && fullScreenState )
            CDevice::Instance().setFullScreen( false );

        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);

        mode.w = CSettings::Instance().getSize().getW();
        mode.h = CSettings::Instance().getSize().getH();
        mode.refresh_rate = 0;

        SDL_SetWindowDisplayMode( CDevice::Instance().getWindow(), &mode );

        if( fullScreenChanged || fullScreenState )
            CDevice::Instance().setFullScreen( fullScreenState );

        SDL_SetWindowSize(
            CDevice::Instance().getWindow(),
            CSettings::Instance().getSize().getW(),
            CSettings::Instance().getSize().getH() );

        SDL_SetWindowPosition(
            CDevice::Instance().getWindow(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED );

        // Reset the transform to recalculate mouse collision
        CMenuManager::Instance().resetTransform();

        // Reset the dynamic positions of menus
        CMenuManager::Instance().resetDynamicOffset();

        // Need to reset the view port the changing the resolution
        glViewport(0, 0, CSettings::Instance().getSize().getW(), CSettings::Instance().getSize().getH());
    }
    else if( fullScreenChanged )
    {
        CDevice::Instance().setFullScreen( fullScreenState );
    }

    // Only save if a change was actually made
    if( settingsChangeMade )
        CSettings::Instance().saveSettings();

    m_pUIControl->changeState(NUIControl::ECS_DISABLED);

    // Reactivate the menu now that we are done
    NGenFunc::DispatchEvent( NMenu::EGE_MENU_REACTIVATE );
}
