
/************************************************************************
*    FILE NAME:       physicscomponent2d.cpp
*
*    DESCRIPTION:     Class for handling the physics part of the sprite
*                     Box2D works in meters
************************************************************************/

// Physical component dependency
#include <physics/physicscomponent2d.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/objectphysicsdata2d.h>
#include <physics/physicsworldmanager2d.h>
#include <physics/physicsworld2d.h>
#include <2d/sprite2d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Box2D lib dependencies
#include <Box2D/Box2D.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPhysicsComponent2D::CPhysicsComponent2D( const CObjectPhysicsData2D & physicsData ) :
    BODY_TYPE( physicsData.GetBodyType() ),
    m_pBody(nullptr),
    PIXELS_TO_METERS(0),
    METERS_TO_PIXELS(0),
    m_pWorld(nullptr)
{
    if( physicsData.IsActive() )
    {
        m_pWorld = &CPhysicsWorldManager2D::Instance().GetWorld( physicsData.GetWorld() );

        // Re-init the constants to the values needed
        const_cast<float&>(METERS_TO_PIXELS) = m_pWorld->GetPixelsPerMeter();
        const_cast<float&>(PIXELS_TO_METERS) = 1 / METERS_TO_PIXELS;
    }
}   // constructor

CPhysicsComponent2D::CPhysicsComponent2D() :
    BODY_TYPE( b2BodyType(-1) ),
    m_pBody(nullptr),
    PIXELS_TO_METERS(0),
    METERS_TO_PIXELS(0),
    m_pWorld(nullptr)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CPhysicsComponent2D::~CPhysicsComponent2D()
{
}   // destructor


/************************************************************************
*    desc:  Init the physics by creating the body and fixture
*           NOTE: Function must be called externally at the right time
*                 when the sprite has been setup with it's initial offsets
************************************************************************/
void CPhysicsComponent2D::Init( const CSprite2D & sprite )
{
    if( sprite.GetObjectData().GetPhysicsData().IsActive() )
    {
        CreateBody( sprite );
        CreateFixture( sprite );
    }

}   // Init


/************************************************************************
*    desc:  Create the body
************************************************************************/
void CPhysicsComponent2D::CreateBody( const CSprite2D & sprite )
{
    const CObjectPhysicsData2D & physicsData = sprite.GetObjectData().GetPhysicsData();

    if( physicsData.GetBodyType() != b2BodyType(-1) )
    {
        // Fill in the body structure
        b2BodyDef bodyDef;
        bodyDef.type = physicsData.GetBodyType();
        bodyDef.linearDamping = physicsData.GetLinearDamping();
        bodyDef.angularDamping = physicsData.GetAngularDamping();
        bodyDef.fixedRotation = physicsData.IsRotationFixed();
        bodyDef.position.Set( sprite.GetPos().GetX() * PIXELS_TO_METERS, -(sprite.GetPos().GetY() * PIXELS_TO_METERS) );
        bodyDef.angle = -sprite.GetRot().GetZ();
        bodyDef.userData = (void*)&sprite;

        // Create the body
        m_pBody = m_pWorld->CreateBody( bodyDef );
    }

}   // CreateBody


/************************************************************************
*    desc:  Create the fixture
************************************************************************/
void CPhysicsComponent2D::CreateFixture( const CSprite2D & sprite )
{
    const auto & fixture = sprite.GetObjectData().GetPhysicsData().GetFixtureVec();

    for( auto & iter : fixture )
    {
        // Create the fixture
        if( iter.m_shape == b2Shape::e_circle )
            CreateCircularShapeFixture( sprite, iter );

        else if( iter.m_shape == b2Shape::e_edge )
            CreateEdgeShapeFixture( sprite, iter );

        else if( iter.m_shape == b2Shape::e_polygon )
            CreatePolygonShapeFixture( sprite, iter );

        else if( iter.m_shape == b2Shape::e_chain )
            CreateChainShapeFixture( sprite, iter );
    }

}   // CreateFixture


/************************************************************************
*    desc:  Create the circular shape fixture
************************************************************************/
void CPhysicsComponent2D::CreateCircularShapeFixture( const CSprite2D & sprite, const CFixture & fixture )
{
    b2CircleShape shape;
    shape.m_radius = (fixture.m_radius * sprite.GetScale().GetX()) * PIXELS_TO_METERS;

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    m_pBody->CreateFixture( &f );

}   // CreateCircularShapeFixture


/************************************************************************
*    desc:  Create the edge shape fixture
*           NOTE: An edge is a line segment of two points
*                 This is no different then making a polygon from points
************************************************************************/
void CPhysicsComponent2D::CreateEdgeShapeFixture( const CSprite2D & sprite, const CFixture & fixture )
{
    // Do a sanity check because we need two points to define an edge
    if( fixture.m_vertVec.size() != 2 )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
            boost::str( boost::format("Physics object has incorrect number of points defined (%d/%s).\n\n%s\nLine: %s")
                % fixture.m_vertVec.size() % sprite.GetObjectData().GetName() % __FUNCTION__ % __LINE__ ));
                
    // Apply scale to the size and divide by 2
    // Object data holds size as int so need to convert it to a float
    const CSize<float> objectSize = sprite.GetObjectData().GetSize();
    const CSize<float> scale( sprite.GetScale().GetX(), sprite.GetScale().GetY() );
    const CSize<float> size = objectSize * scale * 0.5f;

    // Convert the points to world location in meters
    std::vector<b2Vec2> pointVec;
    ConvertPoints( pointVec, fixture, size, scale );

    // These points must exist if this is to work. Use "at()" for bounds checking)
    b2EdgeShape shape;
    shape.Set( pointVec.at(0), pointVec.at(1) );

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    m_pBody->CreateFixture( &f );

}   // CreateEdgeShapeFixture


/************************************************************************
*    desc:  Create the polygon shape fixture
************************************************************************/
void CPhysicsComponent2D::CreatePolygonShapeFixture( const CSprite2D & sprite, const CFixture & fixture )
{
    std::vector<b2Vec2> pointVec;

    // Apply scale to the size and divide by 2
    // Object data holds size as int so need to convert it to a float
    const CSize<float> objectSize = sprite.GetObjectData().GetSize();
    const CSize<float> scale( sprite.GetScale().GetX(), sprite.GetScale().GetY() );
    const CSize<float> size = objectSize * scale * 0.5f;

    // Is this polygon shape defined by a vector of points?
    if( !fixture.m_vertVec.empty() )
    {
        // Convert the points to world location in meters
        ConvertPoints( pointVec, fixture, size, scale );
    }
	// If vector points are not supplied, build a square based on the object size
    else
    {
        pointVec.reserve( 4 );

        // Bottom and left mod have their signs flipped so that a positive mod always means
        // expansion of the side, and a negative mod always means a contraction of the side
        const float topMod = fixture.m_topMod * scale.h;
        const float bottomMod = -fixture.m_bottomMod * scale.h;
        const float leftMod = -fixture.m_leftMod * scale.w;
        const float rightMod = fixture.m_rightMod * scale.w;

        // Convert to meters
        // Box2D polygons are defined using Counter Clockwise Winding (CCW)
        pointVec.emplace_back(
            (-size.w + leftMod) * PIXELS_TO_METERS,
            (size.h + topMod) * PIXELS_TO_METERS );

        pointVec.emplace_back(
            (-size.w + leftMod) * PIXELS_TO_METERS,
            (-size.h + bottomMod) * PIXELS_TO_METERS );

        pointVec.emplace_back(
            (size.w + rightMod) * PIXELS_TO_METERS,
            (-size.h + bottomMod) * PIXELS_TO_METERS );

        pointVec.emplace_back(
            (size.w + rightMod) * PIXELS_TO_METERS,
            (size.h + topMod) * PIXELS_TO_METERS );
    }

    b2PolygonShape shape;
    shape.Set( pointVec.data(), pointVec.size() );

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    m_pBody->CreateFixture( &f );

}   // CreatePolygonShapeFixture


/************************************************************************
*    desc:  Create the chain shape fixture
************************************************************************/
void CPhysicsComponent2D::CreateChainShapeFixture( const CSprite2D & sprite, const CFixture & fixture )
{
    // Do a sanity check because we need more then 1 point to define a chain
    if( fixture.m_vertVec.size() > 1 )
        throw NExcept::CCriticalException("Physics Edge Fixture error!",
            boost::str( boost::format("Physics object has incorrect number of points defined (%d/%s).\n\n%s\nLine: %s")
                % fixture.m_vertVec.size() % sprite.GetObjectData().GetName() % __FUNCTION__ % __LINE__ ));
                
    // Apply scale to the size and divide by 2
    // Object data holds size as int so need to convert it to a float
    const CSize<float> objectSize = sprite.GetObjectData().GetSize();
    const CSize<float> scale( sprite.GetScale().GetX(), sprite.GetScale().GetY() );
    const CSize<float> size = objectSize * scale * 0.5f;

    // Convert the points to world location in meters
    std::vector<b2Vec2> pointVec;
    ConvertPoints( pointVec, fixture, size, scale );

    b2ChainShape shape;
    if( fixture.m_chainLoop )
        shape.CreateLoop( pointVec.data(), pointVec.size() );
    else
        shape.CreateChain( pointVec.data(), pointVec.size() );

    b2FixtureDef f;
    f.shape = &shape;
    f.friction = fixture.m_friction;
    f.density = fixture.m_density;
    f.restitution = fixture.m_restitution;
    f.filter = fixture.m_filter;
    f.isSensor = fixture.m_sensor;
    f.userData = (void*)&sprite;

    m_pBody->CreateFixture( &f );

}   // CreateChainShapeFixture


/************************************************************************
*    desc:  Convert the points to world location in meters
************************************************************************/
void CPhysicsComponent2D::ConvertPoints(
    std::vector<b2Vec2> & polyPointVec,
    const CFixture & fixture,
    const CSize<float> & size,
    const CSize<float> & scale )
{
    polyPointVec.reserve( fixture.m_vertVec.size() );

    // Convert to meters and world coordinates
    // Box2D polygons are defined using Counter Clockwise Winding (CCW)
    for( auto & iter : fixture.m_vertVec )
    {
        polyPointVec.emplace_back(
            ((iter.x * scale.w) - size.w) * PIXELS_TO_METERS,
            ((iter.y * scale.h) - size.h) * PIXELS_TO_METERS );
    }
}   // ConvertPoints


/************************************************************************
*    desc:  Update the physics
************************************************************************/
void CPhysicsComponent2D::Update( CSprite2D * pSprite )
{
    if( m_pBody != nullptr )
    {
        CStatCounter::Instance().IncPhysicsObjectsCounter();
        
        if( (BODY_TYPE > b2_staticBody) && m_pBody->IsAwake() )
        {
            const b2Vec2 & pos = m_pBody->GetPosition();
            const float angle = m_pBody->GetAngle();
            pSprite->SetPosXYZ( pos.x * METERS_TO_PIXELS, -(pos.y * METERS_TO_PIXELS) );
            pSprite->SetRotXYZ( 0, 0, -angle, false );
        }
    }

}   // Update


/************************************************************************
*    desc:  Is this component active?
*           If this sprite is using physics then it must have a fixture
************************************************************************/
bool CPhysicsComponent2D::IsActive()
{
    return (m_pBody != nullptr);

}   // IsActive


/************************************************************************
*    desc:  Destry the body
************************************************************************/
void CPhysicsComponent2D::DestroyBody()
{
    if( m_pBody != nullptr )
    {
        m_pWorld->DestroyBody( m_pBody );
        m_pBody = nullptr;
    }

}   // DestroyBody


/************************************************************************
*    desc:  Set the physics position and rotation
************************************************************************/
void CPhysicsComponent2D::SetTransform( const float x, const float y, const float angle, const bool resetVelocity )
{
    if( m_pBody != nullptr )
    {
        m_pBody->SetTransform( b2Vec2( x * PIXELS_TO_METERS, -(y * PIXELS_TO_METERS) ), angle );
        
        if( resetVelocity )
        {
            m_pBody->SetLinearVelocity( b2Vec2(0,0) );
            m_pBody->SetAngularVelocity( 0 );
        }
    }

}   // SetTransform


/************************************************************************
*    desc:  Set the linear velocity
************************************************************************/
void CPhysicsComponent2D::SetLinearVelocity( const float x, const float y )
{
    if( m_pBody != nullptr )
        m_pBody->SetLinearVelocity( b2Vec2( x * PIXELS_TO_METERS, -(y * PIXELS_TO_METERS) ) );

}   // SetLinearVelocity


/************************************************************************
*    desc:  Get the body
************************************************************************/
b2Body * CPhysicsComponent2D::GetBody()
{
    // Do a sanity check because we need more then 1 point to define a chain
    if( m_pBody == nullptr )
        throw NExcept::CCriticalException("Acess To Null Body!",
            boost::str( boost::format("Physics body in question is NULL.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    return m_pBody;

}   // GetBody
