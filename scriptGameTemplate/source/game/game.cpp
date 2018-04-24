
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

// Game lib dependencies
#include <common/build_defs.h>
#include <system/device.h>
#include <utilities/settings.h>
#include <utilities/statcounter.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <spritestrategy/spritestrategymanager.h>
#include <managers/actionmanager.h>
#include <managers/cameramanager.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/meshmanager.h>
#include <gui/menumanager.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptplaylist.h>
#include <script/scriptpoint.h>
#include <script/scriptglobals.h>
#include <script/scriptsprite.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptcamera.h>
#include <script/scriptcameramanager.h>
#include <script/scriptmenu.h>
#include <script/scriptshadermanager.h>
#include <script/scriptobjectdatamanager.h>
#include <script/scriptstrategymanager.h>
#include <script/scriptactionmanager.h>

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
    //SDL_SetEventFilter(FilterEvents, 0);
    
    // Handle some events on startup
    PollEvents();
    
    // Load the script list table
    CScriptManager::Instance().loadListTable( "source/scriptListTable.lst" );
    
    // Register the script items
    RegisterStdString( CScriptManager::Instance().GetEnginePtr() );
    RegisterScriptArray( CScriptManager::Instance().GetEnginePtr(), false );
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptCamera::Register();
    NScriptSprite::Register();
    NScriptSound::Register();
    NScriptPlayLst::Register();
    NScriptSoundManager::Register();
    NScriptMenu::Register();
    NScriptShaderManager::Register();
    NScriptObjectDataManager::Register();
    NScriptStrategyManager::Register();
    NScriptCameraManager::Register();
    NScriptActionManager::Register();

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
    
    CActionMgr::Instance().clearQueue();

    // Handle events on queue
    while( SDL_PollEvent( &msgEvent ) )
    {
        CActionMgr::Instance().queueEvent( msgEvent );
        
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
        // Clear the buffers
        glClear( m_clearBufferMask );
        
        // Process all game states
        CSpriteStrategyMgr::Instance().MiscProcess();
        CSpriteStrategyMgr::Instance().Update();
        CSpriteStrategyMgr::Instance().Transform();
        CCameraMgr::Instance().transform();
        CSpriteStrategyMgr::Instance().Render();
        
        // Do the back buffer swap
        SDL_GL_SwapWindow( m_pWindow );
        
        // Unbind everything after a round of rendering
        CShaderMgr::Instance().unbind();
        CTextureMgr::Instance().unbind();
        CVertBufMgr::Instance().unbind();

        // Inc the cycle
        if( NBDefs::IsDebugMode() )
            CStatCounter::Instance().IncCycle();
    }
    
    return m_gameRunning;
    
}   // ScriptGameLoop


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
    if( (rEvent.type >= SDL_JOYAXISMOTION) && (rEvent.type <= SDL_JOYBUTTONUP) )
        return false;

    else if( rEvent.type == SDL_CONTROLLERDEVICEADDED )
        CDevice::Instance().AddGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_CONTROLLERDEVICEREMOVED )
        CDevice::Instance().RemoveGamepad( rEvent.cdevice.which );

    else if( rEvent.type == SDL_APP_LOWMEMORY )
        DisplayErrorMsg( "Low Memory Error", "The device is experiencing low memory. Try freeing up some apps." );
    
    // In a traditional game, want the pause menu to display when the game is sent to the background
    else if( (rEvent.type == SDL_APP_WILLENTERBACKGROUND) && !CMenuManager::Instance().isMenuActive() )
        NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION );

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
