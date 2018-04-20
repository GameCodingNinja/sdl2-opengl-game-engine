
/************************************************************************
*    FILE NAME:       level1state.h
*
*    DESCRIPTION:     CStage1State Class State
************************************************************************/

#ifndef __stage_1_state_h__
#define __stage_1_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <script/scriptcomponent.h>
#include <common/camera.h>
#include <2d/object2d.h>
#include <2d/sprite2d.h>
#include <utilities/timer.h>

// Box2D lib dependencies
#include <Box2D/Dynamics/b2WorldCallbacks.h>

// Standard lib dependencies
#include <vector>
#include <random>

// Forward declaration(s)
class CPhysicsWorld2D;
class CBasicSpriteStrategy;
class CUIMeter;
class CUILabel;
class CSpriteData;

class CLevel1State : public CCommonState, b2ContactListener, b2DestructionListener
{
public:

    // Constructor
    CLevel1State();

    // Destructor
    virtual ~CLevel1State();
    
    // Do any pre-game loop init's
    void Init() override;

    // Handle events
    void HandleEvent( const SDL_Event & rEvent ) override;
    
    // Handle any misc processing before the real work is started
    void MiscProcess() override;

    // Handle the physics
    void Physics() override;

    // Update objects that require them
    void Update() override;

    // Transform the game objects
    void Transform() override;

    // Do the rendering
    void PreRender() override;
    
    // Called when two fixtures begin to touch
    void BeginContact(b2Contact* contact) override;
    
    // Called when two fixtures cease to touch
    void EndContact(b2Contact* contact) override;
    
    // Called when any joint is about to be destroyed
    void SayGoodbye(b2Joint* joint) override {}
    
    // Called when any fixture is about to be destroyed
    void SayGoodbye(b2Fixture* fixture) override;

private:
    
    // The script component
    CScriptComponent m_scriptComponent;
    
    // Physics world
    CPhysicsWorld2D & m_rPhysicsWorld;
    
    // Sprite strategy
    CBasicSpriteStrategy & m_rStrategy;
    
    // Strawberry data reference
    CSpriteData & m_rStrawberryData;
    
    // Multiplier label
    CUILabel & m_rMultiplierLabel;
    
    // Reference to win meter
    CUIMeter & m_rWinMeter;
    
    // Vector of prize position values
    std::vector<std::vector<float>> m_multiXPosVec;
    
    // Multiplier index position
    int m_multiIndexPos;
    
    // Win Amount
    int m_totalWin;
    
    // multiplier
    int m_multiplier;
    
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_ballRand;
    
    // Ball type string
    std::vector<std::string> m_ballVec;

    // Sprite peg
    enum
    {
        SPRITE_PEG = -2,
        STRAWBERRY = 0,
    };
};


namespace NLevel1State
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NLevel1State


#endif  // __stage_1_state_h__


