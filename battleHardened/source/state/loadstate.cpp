
/************************************************************************
*    FILE NAME:       loadstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// SDL lib dependencies
#include <SDL.h>

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include "loadstate.h"

// Game dependencies
#include "titlescreenstate.h"
#include "level1state.h"

// Game lib dependencies
#include <system/device.h>
#include <objectdata/objectdatamanager.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/actionmanager.h>
#include <managers/signalmanager.h>
#include <managers/cameramanager.h>
#include <2d/sprite2d.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>

// Standard lib dependencies
#include <thread>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CLoadState::CLoadState( const CStateMessage & stateMsg ) :
    iGameState( NGameDefs::EGS_GAME_LOAD, stateMsg ),
    m_threadActive(true),
    m_time(0),
    m_frame(0)
{
}   // Constructer


/************************************************************************
*    desc:  destructer
************************************************************************/
CLoadState::~CLoadState()
{
    CObjectDataMgr::Instance().freeGroup2D( "(loadingScreen)" );
    
}   // destructer


/************************************************************************
*    desc:  Do any pre-load init's
************************************************************************/
void CLoadState::Init()
{
    // Load the start up animation group
    CObjectDataMgr::Instance().loadGroup2D( "(loadingScreen)" );
    
    // Allocate the sprite to fade in
    m_upSprite.reset( new CSprite2D( CObjectDataMgr::Instance().getData2D( "(loadingScreen)", "loadAnim" ) ) );
    
    // Get the position, scale and half the screen size
    CSize<float> scrnHalf = CSettings::Instance().GetDefaultSizeHalf();

    // Set the position
    m_upSprite->setPos( CPoint<float>(scrnHalf.w, -scrnHalf.h) + CPoint<float>(-150, 150) );
    m_upSprite->transform(); 
    
    // Setup the fade in
    CShaderMgr::Instance().setShaderColor( "shader_2d_spriteSheet", "additive", CColor(1,1,1,1) );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
    
}   // Init


/***************************************************************************
*    desc:  Animate from thread durring the load
****************************************************************************/
void CLoadState::Animate()
{
    // Get the elapsed time
    CHighResTimer::Instance().CalcElapsedTime();

    m_time += CHighResTimer::Instance().GetElapsedTime();
        
    if( m_time > 83.f )
    {
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        m_upSprite->setFrame( m_frame );
        m_frame = (m_frame + 1) % m_upSprite->getFrameCount();

        m_upSprite->render( CCameraMgr::Instance().getDefaultProjMatrix() );

        SDL_GL_SwapWindow( CDevice::Instance().GetWindow() );

        // Apparently it's a good practice to do this at the end of a render cycle
        CShaderMgr::Instance().unbind();
        CTextureMgr::Instance().unbind();
        CVertBufMgr::Instance().unbind();
        
        int dif = m_time / 83;
        m_time = m_time - (dif * 83);
    }

}   // Animate


/***************************************************************************
*    desc:  Object Data Load
****************************************************************************/
void CLoadState::ObjectDataLoad()
{
    try
    {
        // Test code to see loading animation!!!!
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        
        if( m_stateMessage.GetLoadState() == NGameDefs::EGS_TITLE_SCREEN )
            NTitleScreenState::ObjectDataLoad();

        else if( m_stateMessage.GetLoadState() == NGameDefs::EGS_Level_1 )
            NLevel_1::ObjectDataLoad();
    }
    catch (NExcept::CCriticalException & ex)
    {
        m_errorTitle = ex.GetErrorTitle();
        m_errorMsg = ex.GetErrorMsg();
    }
    catch (std::exception const & ex)
    {
        m_errorTitle = "Standard Exception";
        m_errorMsg = ex.what();
    }
    catch (...)
    {
        m_errorTitle = "Unknown Error";
        m_errorMsg = "Something bad happened and I'm not sure what it was.";
    }
    
    // Set the flag to indicate the load is complete
    m_threadActive = false;
    
}   // ObjectDataLoad



/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CLoadState::CriticalLoad()
{ 
    // ----------------- UNLOAD SECTION -----------------
    
    if( m_stateMessage.GetUnloadState() == NGameDefs::EGS_TITLE_SCREEN )
        NTitleScreenState::CriticalUnload();
    
    else if( m_stateMessage.GetUnloadState() == NGameDefs::EGS_Level_1 )
        NLevel_1::CriticalUnload();
    
    
    // ------------------ LOAD SECTION ------------------

    if( m_stateMessage.GetLoadState() == NGameDefs::EGS_TITLE_SCREEN )
        NTitleScreenState::CriticalLoad();

    else if( m_stateMessage.GetLoadState() == NGameDefs::EGS_Level_1 )
        NLevel_1::CriticalLoad();
    
}   // CriticalLoad


/***************************************************************************
*    desc:  Critical inits
****************************************************************************/
void CLoadState::CriticalInit()
{
    if( m_stateMessage.GetLoadState() == NGameDefs::EGS_TITLE_SCREEN )
        NTitleScreenState::CriticalInit();

    else if( m_stateMessage.GetLoadState() == NGameDefs::EGS_Level_1 )
        NLevel_1::CriticalInit();
    
}   // CriticalInit


/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CLoadState::AssetsLoad()
{
    try
    {
        // ----------------- UNLOAD SECTION -----------------

        if( m_stateMessage.GetUnloadState() == NGameDefs::EGS_TITLE_SCREEN )
            NTitleScreenState::Unload();

        else if( m_stateMessage.GetUnloadState() == NGameDefs::EGS_Level_1 )
            NLevel_1::Unload();


        // ------------------ LOAD SECTION ------------------

        if( m_stateMessage.GetLoadState() == NGameDefs::EGS_TITLE_SCREEN )
            NTitleScreenState::Load();

        else if( m_stateMessage.GetLoadState() == NGameDefs::EGS_Level_1 )
            NLevel_1::Load();
    }
    catch (NExcept::CCriticalException & ex)
    {
        m_errorTitle = ex.GetErrorTitle();
        m_errorMsg = ex.GetErrorMsg();
    }
    catch (std::exception const & ex)
    {
        m_errorTitle = "Standard Exception";
        m_errorMsg = ex.what();
    }
    catch (...)
    {
        m_errorTitle = "Unknown Error";
        m_errorMsg = "Something bad happened and I'm not sure what it was.";
    }
    
    // Set the flag to indicate the load is complete
    m_threadActive = false;

}   // AssetsLoad


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CLoadState::DoStateChange()
{
    CActionMgr::Instance().resetLastUsedDevice();

    std::thread objLoadThread( &CLoadState::ObjectDataLoad, this );
    objLoadThread.detach();
    
    do
    {
        Animate();
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
    while(m_threadActive);
    
    // If there was an error in the load thread, re-throw exception
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException(m_errorTitle, m_errorMsg);
    
    CriticalLoad();
    
    // Start the loading/unloading thread
    m_threadActive = true;
    std::thread loadThread( &CLoadState::AssetsLoad, this );
    loadThread.detach();
    
    do
    {
        Animate();
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
    while(m_threadActive);
    
    // If there was an error in the load thread, re-throw exception
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException(m_errorTitle, m_errorMsg);
    
    CriticalInit();
    
    return true;

}   // DoStateChange


/***************************************************************************
*    desc:  Get the next state to load
****************************************************************************/
NGameDefs::EGameState CLoadState::GetNextState()
{
    return m_stateMessage.GetLoadState();

}   // GetNextState

