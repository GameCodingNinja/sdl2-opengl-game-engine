
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
#include <common/camera.h>

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
    CCommonState::Init();
    
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().Allow();
    CMenuManager::Instance().ActivateTree( "title_screen_tree");
    
    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
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
*    desc:  Is the state done
****************************************************************************/
/*bool CTitleScreenState::DoStateChange()
{
    if( m_changeState )
    {
        m_stateMessage.Clear();
        m_stateMessage.m_nextState = EGS_Level_1;
        m_stateMessage.m_lastState = EGS_TITLE_SCREEN;

        // Unload the object data
        m_stateMessage.m_groupUnload.push_back("(title_screen)");

        // Load the object data
        m_stateMessage.m_groupLoad.push_back("(stage1)");
        m_stateMessage.m_groupLoad.push_back("(actor)");
        
        // Load script group
        m_stateMessage.m_scriptLoad.push_back("(actor)");

        // Stage to load
        m_stateMessage.m_stageDataLoad = "(stage1)";
        
        // Actor data load
        m_stateMessage.m_actorDataLoad = "(actor)";

        return true;
    }

    return false;

}*/   // DoStateChange


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

    m_background.transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::PreRender()
{
    CCommonState::PreRender();

    m_background.Render( CCameraMgr::Instance().GetDefaultProjMatrix()  );

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