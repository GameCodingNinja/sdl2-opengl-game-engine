
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

// Boost lib dependencies
#include <boost/format.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CCommonState::CCommonState( const NGameDefs::EGameState gameState, const NGameDefs::EGameState nextState ) :
    iGameState( gameState, nextState )
{
    m_gameStateMap.emplace( "title_screen_state", NGameDefs::EGS_TITLE_SCREEN );
    m_gameStateMap.emplace( "run_state", NGameDefs::EGS_RUN );
    
}   // Constructer


/************************************************************************
*    desc:  Destructer                                                             
************************************************************************/
CCommonState::~CCommonState()
{

}   // Destructer


/***************************************************************************
*    desc:  handle events
****************************************************************************/
void CCommonState::HandleEvent( const SDL_Event & rEvent )
{
    // Have the menu manager handle events
    CMenuManager::Instance().HandleEvent( rEvent );
    
    // Check for the "game change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        if( rEvent.user.code == NMenu::ETC_BEGIN )
        {
            // Block all message processing in the menu manager
            CMenuManager::Instance().Allow( false );
            
            // Set the message to load and unload the states
            m_stateMessage.SetMsg( GetLoadState( rEvent.user.data1 ), m_gameState );
        }
        else if( rEvent.user.code == NMenu::ETC_END )
        {
            // Clear out all the trees
            CMenuManager::Instance().ClearActiveTrees();
            
            // Set the flag to change the state
            m_changeState = true;
        }
    }

}   // HandleEvent


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CCommonState::Update()
{
    // Update the menus
    CMenuManager::Instance().Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CCommonState::Transform()
{
    // Transform the menus
    CMenuManager::Instance().Transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CCommonState::PreRender()
{
}   // PreRender


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CCommonState::PostRender()
{
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    CMenuManager::Instance().Render( matrix );

}   // PostRender


/***************************************************************************
*    desc:  Get the load state
****************************************************************************/
NGameDefs::EGameState CCommonState::GetLoadState( void * pVoid )
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
    
}   // GetLoadState
