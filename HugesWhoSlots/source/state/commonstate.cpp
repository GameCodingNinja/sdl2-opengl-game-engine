
/************************************************************************
*    FILE NAME:       commonstate.cpp
*
*    DESCRIPTION:     CCommonState Class State
************************************************************************/

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <system/device.h>
#include <utilities/exceptionhandling.h>
#include <managers/cameramanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CCommonState::CCommonState( const NGameDefs::EGameState gameState, const NGameDefs::EGameState nextState ) :
    iGameState( gameState, nextState )
{
    m_gameStateMap.emplace( "lobby_state", NGameDefs::EGS_LOBBY );
    m_gameStateMap.emplace( "big_pay_back_state", NGameDefs::EGS_BIG_CASH_BACK );

}


/************************************************************************
*    desc:  Destructor
************************************************************************/
CCommonState::~CCommonState()
{
}


/***************************************************************************
*    desc:  handle events
****************************************************************************/
void CCommonState::handleEvent( const SDL_Event & rEvent )
{
    // Have the menu manager handle events
    CMenuManager::Instance().handleEvent( rEvent );

    // Block for the "game change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        if( rEvent.user.code == NMenu::ETC_BEGIN )
        {
            // Stop all message processing in the menu manager
            CMenuManager::Instance().allow( false );

            // Set the message to load and unload the states
            m_stateMessage.setMsg( getLoadState( rEvent.user.data1 ), m_gameState );
        }
        else if( rEvent.user.code == NMenu::ETC_END )
        {
            // Clear out all the trees
            CMenuManager::Instance().clearActiveTrees();

            // Set the flag to change the state
            m_changeState = true;
        }
    }
}


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::update()
{
    // Update the menus
    CMenuManager::Instance().update();
}


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CCommonState::transform()
{
    // Transform the menus
    CMenuManager::Instance().transformMenu();
}


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CCommonState::preRender()
{
    CMenuManager::Instance().renderInterface( CCameraMgr::Instance().getDefaultProjMatrix() );
}


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CCommonState::postRender()
{
    CMenuManager::Instance().render( CCameraMgr::Instance().getDefaultProjMatrix() );
}


/***************************************************************************
*    desc:  Get the load state
****************************************************************************/
NGameDefs::EGameState CCommonState::getLoadState( void * pVoid )
{
    if( pVoid == nullptr )
    {
        throw NExcept::CCriticalException("Load state not defined!",
            boost::str( boost::format("Message pointer is null!\n\n%s\nLine: %s")
                % __func__ % __LINE__ ));
    }

    const std::string & rNameStr = *(std::string *)pVoid;
    if( rNameStr.empty() )
    {
        throw NExcept::CCriticalException("Load state not defined!",
            boost::str( boost::format("Need to define a state to load.\n\n%s\nLine: %s")
                % __func__ % __LINE__ ));
    }

    // Find the state to return
    auto iter = m_gameStateMap.find( rNameStr );
    if( iter == m_gameStateMap.end() )
    {
        throw NExcept::CCriticalException("Load state not found!",
            boost::str( boost::format("Load state not defined or incorrectly defined! (%s).\n\n%s\nLine: %s")
                % rNameStr % __func__ % __LINE__ ));
    }

    return iter->second;
}
