
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
#include <utilities/settings.h>
#include <utilities/mathfunc.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <physics/physicsworldmanager3d.h>
#include <physics/physicsworld3d.h>
#include <physics/physicscomponent3d.h>
#include <managers/spritestrategymanager.h>
#include <3d/basicstagestrategy3d.h>
#include <3d/basicspritestrategy3d.h>

// Standard lib dependencies
#include <vector>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
    m_rPhysicsWorld( CPhysicsWorldManager3D::Instance().GetWorld( "(cube)" ) )
{
}   // Constructor


/************************************************************************
*    desc:  destructer
************************************************************************/
CRunState::~CRunState()
{
    /*m_rPhysicsWorld.GetWorld().SetContactListener( nullptr );
    CSignalMgr::Instance().Disconnect_ResolutionChange();*/

    CSpriteStrategyMgr::Instance().Clear();

}   // destructer


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CRunState::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().Allow();
    CMenuManager::Instance().ActivateTree("pause_tree");

    m_camera.SetPosXYZ( 0, 0, 20 );
    //m_camera.SetRotXYZ( 10, 0, 0 );

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
    else if( rEvent.type == SDL_MOUSEBUTTONUP )
    {
        if( !CMenuManager::Instance().IsMenuActive() && !CMenuManager::Instance().IsActive() )
        {
            CPoint<float> r = NMathFunc::MouseTo3D( 
                rEvent.button.x, rEvent.button.y, m_camera,
                CDevice::Instance().GetProjectionMatrix( NDefs::EPT_PERSPECTIVE ) );

            //rEvent.button.x
            CSpriteStrategyMgr::Instance().Create( "(sprite)", "cube", r, CPoint<float>( 20, 30, 40 ) );
        }
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
        CPhysicsWorldManager3D::Instance().CreateWorld( "(cube)" );

        // The unordered map run these in reverse order
        //CSpriteStrategyMgr::Instance().Load( "(stage0)", new CBasicStageStrategy3D );
        //CSpriteStrategyMgr::Instance().Load( "(sprite)", new CBasicSpriteStrategy3D );
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
        CPhysicsWorldManager3D::Instance().DestroyWorld( "(cube)" );
    }

}   // NTitleScreenState
