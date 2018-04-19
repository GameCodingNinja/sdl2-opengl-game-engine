
/************************************************************************
*    FILE NAME:       isprite.h
*
*    DESCRIPTION:     iSprite Class
************************************************************************/

#ifndef __i_sprite_h__
#define __i_sprite_h__

// Physical component dependency
#include <2d/object2d.h>

// Forward declaration(s)
union SDL_Event;
class iAIBase;
class CPhysicsComponent2D;

class iSprite : public virtual CObject2D
{
public:
    
    // Init the sprite
    virtual void Init(){};
    
    // Clean up the sprite
    virtual void CleanUp(){};
    
    // Init the physics
    virtual void InitPhysics() = 0;
    
    // React to what the player is doing
    virtual void HandleEvent( const SDL_Event & rEvent ) = 0;

    // Update the sprite 
    virtual void Update() = 0;
    
    // Update the physics 
    virtual void PhysicsUpdate() = 0;

    // do the render
    virtual void Render( const CMatrix & matrix ){};
    virtual void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ){};
    
    // Get the unique id number
    virtual int GetId() const = 0;
    
    // Set/Get the AI pointer
    virtual void SetAI( iAIBase * pAIBase ) = 0;
    
    // Is this a 2D sprite?
    bool IsSprite2D();
    
    // Is this a 2D actor?
    bool IsActor2D();
    
    // Is this a 3D sprite?
    bool IsSprite3D();
    
    // Is this a 3D actor?
    bool IsActor3D();
};

#endif  // __i_sprite_2d_h__
