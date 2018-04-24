
/************************************************************************
 *    FILE NAME:       physicscomponent3d.cpp
 *
 *    DESCRIPTION:     Class for handling the physics part of the sprite
 *                     Box2D works in meters
 ************************************************************************/

// Physical component dependency
#include <physics/physicscomponent3d.h>

// Game lib dependencies
#include <objectdata/objectdata3d.h>
#include <objectdata/objectphysicsdata3d.h>
#include <physics/physicsworldmanager3d.h>
#include <physics/physicsworld3d.h>
#include <3d/sprite3d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/matrix.h>

// Bullet Physics lib dependencies
#include <btBulletCollisionCommon.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
 *    desc:  Constructor
 ************************************************************************/
CPhysicsComponent3D::CPhysicsComponent3D() :
    m_pWorld( nullptr )
{
}

CPhysicsComponent3D::CPhysicsComponent3D( const CObjectPhysicsData3D & physicsData ) :
    m_pWorld(nullptr)
{
    if( physicsData.isActive() )
        m_pWorld = &CPhysicsWorldManager3D::Instance().getWorld( physicsData.getWorld() );
}

/************************************************************************
 *    desc:  destructor
 ************************************************************************/
CPhysicsComponent3D::~CPhysicsComponent3D()
{
    removeBody();
}

/************************************************************************
 *    desc:  Init the physics by creating the body and fixture
 *           NOTE: Function must be called externally at the right time
 *                 when the sprite has been setup with it's initial offsets
 ************************************************************************/
void CPhysicsComponent3D::init( const CSprite3D & sprite )
{
    if( sprite.getObjectData().getPhysicsData().isActive() )
    {
        const CObjectPhysicsData3D & rPhysicsData = sprite.getObjectData().getPhysicsData();
        std::string bodyShape = rPhysicsData.getBodyShape();
        btCollisionShape * pColShape( nullptr );

        // TODO: Sprite/object data  need to define the below
        //       Hard codeded for now
        float width( 2 );
        float height( 2 );
        float depth( 2 );
        float raduis( 1 );
        float diameter( 1 );

        const CPoint<float> scale( sprite.getScale() );
        const CPoint<float> pos( sprite.getPos() );
        const CPoint<float> rot( sprite.getRot() );

        const float mass( rPhysicsData.getMass() );

        if( bodyShape == "box" )
        {
            const btVector3 size( (width / 2.0) * scale.x, (height / 2.0) * scale.y, (depth / 2.0) * scale.z );
            pColShape = new btBoxShape( size );
        }
        else if( bodyShape == "sphere" )
        {
            pColShape = new btSphereShape( raduis );
        }
        else if( bodyShape == "cone" )
        {
            pColShape = new btConeShape( diameter, height );
        }
        else if( bodyShape == "cylinder" )
        {
            pColShape = new btCylinderShape( btVector3( diameter / 2.0, height / 2.0, diameter / 2.0 ) );
        }
        else if( bodyShape == "plane" )
        {
            CPoint<float> pn = rPhysicsData.getPlaneNormal();
            pColShape = new btStaticPlaneShape( btVector3( pn.x, pn.y, pn.z ), 0 );
        }

        btTransform trans;
        trans.setIdentity();
        trans.setOrigin( btVector3( pos.x, pos.y, pos.z ) );
        btQuaternion quat;
        quat.setEulerZYX( rot.z, rot.y, rot.x );
        trans.setRotation(quat);

        btVector3 inertia( 0, 0, 0 );

        if( mass > 0.0001f )
            pColShape->calculateLocalInertia( mass, inertia );

        btMotionState * pMotion = new btDefaultMotionState( trans );
        btRigidBody::btRigidBodyConstructionInfo info( mass, pMotion, pColShape, inertia );

        m_upRigidBody.reset( new btRigidBody( info ) );

        // Set some custom defaults
        m_upRigidBody->setRestitution( rPhysicsData.getRestitution() );
        m_upRigidBody->setRollingFriction( rPhysicsData.getRollingFriction() );
        m_upRigidBody->setFriction( rPhysicsData.getFriction() );
        m_upRigidBody->setDamping( rPhysicsData.getLinearDamping(), rPhysicsData.getAngularDamping() );

        // See if we need any collision filtering
        const short collisionGroup = rPhysicsData.getCollisionGroup();
        const short collisionMask = rPhysicsData.getCollisionMask();

        // Add the rigid body to the world
        if( (collisionGroup != 0) && (collisionMask != 0) )
            m_pWorld->addRigidBody( m_upRigidBody.get(), collisionGroup, collisionMask );
        else
            m_pWorld->addRigidBody( m_upRigidBody.get() );
    }
}

/************************************************************************
 *    desc:  Update the physics
 ************************************************************************/
void CPhysicsComponent3D::update( CSprite3D * pSprite )
{
    if( isActive() && m_upRigidBody->isActive() )
    {
        // Get the transform
        btTransform trans;
        m_upRigidBody->getMotionState()->getWorldTransform(trans);

        // transform the sprite based on the physics
        pSprite->setTransform( trans );
    }
}

/************************************************************************
 *    desc:  Is this component active?
 *           If this sprite is using physics then it must have a fixture
 ************************************************************************/
bool CPhysicsComponent3D::isActive()
{
    return (m_upRigidBody != nullptr);
}

/************************************************************************
 *    desc:  Add the body
 ************************************************************************/
void CPhysicsComponent3D::addBody()
{
    if( m_upRigidBody != nullptr )
        m_pWorld->addRigidBody( m_upRigidBody.get() );
}

/************************************************************************
 *    desc:  Remove the body
 ************************************************************************/
void CPhysicsComponent3D::removeBody()
{
    if( m_upRigidBody != nullptr )
        m_pWorld->removeRigidBody( m_upRigidBody.get() );
}

/************************************************************************
 *    desc:  Set the linear velocity
 ************************************************************************/
void CPhysicsComponent3D::setLinearVelocity( const CPoint<float> & vec )
{
    if( m_upRigidBody != nullptr )
        m_upRigidBody->setLinearVelocity( btVector3( vec.x, vec.y, vec.z ) );
}

/************************************************************************
 *    desc:  Set the angular velocity
 ************************************************************************/
void CPhysicsComponent3D::setAngularVelocity( const CPoint<float> & vec )
{
    if( m_upRigidBody != nullptr )
        m_upRigidBody->setAngularVelocity( btVector3( vec.x, vec.y, vec.z ) );
}

/************************************************************************
 *    desc:  Set the restitution
 ************************************************************************/
void CPhysicsComponent3D::setRestitution( const float rest )
{
    if( m_upRigidBody != nullptr )
        m_upRigidBody->setRestitution( rest );
}
