
/************************************************************************
*    FILE NAME:       actorsprite2d.h
*
*    DESCRIPTION:     2D Actor Sprite Class
************************************************************************/

#ifndef __actor_sprite_2d_h__
#define __actor_sprite_2d_h__

// Physical component dependency
#include <2d/isprite2d.h>

// Game lib dependencies
#include <common/defs.h>
#include <2d/sprite2d.h>
#include <physics/physicscomponent2d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <deque>
#include <map>
#include <memory>

// Forward declaration(s)
class CActorData;

class CActorSprite2D : public iSprite2D, boost::noncopyable
{
public:

    // Constructor
    CActorSprite2D( const CActorData & actorData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CActorSprite2D();
    
    // Init the physics
    void InitPhysics() override;
    
    // React to what the player is doing
    void HandleEvent( const SDL_Event & rEvent ) override;

    // Update the actor
    void Update() override;
    
    // Update the physics
    void PhysicsUpdate() override;

    // Transform the actor
    void Transform() override;
    void Transform( const CObject2D & object ) override;
    void Transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Render the actor
    void Render( const CMatrix & matrix ) override;
    
    // Get the physics component
    CPhysicsComponent2D & GetPhysicsComponent() override;
    
    // Set/Get the AI pointer
    void SetAI( iAIBase2D * pAIBase ) override;

    // Is the actor in view
    bool InView();
    
    // Get the sprite
    CSprite2D & GetSprite( int index = 0 );
    
    // Get the sprite group
    CSprite2D & GetSprite( const std::string & name );
    
    // Get the unique id number
    int GetId() const override;
    
    // Get the collision group
    uint GetCollisionGroup() const;
    
    // Get the collision radius
    float GetCollisionRadius() const;
    
    // Check for collision against other actor sprite
    bool IsCollision( CActorSprite2D & rPlayerActor );
    
protected:
    
    // Apply the scale
    void ApplyScale( CMatrix & matrix ) override;
    
private:
    
    // Create the actor
    void Create( const CActorData & actorData );
    
    // Check if an object is within the orthographic view frustum
    bool InOrthographicView();
    
    // Check if an object is within the perspective view frustum
    bool InPerspectiveView();
    
    // Check for collision against other actor sprite
    bool CheckBroadPhase( CActorSprite2D & rPlayerActor );
    
private:
    
    // The physics part of the 2d sprite
    CPhysicsComponent2D m_physicsComponent;
    
    // Base AI scoped pointer
    std::unique_ptr<iAIBase2D> m_upAI;
    
    // sprite allocation vector
    std::deque<CSprite2D> m_spriteDeq;
    
    // Sprite map for getting sprite pointer by name
    std::map<std::string, CSprite2D *> m_pSpriteMap;
    
    // The radius
    float m_radius;
    
    // The scaled radius
    float m_scaledRadius;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Unique Id number
    int m_id;
    
    // Collision group
    uint m_collisionGroup;
    
    // Collision mask
    uint m_collisionMask;
    
    // Scalar to apply to the radius
    float m_collisionRadiusScalar;
    
    // Collision radius
    float m_collisionRadius;

};

#endif  // __actor_sprite_2d_h__
