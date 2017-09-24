
/************************************************************************
*    FILE NAME:       wheelview.h
*
*    DESCRIPTION:     View class for the wheel implementation.
*                     NOTE: Even though this is a wheel class and wheels have wedges, we're
*                           using the Symbol2D as the wedge because it does the same thing.
************************************************************************/

#ifndef __wheel_view_h__
#define __wheel_view_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <slot/spinprofile.h>
#include <slot/slotdefs.h>
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <deque>

// Forward declaration(s)
class CSlotStripModel;
class CSymbolSetView;
class CSymbol2d;
class CSprite2D;
class CSound;
struct XMLNode;

class CWheelView : public CObject2D, boost::noncopyable
{
public:
    
    // Boost signal definition
    typedef boost::signals2::signal<void (int, NSlotDefs::ESpinState)> SpinStateSignal;

    // Constructor
    CWheelView( const CSlotStripModel & rSlotStripModel, CSymbolSetView & rSymbolSetView, const int wheelId );

    // Destructor
    virtual ~CWheelView();
    
    // Create the reel strip from data node
    void Create( const XMLNode & node, const std::string & group );
    
    // One time transform to set the non-spinning wheel sprites
    void PreTransform();
    
    // Update the reel strip
    void Update();
    
    // Transform the reel strip
    void Transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;
    
    // do the render
    void Render( const CMatrix & matrix );
    
    // Start the spin
    void StartSpin();
    
    // Stop the spin
    void StopSpin();
    
    // Set the spin profile
    void SetSpinProfile( const CSpinProfile & spinProfile );
    
    // Get the spin state
    NSlotDefs::ESpinState GetSpinState() const;
    
    // Connect to the spin state signal
    void Connect_SpinState( const SpinStateSignal::slot_type & slot );
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow );
    
    // Get the number of visible symbols (wedges) on this wheel
    size_t GetVisibleSymbolCount() const;
    
    // Get the symbol for this spot on the wheel
    CSymbol2d & GetSymbol( int index );
    
private:
    
    // Load the wheel wedges from node
    void LoadWheelSprites( const XMLNode & node, const std::string & group );
    
    // Load the wheel wedges from node
    void LoadWedges( const XMLNode & node, const std::string & group );
    
    // Load the wheel wedges from node
    void LoadSprites( const XMLNode & node, const std::string & group );
    
    // Inc the reel spin
    void IncSpin( float speed );
    
private:
    
    // Spin state signal
    SpinStateSignal m_spinStateSignal;
    
    // Reel strip model
    const CSlotStripModel & m_rSlotStripModel;
    
    // Symbol set view
    CSymbolSetView & m_rSymbolSetView;
    
    // The wheel id
    const int m_wheelId;
    
    // Spin state
    NSlotDefs::ESpinState m_spinState;
    
    // Spin direction
    NSlotDefs::ESpinDirection m_spinDir;
    
    // Active display symbols
    std::deque<CSymbol2d> m_symbolDeq;
    
    // sprite deque
    std::deque<CSprite2D> m_wheelSpriteDeq;
    
    // sprite deque
    std::deque<CSprite2D> m_spriteDeq;
    
    // Spin Profile
    CSpinProfile m_spinProfile;
    
    // spin the wheel
    bool m_spin;
    
    // Velocity of the spin
    float m_velocity;
    
    // Acceleration
    float m_acceleration;
    
    // Spin direction vector
    float m_spinDirVector;
    
    // 360 degrees in radians
    float m_PI_2;
    
    // The win point on the wheel
    float m_winPointDegree;
    
    // The degrees per math wedge
    float m_degreePerWedge;
    
    // Check to make sure we don't miss the desired wedge
    float m_saftyCheckDegree;
    
    // The rotation amount that includes correction
    float m_rotation;
    
    // The rotation amount that does not include correction
    float m_currentRotation;
    
    // spin timer
    CTimer m_spinTimer;
    
    // Flag to disable spin timer
    bool m_disableSpinTimer;
    
    // Pointer to spin stop sound
    CSound * m_pSpinStopSnd;
    
    // Do we allow stop sounds
    bool m_allowStopSounds;

};

#endif  // __wheel_view_h__


