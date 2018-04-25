
/************************************************************************
*    FILE NAME:       loadstate.cpp
*
*    DESCRIPTION:     All this state does is unload/load and
*                     does a state change.
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
#include "lobbystate.h"
#include "bigpaybackstate.h"

// Game lib dependencies
#include <system/device.h>
#include <objectdata/objectdatamanager.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/spritesheetmanager.h>
#include <managers/actionmanager.h>
#include <managers/cameramanager.h>
#include <common/color.h>
#include <2d/sprite2d.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>

// Standard lib dependencies
#include <thread>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLoadState::CLoadState( const CStateMessage & stateMsg ) :
    iGameState( NGameDefs::EGS_GAME_LOAD, stateMsg ),
    m_threadActive(true),
    m_time(0),
    m_frame(0)
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CLoadState::~CLoadState()
{
    CObjectDataMgr::Instance().freeGroup2D( "(loadingScreen)" );
}


/************************************************************************
*    desc:  Do any pre-load init's
************************************************************************/
void CLoadState::init()
{
    // Load the start up animation group
    CObjectDataMgr::Instance().loadGroup2D( "(loadingScreen)" );

    // Allocate the sprite to fade in
    m_upSprite.reset( new CSprite2D( CObjectDataMgr::Instance().getData2D( "(loadingScreen)", "loadAnim" ) ) );

    // Get the position, scale and half the screen size
    CSize<float> scrnHalf = CSettings::Instance().getDefaultSizeHalf();

    // Set the position
    m_upSprite->setPos( CPoint<float>(scrnHalf.w, -scrnHalf.h) + CPoint<float>(-150, 150) );
    m_upSprite->transform();

    // Setup the fade in
    CShaderMgr::Instance().setShaderColor( "shader_2d_spriteSheet", "additive", CColor(1,1,1,1) );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/***************************************************************************
*    desc:  Animate from thread durring the load
****************************************************************************/
void CLoadState::animate()
{
    // Get the elapsed time
    CHighResTimer::Instance().calcElapsedTime();

    m_time += CHighResTimer::Instance().getElapsedTime();

    if( m_time > 83.f )
    {
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        m_upSprite->setFrame( m_frame );
        m_frame = (m_frame + 1) % m_upSprite->getFrameCount();

        m_upSprite->render( CCameraMgr::Instance().getDefaultProjMatrix() );

        SDL_GL_SwapWindow( CDevice::Instance().getWindow() );

        // Apparently it's a good practice to do this at the end of a render cycle
        CShaderMgr::Instance().unbind();
        CTextureMgr::Instance().unbind();
        CVertBufMgr::Instance().unbind();

        int dif = m_time / 83;
        m_time = m_time - (dif * 83);
    }
}


/***************************************************************************
*    desc:  Object Data Load
****************************************************************************/
void CLoadState::objectDataLoad()
{
    try
    {
        if( m_stateMessage.getLoadState() == NGameDefs::EGS_LOBBY )
            NLobby::ObjectDataLoad();

        else if( m_stateMessage.getLoadState() == NGameDefs::EGS_BIG_CASH_BACK )
            NBigPayBack::ObjectDataLoad();
    }
    catch (NExcept::CCriticalException & ex)
    {
        m_errorTitle = ex.getErrorTitle();
        m_errorMsg = ex.getErrorMsg();
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
}


/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CLoadState::criticalLoad()
{
    // ----------------- UNLOAD SECTION -----------------

    if( m_stateMessage.getUnloadState() == NGameDefs::EGS_LOBBY )
        NLobby::CriticalUnload();

    else if( m_stateMessage.getUnloadState() == NGameDefs::EGS_BIG_CASH_BACK )
        NBigPayBack::CriticalUnload();


    // ------------------ LOAD SECTION ------------------

    if( m_stateMessage.getLoadState() == NGameDefs::EGS_LOBBY )
        NLobby::CriticalLoad();

    else if( m_stateMessage.getLoadState() == NGameDefs::EGS_BIG_CASH_BACK )
        NBigPayBack::CriticalLoad();
}


/***************************************************************************
*    desc:  Critical inits
****************************************************************************/
void CLoadState::criticalInit()
{
    if( m_stateMessage.getLoadState() == NGameDefs::EGS_LOBBY )
        NLobby::CriticalInit();

    else if( m_stateMessage.getLoadState() == NGameDefs::EGS_BIG_CASH_BACK )
        NBigPayBack::CriticalInit();
}


/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CLoadState::assetsLoad()
{
    try
    {
        // ----------------- UNLOAD SECTION -----------------

        if( m_stateMessage.getUnloadState() == NGameDefs::EGS_LOBBY )
            NLobby::Unload();

        else if( m_stateMessage.getUnloadState() == NGameDefs::EGS_BIG_CASH_BACK )
            NBigPayBack::Unload();


        // ------------------ LOAD SECTION ------------------

        if( m_stateMessage.getLoadState() == NGameDefs::EGS_LOBBY )
            NLobby::Load();

        else if( m_stateMessage.getLoadState() == NGameDefs::EGS_BIG_CASH_BACK )
            NBigPayBack::Load();
    }
    catch (NExcept::CCriticalException & ex)
    {
        m_errorTitle = ex.getErrorTitle();
        m_errorMsg = ex.getErrorMsg();
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
}


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CLoadState::doStateChange()
{
    CActionMgr::Instance().resetLastUsedDevice();

    std::thread objLoadThread( &CLoadState::objectDataLoad, this );
    objLoadThread.detach();

    do
    {
        animate();
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
    while(m_threadActive);

    // If there was an error in the load thread, re-throw exception
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException(m_errorTitle, m_errorMsg);

    criticalLoad();

    // Start the loading/unloading thread
    m_threadActive = true;
    std::thread loadThread( &CLoadState::assetsLoad, this );
    loadThread.detach();

    do
    {
        animate();
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
    while(m_threadActive);

    // If there was an error in the load thread, re-throw exception
    if( !m_errorMsg.empty() )
        throw NExcept::CCriticalException(m_errorTitle, m_errorMsg);

    criticalInit();

    return true;
}


/***************************************************************************
*    desc:  Get the next state to load
****************************************************************************/
NGameDefs::EGameState CLoadState::getNextState()
{
    return m_stateMessage.getLoadState();
}
