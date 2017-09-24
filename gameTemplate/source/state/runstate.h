
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
#include <2d/sprite2d.h>

// Forward declaration(s)
class CPhysicsWorld2D;

class CRunState : public CCommonState
{
public:

    // Constructor
    CRunState();

    // Destructor
    virtual ~CRunState(){};
    
    // Do any pre-game loop init's
    void Init() override;

    // Handle events
    void HandleEvent( const SDL_Event & rEvent ) override;

    // Handle the physics
    void Physics() override;

    // Update objects that require them
    void Update() override;

    // Transform the game objects
    void Transform() override;

    // Do the rendering
    void PreRender() override;

private:
    
    // The script conponent
    CScriptComponent m_scriptComponent;
    
    CSprite2D m_box;
    CSprite2D m_triangle;
    CSprite2D m_circle;
    CSprite2D m_floor;
    
    CPhysicsWorld2D & m_rPhysicsWorld;
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


