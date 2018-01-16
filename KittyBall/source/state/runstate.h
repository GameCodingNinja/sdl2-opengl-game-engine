
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
#include <3d/sprite3d.h>
#include <common/camera.h>

// Forward declaration(s)
class CPhysicsWorld2D;

class CRunState : public CCommonState
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
    
    // Physics world
    //CPhysicsWorld2D & m_rPhysicsWorld;
    CPhysicsWorld3D & m_rPhysicsWorld;

    CCamera m_camera;
};


namespace NRunState
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void ObjectDataUnload();
    void Unload();
    void CriticalInit();
    
}   // NTitleScreenState


#endif  // __run_up_state_h__


