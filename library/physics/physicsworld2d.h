
/************************************************************************
*    FILE NAME:       physicsworld2d.h
*
*    DESCRIPTION:     Wrapper class for Box2D's b2World
************************************************************************/

#ifndef __physics_world_2d_h__
#define __physics_world_2d_h__

// Game lib dependencies
#include <Box2D/Box2D.h>
#include <common/point.h>

// Standard lib dependencies
#include <set>

// Forward declaration(s)
struct XMLNode;

class CPhysicsWorld2D
{
public:

    // Constructor
    CPhysicsWorld2D();

    // Destructor
    virtual ~CPhysicsWorld2D();

    // Load the physics world from the passed in node
    void loadFromNode( const XMLNode & node );

    // Get the physics world
    const b2World & getWorld() const;
    b2World & getWorld();

    // Create and destroy a physics body
    b2Body * createBody( const b2BodyDef & pDef );
    void destroyBody( b2Body * pBody );

    // Perform fixed time step physics simulation
    void fixedTimeStep();
    
    // Perform variable time step physics simulation
    void variableTimeStep();

    // Get the focus
    const CPoint<int> & getFocus() const;

    // Set the fps to run the simulation at
    void setFPS( float fps );

    // The the time ratio
    float getTimeRatio() const;

    // Set-Get the activity of the physics world
    void setActive( bool value );
    bool isActive() const;
    
    // Get the pixels per meter
    double getPixelsPerMeter() const;

private:

    // Box2D world
    b2World m_world;

    // World focus point
    CPoint<int> m_focus;

    // All bodies that are handled by this physics world
    // NOTE: Class doesn't not own the data. Do Not Delete!
    std::set<b2Body *> m_pBodySet;

    // If we're actively running simulations
    bool m_active;

    // If we're going to start a step this frame
    bool m_beginStep;

    // Timer to let us know when to do another step
    float m_timer;

    // The ammount of time to simulate in milliseconds
    float m_stepTime;

	// The ammount of time to simulate in Seconds
    float m_stepTimeSec;

    // The ratio of time between steps
    float m_timeRatio;

    // The number of velocity and position steps to calculate
    int m_velStepCount;
    int m_posStepCount;
    
    // pixels per meter scaler
    float m_pixelsPerMeter;

};

#endif  // __physics_world_2d_h__