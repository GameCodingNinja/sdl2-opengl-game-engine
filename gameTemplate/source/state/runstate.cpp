
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <physics/physicscomponent2d.h>
#include <managers/cameramanager.h>
#include <spritestrategy/basicstagestrategy2d.h>
#include <spritestrategy/basicspritestrategy2d.h>
#include <spritestrategy/spritestrategymanager.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().GetWorld( "(game)" ) )
{
}   // Constructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CRunState::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().Allow();
    CMenuManager::Instance().ActivateTree("pause_tree");
    
    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
    
}   // Init


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CRunState::HandleEvent( const SDL_Event & rEvent )
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
*    desc:  Handle the physics
****************************************************************************/
void CRunState::Physics()
{
    if( !CMenuManager::Instance().IsActive() )
    {
        m_rPhysicsWorld.FixedTimeStep();
    }

}   // Physics


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();
    
    m_scriptComponent.Update();
    
    if( !CMenuManager::Instance().IsActive() )
        CSpriteStrategyMgr::Instance().Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();
    
    if( !CMenuManager::Instance().IsActive() )
        CSpriteStrategyMgr::Instance().Transform();

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();
    
    CSpriteStrategyMgr::Instance().Render( CCameraMgr::Instance().GetDefaultProjMatrix() );

}   // PreRender


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NRunState
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().LoadGroup2D( "(run)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData2D( "(run)" );
    }
    
    void Load()
    {
	// All physics entities are destroyed and all heap memory is released.
        CPhysicsWorldManager2D::Instance().CreateWorld( "(game)" );
        CSpriteStrategyMgr::Instance().AddStrategy( "(stage1)", new CBasicStageStrategy2D );
        CSpriteStrategyMgr::Instance().AddStrategy( "(sprite)", new CBasicSpriteStrategy2D );
    }
    
    void CriticalInit()
    {
        const char* shapes[] = {"triangle_blue", "triangle_green", "circle_blue", "circle_green", "circle_red", "square_red"};
        
        for( int i = 0; i < 24; ++i )
            CSpriteStrategyMgr::Instance().Create( "(sprite)", shapes[i % 6] );
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CSpriteStrategyMgr::Instance().CleanUp();
        CObjectDataMgr::Instance().FreeGroup2D( "(run)" );
    }
    
    void Unload()
    {
        CSpriteStrategyMgr::Instance().Clear();
        CPhysicsWorldManager2D::Instance().DestroyWorld( "(game)" );
        
    }

}   // NTitleScreenState
