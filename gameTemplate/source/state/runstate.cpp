
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
#include <physics/physicsworldmanager.h>
#include <physics/physicsworld2d.h>
#include <physics/physicscomponent2d.h>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
        m_box( CObjectDataMgr::Instance().GetData2D( "(run)", "box" ) ),
        m_triangle( CObjectDataMgr::Instance().GetData2D( "(run)", "triangle" ) ),
        m_circle( CObjectDataMgr::Instance().GetData2D( "(run)", "circle" ) ),
        m_floor( CObjectDataMgr::Instance().GetData2D( "(run)", "floor" ) ),
        m_rPhysicsWorld( CPhysicsWorldManager::Instance().GetWorld2D( "(game)" ) )
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
    
    m_floor.SetPosXYZ( 0, -400, 0 );
    //m_floor.SetRotXYZ( 0, 0, 15 );
    m_floor.InitPhysics();
    
    m_box.SetPosXYZ( 50, 600, 0 );
    m_box.SetRotXYZ( 0, 0, 25 );
    //m_box.SetScaleXYZ( 3, 3, 3 );
    m_box.InitPhysics();
    
    m_triangle.SetPosXYZ( 0, 500, 0 );
    m_triangle.SetRotXYZ( 0, 0, 25 );
    //m_triangle.SetScaleXYZ( 3, 3, 3 );
    m_triangle.InitPhysics();
    
    m_circle.SetPosXYZ( -50, 700, 0 );
    //m_circle.SetScaleXYZ( 3, 3, 3 );
    m_circle.InitPhysics();
    
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
    {
        m_box.Update();
        m_triangle.Update();
        m_circle.Update();
        
        m_box.PhysicsUpdate();
        m_triangle.PhysicsUpdate();
        m_circle.PhysicsUpdate();

        // If they fall off the floor, reposition to the top again
        if( m_box.GetPos().y < -600.0f )
            m_box.GetPhysicsComponent().SetTransform( 50, 600, 25 );

        if( m_triangle.GetPos().y < -600.0f )
            m_triangle.GetPhysicsComponent().SetTransform( 0, 600, 25 );

        if( m_circle.GetPos().y < -600.0f )
            m_circle.GetPhysicsComponent().SetTransform( -50, 600, 0 );
    }

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();
    
    m_box.Transform();
    m_triangle.Transform();
    m_circle.Transform();
    m_floor.Transform();

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();
    
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    m_box.Render( matrix );
    m_triangle.Render( matrix );
    m_circle.Render( matrix );
    m_floor.Render( matrix );

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
        CPhysicsWorldManager::Instance().CreateWorld2D( "(game)" );
        
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
        CObjectDataMgr::Instance().FreeGroup2D( "(run)" );
    }
    
    void Unload()
    {
        CPhysicsWorldManager::Instance().DestroyWorld2D( "(game)" );
        
    }   // Unload

}   // NTitleScreenState
