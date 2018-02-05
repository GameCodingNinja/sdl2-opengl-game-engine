
/************************************************************************
*    FILE NAME:       level1state.cpp
*
*    DESCRIPTION:     CStage1State Class State
************************************************************************/

// Physical component dependency
#include "level1state.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <system/device.h>
#include <physics/physicsworldmanager.h>
#include <physics/physicsworld2d.h>
#include <physics/physicscomponent2d.h>
#include <2d/basicstagestrategy2d.h>
#include <2d/basicspritestrategy2d.h>
#include <2d/sprite2d.h>
#include <managers/spritestrategymanager.h>
#include <managers/soundmanager.h>
#include <managers/signalmanager.h>
#include <common/camera.h>
#include <common/spritedata.h>
#include <common/worldvalue.h>
#include <gui/uimeter.h>
#include <gui/uibutton.h>
#include <script/scriptmanager.h>

// Box2D lib dependencies
#include <Box2D/Box2D.h>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CLevel1State::CLevel1State() :
    CCommonState( NGameDefs::EGS_LEVEL_1, NGameDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager::Instance().GetWorld2D( "(game)" ) ),
        m_rStrategy(CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy2D>("(level1_spriteStrategy)")),
        m_rStrawberryData(m_rStrategy.GetData("strawberry").Get<CSpriteData>()),
        m_rWinMeter(CMenuManager::Instance().GetMenuControl<CUIMeter>( "base_game_menu", "win_meter" )),
        m_rMenuBtn(CMenuManager::Instance().GetMenuControl<CUIButton>( "base_game_menu", "menu_btn" )),
        m_winCounter(0),
        m_generator(std::random_device{}()),
        m_ballRand(0, 8),
        m_prizePosRand(0, 8)
{
    // The state inherits from b2ContactListener to handle physics collisions
    // so this state is the collision listener
    m_rPhysicsWorld.GetWorld().SetContactListener(this);
    
}   // Constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CLevel1State::~CLevel1State()
{
    m_rPhysicsWorld.GetWorld().SetContactListener(nullptr);
    CSignalMgr::Instance().Disconnect_ResolutionChange();
    
}   // destructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CLevel1State::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().Allow();
    CMenuManager::Instance().ActivateTree("pause_tree");
    CMenuManager::Instance().ActivateTree("base_game_tree");
    
    m_winAmount = {9, 1, 5, 2, 4, 2, 5, 1, 9};
    m_ballVec = {"circle_green", "circle_blue", "circle_red",
                 "square_green", "square_blue", "square_red",
                 "triangle_green", "triangle_blue", "triangle_red"};
    m_prizeXPosVec = {-640,-480,-320,-160,0,160,320,480,640};
    
    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Set the initial camera scale
    m_camera.GenerateOrthographicProjection( 1.65 );
    
    // Reset the camera
    //m_camera.SetPos( CSpriteStrategyMgr::Instance().Get<CBasicStageStrategy2D>("(stage1)").GetDefaultCameraPos().GetPos() );
    //m_camera.Transform();
    
    // Flush any user inputs that have been queued up
    SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);
    
    // Add the first strawberry
    CSpriteStrategyMgr::Instance().Create("(level1_spriteStrategy)", "strawberry", CPoint<float>(m_prizeXPosVec.at(m_prizePosRand(m_generator)),-1450.f) );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
    
}   // Init


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CLevel1State::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN ) 
            m_scriptComponent.Prepare( "(menu)", "Screen_FadeOut" );
    }
    else if( rEvent.type == SDL_MOUSEBUTTONUP)
    {
        if( !CMenuManager::Instance().IsMenuActive() && m_rMenuBtn.IsInactive() )
        {
            const float ratio = 1.f / m_camera.GetOrthoHeightAspectRatio();
            const float x = (ratio * (float)rEvent.motion.x) - m_camera.GetOrthoProjSizeHalf().w;

            CSpriteStrategyMgr::Instance().Create("(level1_spriteStrategy)", m_ballVec.at(m_ballRand(m_generator)), CPoint<float>(x, 1350));
        }
    }

}   // HandleEvent


/************************************************************************
*    desc:  Handle any misc processing before the real work is started
************************************************************************/
void CLevel1State::MiscProcess()
{
    if( !CMenuManager::Instance().IsMenuActive() )
    {
        CSpriteStrategyMgr::Instance().MiscProcess();
    }
    
}   // MiscProcess


/***************************************************************************
*    desc:  Handle the physics
****************************************************************************/
void CLevel1State::Physics()
{
    if( !CMenuManager::Instance().IsMenuActive() )
    {
        m_rPhysicsWorld.VariableTimeStep();
    }

}   // Physics


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CLevel1State::Update()
{
    CCommonState::Update();
    
    m_scriptComponent.Update();
    
    if( !CMenuManager::Instance().IsMenuActive() )
    {
        CSpriteStrategyMgr::Instance().Update();
    }

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CLevel1State::Transform()
{
    CCommonState::Transform();
    
    CSpriteStrategyMgr::Instance().Transform();
    
    CMenuManager::Instance().TransformInterface( m_Object2D );

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CLevel1State::PreRender()
{
    CCommonState::PreRender();
    
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    CSpriteStrategyMgr::Instance().Render( m_camera.GetMatrix() * m_camera.GetProjectionMatrix() );

    CMenuManager::Instance().RenderInterface( matrix );

}   // PreRender


/************************************************************************
*    desc:  Called when two fixtures begin to touch
************************************************************************/
void CLevel1State::BeginContact(b2Contact* contact)
{
    CSprite2D * pSpriteA = (CSprite2D *)contact->GetFixtureA()->GetUserData();
    CSprite2D * pSpriteB = (CSprite2D *)contact->GetFixtureB()->GetUserData();
    
    if( (pSpriteA != nullptr) && (pSpriteB != nullptr) )
    {
        const int spriteAid = pSpriteA->GetId();
        const int spriteBid = pSpriteB->GetId();
        
        if( spriteAid == SPRITE_PEG )
            pSpriteA->SetFrame(1);
        
        else if( spriteBid == SPRITE_PEG )
            pSpriteB->SetFrame(1);
        
        else if( (spriteAid == STRAWBERRY) || (spriteBid == STRAWBERRY) )
        {
            m_winCounter += 1;
            m_rWinMeter.StartBangUp(m_winCounter);
            m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, STRAWBERRY );
            
            // Add another strawberry
            m_rStrawberryData.SetPosXYZ( m_prizeXPosVec.at(m_prizePosRand(m_generator)), -1450.f);
            m_rStrategy.HandleMessage( NDefs::ESM_CREATE_SPRITE, "strawberry" );
        }
                
        /*else if( (spriteAid > -1) && (spriteAid < 10) )
        {
            m_winCounter += m_winAmount.at(spriteAid);
            m_rWinMeter.StartBangUp(m_winCounter);
            m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, spriteBid );
        }

        else if( (spriteBid > -1) && (spriteBid < 10) )
        {
            m_winCounter += m_winAmount.at(spriteBid);
            m_rWinMeter.StartBangUp(m_winCounter);
            m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, spriteAid );
        }*/
    }
    
}   // BeginContact


/************************************************************************
*    desc:  Called when two fixtures cease to touch
************************************************************************/
void CLevel1State::EndContact(b2Contact* contact)
{
    CSprite2D * pSpriteA = (CSprite2D *)contact->GetFixtureA()->GetUserData();
    CSprite2D * pSpriteB = (CSprite2D *)contact->GetFixtureB()->GetUserData();
    
    if( (pSpriteA != nullptr) && (pSpriteB != nullptr) )
    {
        if( pSpriteA->GetId() == SPRITE_PEG )
            pSpriteA->SetFrame(0);
        
        else if( pSpriteB->GetId() == SPRITE_PEG )
            pSpriteB->SetFrame(0);
    }
    
}   // EndContact


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NLevel1State
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().LoadGroup2D( "(level1)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData2D( "(level1)" );
    }
    
    void Load()
    {
        // Load state specific AngelScript functions
        CScriptManager::Instance().LoadGroup("(level1)");
        
        CPhysicsWorldManager::Instance().CreateWorld2D( "(game)" );
        
        // Load the sprite strategies
        CSpriteStrategyMgr::Instance().Load( "(level1_spriteStrategy)", new CBasicSpriteStrategy2D(1000) );
        CSpriteStrategyMgr::Instance().Load( "(level1_stage1Strategy)", new CBasicStageStrategy2D );
    }
    
    void CriticalInit()
    {
        CSpriteStrategyMgr::Instance().Init();
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CSpriteStrategyMgr::Instance().CleanUp();
        CObjectDataMgr::Instance().FreeGroup2D( "(level1)" );
    }
    
    void Unload()
    {
        CSpriteStrategyMgr::Instance().Clear();
        
        // Unload state specific AngelScript functions
        CScriptManager::Instance().FreeGroup("(level1)");
        
        // All physics entities are destroyed and all heap memory is released.
        CPhysicsWorldManager::Instance().DestroyWorld2D( "(game)" );
    }

}   // NTitleScreenState
