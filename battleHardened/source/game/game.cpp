
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

// Physical component dependency
#include "game.h"

// Game dependencies
#include "../state/startupstate.h"
#include "../state/titlescreenstate.h"
#include "../state/loadstate.h"
#include "../state/level1state.h"
#include "../smartGUI/smartconfirmbtn.h"

// Game lib dependencies
#include <system/device.h>
#include <managers/signalmanager.h>
#include <managers/shadermanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <2d/actorsprite2d.h>
#include <common/color.h>
#include <common/build_defs.h>
#include <objectdata/objectdatamanager.h>

// Game dependencies
#include "../ai/playershipai.h"
#include "../ai/projectileai.h"

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CGame::CGame()
{
    CSignalMgr::Instance().Connect_SmartGui( boost::bind(&CGame::SmartGuiControlCreateCallBack, this, _1) );
    CSignalMgr::Instance().Connect_SmartMenu( boost::bind(&CGame::SmartMenuCreateCallBack, this, _1) );
    CSignalMgr::Instance().Connect_AICreate( boost::bind(&CGame::AICreateCallBack, this, _1, _2) );
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

    // Create the startup state
    upGameState.reset( new CStartUpState );
    upGameState->Init();

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
    
}   // SmartGuiControlCreate


/************************************************************************
*    desc:  Callback for when a smart menu is created
************************************************************************/
void CGame::SmartMenuCreateCallBack( CMenu * pMenu )
{

}   // SmartMenuCreateCallBack


/***************************************************************************
*    decs:  Call back function to create actor ai
****************************************************************************/
void CGame::AICreateCallBack( const std::string & aiName, iSprite * pSprite )
{
    if( aiName == "player_ship" )
        pSprite->setAI( new CPlayerShipAI( pSprite ) );
    
    else if( aiName == "player_projectile" )
        pSprite->setAI( new CProjectileAI( pSprite ) );
    
}   // AICreateCallBack


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
    if( !CSettings::Instance().GetFullScreen() )
        SDL_SetWindowTitle( m_pWindow, statStr.c_str() );

}   // StatStringCallBack


/***************************************************************************
*    decs:  Handle the state change
****************************************************************************/
void CGame::DoStateChange()
{
    if( upGameState->DoStateChange() )
    {
        // Get the game state we are moving to
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
        
        if( nextState == NGameDefs::EGS_TITLE_SCREEN )
            upGameState.reset( new CTitleScreenState );

        else if( nextState == NGameDefs::EGS_GAME_LOAD )
            upGameState.reset( new CLoadState( stateMessage ) );

        else if( nextState == NGameDefs::EGS_Level_1 )
            upGameState.reset( new CLevel1State );

        else
            throw NExcept::CCriticalException("Error Invalid game state",
                boost::str( boost::format("Next state not valid (cur %d, next %d).\n\n%s\nLine: %s")
                    % curState % nextState % __FUNCTION__ % __LINE__ ));
        
        // Do any pre-game loop init's
        upGameState->Init();
    }

}   // DoStateChange */


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
    
    // In a traditional game, want the pause menu to display when the game is sent to the background
    else if( NBDefs::IsDebugMode() && (rEvent.type == SDL_APP_WILLENTERBACKGROUND) && !CMenuManager::Instance().isMenuActive() )
        NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION );

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
*    decs:  3D/2D Render of game content
****************************************************************************/
void CGame::Render()
{
    upGameState->PreRender();
    upGameState->PostRender();

}   // Render


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
