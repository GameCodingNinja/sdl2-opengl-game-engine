
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
#include <utilities/xmlpreloader.h>
#include <managers/cameramanager.h>
#include <gui/menumanager.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <spritestrategy/spritestrategymanager.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD ),
        m_background( CObjectDataMgr::Instance().getData2D( "(title_screen)", "background" ) ),
        //m_spriteSheetTest( CObjectDataMgr::Instance().GetData2D( "(title_screen)", "spriteSheetTest2" ) ),
        m_cube( CObjectDataMgr::Instance().getData3D( "(cube)", "cube" ) )
{
}   // Constructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree( "title_screen_tree" );

    m_cube.setScale( 3, 3, 3 );
    
    CCameraMgr::Instance().createPerspective( "cube" );
    CCameraMgr::Instance().setActiveCameraPos( 0, 0, 20 );
    CCameraMgr::Instance().setActiveCameraRot( 10, 0, 0 );
    
    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );
    
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
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
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
    
    m_scriptComponent.update();

    float rot = CHighResTimer::Instance().GetElapsedTime() * 0.04;
    m_cube.incRot( rot, rot, 0 );

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CTitleScreenState::Transform()
{
    CCommonState::Transform();

    m_background.transform();
    
    CCameraMgr::Instance().transform();
    
    //m_spriteSheetTest.Transform();
    
    m_cube.transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::PreRender()
{
    m_background.render( CCameraMgr::Instance().getDefaultProjMatrix() );
    
    //m_spriteSheetTest.Render( orthoMatrix );
    
    CCommonState::PreRender();
    
    auto & camera = CCameraMgr::Instance().getActiveCamera();
    
    m_cube.render( camera.getFinalMatrix(), camera.getRotMatrix() );
    
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
        CObjectDataMgr::Instance().loadGroup2D( "(title_screen)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        
        //CObjectDataMgr::Instance().LoadGroup2D( "(actor)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        CObjectDataMgr::Instance().loadGroup3D( "(cube)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(title_screen)" );
        CObjectDataMgr::Instance().createFromData3D( "(cube)" );
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
        CObjectDataMgr::Instance().freeGroup2D( "(title_screen)" );
        CObjectDataMgr::Instance().freeGroup3D( "(cube)" );
    }
    
    void Unload()
    {
        
    }   // Unload

}   // NTitleScreenState
