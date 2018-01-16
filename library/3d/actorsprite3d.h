
/************************************************************************
*    FILE NAME:       actorsprite3d.h
*
*    DESCRIPTION:     3D Actor Sprite Class
************************************************************************/

#ifndef __actor_sprite_3d_h__
#define __actor_sprite_3d_h__

// Physical component dependency
#include <3d/isprite3d.h>

// Game lib dependencies
#include <common/defs.h>
#include <3d/sprite3d.h>
#include <physics/physicscomponent3d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <deque>
#include <map>
#include <memory>

// Forward declaration(s)
class CActorData;

class CActorSprite3D : public iSprite3D, boost::noncopyable
{
public:

    // Constructor
    CActorSprite3D( const CActorData & actorData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CActorSprite3D();
    
    // Init the physics
    void InitPhysics() override;
    
    // React to what the player is doing
    void HandleEvent( const SDL_Event & rEvent ) override;

    // Update the actor
    void Update() override;
    
    // Update the physics
    //void PhysicsUpdate() override;

    // Transform the actor
    void Transform() override;
    //void Transform( const CObject3D & object ) override;
    void Transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Render the actor
    void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ) override;
    
    // Get the physics component
    CPhysicsComponent3D & GetPhysicsComponent() override;
    
    // Set/Get the AI pointer
    void SetAI( iAIBase2D * pAIBase ) override;

    // Is the actor in view
    bool InView();
    
    // Get the sprite
    CSprite3D & GetSprite( int index = 0 );
    
    // Get the sprite group
    CSprite3D & GetSprite( const std::string & name );
    
    // Get the unique id number
    //int GetId() const override;
    
    // Get the collision group
    uint GetCollisionGroup() const;
    
    // Get the collision radius
    float GetCollisionRadius() const;
    
    // Check for collision against other actor sprite
    bool IsCollision( CActorSprite3D & rPlayerActor );
    
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
    bool CheckBroadPhase( CActorSprite3D & rPlayerActor );
    
private:
    
    // The physics part of the 3d sprite
    CPhysicsComponent3D m_physicsComponent;
    
    // Base AI scoped pointer
    std::unique_ptr<iAIBase2D> m_upAI;
    
    // sprite allocation vector
    std::deque<CSprite3D> m_spriteDeq;
    
    // Sprite map for getting sprite pointer by name
    std::map<std::string, CSprite3D *> m_pSpriteMap;
    
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

#endif  // __actor_sprite_3d_h__
