
/************************************************************************
*    FILE NAME:       spinprofile.h
*
*    DESCRIPTION:     Spin profile data
*                     Movement is in pixels per second.
*                     Time is in milliseconds.
************************************************************************/

#ifndef __spin_profile_h__
#define __spin_profile_h__

// Physical component dependency

// Game lib dependencies

// Boost lib dependencies

// Standard lib dependencies

// Forward declaration(s)
struct XMLNode;

class CSpinProfile
{
public:

    // Constructor
    CSpinProfile();

    // Destructor
    virtual ~CSpinProfile();
    
    // Load thes reel strip data from node
    void LoadFromNode( const XMLNode & node );
    
    // Get the start delay
    int GetStartDelay() const;
    
    // Get the acceleration
    double GetAccelation() const;
    
    // Get the impulse
    double GetImpulse() const;
    
    // Get the max velocity
    double GetMaxVelocity() const;
    
    // Get the max velocity time
    int GetMaxVelocityTime() const;
    
    // Get the bounce drag
    double GetBounceDrag() const;
    
    // Get the bounce correction
    int GetBounceCorrection() const;
    
    // Get the time out delay
    double GetTimeOutDelay() const;

    // Get the deceleration rotation count
    double GetDecelerationRotationCount() const;
    
    // GGet the safety check divisor
    double GetSafetyCheckDivisor() const;
    
private:
    
    // Start delay
    int m_startDelay;
    
    // Acceleration
    double m_accelation;
    
    // Impulse
    double m_impulse;
    
    // max velocity
    double m_maxVelocity;
    
    // max velocity time
    int m_maxVelocityTime;
    
    // Bounce correction
    double m_bounceDrag;
    
    // Bounce correction
    int m_bounceCorrection;
    
    // Time out delay is used as safty valve in the 
    // event of an error to force the reel to stop
    double m_timeOutDelay;
    
    // For a wheel, number of 360 rotations before stopping
    double m_decelerationRotationCount = 1;

    // For a wheel to divide against the wedge size for max and min safety checks
    double m_safetyCheckDivisor = 0;

};

#endif  // __spin_profile_h__


