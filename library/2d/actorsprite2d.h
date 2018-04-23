
/************************************************************************
*    FILE NAME:       actorsprite2d.h
*
*    DESCRIPTION:     2D Actor Sprite Class
************************************************************************/

#ifndef __actor_sprite_2d_h__
#define __actor_sprite_2d_h__

// Physical component dependency
#include <common/isprite.h>
#include <3d/object3d.h>

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

class CActorSprite2D : public iSprite, public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CActorSprite2D( const CActorData & actorData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CActorSprite2D();
    
    // Get the physics component
    CPhysicsComponent2D & GetPhysicsComponent();
    
    // Init the sprite
    void Init() override;
    
    // Clean up the sprite
    void CleanUp() override;
    
    // Init the physics
    void InitPhysics() override;
    
    // React to what the player is doing
    void HandleEvent( const SDL_Event & rEvent ) override;

    // Update the actor
    void Update() override;
    
    // Update the physics
    void PhysicsUpdate() override;

    // Transform the actor
    void transform() override;
    void transform( const CObject2D & object ) override;
    void transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Render the actor
    void Render( const CMatrix & matrix ) override;
    
    // Set/Get the AI pointer
    void SetAI( iAIBase * pAIBase ) override;

    // Is the actor in view
    bool InView();
    
    // Get the sprite
    CSprite2D & GetSprite( int index = 0 );
    
    // Get the sprite group
    CSprite2D & GetSprite( const std::string & name );
    
    // Get the collision group
    uint GetCollisionGroup() const;
    
    // Get the collision radius
    float GetCollisionRadius() const;
    
    // Check for collision against other actor sprite
    bool IsCollision( CActorSprite2D & rPlayerActor );
    
    
    // Set/Get the color
    void SetColor( const CColor & color ) override {}
    void SetColor( float r, float g, float b, float a ) override {}
    void SetDefaultColor() override {}
    const CColor & GetColor() const override {return m_dummy;}
    const CColor & GetDefaultColor() const override {return m_dummy;}

    // Set/Get the alpha
    void SetAlpha( float alpha ) override {}
    float GetAlpha() const override {return 0;}
    void SetDefaultAlpha() override {}
    float GetDefaultAlpha() const override {return 0;}

    // Get the frame count
    uint GetFrameCount() const override {return 0;}

    // Set the texture ID from index
    void SetFrame( uint index ) override {}
    
    // Get the current frame
    uint GetCurrentFrame() const override {return 0;}
    
    // Create the font string
    void CreateFontString( const std::string & fontString ) override {}
    
    void SetPhysicsTransform( float x, float y, float angle, bool resetVelocity = true ) override {}
    
protected:
    
    // Apply the scale
    void applyScale( CMatrix & matrix ) override;
    
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
    std::unique_ptr<iAIBase> m_upAI;
    
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
    
    // Collision group
    uint m_collisionGroup;
    
    // Collision mask
    uint m_collisionMask;
    
    // Scalar to apply to the radius
    float m_collisionRadiusScalar;
    
    // Collision radius
    float m_collisionRadius;
    
    CColor m_dummy;

};

#endif  // __actor_sprite_2d_h__
