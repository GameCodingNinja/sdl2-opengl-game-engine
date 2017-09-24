
/************************************************************************
 *    FILE NAME:       physicscomponent3d.h
 *
 *    DESCRIPTION:     Class for handling the physics part of the sprite
 ************************************************************************/

#ifndef __physics_component_3d_h__
#define __physics_component_3d_h__

// Game lib dependencies
#include <common/point.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CObjectPhysicsData3D;
class CSprite3D;
class CPhysicsWorld3D;
class btRigidBody;
class asIScriptEngine;

class CPhysicsComponent3D : boost::noncopyable
{
public:

    // Constructor
    CPhysicsComponent3D( const CObjectPhysicsData3D & physicsData );

    // Destructor
    ~CPhysicsComponent3D();

    //////////////////////////////////////////////////////
    // Register CScriptComponent2d Object with AngelScript
    //////////////////////////////////////////////////////
    //static void Register( asIScriptEngine * pEngine );

    // Init the physics by creating the body and fixture
    // NOTE: Function must be called externally at the right time
    //       when the sprite has been setup with it's initial offsets
    void Init(const CSprite3D & sprite);

    // Remove the body
    void RemoveBody();

    // Add the body
    void AddBody();

    // Set the linear velocity
    void SetLinearVelocity(const CPoint<float> & vec);

    // Set the angular velocity
    void SetAngularVelocity(const CPoint<float> & vec);

    // Set the restitution
    void SetRestitution(const float rest);

    // Update the physics
    void Update(CSprite3D * pSprite);

    // Is this component active?
    bool IsActive();

private:

    // Pointer to the world
    // NOTE: Do not free. We don't own this pointer.
    CPhysicsWorld3D * m_pWorld;

    // Rigid body safe pointer
    std::unique_ptr<btRigidBody> m_upRigidBody;
};

#endif  // __physics_component_3d_h__

