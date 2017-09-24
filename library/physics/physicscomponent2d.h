
/************************************************************************
*    FILE NAME:       physicscomponent2d.h
*
*    DESCRIPTION:     Class for handling the physics part of the sprite
************************************************************************/

#ifndef __physics_component_2d_h__
#define __physics_component_2d_h__

// Game lib dependencies
#include <common/size.h>

// Box2D lib dependencies
#include <Box2D/Dynamics/b2Body.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class CObjectPhysicsData2D;
class CSprite2D;
class CPhysicsWorld2D;
class b2Body;
class CFixture;

class CPhysicsComponent2D : boost::noncopyable
{
public:

    // Constructor
    CPhysicsComponent2D();
    CPhysicsComponent2D( const CObjectPhysicsData2D & physicsData );

    // Destructor
    ~CPhysicsComponent2D();

    // Init the physics by creating the body and fixture
    // NOTE: Function must be called externally at the right time
    //       when the sprite has been setup with it's initial offsets
    void Init( const CSprite2D & sprite );

    // Destry the body - will only work if this is the parent sprite
    void DestroyBody();

    // Update the physics
    void Update( CSprite2D * pSprite );

    // Is this component active?
    bool IsActive();
    
    // Set the physics position and rotation
    void SetTransform( const float x, const float y, const float angle = 0, const bool resetVelocity = true );
    
    // Set the linear velocity
    void SetLinearVelocity( const float x, const float y );
    
    // Get the body
    b2Body * GetBody();

private:

    // Create the body
    void CreateBody( const CSprite2D & sprite );

    // Create the fixture
    void CreateFixture( const CSprite2D & sprite );

    // Create the circular shape fixture
    void CreateCircularShapeFixture( const CSprite2D & sprite, const CFixture & fixture );

    // Create the edge shape fixture
    void CreateEdgeShapeFixture( const CSprite2D & sprite, const CFixture & fixture );

    // Create the polygon shape fixture
    void CreatePolygonShapeFixture( const CSprite2D & sprite, const CFixture & fixture );

    // Create the chain shape fixture
    void CreateChainShapeFixture( const CSprite2D & sprite, const CFixture & fixture );
    
    // Convert the points to world location in meters
    void ConvertPoints(
        std::vector<b2Vec2> & polyPointVec,
        const CFixture & fixture,
        const CSize<float> & size,
        const CSize<float> & scale );

private:
    
    // Body type
    const b2BodyType BODY_TYPE;
    
    // The physics body the sprite belongs to
    // NOTE: If we own this pointer, we'll need to free it
    b2Body * m_pBody = nullptr;

    // Pixels to meters conversion
    const float PIXELS_TO_METERS;
    const float METERS_TO_PIXELS;

    // Pointer to the world
    // NOTE: Do not free. We don't own this pointer.
    CPhysicsWorld2D * m_pWorld;

};

#endif  // __physics_component_2d_h__
