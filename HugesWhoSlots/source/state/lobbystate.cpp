
/************************************************************************
*    FILE NAME:       lobbystate.cpp
*
*    DESCRIPTION:     The lobby state
************************************************************************/

// Physical component dependency
#include "lobbystate.h"

// Game dependencies
#include "../game/gamesave.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/xmlpreloader.h>
#include <utilities/highresolutiontimer.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <gui/uibuttonlist.h>
#include <slot/slotmathmanager.h>
#include <slot/symbolsetviewmanager.h>
#include <slot/slotgroupmodel.h>
#include <slot/betmanager.h>
#include <script/scriptmanager.h>
#include <managers/soundmanager.h>
#include <managers/spritesheetmanager.h>
#include <managers/cameramanager.h>

// Boost lib dependencies
#include <boost/bind.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLobbyState::CLobbyState( const std::string & group ) :
    CCommonState( NGameDefs::EGS_LOBBY, NGameDefs::EGS_GAME_LOAD ),
        m_group( group ),
        m_background( CObjectDataMgr::Instance().getData2D( group, "background" ) )
{
}


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CLobbyState::init()
{
    CCommonState::init();

    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree( "menu_tree");
    CMenuManager::Instance().activateTree( "buy_tree");
    CMenuManager::Instance().activateTree( "confirmation_tree");
    CMenuManager::Instance().activateTree( "lobby_tree");

    // Init the credit meter
    CMenuManager::Instance().getMenuControl<CUIMeter>( "lobby_menu", "credit_meter" ).set( CBetMgr::Instance().getCredits() );

    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( m_group, "Screen_FadeIn" );

    if( CGameSave::Instance().getPlayLobbyMusic() )
        m_scriptComponent.prepare( m_group, "Lobby_PlayMusic" );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    desc:  Do we allow lobby music
************************************************************************/
void CLobbyState::allowMusic( bool allow )
{
    if( allow )
    {
        m_scriptComponent.stopAndRecycle( "Lobby_FastStopMusic" );
        m_scriptComponent.prepare( m_group, "Lobby_PlayMusic" );
    }
    else
    {
        m_scriptComponent.stopAndRecycle( "Lobby_PlayMusic" );
        m_scriptComponent.prepare( m_group, "Lobby_FastStopMusic" );
    }
}


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CLobbyState::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Check for the "game change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
        {
            m_scriptComponent.prepare( m_group, "Screen_FadeOut" );
            m_scriptComponent.prepare( m_group, "Lobby_FastStopMusic" );
        }
    }
}


/***************************************************************************
*    desc:  Handle any misc processing before the real work is started
****************************************************************************/
void CLobbyState::miscProcess()
{
}


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CLobbyState::update()
{
    CCommonState::update();

    m_scriptComponent.update();
}


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CLobbyState::transform()
{
    CCommonState::transform();

    CMenuManager::Instance().transformInterface();

    m_background.transform();
}


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CLobbyState::preRender()
{
    m_background.render( CCameraMgr::Instance().getDefaultProjMatrix() );

    CCommonState::preRender();
}


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NLobby
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().loadGroup2D( "(lobby)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(lobby)" );
    }

    void Load()
    {
        // Load the state specific menu group
        CMenuManager::Instance().loadGroup("(lobby)", CMenuManager::DONT_INIT_GROUP);

        CSoundMgr::Instance().loadGroup("(lobby)");

        CScriptManager::Instance().loadGroup("(lobby)");

        // Free the sprite sheet data manager because it's no longer needed
        CSpriteSheetMgr::Instance().clear();
        CXMLPreloader::Instance().clear();
    }

    void CriticalInit()
    {
        // Creates the font strings, run init scripts
        CMenuManager::Instance().initGroup("(lobby)");
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
	CMenuManager::Instance().cleanUpGroup("(lobby)");
        CObjectDataMgr::Instance().freeGroup2D( "(lobby)" );
    }

    void Unload()
    {
        CMenuManager::Instance().freeGroup("(lobby)");
        CScriptManager::Instance().freeGroup("(lobby)");
        CSoundMgr::Instance().freeGroup("(lobby)");

    }
}
