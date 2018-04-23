
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
    void initPhysics() override;
    
    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent ) override;

    // Update the actor
    void update() override;
    
    // Update the physics
    //void PhysicsUpdate() override;

    // Transform the actor
    void transform() override;
    //void Transform( const CObject3D & object ) override;
    void transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Render the actor
    void render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ) override;
    
    // Get the physics component
    CPhysicsComponent3D & getPhysicsComponent() override;
    
    // Set/Get the AI pointer
    void setAI( iAIBase2D * pAIBase ) override;

    // Is the actor in view
    bool inView();
    
    // Get the sprite
    CSprite3D & getSprite( int index = 0 );
    
    // Get the sprite group
    CSprite3D & getSprite( const std::string & name );
    
    // Get the unique id number
    //int GetId() const override;
    
    // Get the collision group
    uint getCollisionGroup() const;
    
    // Get the collision radius
    float getCollisionRadius() const;
    
    // Check for collision against other actor sprite
    bool isCollision( CActorSprite3D & rPlayerActor );
    
protected:
    
    // Apply the scale
    void applyScale( CMatrix & matrix ) override;
    
private:
    
    // Create the actor
    void create( const CActorData & actorData );
    
    // Check if an object is within the orthographic view frustum
    bool inOrthographicView();
    
    // Check if an object is within the perspective view frustum
    bool inPerspectiveView();
    
    // Check for collision against other actor sprite
    bool checkBroadPhase( CActorSprite3D & rPlayerActor );
    
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
