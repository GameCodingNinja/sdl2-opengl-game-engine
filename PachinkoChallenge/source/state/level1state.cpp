
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
        m_rPhysicsWorld( CPhysicsWorldManager2D::Instance().getWorld( "(game)" ) ),
        m_rStrategy(CSpriteStrategyMgr::Instance().get<CBasicSpriteStrategy>("(level1_spriteStrategy)")),
        m_rStrawberryData(m_rStrategy.getData("strawberry").get<CSpriteData>()),
        m_rMultiplierLabel(CMenuManager::Instance().getMenuControl<CUILabel>( "base_game_menu", "multiplier_label" )),
        m_rWinMeter(CMenuManager::Instance().getMenuControl<CUIMeter>( "base_game_menu", "win_meter" )),
        m_multiIndexPos(0),
        m_totalWin(0),
        m_multiplier(1),
        m_generator(std::random_device{}()),
        m_ballRand(0, 8)
{
    // The state inherits from b2ContactListener to handle physics collisions
    // so this state is the collision listener
    m_rPhysicsWorld.getWorld().SetContactListener(this);
    m_rPhysicsWorld.getWorld().SetDestructionListener(this);
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CLevel1State::~CLevel1State()
{
    m_rPhysicsWorld.getWorld().SetDestructionListener(nullptr);
    m_rPhysicsWorld.getWorld().SetContactListener(nullptr);
    CSignalMgr::Instance().disconnect_resolutionChange();
}


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CLevel1State::init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree("pause_tree");
    CMenuManager::Instance().activateTree("base_game_tree");

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
    m_scriptComponent.prepare( "(menu)", "Screen_FadeIn" );

    // Set the initial camera scale
    CCameraMgr::Instance().createOrthographic("game", 5.f, 1000.f, 1.65);
    CCameraMgr::Instance().transform();

    // Reset the camera
    //m_camera.SetPos( CSpriteStrategyMgr::Instance().Get<CBasicStageStrategy2D>("(stage1)").GetDefaultCameraPos().GetPos() );
    //m_camera.Transform();

    // Since these interface elements don't move only need to transform them once.
    CMenuManager::Instance().transformInterface();

    // Flush any user inputs that have been queued up
    SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);

    // Add the first strawberry
    std::uniform_int_distribution<int> multiPosRand(0, m_multiXPosVec.back().size()-1);
    m_multiIndexPos = multiPosRand(m_generator);
    CSpriteStrategyMgr::Instance().create(
        "(level1_spriteStrategy)",
        "strawberry",
        CPoint<float>(m_multiXPosVec.back().at(m_multiIndexPos),-1450.f) );

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CLevel1State::handleEvent( const SDL_Event & rEvent )
{
    CCommonState::handleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
            m_scriptComponent.prepare( "(menu)", "Screen_FadeOut" );
    }
    else if( rEvent.type == SDL_MOUSEBUTTONUP)
    {
        if( !CMenuManager::Instance().isMenuActive() )
        {
            auto camera = CCameraMgr::Instance().getActiveCamera();
            const float ratio = 1.f / camera.getOrthoHeightAspectRatio();
            const float x = (ratio * (float)rEvent.motion.x) - camera.getOrthoProjSizeHalf().w;

            CSpriteStrategyMgr::Instance().create("(level1_spriteStrategy)", m_ballVec.at(m_ballRand(m_generator)), CPoint<float>(x, 1050));
        }
    }
}


/************************************************************************
*    desc:  Handle any misc processing before the real work is started
************************************************************************/
void CLevel1State::miscProcess()
{
    if( !CMenuManager::Instance().isMenuActive() )
    {
        CSpriteStrategyMgr::Instance().miscProcess();
    }
}


/***************************************************************************
*    desc:  Handle the physics
****************************************************************************/
void CLevel1State::physics()
{
    if( !CMenuManager::Instance().isMenuActive() )
    {
        m_rPhysicsWorld.variableTimeStep();
    }
}


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CLevel1State::update()
{
    CCommonState::update();

    m_scriptComponent.update();

    CScriptManager::Instance().update();

    if( !CMenuManager::Instance().isMenuActive() )
    {
        CSpriteStrategyMgr::Instance().update();
    }
}


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CLevel1State::transform()
{
    CCommonState::transform();

    CSpriteStrategyMgr::Instance().transform();
}


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CLevel1State::preRender()
{
    CCommonState::preRender();

    CSpriteStrategyMgr::Instance().render( CCameraMgr::Instance().getActiveCameraMatrix() );

    CMenuManager::Instance().renderInterface( CCameraMgr::Instance().getDefaultProjMatrix() );
}


/************************************************************************
*    desc:  Called when two fixtures begin to touch
************************************************************************/
void CLevel1State::BeginContact(b2Contact* contact)
{
    CSprite2D * pSpriteA = (CSprite2D *)contact->GetFixtureA()->GetUserData();
    CSprite2D * pSpriteB = (CSprite2D *)contact->GetFixtureB()->GetUserData();

    if( (pSpriteA != nullptr) && (pSpriteB != nullptr) )
    {
        const int spriteAid = pSpriteA->getId();
        const int spriteBid = pSpriteB->getId();

        if( spriteAid == SPRITE_PEG )
            pSpriteA->setFrame(1);

        else if( spriteBid == SPRITE_PEG )
            pSpriteB->setFrame(1);

        else if( (spriteAid == STRAWBERRY) || (spriteBid == STRAWBERRY) )
        {
            m_rStrategy.setToDestroy( STRAWBERRY );

            m_rMultiplierLabel.createFontString( std::to_string(++m_multiplier) + "x" );

            // Randomly pick the new position of the multiplier
            std::uniform_int_distribution<int> multiPosRand(0, m_multiXPosVec.at(m_multiIndexPos).size()-1);
            int multiPos = m_multiXPosVec.at(m_multiIndexPos).at(multiPosRand(m_generator));
            m_multiIndexPos = std::find(m_multiXPosVec.back().begin(), m_multiXPosVec.back().end(), multiPos) - m_multiXPosVec.back().begin();

            // Add another strawberry
            m_rStrawberryData.setPos( multiPos, -1450.f );
            m_rStrategy.setToCreate( "strawberry" );
        }
    }
}


/************************************************************************
*    desc:  Called when two fixtures cease to touch
************************************************************************/
void CLevel1State::EndContact(b2Contact* contact)
{
    CSprite2D * pSpriteA = reinterpret_cast<CSprite2D *>(contact->GetFixtureA()->GetUserData());
    CSprite2D * pSpriteB = reinterpret_cast<CSprite2D *>(contact->GetFixtureB()->GetUserData());

    if( (pSpriteA != nullptr) && (pSpriteB != nullptr) )
    {
        if( pSpriteA->getId() == SPRITE_PEG )
            pSpriteA->setFrame(0);

        else if( pSpriteB->getId() == SPRITE_PEG )
            pSpriteB->setFrame(0);
    }
}


/************************************************************************
*    desc:  Called when any fixture is about to be destroyed
************************************************************************/
void CLevel1State::SayGoodbye(b2Fixture* fixture)
{
    CSprite2D * pSprite = reinterpret_cast<CSprite2D *>(fixture->GetUserData());

    if( (pSprite->getId() > 1000) && (std::fabs( pSprite->getPos().getX() ) < 720.f) )
    {
        m_totalWin += m_multiplier;
        m_rWinMeter.startBangUp( m_totalWin );
    }
}


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
        CObjectDataMgr::Instance().loadGroup2D( "(level1)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        CObjectDataMgr::Instance().loadGroup2D( "(levels)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }

    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(level1)" );
        CObjectDataMgr::Instance().createFromData2D( "(levels)" );
    }

    void Load()
    {
        // Load the state specific menu group
        CMenuManager::Instance().loadGroup("(levels)", CMenuManager::DONT_INIT_GROUP);

        // Load state specific AngelScript functions
        CScriptManager::Instance().loadGroup("(level1)");

        CPhysicsWorldManager2D::Instance().createWorld( "(game)" );

        // Load the sprite strategies
        CSpriteStrategyMgr::Instance().addStrategy( "(level1_spriteStrategy)", new CBasicSpriteStrategy(1000) );
        CSpriteStrategyMgr::Instance().addStrategy( "(level1_stage1Strategy)", new CBasicStageStrategy );
    }

    void CriticalInit()
    {
        // Creates the font strings, run init scripts
        CMenuManager::Instance().initGroup("(levels)");

        CSpriteStrategyMgr::Instance().init();
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CMenuManager::Instance().cleanUpGroup("(levels)");
        CSpriteStrategyMgr::Instance().cleanUp();
        CObjectDataMgr::Instance().freeGroup2D( "(level1)" );
        CObjectDataMgr::Instance().freeGroup2D( "(levels)" );
    }

    void Unload()
    {
        // Unload the state specific menu group
        CMenuManager::Instance().freeGroup("(levels)");

        // Unload the strategy group stuff
        CSpriteStrategyMgr::Instance().clear();

        // Unload state specific AngelScript functions
        CScriptManager::Instance().freeGroup("(level1)");

        // All physics entities are destroyed and all heap memory is released.
        CPhysicsWorldManager2D::Instance().destroyWorld( "(game)" );
    }
}
