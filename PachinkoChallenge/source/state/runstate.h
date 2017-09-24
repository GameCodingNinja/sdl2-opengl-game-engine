
/************************************************************************
*    FILE NAME:       runstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __run_up_state_h__
#define __run_up_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <script/scriptcomponent.h>
#include <common/camera.h>

// Box2D lib dependencies
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <utilities/timer.h>

// Standard lib dependencies
#include <vector>
#include <random>

// Forward declaration(s)
class CPhysicsWorld2D;
class CBasicSpriteStrategy2D;
class CUIMeter;
class CUIButton;
class CSpriteData;

class CRunState : public CCommonState, b2ContactListener
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState();
    
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
    virtual void BeginContact(b2Contact* contact);
    
    // Called when two fixtures cease to touch
    virtual void EndContact(b2Contact* contact);
    
private:
    
    // Resolution change call back
    void ResolutionChangeCallBack();

private:
    
    // The script component
    CScriptComponent m_scriptComponent;
    
    // Physics world
    CPhysicsWorld2D & m_rPhysicsWorld;
    
    // Sprite strategy
    CBasicSpriteStrategy2D & m_rStrategy;
    
    // Strawberry data reference
    CSpriteData & m_rStrawberryData;
    
    // Reference to win meter
    CUIMeter & m_rWinMeter;
    
    // Menu button
    CUIButton & m_rMenuBtn;
    
    // Vector of win values
    std::vector<int> m_winAmount;
    
    // Vector of prize position values
    std::vector<float> m_prizeXPosVec;
    
    // Win Counter
    int m_winCounter;
    
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_ballRand;
    std::uniform_int_distribution<int> m_prizePosRand;
    
    // local camera
    CCamera m_camera;
    
    // Ball type string
    std::vector<std::string> m_ballVec;
    
    // Sprite peg
    enum
    {
        SPRITE_PEG = -2,
        STRAWBERRY = 0,
    };
};


namespace NRunState
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NTitleScreenState


#endif  // __run_up_state_h__


