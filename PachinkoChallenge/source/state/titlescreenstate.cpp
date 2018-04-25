
/************************************************************************
*    FILE NAME:       titlescreenstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/highresolutiontimer.h>
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD ),
        m_background( CObjectDataMgr::Instance().getData2D( "(title_screen)", "background" ) ),
        m_title( CObjectDataMgr::Instance().getData2D( "(title_screen)", "title" ) )
{
}


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree( "title_screen_tree");

    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );

    // Flush any user inputs that have been queued up
    SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);

    // Transform what doesn't change
    m_background.setRot(0,0,90);
    m_background.transform();

    m_title.setPos( 0, 700 );
    m_title.transform();

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CTitleScreenState::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }
}


/***************************************************************************
*    desc:  Handle any misc processing before the real work is started
****************************************************************************/
void CTitleScreenState::miscProcess()
{
}


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CTitleScreenState::update()
{
    CCommonState::update();

    m_scriptComponent.update();
}


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CTitleScreenState::transform()
{
    CCommonState::transform();
}


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::preRender()
{
    const CMatrix & orthoMatrix = CCameraMgr::Instance().getDefaultProjMatrix();
    m_background.render( orthoMatrix );
    m_title.render( orthoMatrix );

    CCommonState::preRender();
}


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().loadGroup2D( "(title_screen)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(title_screen)" );
    }

    void Load()
    {

    }

    void CriticalInit()
    {

    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CObjectDataMgr::Instance().freeGroup2D( "(title_screen)" );
    }

    void Unload()
    {

    }
}
