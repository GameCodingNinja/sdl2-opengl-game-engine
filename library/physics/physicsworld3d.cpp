
/************************************************************************
 *    FILE NAME:       physicsworld3d.cpp
 *
 *    DESCRIPTION:     Wrapper class for Bullet physics
 ************************************************************************/

// Physical component dependency
#include <physics/physicsworld3d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/mathfunc.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
 *    desc:  Constructor
 ************************************************************************/
CPhysicsWorld3D::CPhysicsWorld3D() :
m_colDisp( &m_defColConf ),
m_world( &m_colDisp, &m_broadphase, &m_conSolv, &m_defColConf ),
m_active(false),
m_timer(0),
m_stepTime(0),
m_stepTimeSec(0),
m_timeRatio(0)
{
    // Init with default values
    m_world.setGravity( btVector3(0.f, -10.f, 0.f) );
    SetFPS(30);

}   // constructor

/************************************************************************
 *    desc:  destructor
 ************************************************************************/
CPhysicsWorld3D::~CPhysicsWorld3D()
{
}   // destructor

/************************************************************************
 *    desc:  Load the physics world from the passed in XML node
 ************************************************************************/
void CPhysicsWorld3D::LoadFromNode( const XMLNode & node )
{
    // Get the world's settings, if any are set
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "active" ) )
        {
            if( std::strcmp(settingsNode.getAttribute( "active" ), "true") == 0 )
                m_active = true;
        }
    }

    // Get the world's gravity, if any are set
    XMLNode gravityNode = node.getChildNode( "gravity" );
    if( !gravityNode.isEmpty() )
    {
        float x = std::atof( gravityNode.getAttribute( "x" ) );
        float y = std::atof( gravityNode.getAttribute( "y" ) );
        float z = std::atof( gravityNode.getAttribute( "z" ) );

        m_world.setGravity( btVector3(x, y, z) );
    }

    // Get the stepping which determins how accurate the physics are
    XMLNode steppingNode = node.getChildNode( "stepping" );
    if( !steppingNode.isEmpty() )
        SetFPS( std::atof( steppingNode.getAttribute( "fps" ) ) );

}   // LoadFromXML

/************************************************************************
 *    desc:  Get the world
 ************************************************************************/
const btDiscreteDynamicsWorld & CPhysicsWorld3D::GetWorld() const
{
    return m_world;

}	// GetWorld

/************************************************************************
 *    desc:  Add a rigid body to the world
 ************************************************************************/
void CPhysicsWorld3D::AddRigidBody( btRigidBody * pBody )
{
    m_world.addRigidBody( pBody );

}   // AddRigidBody

/************************************************************************
 *    desc:  Add collision filtering when adding a rigid body
 ************************************************************************/
void CPhysicsWorld3D::AddRigidBody( btRigidBody * pBody, short filterGroup, short mask )
{
    m_world.addRigidBody( pBody, filterGroup, mask );

}   // AddRigidBody

/************************************************************************
 *    desc:  Remove a rigid body to the world
 ************************************************************************/
void CPhysicsWorld3D::RemoveRigidBody( btRigidBody * pBody )
{
    m_world.removeRigidBody( pBody );

}   // AddRigidBody

/************************************************************************
 *    desc:  Perform fixed time step physics simulation
 ************************************************************************/
void CPhysicsWorld3D::FixedTimeStep()
{
    if( m_active )
    {
        // Increment the timer
        m_timer += CHighResTimer::Instance().GetElapsedTime();

        if( m_timer > m_stepTime )
        {
            m_timer = NMathFunc::Modulus( m_timer, m_stepTime );

            // Begin the physics world step - same as m_stepTime / 1000.0f
            m_world.stepSimulation( m_stepTimeSec, 1, m_stepTimeSec );
        }

        m_timeRatio = m_timer / m_stepTime;
    }

}   // FixedTimeStep

/************************************************************************
 *    desc:  Perform variable time step physics simulation
 ************************************************************************/
void CPhysicsWorld3D::VariableTimeStep()
{
    if( m_active )
    {
        auto elapsedTime = CHighResTimer::Instance().GetElapsedTime() / 1000.f;
        m_world.stepSimulation( elapsedTime, 1, elapsedTime );
    }
    
}   // VariableTimeStep

/************************************************************************
 *    desc:  Set the fps to run the simulation at
 ************************************************************************/
void CPhysicsWorld3D::SetFPS( float fps )
{
    // Make sure we don't have a negative or zero fps
    if( fps > 0.f )
    {
        // Calculate the step paramaters
        m_stepTimeSec = 1.f / fps;
        m_stepTime = m_stepTimeSec * 1000.f;

        // Set the timer so that we'll begin a step next time we call Update
        m_timer = m_stepTime;
    }

}   // SetFPS

/************************************************************************
 *    desc:  The the time ratio
 ************************************************************************/
float CPhysicsWorld3D::GetTimeRatio() const
{
    return m_timeRatio;

}   // GetTimeRatio

/************************************************************************
 *    desc:  Set the activity of the physics world
 ************************************************************************/
void CPhysicsWorld3D::SetActive( bool value )
{
    m_active = value;

}   // SetActive

/************************************************************************
 *    desc:  Is the physics world active
 ************************************************************************/
bool CPhysicsWorld3D::IsActive() const
{
    return m_active;

}   // IsActive

