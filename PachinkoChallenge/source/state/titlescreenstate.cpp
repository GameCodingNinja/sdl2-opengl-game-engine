
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
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD ),
        m_background( CObjectDataMgr::Instance().GetData2D( "(title_screen)", "background" ) )
{
}   // Constructer


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().Allow();
    CMenuManager::Instance().ActivateTree( "title_screen_tree");
    
    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Flush any user inputs that have been queued up
    SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
    
}   // Init


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CTitleScreenState::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN ) 
            m_scriptComponent.Prepare( "(menu)", "Screen_FadeOut" );
    }

}   // HandleEvent


/***************************************************************************
*    desc:  Handle any misc processing before the real work is started
****************************************************************************/
void CTitleScreenState::MiscProcess()
{
    
}   // MiscProcess


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CTitleScreenState::Update()
{
    CCommonState::Update();
    
    m_scriptComponent.Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CTitleScreenState::Transform()
{
    CCommonState::Transform();

    m_background.Transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::PreRender()
{
    const CMatrix & orthoMatrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    m_background.Render( orthoMatrix );
    
    CCommonState::PreRender();

}   // PreRender


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
        CObjectDataMgr::Instance().LoadGroup2D( "(title_screen)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData2D( "(title_screen)" );
    }
    
    void Load()
    {
        
    }   // ThreadLoad
    
    void CriticalInit()
    {

    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CObjectDataMgr::Instance().FreeGroup2D( "(title_screen)" );
    }
    
    void Unload()
    {

    }   // Unload

}   // NTitleScreenState