
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
#include <physics/physicsworld3d.h>
#include <physics/physicscomponent3d.h>
#include <managers/spritestrategymanager.h>
#include <3d/basicstagestrategy3d.h>
#include <2d/basicspritestrategy2d.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
    m_rPhysicsWorld( CPhysicsWorldManager::Instance().GetWorld3D( "(cube)" ) )/*,
    m_upFloorPlane( CObjectDataMgr::Instance().GetData3D( "(cube)", "plane" ) ),
    m_cube( CObjectDataMgr::Instance().GetData3D( "(cube)", "cubePhysics") )*/
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

    m_camera.SetPosXYZ( 0, 0, 20 );
    m_camera.SetRotXYZ( 10, 0, 0 );

    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Create the plane on the left of the screen
    /*m_upFloorPlane->SetPos( CPoint<float>( 0.f, 0.f, 0.f ) );
    m_upFloorPlane->SetRot( CPoint<float>( 0.f, 0.f, 0.f ) );*/

    //m_cube.SetRotXYZ( 50, 20, 30 );
    //m_cube.SetPos( CPoint<float>( -5, 8, 0 ) );
    //m_upFloorPlane.SetPos( CPoint<float>( 0, -5, 0 ) );

    //m_upFloorPlane.InitPhysics();
    //m_cube.InitPhysics();

    //m_cube.GetPhysicsComponent().SetAngularVelocity( CPoint<float>( 0, 0, 5 ) );
    //m_cube.GetPhysicsComponent().SetLinearVelocity( CPoint<float>( 3, 0, 0 ) );

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

    //m_cube.Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CRunState::Transform()
{
    CCommonState::Transform();

    //m_upFloorPlane.Transform();
    //m_cube.Transform();
    m_camera.Transform();

    if( !CMenuManager::Instance().IsActive() )
        CSpriteStrategyMgr::Instance().Transform();

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();
    
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_PERSPECTIVE );
    //CSpriteStrategyMgr::Instance().Render( matrix );
    CSpriteStrategyMgr::Instance().Render( m_camera.GetMatrix() * matrix, m_camera.GetRotMatrix() );

    //m_cube.Render( m_camera.GetMatrix() * matrix, m_camera.GetRotMatrix() );
    //m_upFloorPlane.Render( m_camera.GetMatrix() * matrix, m_camera.GetRotMatrix() );

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
        CObjectDataMgr::Instance().LoadGroup3D( "(cube)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData3D( "(cube)" );
    }
    
    void Load()
    {
	    // All physics entities are destroyed and all heap memory is released.
        /*CPhysicsWorldManager::Instance().CreateWorld2D( "(game)" );
        CSpriteStrategyMgr::Instance().Load( "(stage1)", new CBasicStageStrategy2D );
        CSpriteStrategyMgr::Instance().Load( "(sprite)", new CBasicSpriteStrategy2D );*/
        CPhysicsWorldManager::Instance().CreateWorld3D( "(cube)" );

        // The unordered map run these in reverse order
        CSpriteStrategyMgr::Instance().Load( "(stage0)", new CBasicStageStrategy3D );
    }
    
    void CriticalInit()
    {
        /*const char* shapes[] = {"triangle_blue", "triangle_green", "circle_blue", "circle_green", "circle_red", "square_red"};
        
        for( int i = 0; i < 24; ++i )
            CSpriteStrategyMgr::Instance().Create( "(sprite)", shapes[i % 6] );*/

        //CSpriteStrategyMgr::Instance().Create( "(sprite)", shapes[i % 6] );
        CSpriteStrategyMgr::Instance().Init();
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataUnload()
    {
        CObjectDataMgr::Instance().FreeGroup3D( "(cube)", CObjectDataMgr::DONT_FREE_OPENGL_OBJECTS );
    }
    
    void CriticalUnload()
    {
        CSpriteStrategyMgr::Instance().CleanUp();
        /*CObjectDataMgr::Instance().FreeGroup2D( "(run)" );*/
        CObjectDataMgr::Instance().FreeOpenGL3D( "(cube)" );
    }
    
    void Unload()
    {
        CSpriteStrategyMgr::Instance().Clear();
        //CPhysicsWorldManager::Instance().DestroyWorld2D( "(game)" );
        CPhysicsWorldManager::Instance().DestroyWorld3D( "(cube)" );
    }

}   // NTitleScreenState
