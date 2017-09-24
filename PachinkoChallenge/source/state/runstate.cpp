
/************************************************************************
*    FILE NAME:       runstate.cpp
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

// Physical component dependency
#include "runstate.h"

// Game dependencies
#include "../strategy/ballstrategy.h"

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
#include <gui/uimeter.h>
#include <gui/uibutton.h>

// Box2D lib dependencies
#include <Box2D/Box2D.h>

/************************************************************************
*    desc:  Constructer                                                             
************************************************************************/
CRunState::CRunState() :
    CCommonState( NGameDefs::EGS_RUN, NGameDefs::EGS_GAME_LOAD ),
        m_rPhysicsWorld( CPhysicsWorldManager::Instance().GetWorld2D( "(game)" ) ),
        m_rStrategy(CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy2D>("(sprite)")),
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
    
    // Connect to the Resolution Change signal
    CSignalMgr::Instance().Connect_ResolutionChange( boost::bind(&CRunState::ResolutionChangeCallBack, this) );
    
}   // Constructer


/************************************************************************
*    desc:  destructer
************************************************************************/
CRunState::~CRunState()
{
    m_rPhysicsWorld.GetWorld().SetContactListener(nullptr);
    CSignalMgr::Instance().Disconnect_ResolutionChange();
    
}   // destructer


/************************************************************************
*    desc:  Resolution change call back
************************************************************************/
void CRunState::ResolutionChangeCallBack()
{
    // Scale the playing field to match the aspect ratio
    m_camera.GenerateOrthographicProjection(
        CSettings::Instance().GetNativeSize().w / CSettings::Instance().GetDefaultSize().w );
    
}   // ResolutionChangeCallBack


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CRunState::Init()
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
    
    // Reset the camera
    m_camera.SetPos( CSpriteStrategyMgr::Instance().Get<CBasicStageStrategy2D>("(stage1)").GetDefaultCameraPos().GetPos() );
    m_camera.Transform();
    
    // Flush any user inputs that have been queued up
    SDL_FlushEvents(SDL_KEYDOWN, SDL_MULTIGESTURE);
    
    // Scale the playing field to match the aspect ratio
    m_camera.GenerateOrthographicProjection(
        CSettings::Instance().GetNativeSize().w / CSettings::Instance().GetDefaultSize().w );
    
    // Add the first strawberry
    CSpriteStrategyMgr::Instance().Create("(sprite)", "strawberry", CPoint<float>(m_prizeXPosVec.at(m_prizePosRand(m_generator)),-550.f) );
    
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
    else if( rEvent.type == SDL_MOUSEBUTTONUP)
    {
        if( !CMenuManager::Instance().IsMenuActive() && m_rMenuBtn.IsInactive() )
        {
            const float ratio = 1.f / m_camera.GetOrthoHeightAspectRatio();
            const float x = (ratio * (float)rEvent.motion.x) - m_camera.GetOrthoProjSizeHalf().w;

            CSpriteStrategyMgr::Instance().Create("(sprite)", m_ballVec.at(m_ballRand(m_generator)), CPoint<float>(x, 500));
        }
    }

}   // HandleEvent


/************************************************************************
*    desc:  Handle any misc processing before the real work is started
************************************************************************/
void CRunState::MiscProcess()
{
    if( !CMenuManager::Instance().IsMenuActive() )
    {
        CSpriteStrategyMgr::Instance().MiscProcess();
    }
    
}   // MiscProcess


/***************************************************************************
*    desc:  Handle the physics
****************************************************************************/
void CRunState::Physics()
{
    if( !CMenuManager::Instance().IsMenuActive() )
    {
        m_rPhysicsWorld.VariableTimeStep();
    }

}   // Physics


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CRunState::Update()
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
void CRunState::Transform()
{
    CCommonState::Transform();
    
    CSpriteStrategyMgr::Instance().Transform();

}   // Transform


/***************************************************************************
*    desc:  Do the 2D rendering
****************************************************************************/
void CRunState::PreRender()
{
    CCommonState::PreRender();
    
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    CSpriteStrategyMgr::Instance().Render( m_camera.GetMatrix() * m_camera.GetProjectionMatrix() );

    CMenuManager::Instance().RenderInterface( matrix );

}   // PreRender


/************************************************************************
*    desc:  Called when two fixtures begin to touch
************************************************************************/
void CRunState::BeginContact(b2Contact* contact)
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
            m_rStrawberryData.SetPos( CPoint<float>( m_prizeXPosVec.at(m_prizePosRand(m_generator)), -550.f ));
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
void CRunState::EndContact(b2Contact* contact)
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
        CPhysicsWorldManager::Instance().CreateWorld2D( "(game)" );
        
        // The unordered map run these in reverse order
        CSpriteStrategyMgr::Instance().Load( "(stage1)", new CBasicStageStrategy2D );
        CSpriteStrategyMgr::Instance().Load( "(sprite)", new CBallStrategy(1000) );
        
    }   // ThreadLoad
    
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
        CObjectDataMgr::Instance().FreeGroup2D( "(run)" );
    }
    
    void Unload()
    {
        CSpriteStrategyMgr::Instance().Clear();
        
        // All physics entities are destroyed and all heap memory is released.
        CPhysicsWorldManager::Instance().DestroyWorld2D( "(game)" );
        
    }   // Unload

}   // NTitleScreenState
