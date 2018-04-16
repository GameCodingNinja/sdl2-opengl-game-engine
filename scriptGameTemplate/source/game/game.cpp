
/************************************************************************
*    FILE NAME:       game.cpp
*
*    DESCRIPTION:     CGame class
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include "game.h"

// Game dependencies
/*#include "../state/startupstate.h"
#include "../state/titlescreenstate.h"
#include "../state/loadstate.h"
#include "../state/runstate.h"
#include "../ai/ballai.h"
#include "../smartGUI/smartconfirmbtn.h"
#include "../smartGUI/smartresolutionbtn.h"
#include "../smartGUI/smartapplysettingsbtn.h"
#include "../smartGUI/smartfullscreencheckbox.h"
#include "../smartGUI/smartvsynccheckbox.h"
#include "../smartGUI/smartdeadzoneslider.h"
#include "../smartGUI/smartkeybindbtn.h"*/

// Game lib dependencies
#include <system/device.h>
#include <managers/signalmanager.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/meshmanager.h>
#include <managers/spritestrategymanager.h>
#include <gui/menumanager.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <utilities/highresolutiontimer.h>
#include <2d/isprite2d.h>
#include <common/color.h>
#include <common/build_defs.h>
#include <objectdata/objectdatamanager.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptplaylist.h>
#include <script/scriptpoint.h>
#include <script/scriptglobals.h>
#include <script/scriptactorsprite2d.h>
#include <script/scriptsprite2d.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptmenu.h>
#include <script/scriptshadermanager.h>
#include <script/scriptobjectdatamanager.h>
#include <script/scriptstrategymanager.h>
#include <system/device.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

// Boost lib dependencies
#include <boost/bind.hpp>
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CGame::CGame()
    : m_pWindow(nullptr),
      m_context(nullptr),
      m_gameRunning(false),
      m_clearBufferMask(0)
{
    CSignalMgr::Instance().Connect_SmartGui( boost::bind(&CGame::SmartGuiControlCreateCallBack, this, _1) );
    CSignalMgr::Instance().Connect_SmartMenu( boost::bind(&CGame::SmartMenuCreateCallBack, this, _1) );
    CSignalMgr::Instance().Connect_AICreate( boost::bind(&CGame::AICreateCallBack, this, _1, _2) );
    
    if( NBDefs::IsDebugMode() )
        CStatCounter::Instance().Connect( boost::bind(&CGame::StatStringCallBack, this, _1) );

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CGame::~CGame()
{
    // Destroy the OpenGL context
    if( m_context != nullptr )
        SDL_GL_DeleteContext( m_context );

    // Destroy window
    if( m_pWindow != nullptr )
        SDL_DestroyWindow( m_pWindow );

    // Quit SDL subsystems
    SDL_Quit();
    
}   // destructor


/***************************************************************************
*   desc:  Create the game Window
 ****************************************************************************/
void CGame::Create()
{
    // Create the window and OpenGL context
    CDevice::Instance().Create();

    // Get local copies of the device handles
    m_pWindow = CDevice::Instance().GetWindow();
    m_context = CDevice::Instance().GetContext();

    // Game start init
    Init();

}   // Create


/************************************************************************
*    desc:  OpenGL Init
************************************************************************/
void CGame::OpenGLInit()
{
    // Init the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Init the stencil clear mask based on the bit size of the mask
    // Stencil buffer can only be 1 or 8 bits per pixel
    if( CSettings::Instance().GetStencilBufferBitSize() == 1 )
        glStencilMask(0x1);
    else if( CSettings::Instance().GetStencilBufferBitSize() == 8 )
        glStencilMask(0xff);

    // Cull the back face
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Make the zero texture the active texture
    glActiveTexture(GL_TEXTURE0);

    // Init the clear buffer mask
    if( CSettings::Instance().GetClearTargetBuffer() )
        m_clearBufferMask |= GL_COLOR_BUFFER_BIT;

    if( CSettings::Instance().GetEnableDepthBuffer() )
        m_clearBufferMask |= GL_DEPTH_BUFFER_BIT;
    
    if( CSettings::Instance().GetClearStencilBuffer() )
        m_clearBufferMask |= GL_STENCIL_BUFFER_BIT;
        
    if( CSettings::Instance().GetEnableDepthBuffer() )
        glEnable( GL_DEPTH_TEST );
    
    // Clear the back buffer and flip it prior to showing the window
    // Keeps us from seeing a flash or flicker of pre init junk
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( m_pWindow );
    
    // Show the window
    CDevice::Instance().ShowWindow( true );
    
    // Display a black screen
    glClear( GL_COLOR_BUFFER_BIT );
    SDL_GL_SwapWindow( m_pWindow );

}   // OpenGLInit


/************************************************************************
*    desc:  Init the game
************************************************************************/
void CGame::Init()
{
    OpenGLInit();

    // Setup the message filtering
    SDL_SetEventFilter(FilterEvents, 0);
    
    // Handle some events on startup
    PollEvents();
    
    // Load the script list table
    CScriptManager::Instance().LoadListTable( "source/scriptListTable.lst" );
    
    // Register the script items
    RegisterStdString( CScriptManager::Instance().GetEnginePtr() );
    RegisterScriptArray( CScriptManager::Instance().GetEnginePtr(), false );
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptActorSprite2D::Register();
    NScriptSprite2D::Register();
    NScriptSound::Register();
    NScriptPlayLst::Register();
    NScriptSoundManager::Register();
    NScriptMenu::Register();
    NScriptShaderManager::Register();
    NScriptObjectDataManager::Register();
    NScriptStrategyManager::Register();

    CScriptManager::Instance().LoadGroup("(main)");
    CScriptManager::Instance().Prepare("(main)", "main");
    
    CHighResTimer::Instance().CalcElapsedTime();
    
    // Let the games begin
    StartGame();

}   // Init


/***************************************************************************
*   desc:  Poll for game events
****************************************************************************/
void CGame::PollEvents()
{
    // Event handler
    SDL_Event msgEvent;

    // Handle events on queue
    while( SDL_PollEvent( &msgEvent ) )
    {
        // let the game handle the event
        // turns true on quit
        if( HandleEvent( msgEvent ) )
        {
            // Stop the game
            m_gameRunning = false;

            // Hide the window to give the impression of a quick exit
            CDevice::Instance().ShowWindow( false );

            break;
        }
    }
}   // PollEvents


/***************************************************************************
*   desc:  Main game loop
****************************************************************************/
bool CGame::GameLoop()
{
    // Poll for game events
    PollEvents();
    
    // Get our elapsed time
    CHighResTimer::Instance().CalcElapsedTime();
    
    // Main script update
    CScriptManager::Instance().Update();
    
    if( m_gameRunning )
    {
        
        CSpriteStrategyMgr::Instance().MiscProcess();
        CSpriteStrategyMgr::Instance().Update();
        CSpriteStrategyMgr::Instance().Transform();
        
        // Clear the buffers
        glClear( m_clearBufferMask );
        
        const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
        CSpriteStrategyMgr::Instance().Render( matrix );//m_camera.GetMatrix() * m_camera.GetProjectionMatrix() );
        
        // Do the back buffer swap
        SDL_GL_SwapWindow( m_pWindow );
        
        // Unbind everything after a round of rendering
        CShaderMgr::Instance().Unbind();
        CTextureMgr::Instance().Unbind();
        CVertBufMgr::Instance().Unbind();

        // Inc the cycle
        if( NBDefs::IsDebugMode() )
            CStatCounter::Instance().IncCycle();
    }
    
    return m_gameRunning;
    
}   // ScriptGameLoop


/************************************************************************
*    desc:  Callback for when a smart gui control is created
************************************************************************/
void CGame::SmartGuiControlCreateCallBack( CUIControl * pUIControl )
{
    /*if( pUIControl->GetFaction() == "decision_btn" )
        pUIControl->SetSmartGui( new CSmartConfirmBtn( pUIControl ) );
    
    else if( pUIControl->GetFaction() == "key_binding_btn" )
        pUIControl->SetSmartGui( new CSmartKeyBindBtn( pUIControl ) );

    else if( pUIControl->GetName() == "resolution_btn_lst" )
        pUIControl->SetSmartGui( new CSmartResolutionBtn( pUIControl ) );

    else if( pUIControl->GetName() == "settings_apply_btn" )
        pUIControl->SetSmartGui( new CSmartApplySettingsBtn( pUIControl ) );

    else if( pUIControl->GetName() == "full_screen_check_box" )
        pUIControl->SetSmartGui( new CSmartScrnCheckBox( pUIControl ) );

    else if( pUIControl->GetName() == "v-sync_check_box" )
            pUIControl->SetSmartGui( new CSmartVSyncCheckBox( pUIControl ) );

    else if( pUIControl->GetName() == "settings_dead_zone_slider" )
            pUIControl->SetSmartGui( new CSmartDeadZoneSlider( pUIControl ) );*/

}   // SmartGuiControlCreateCallBack


/************************************************************************
*    desc:  Callback for when a smart menu is created
************************************************************************/
void CGame::SmartMenuCreateCallBack( CMenu * pMenu )
{

}   // SmartMenuCreateCallBack


/***************************************************************************
*    decs:  Call back function to create sprite ai
****************************************************************************/
void CGame::AICreateCallBack( const std::string & aiName, iSprite2D * pSprite )
{
    /*if( aiName == "aiBall" )
        pSprite->SetAI( new CBallAI( pSprite ) );*/

}   // AICreateCallBack


/************************************************************************
*    desc:  Callback for the state string
************************************************************************/
void CGame::StatStringCallBack( const std::string & statStr )
{
    if( !CSettings::Instance().GetFullScreen() )
        SDL_SetWindowTitle( m_pWindow, statStr.c_str() );

}   // StatStringCallBack


/************************************************************************
*    desc:  Handle events
************************************************************************/
bool CGame::HandleEvent( const SDL_Event & rEvent )
{
    if( (rEvent.type == SDL_QUIT) || (rEvent.type == SDL_APP_TERMINATING) )
        return true;

    // Filter out these events. Can't do this through the normal event filter
    /*if( (rEvent.type >= SDL_JOYAXISMOTION) && (rEvent.type <= SDL_JOYBUTTONUP) )
        return false;

    else if( rEvent.type == SDL_CONTROLLERDEVICEADDED )
        CDevice::Instance().AddGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_CONTROLLERDEVICEREMOVED )
        CDevice::Instance().RemoveGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_APP_LOWMEMORY )
        DisplayErrorMsg( "Low Memory Error", "The device is experiencing low memory. Try freeing up some apps." );
    
    // In a traditional game, want the pause menu to display when the game is sent to the background
    else if( (rEvent.type == SDL_APP_WILLENTERBACKGROUND) && !CMenuManager::Instance().IsMenuActive() )
        NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION );*/

    return false;

}   // HandleEvent


/***************************************************************************
*   desc:  Display error massage
****************************************************************************/
void CGame::DisplayErrorMsg( const std::string & title, const std::string & msg )
{
    printf("Error: %s, %s", title.c_str(), msg.c_str() );

    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), m_pWindow );

}   // DisplayErrorMsg


/***************************************************************************
*   desc:  Start the game
****************************************************************************/
void CGame::StartGame()
{
    m_gameRunning = true;

}   // StartGame


/***************************************************************************
*   desc:  Stop the game
****************************************************************************/
void CGame::StopGame()
{
    m_gameRunning = false;

}   // StopGame


/***************************************************************************
*  desc:  Is the game running?
*
*  ret: bool - true or false if game is running
****************************************************************************/
bool CGame::IsGameRunning() const
{
    return m_gameRunning;

}   // IsGameRunning


/***************************************************************************
*    decs:  Filter out events we don't want
****************************************************************************/
int FilterEvents( void * userdata, SDL_Event * pEvent )
{
    // Return 0 to indicate that the event should be removed from the event queue

    // Do our own deadzone filtering
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
