
/************************************************************************
*    FILE NAME:       level1state.cpp
*
*    DESCRIPTION:     CState1State Class State
************************************************************************/

// Physical component dependency
#include "level1state.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/statcounter.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <physics/physicsworld2d.h>
#include <managers/cameramanager.h>
#include <common/camera.h>
#include <script/scriptmanager.h>
#include <spritestrategy/basicspritestrategy.h>
#include <spritestrategy/loopstagestrategy.h>
#include <spritestrategy/spritestrategymanager.h>
#include <2d/actorsprite2d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLevel1State::CLevel1State() :
    CCommonState( NGameDefs::EGS_Level_1, NGameDefs::EGS_GAME_LOAD )
{
}   // Constructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CLevel1State::Init()
{
    CCommonState::Init();
    
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree("pause_tree");
    
    // Reset the camera
    //CCamera::Instance().SetPos( CStageMgr::Instance().GetDefaultCameraPos().GetPos() );

    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Create the actors
    CSpriteStrategyMgr::Instance().Create("(actor)", "enemy_ship");
    m_pPlayerShip = CSpriteStrategyMgr::Instance().CreateSprite<CActorSprite2D>("(actor)", "player_ship");
    
    // Get pointer to the player ship
    //m_pPlayerShip = &CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy2D>("(actor)").Get<CActorSprite2D>(id);
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
    
}   // Init


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CLevel1State::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );
    
    if( !CMenuManager::Instance().isActive() )
    {
        m_pPlayerShip->handleEvent( rEvent );
    }
    
    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN ) 
            m_scriptComponent.Prepare( "(menu)", "Screen_FadeOut" );
    }

    // Check for the "change state" message
    /*if( rEvent.type == NUIControl::ECAT_GAME_STATE_CHANGE )
    {
        m_stateMessage.Clear();
        m_stateMessage.m_nextState = EGS_TITLE_SCREEN;
        m_stateMessage.m_lastState = EGS_Level_1;

        // Load the object data
        m_stateMessage.m_groupLoad.push_back("(title_screen)");

        // Unload the object data
        m_stateMessage.m_groupUnload.push_back("(actor)");
        m_stateMessage.m_groupUnload.push_back("(stage1)");
        
        // Unload scripts
        m_stateMessage.m_scriptUnload.push_back("(actor)");

        m_nextState = EGS_GAME_LOAD;

        m_changeState = true;
    }*/

}   // HandleEvent


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CLevel1State::MiscProcess()
{
    CSpriteStrategyMgr::Instance().MiscProcess();
    
}   // MiscProcess


/***************************************************************************
*    desc:  Handle the physics
****************************************************************************/
void CLevel1State::Physics()
{
    if( !CMenuManager::Instance().isActive() )
    {

    }

}   // Physics


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CLevel1State::Update()
{
    CCommonState::Update();
    
    m_scriptComponent.Update();

    if( !CMenuManager::Instance().isActive() )
    {
        //CCamera::Instance().IncPos( CPoint<float>( -0.05f * CHighResTimer::Instance().GetElapsedTime(), 0.f ) );
        CSpriteStrategyMgr::Instance().Update();
    }

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CLevel1State::Transform()
{
    CCommonState::Transform();

    //CCamera::Instance().Transform();
    CSpriteStrategyMgr::Instance().Transform();

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CLevel1State::PreRender()
{
    CCommonState::PreRender();
    
    CSpriteStrategyMgr::Instance().Render( CCameraMgr::Instance().GetDefaultProjMatrix() );

}   // PreRender



/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NLevel_1
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().LoadGroup2D( "(stage1)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        CObjectDataMgr::Instance().LoadGroup2D( "(actor)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData2D( "(actor)" );
        CObjectDataMgr::Instance().CreateFromData2D( "(stage1)" );
    }
    
    void Load()
    {
        CSpriteStrategyMgr::Instance().AddStrategy( "(actor)", new CBasicSpriteStrategy );
        CSpriteStrategyMgr::Instance().AddStrategy( "(stage1)", new CLoopStageStrategy );
        
        // Load state specific AngelScript functions
        CScriptManager::Instance().LoadGroup("(actor)");
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
        CObjectDataMgr::Instance().FreeGroup2D( "(stage1)" );
        CObjectDataMgr::Instance().FreeGroup2D( "(actor)" );
    }
    
    void Unload()
    {
        CSpriteStrategyMgr::Instance().Clear();
        
    }   // Unload

}   // NLevel_1
