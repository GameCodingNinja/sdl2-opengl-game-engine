
/************************************************************************
*    FILE NAME:       objectphysics3d.h
*
*    DESCRIPTION:     Class containing the 3D object's physics data
************************************************************************/

#ifndef __object_physics_data_3d_h__
#define __object_physics_data_3d_h__

// Game lib dependencies
#include <common/point.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
struct XMLNode;

class CObjectPhysicsData3D
{
public:

    // Constructor
    CObjectPhysicsData3D();
    ~CObjectPhysicsData3D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );
    
    // Get the name of the physics world
    const std::string & GetWorld() const;

    // Get the body type
    const std::string & GetBodyShape() const;

    // Get the mass of physics body
    float GetMass() const;

    // Get the damping
    float GetLinearDamping() const;
    float GetAngularDamping() const;

    // Get the friction
    float GetFriction() const;
    float GetRollingFriction() const;

    // Get the restitution - the amount of bounce
    float GetRestitution() const;

    // Get the collision filter
    short GetCollisionGroup() const;

    // Get the collision mask
    short GetCollisionMask() const;

    // Get the plane normal
    const CPoint<float> & GetPlaneNormal() const;

    // Specify if physics is active
    bool IsActive() const;

private:

    // The name of the physics world
    std::string m_world;
    
    // Type of physics body
    std::string m_bodyShape;

    // Plane normal - direction the plane is facing
    CPoint<float> m_planeNormal;

    // The mass of this shape
    float m_mass;

    // The friction is how much drag this object has on another object  
    float m_friction;

    // The amount of friction when this object rolls  
    float m_rollingFiction;

    // The percetange of velocity retained upon colliding with this object
    float m_restitution;

    // The constant decceleration of movement and rotation
    float m_linearDamping, m_angularDamping;

    // Collision group and mask
    short m_collisionGroup, m_collisionMask;
};

#endif  // __object_physics_data_3d_h__
