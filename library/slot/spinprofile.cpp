
/************************************************************************
*    FILE NAME:       spinprofile.cpp
*
*    DESCRIPTION:     Spin profile data
*                     Movement is in pixels per second.
*                     Time is in milliseconds.
************************************************************************/

// Physical component dependency
#include <slot/spinprofile.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

// Boost lib dependencies

// Standard lib dependencies
#include <cstdlib>
#include <string>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CSpinProfile::CSpinProfile() :
    m_startDelay(0),
    m_accelation(.0004f),
    m_impulse(0.0),
    m_maxVelocity(2.5),
    m_maxVelocityTime(700),
    m_bounceDrag(0.0),
    m_bounceCorrection(0),
    m_timeOutDelay(0.0),
    m_decelerationRotationCount(1),
    m_safetyCheckDivisor(0)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSpinProfile::~CSpinProfile()
{
}   // destructor


/************************************************************************
*    desc:  Load thes reel strip data from node
************************************************************************/
void CSpinProfile::LoadFromNode( const XMLNode & node )
{
    // Get the symbols per reel
    if( node.isAttributeSet( "startDelay" ) )
        m_startDelay = std::atoi(node.getAttribute( "startDelay" ));
    
    // Get the acceleration
    if( node.isAttributeSet( "accelation" ) )
        m_accelation = std::atof(node.getAttribute( "accelation" )) / 1000.f;
    
    // Get the acceleration
    if( node.isAttributeSet( "impulse" ) )
        m_impulse = std::atof(node.getAttribute( "impulse" )) / 1000.f;
    
    // Get the max velocity
    if( node.isAttributeSet( "maxVelocity" ) )
        m_maxVelocity = std::atof(node.getAttribute( "maxVelocity" )) / 1000.f;
    
    // Get the max speed time
    if( node.isAttributeSet( "maxVelocityTime" ) )
        m_maxVelocityTime = std::atoi(node.getAttribute( "maxVelocityTime" ));
    
    // Get the bounce drag
    if( node.isAttributeSet( "bounceDrag" ) )
        m_bounceDrag = std::atof(node.getAttribute( "bounceDrag" )) / 1000.f;
    
    // Get the bounce distance
    if( node.isAttributeSet( "bounceCorrection" ) )
        m_bounceCorrection = std::atoi(node.getAttribute( "bounceCorrection" ));
    
    // Get the time out delay
    if( node.isAttributeSet( "timeOutDelay" ) )
        m_timeOutDelay = std::atof(node.getAttribute( "timeOutDelay" ));
    
    
    // Get the time out delay
    if( node.isAttributeSet( "decelerationRotationCount" ) )
        m_decelerationRotationCount = std::atof(node.getAttribute( "decelerationRotationCount" ));
    
    // Get the time out delay
    if( node.isAttributeSet( "safetyCheckDivisor" ) )
        m_safetyCheckDivisor = std::atof(node.getAttribute( "safetyCheckDivisor" ));

}   // LoadFromNode


/************************************************************************
*    desc:  Get the start delay
************************************************************************/
int CSpinProfile::GetStartDelay() const
{
    return m_startDelay;
    
}   // GetStartDelay


/************************************************************************
*    desc:  Get the acceleration
************************************************************************/
double CSpinProfile::GetAccelation() const
{
    return m_accelation;
    
}   // GetAccelation

/************************************************************************
*    desc:  Get the impulse
************************************************************************/
double CSpinProfile::GetImpulse() const
{
    return m_impulse;
    
}   // GetImpulse


/************************************************************************
*    desc:  Get the max velocity
************************************************************************/
double CSpinProfile::GetMaxVelocity() const
{
    return m_maxVelocity;
    
}   // GetMaxVelocity


/************************************************************************
*    desc:  Get the max speed time
************************************************************************/
int CSpinProfile::GetMaxVelocityTime() const
{
    return m_maxVelocityTime;
    
}   // GetMaxVelocityTime


/************************************************************************
*    desc:  Get the bounce drag
************************************************************************/
double CSpinProfile::GetBounceDrag() const
{
    return m_bounceDrag;
    
}   // GetBounceDrag


/************************************************************************
*    desc:  Get the bounce correction
************************************************************************/
int CSpinProfile::GetBounceCorrection() const
{
    return m_bounceCorrection;
    
}   // GetBounceCorrection


/************************************************************************
*    desc:  Get the time out delay
************************************************************************/
double CSpinProfile::GetTimeOutDelay() const
{
    return m_timeOutDelay;
    
}   // GetTimeOutDelay


/************************************************************************
*    desc:  Get the deceleration rotation count
************************************************************************/
double CSpinProfile::GetDecelerationRotationCount() const
{
    return m_decelerationRotationCount;
    
}   // GetDecelerationRotationCount


/************************************************************************
*    desc:  Get the safety check divisor
************************************************************************/
double CSpinProfile::GetSafetyCheckDivisor() const
{
    return m_safetyCheckDivisor;
    
}   // GetSafetyCheckDivisor
