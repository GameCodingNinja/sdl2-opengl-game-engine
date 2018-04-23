
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Game dependencies
#include "gamesave.h"
#include "../state/startupstate.h"
#include "../state/lobbystate.h"
#include "../state/bigpaybackstate.h"
#include "../state/loadstate.h"
#include "../smartGUI/smartconfirmbtn.h"
#include "../smartGUI/smartresolutionbtn.h"
#include "../smartGUI/smartapplysettingsbtn.h"
#include "../smartGUI/smartfullscreencheckbox.h"
#include "../smartGUI/smartvsynccheckbox.h"
#include "../smartGUI/smartdeadzoneslider.h"
#include "../smartGUI/smartkeybindbtn.h"

// Game lib dependencies
#include <system/device.h>
#include <managers/signalmanager.h>
#include <managers/shadermanager.h>
#include <managers/fontmanager.h>
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <gui/uicheckbox.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <common/color.h>
#include <common/size.h>
#include <common/point.h>
#include <common/build_defs.h>
#include <2d/sprite2d.h>
#include <objectdata/objectdatamanager.h>
#include <slot/betmanager.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CGame::CGame()
{
    CSignalMgr::Instance().Connect_SmartGui( boost::bind(&CGame::SmartGuiControlCreateCallBack, this, _1) );
    CSignalMgr::Instance().Connect_SmartMenu( boost::bind(&CGame::SmartMenuCreateCallBack, this, _1) );  
    CShaderMgr::Instance().Connect_InitShader( boost::bind(&CGame::ShaderInitCallBack, this, _1) );
    
    if( NBDefs::IsDebugMode() )
        CStatCounter::Instance().Connect( boost::bind(&CGame::StatStringCallBack, this, _1) );

}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CGame::~CGame()
{
}   // destructer


/************************************************************************
*    desc:  Init the game
************************************************************************/
void CGame::Init()
{
    CBaseGame::Init();

    // Setup the message filtering
    //SDL_SetEventFilter(FilterEvents, 0);
    
    // Handle some events on startup
    PollEvents();
    
    // Load the game save data
    CGameSave::Instance().Load();
    
    // Init with the total amount of credits
    CBetMgr::Instance().SetCredits( CGameSave::Instance().GetTotalCredits() );

    // Create the startup state
    upGameState.reset( new CStartUpState );
    upGameState->Init();
    
    // Setup the sprite to be used as a debug string
    if( NBDefs::IsDebugMode() )
    {
        // Debug output - Object data list should be loaded at this point
        CObjectDataMgr::Instance().LoadGroup2D( "(debug)" );
        upDebugDisplay.reset( new CSprite2D( CObjectDataMgr::Instance().GetData2D("(debug)", "debugString_font") ) );
        const CSize<float> & rSize = CSettings::Instance().GetDefaultSizeHalf();
        CFontMgr::Instance().LoadFromXML( "data/textures/fonts/font_debug.lst" );
        upDebugDisplay->setPos( CPoint<float>( -(rSize.w-10), rSize.h-10 ) );
        upDebugDisplay->getVisualComponent().setFontProperties( CFontProperties("dejavu_sans_reg_outline_24", NDefs::EHA_HORZ_LEFT, NDefs::EVA_VERT_TOP) );
        upDebugDisplay->transform();
    }

    // Let the games begin
    StartGame();

}   // Init


/************************************************************************
*    desc:  Callback for when a smart gui control is created
************************************************************************/
void CGame::SmartGuiControlCreateCallBack( CUIControl * pUIControl )
{
    if( pUIControl->getFaction() == "decision_btn" )
        pUIControl->setSmartGui( new CSmartConfirmBtn( pUIControl ) );
    
    else if( pUIControl->getFaction() == "key_binding_btn" )
        pUIControl->setSmartGui( new CSmartKeyBindBtn( pUIControl ) );

    else if( pUIControl->getName() == "resolution_btn_lst" )
        pUIControl->setSmartGui( new CSmartResolutionBtn( pUIControl ) );

    else if( pUIControl->getName() == "settings_apply_btn" )
        pUIControl->setSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

    else if( pUIControl->getName() == "full_screen_check_box" )
        pUIControl->setSmartGui( new CSmartScrnCheckBox( pUIControl ) );

    else if( pUIControl->getName() == "v-sync_check_box" )
        pUIControl->setSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

    else if( pUIControl->getName() == "settings_dead_zone_slider" )
        pUIControl->setSmartGui( new CSmartDeadZoneSlider( pUIControl ) );
    
    else if( pUIControl->getName() == "lobby_music_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CGameSave::Instance().GetPlayLobbyMusic() );
        
        pUIControl->connect_executionAction( boost::bind(&CGame::LobbyMusicCallBack, this, _1) );
    }
    else if( pUIControl->getName() == "spin_music_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CGameSave::Instance().GetPlaySpinMusic() );
        
        pUIControl->connect_executionAction( boost::bind(&CGame::SpinMusicCallBack, this, _1) );
    }
    else if( pUIControl->getName() == "stop_sounds_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CGameSave::Instance().GetPlayStopSounds() );
        
        pUIControl->connect_executionAction( boost::bind(&CGame::StopSoundsCallBack, this, _1) );
    }
    
    // Dev Debug call backs
    else if( pUIControl->getName() == "dev-option-v-sync_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CSettings::Instance().GetVSync() );
        
        pUIControl->connect_executionAction( boost::bind(&CGame::DevDebugVSyncCallBack, this, _1) );
    }
    else if( pUIControl->getName() == "debug_string_check_box" )
    {
        NGenFunc::DynCast<CUICheckBox>(pUIControl)->setToggleState( CSettings::Instance().GetDebugStrVisible() );
        
        pUIControl->connect_executionAction( boost::bind(&CGame::DevDebugStringCallBack, this, _1) );
    }

}   // SmartGuiControlCreate


/************************************************************************
*    desc:  Callback for when a smart menu is created
************************************************************************/
void CGame::SmartMenuCreateCallBack( CMenu * pMenu )
{
}   // SmartMenuCreate


/************************************************************************
*    desc:  Game settings call back fuctions
************************************************************************/
void CGame::LobbyMusicCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    CGameSave::Instance().SetPlayLobbyMusic( pCtrl->getToggleState() );
    CGameSave::Instance().OpenSaveClose();
    
    if( (upGameState != nullptr) && (upGameState->GetState() == NGameDefs::EGS_LOBBY) )
        upGameState->AllowMusic( pCtrl->getToggleState() );
    
}   // LobbyMusicCallBack

void CGame::SpinMusicCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    CGameSave::Instance().SetPlaySpinMusic( pCtrl->getToggleState() );
    CGameSave::Instance().OpenSaveClose();
    
    if( (upGameState != nullptr) && (upGameState->GetState() != NGameDefs::EGS_LOBBY) )
        upGameState->AllowMusic( pCtrl->getToggleState() );
    
}   // SpinMusicCallBack

void CGame::StopSoundsCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    CGameSave::Instance().SetPlayStopSounds( pCtrl->getToggleState() );
    CGameSave::Instance().OpenSaveClose();
    
    if( (upGameState != nullptr) && (upGameState->GetState() != NGameDefs::EGS_LOBBY) )
        upGameState->AllowStopSounds( pCtrl->getToggleState() );
    
}   // SpinMusicCallBack


/************************************************************************
*    desc:  Callback for shader init
************************************************************************/
void CGame::ShaderInitCallBack( const std::string & shaderId )
{
    // Init the color for fading in
    CShaderMgr::Instance().SetShaderColor( shaderId, "additive", CColor(0,0,0,1) );
    
}   // ShaderInitCallBack


/************************************************************************
*    desc:  Callback for the state string
************************************************************************/
void CGame::StatStringCallBack( const std::string & statStr )
{
    if( NBDefs::IsMobileDevice() )
    {
        if( CSettings::Instance().GetDebugStrVisible() && upDebugDisplay )
            upDebugDisplay->createFontString( statStr );
    }
    else
    {
        if( !CSettings::Instance().GetFullScreen() )
            SDL_SetWindowTitle( m_pWindow, statStr.c_str() );
        else
            upDebugDisplay->createFontString( statStr );
    }

}   // StatStringCallBack


/************************************************************************
*    desc:  Dev debug settings call back fuctions
************************************************************************/
void CGame::DevDebugVSyncCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    
    CDevice::Instance().EnableVSync( pCtrl->getToggleState() );
    
}   // DevDebugVSyncCallBack

void CGame::DevDebugStringCallBack( CUIControl * pUIControl )
{
    auto pCtrl = NGenFunc::DynCast<CUICheckBox>(pUIControl);
    
    CSettings::Instance().SetDebugStrVisible( pCtrl->getToggleState() );
    
}   // DevDebugVSyncCallBack


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::DoStateChange()
{
    if( upGameState->DoStateChange() )
    {
        // Get the game state we are currently in
        const NGameDefs::EGameState curState = upGameState->GetState();

        // Get the game state we are moving to
        const NGameDefs::EGameState nextState = upGameState->GetNextState();

        // Get the message to the next state
        const CStateMessage stateMessage = upGameState->GetStateMessage();
        
        // Free the current state to ensure no messages will be processed by a state
        upGameState.reset();
        
        // Process any lingering messages so that the new state isn't
        // getting hammered by a bunch of queued up messages
        PollEvents();
        
        if( nextState == NGameDefs::EGS_LOBBY )
            upGameState.reset( new CLobbyState("(lobby)") );
        
        else if( nextState == NGameDefs::EGS_BIG_CASH_BACK )
            upGameState.reset( new CBigPayBackState("(big_pay_back)") );

        else if( nextState == NGameDefs::EGS_GAME_LOAD )
            upGameState.reset( new CLoadState( stateMessage ) );

        else
            throw NExcept::CCriticalException("Error Invalid game state",
                boost::str( boost::format("Next state not valid (cur %d, next %d).\n\n%s\nLine: %s")
                    % curState % nextState % __FUNCTION__ % __LINE__ ));
        
        // Do any pre-game loop init's
        upGameState->Init();
    }

}   // DoStateChange


/************************************************************************
*    desc:  Handle events
************************************************************************/
bool CGame::HandleEvent( const SDL_Event & rEvent )
{
    if( (rEvent.type == SDL_QUIT) || (rEvent.type == SDL_APP_TERMINATING) )
        return true;

    // Filter out these events. Can't do this through the normal event filter
    if( (rEvent.type >= SDL_JOYAXISMOTION) && (rEvent.type <= SDL_JOYBUTTONUP) )
        return false;

    else if( rEvent.type == SDL_CONTROLLERDEVICEADDED )
        CDevice::Instance().AddGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_CONTROLLERDEVICEREMOVED )
        CDevice::Instance().RemoveGamepad( rEvent.cdevice.which );
    
    else if( rEvent.type == SDL_APP_LOWMEMORY )
        DisplayErrorMsg( "Low Memory Error", "The device is experiencing low memory. Try freeing up some apps." );
    
    else if( rEvent.type == SDL_WINDOWEVENT )
    {
        if( (rEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) && upDebugDisplay )
        {
            const CSize<float> & rSize = CSettings::Instance().GetDefaultSizeHalf();
            upDebugDisplay->setPos( CPoint<float>( -(rSize.w-5), rSize.h-7 ) );
            upDebugDisplay->transform();
        }
    }

    // Handle events
    if( upGameState )
        upGameState->HandleEvent( rEvent );

    return false;

}   // HandleEvent


/************************************************************************
*    desc:  Handle any misc processing before the real work is started.
************************************************************************/
void CGame::MiscProcess()
{
    upGameState->MiscProcess();
    
}   // MiscProcess


/************************************************************************
*    desc:  Handle the physics.
************************************************************************/
void CGame::Physics()
{
    upGameState->Physics();
    
}   // Physics


/***************************************************************************
*    decs:  Update animations
****************************************************************************/
void CGame::Update()
{
    upGameState->Update();

}   // Update


/***************************************************************************
*    desc:  Transform game objects
****************************************************************************/
void CGame::Transform()
{
    upGameState->Transform();

}   // Transform


/***************************************************************************
*    decs:  3D/2D Render of content after post process effects
****************************************************************************/
void CGame::Render()
{
    upGameState->PreRender();
    
    upGameState->PostRender();

    if( NBDefs::IsDebugMode() )
    {
        if( NBDefs::IsMobileDevice() )
        {
            if( CSettings::Instance().GetDebugStrVisible() && upDebugDisplay )
                upDebugDisplay->render( CCameraMgr::Instance().GetDefaultProjMatrix() );
        }
        else
        {
            if( CSettings::Instance().GetFullScreen() )
                upDebugDisplay->render( CCameraMgr::Instance().GetDefaultProjMatrix() );
        }
    }

}   // PostGameRender2D


/***************************************************************************
*    decs:  Filter out events we don't want
****************************************************************************/
int FilterEvents( void * userdata, SDL_Event * pEvent )
{
    // Return 0 to indicate that the event should be removed from the event queue

    // Do our own dead zone filtering
    if( pEvent->type == SDL_CONTROLLERAXISMOTION )
    {
        // Analog stick max values -32768 to 32767
        const int deadZone = CSettings::Instance().GetGamePadStickDeadZone() * 
            defs_ANALOG_PERCENTAGE_CONVERTION;

        if( ((pEvent->caxis.axis >= SDL_CONTROLLER_AXIS_LEFTX) &&
            (pEvent->caxis.axis <= SDL_CONTROLLER_AXIS_RIGHTY)) &&
            (((pEvent->caxis.value >= 0) && (pEvent->caxis.value < deadZone)) ||
            ((pEvent->caxis.value <= 0) && (pEvent->caxis.value > -deadZone))) )
            return 0;
    }

    // Return 1 to indicate that the event should stay in the event queue
    return 1;

}   // FilterEvents
