
/************************************************************************
*    FILE NAME:       basegame.cpp
*
*    DESCRIPTION:     base game class
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <system/basegame.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <system/device.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/meshmanager.h>
#include <common/build_defs.h>

// Standard lib dependencies
#include <stdio.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBaseGame::CBaseGame()
    : m_pWindow(nullptr),
      m_context(nullptr),
      m_gameRunning(false),
      m_clearBufferMask(0)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CBaseGame::~CBaseGame()
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
void CBaseGame::Create()
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
*    desc:  Init the game
************************************************************************/
void CBaseGame::Init()
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

}   // Init


/***************************************************************************
*   desc:  Poll for game events
****************************************************************************/
void CBaseGame::PollEvents()
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
bool CBaseGame::GameLoop()
{
    // Handle the state change
    DoStateChange();

    // Poll for game events
    PollEvents();

    // Get our elapsed time
    CHighResTimer::Instance().CalcElapsedTime();

    if( m_gameRunning )
    {
        // Handle any misc processing before the real work is started
        MiscProcess();
        
        // Handle the physics
        Physics();

        // Update animations, Move sprites, Check for collision
        Update();

        // Transform game objects
        Transform();
        
        // Clear the buffers
        glClear( m_clearBufferMask );

        // Do the rendering
        Render();
        
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

}   // GameLoop


/***************************************************************************
*   desc:  Display error massage
****************************************************************************/
void CBaseGame::DisplayErrorMsg( const std::string & title, const std::string & msg )
{
    printf("Error: %s, %s", title.c_str(), msg.c_str() );

    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), m_pWindow );

}   // DisplayErrorMsg


/***************************************************************************
*   desc:  Start the game
****************************************************************************/
void CBaseGame::StartGame()
{
    m_gameRunning = true;

}   // StartGame


/***************************************************************************
*   desc:  Stop the game
****************************************************************************/
void CBaseGame::StopGame()
{
    m_gameRunning = false;

}   // StopGame


/***************************************************************************
*  desc:  Is the game running?
*
*  ret: bool - true or false if game is running
****************************************************************************/
bool CBaseGame::IsGameRunning() const
{
    return m_gameRunning;

}   // IsGameRunning

