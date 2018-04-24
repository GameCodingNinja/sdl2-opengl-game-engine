
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
#include <spritestrategy/basicstagestrategy.h>
#include <spritestrategy/basicspritestrategy.h>
#include <spritestrategy/spritestrategymanager.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().getWorld( "(game)" ) )
{
}   // Constructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CRunState::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree("pause_tree");
    
    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
    
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
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }

}   // HandleEvent


/***************************************************************************
*    desc:  Handle the physics
****************************************************************************/
void CRunState::Physics()
{
    if( !CMenuManager::Instance().isActive() )
    {
        m_rPhysicsWorld.fixedTimeStep();
    }

}   // Physics


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
{
    CCommonState::Update();
    
    m_scriptComponent.update();
    
    if( !CMenuManager::Instance().isActive() )
        CSpriteStrategyMgr::Instance().update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();
    
    if( !CMenuManager::Instance().isActive() )
        CSpriteStrategyMgr::Instance().transform();

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();
    
    CSpriteStrategyMgr::Instance().render( CCameraMgr::Instance().getDefaultProjMatrix() );

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
        CObjectDataMgr::Instance().loadGroup2D( "(run)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(run)" );
    }
    
    void Load()
    {
	// All physics entities are destroyed and all heap memory is released.
        CPhysicsWorldManager2D::Instance().createWorld( "(game)" );
        CSpriteStrategyMgr::Instance().addStrategy( "(stage1)", new CBasicStageStrategy );
        CSpriteStrategyMgr::Instance().addStrategy( "(sprite)", new CBasicSpriteStrategy );
    }
    
    void CriticalInit()
    {
        const char* shapes[] = {"triangle_blue", "triangle_green", "circle_blue", "circle_green", "circle_red", "square_red"};
        
        for( int i = 0; i < 24; ++i )
            CSpriteStrategyMgr::Instance().create( "(sprite)", shapes[i % 6] );
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CSpriteStrategyMgr::Instance().cleanUp();
        CObjectDataMgr::Instance().freeGroup2D( "(run)" );
    }
    
    void Unload()
    {
        CSpriteStrategyMgr::Instance().clear();
        CPhysicsWorldManager2D::Instance().destroyWorld( "(game)" );
    }

}   // NTitleScreenState
