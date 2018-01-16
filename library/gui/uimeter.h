/************************************************************************
*    FILE NAME:       uimeter.h
*
*    DESCRIPTION:     Class for user interface meters
************************************************************************/

#ifndef __ui_meter_h__
#define __ui_meter_h__

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <common/defs.h>
#include <common/size.h>
#include <common/point.h>
#include <utilities/timer.h>

// Standard lib dependencies
#include <string>
#include <map>

// Forward declaration(s)
class CSprite2D;

class CUIMeter : public CUIControl
{
public:
    
    // Constructor
    CUIMeter( const std::string & group );

    // Destructor
    virtual ~CUIMeter();
    
    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node ) override;
    
    // Update the control
    virtual void Update();
    
    // Set the amount to the meter without banging up
    void Set( const double amount );
    
    // Start the bang range
    void StartBangUp( const double amount );
    
    // Do a fast bang
    void FastBang();
    
    // Is the meter banging
    bool IsBanging();
    
    // Clear the meter
    void Clear();
    
protected:

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & node );
    
    // Display the value in the meter
    void DisplayValue();
    
private:
    
    enum EBangType
    {
        EBT_RAMP_UP,
        EBT_LINEAR,
        EBT_HYBRID
    };
    
    enum EScaleType
    {
        EST_AXIS,
        EST_ACCURATE
    };
    
    class CBangRange
    {
    public:
        CBangRange( int target, EBangType bangType, double velocity, double estimatedTime, double slowStartTime ) :
            m_target(target),
            m_bangType(bangType),
            m_velocity(velocity),
            m_estimatedTime(estimatedTime),
            m_slowStartTime(slowStartTime)
        {}
            
        CBangRange() {}
            
    public:
        
        int m_target;
        EBangType m_bangType;
        double m_velocity;
        double m_estimatedTime;
        double m_slowStartTime;
    };
    
    // Set the bang range
    void SetBangRange();
    
    // Init the bang range
    void InitBangRange( const CBangRange & bangRange );
    
private:
        
    // Current value
    double m_currentValue;
    
    // Target value
    double m_targetValue;
    
    // Velocity value
    double m_velocity;
    
    // Terminal velocity value
    double m_terminalVelocity;
    
    // Acceleration value
    double m_acceleration;
    
    // Impulse value
    double m_impulse;
    
    // last value
    double m_lastValue;
    
    // fast bang time amount
    double m_fastBangTime;
    
    // bang up flag
    bool m_bangUp;
    
    // spin timer
    CTimer m_startUpTimer;
    
    // Current bang range
    CBangRange m_bangRange;
    
    // Sprite pointer
    // We DO NOT own this pointer. Don't free it!
    CSprite2D * m_pSprite;
    
    // Bang range value
    std::vector<CBangRange> m_bangRangeVec;
    
    // Max Font string size for this meter
    CSize<float> m_maxFontStrSize;
    
    // The amount to scale the font by to fit within the meter
    CSize<float> m_bangScaleAdjustment;
    
    // Scale on axis or accurate
    EScaleType m_scaleType;
};

#endif  // __ui_label_h__