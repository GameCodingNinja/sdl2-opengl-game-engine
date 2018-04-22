
/************************************************************************
*    FILE NAME:       level1state.cpp
*
*    DESCRIPTION:     CStage1State Class State
************************************************************************/

// Physical component dependency
#include "level1state.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "Box2D/Dynamics/b2World.h"

// Game lib dependencies
#include <gui/menumanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <physics/physicscomponent2d.h>
#include <2d/sprite2d.h>
#include <spritestrategy/basicstagestrategy.h>
#include <spritestrategy/basicspritestrategy.h>
#include <spritestrategy/spritestrategymanager.h>
#include <managers/soundmanager.h>
#include <managers/cameramanager.h>
#include <script/scriptmanager.h>
#include <managers/signalmanager.h>
#include <common/camera.h>
#include <common/spritedata.h>
#include <common/worldvalue.h>
#include <gui/uimeter.h>
#include <gui/uibutton.h>
#include <gui/uilabel.h>
#include <script/scriptmanager.h>

// Box2D lib dependencies
#include <Box2D/Box2D.h>

// Standard lib dependencies
#include <cstdlib>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CLevel1State::CLevel1State() :
    CCommonState( NGameDefs::EGS_LEVEL_1, NGameDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().GetWorld( "(game)" ) ),
        m_rStrategy(CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy>("(level1_spriteStrategy)")),
        m_rStrawberryData(m_rStrategy.GetData("strawberry").Get<CSpriteData>()),
        m_rMultiplierLabel(CMenuManager::Instance().GetMenuControl<CUILabel>( "base_game_menu", "multiplier_label" )),
        m_rWinMeter(CMenuManager::Instance().GetMenuControl<CUIMeter>( "base_game_menu", "win_meter" )),
        m_multiIndexPos(0),
        m_totalWin(0),
        m_multiplier(1),
        m_generator(std::random_device{}()),
        m_ballRand(0, 8)
{
    // The state inherits from b2ContactListener to handle physics collisions
    // so this state is the collision listener
    m_rPhysicsWorld.GetWorld().SetContactListener(this);
    m_rPhysicsWorld.GetWorld().SetDestructionListener(this);
    
}   // Constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CLevel1State::~CLevel1State()
{
    m_rPhysicsWorld.GetWorld().SetDestructionListener(nullptr);
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
    
    //CMenuManager::Instance().ActivateMenu("pause_tree", "confirmation_menu");
    
    m_ballVec = {"circle_green", "circle_blue", "circle_red",
                 "square_green", "square_blue", "square_red",
                 "triangle_green", "triangle_blue", "triangle_red"};
    m_multiXPosVec = 
        { {-160,0,160,320,480,640},
          {0,160,320,480,640},
          {160,320,480,640},
          {-640,320,480,640},
          {-640,-480,480,640},
          {-640,-480,-320,640},
          {-640,-480,-320,-160},
          {-640,-480,-320,-160,0},
          {-640,-480,-320,-160,0,160},
          {-640,-480,-320,-160,0,160,320,480,640} };
    
    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Set the initial camera scale
    CCameraMgr::Instance().CreateOrthographic("game", 5.f, 1000.f, 1.65);
    CCameraMgr::Instance().Transform();
    
    // Reset the camera
    //m_camera.SetPos( CSpriteStrategyMgr::Instance().Get<CBasicStageStrategy2D>("(stage1)").GetDefaultCameraPos().GetPos() );
    //m_camera.Transform();
    
    // Since these interface elements don't move only need to transform them once.
    CMenuManager::Instance().TransformInterface();
    
    // Flush any user inputs that have been queued up
    SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);
    
    // Add the first strawberry
    std::uniform_int_distribution<int> multiPosRand(0, m_multiXPosVec.back().size()-1);
    m_multiIndexPos = multiPosRand(m_generator);
    CSpriteStrategyMgr::Instance().Create(
        "(level1_spriteStrategy)",
        "strawberry",
        CPoint<float>(m_multiXPosVec.back().at(m_multiIndexPos),-1450.f) );
    
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
        if( !CMenuManager::Instance().IsMenuActive() )
        {
            auto camera = CCameraMgr::Instance().GetActiveCamera();
            const float ratio = 1.f / camera.GetOrthoHeightAspectRatio();
            const float x = (ratio * (float)rEvent.motion.x) - camera.GetOrthoProjSizeHalf().w;

            CSpriteStrategyMgr::Instance().Create("(level1_spriteStrategy)", m_ballVec.at(m_ballRand(m_generator)), CPoint<float>(x, 1050));
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
    
    CScriptManager::Instance().Update();
    
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

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CLevel1State::PreRender()
{
    CCommonState::PreRender();
    
    CSpriteStrategyMgr::Instance().Render( CCameraMgr::Instance().GetActiveCameraMatrix() );

    CMenuManager::Instance().RenderInterface( CCameraMgr::Instance().GetDefaultProjMatrix() );

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
            m_rStrategy.SetToDestroy( STRAWBERRY );
            
            m_rMultiplierLabel.CreateFontString( std::to_string(++m_multiplier) + "x" );
            
            // Randomly pick the new position of the multiplier
            std::uniform_int_distribution<int> multiPosRand(0, m_multiXPosVec.at(m_multiIndexPos).size()-1);
            int multiPos = m_multiXPosVec.at(m_multiIndexPos).at(multiPosRand(m_generator));
            m_multiIndexPos = std::find(m_multiXPosVec.back().begin(), m_multiXPosVec.back().end(), multiPos) - m_multiXPosVec.back().begin();
            
            // Add another strawberry
            m_rStrawberryData.SetPos( multiPos, -1450.f );
            m_rStrategy.SetToCreate( "strawberry" );
        }
    }
    
}   // BeginContact


/************************************************************************
*    desc:  Called when two fixtures cease to touch
************************************************************************/
void CLevel1State::EndContact(b2Contact* contact)
{
    CSprite2D * pSpriteA = reinterpret_cast<CSprite2D *>(contact->GetFixtureA()->GetUserData());
    CSprite2D * pSpriteB = reinterpret_cast<CSprite2D *>(contact->GetFixtureB()->GetUserData());
    
    if( (pSpriteA != nullptr) && (pSpriteB != nullptr) )
    {
        if( pSpriteA->GetId() == SPRITE_PEG )
            pSpriteA->SetFrame(0);
        
        else if( pSpriteB->GetId() == SPRITE_PEG )
            pSpriteB->SetFrame(0);
    }
    
}   // EndContact


/************************************************************************
*    desc:  Called when any fixture is about to be destroyed
************************************************************************/
void CLevel1State::SayGoodbye(b2Fixture* fixture)
{
    CSprite2D * pSprite = reinterpret_cast<CSprite2D *>(fixture->GetUserData());
    
    if( (pSprite->GetId() > 1000) && (std::fabs( pSprite->GetPos().getX() ) < 720.f) )
    {
        m_totalWin += m_multiplier;
        m_rWinMeter.StartBangUp( m_totalWin );
    }
    
}   // SayGoodbye


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
        CObjectDataMgr::Instance().LoadGroup2D( "(levels)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData2D( "(level1)" );
        CObjectDataMgr::Instance().CreateFromData2D( "(levels)" );
    }
    
    void Load()
    {
        // Load the state specific menu group
        CMenuManager::Instance().LoadGroup("(levels)", CMenuManager::DONT_INIT_GROUP);
        
        // Load state specific AngelScript functions
        CScriptManager::Instance().LoadGroup("(level1)");
        
        CPhysicsWorldManager2D::Instance().CreateWorld( "(game)" );
        
        // Load the sprite strategies
        CSpriteStrategyMgr::Instance().AddStrategy( "(level1_spriteStrategy)", new CBasicSpriteStrategy(1000) );
        CSpriteStrategyMgr::Instance().AddStrategy( "(level1_stage1Strategy)", new CBasicStageStrategy );
    }
    
    void CriticalInit()
    {
        // Creates the font strings, run init scripts
        CMenuManager::Instance().InitGroup("(levels)");
        
        CSpriteStrategyMgr::Instance().Init();
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {        
        CMenuManager::Instance().CleanUpGroup("(levels)");
        CSpriteStrategyMgr::Instance().CleanUp();
        CObjectDataMgr::Instance().FreeGroup2D( "(level1)" );
        CObjectDataMgr::Instance().FreeGroup2D( "(levels)" );
    }
    
    void Unload()
    {
        // Unload the state specific menu group
        CMenuManager::Instance().FreeGroup("(levels)");
        
        // Unload the strategy group stuff
        CSpriteStrategyMgr::Instance().Clear();
        
        // Unload state specific AngelScript functions
        CScriptManager::Instance().FreeGroup("(level1)");
        
        // All physics entities are destroyed and all heap memory is released.
        CPhysicsWorldManager2D::Instance().DestroyWorld( "(game)" );
    }

}   // NTitleScreenState
