
/************************************************************************
*    FILE NAME:       reelstripview.h
*
*    DESCRIPTION:     Reel strip view class
************************************************************************/

#ifndef __reel_strip_view_h__
#define __reel_strip_view_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <common/size.h>
#include <common/point.h>
#include <slot/slotdefs.h>
#include <slot/spinprofile.h>
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <memory>
#include <string>
#include <deque>

// Forward declaration(s)
class CSlotStripModel;
class CSymbolSetView;
class CSymbol2d;
class CSprite2D;
class CSound;
class CMatrix;
struct XMLNode;

class CReelStripView : public CObject2D, boost::noncopyable
{
public:

    // Boost signal definition
    typedef boost::signals2::signal<void (int, NSlotDefs::ESpinState)> SpinStateSignal;
    
    // Constructor
    CReelStripView( const CSlotStripModel & rSlotStripModel, CSymbolSetView & rSymbolSetView, const int reelId );

    // Destructor
    virtual ~CReelStripView();
    
    // Create the reel strip from data node
    void Create( const XMLNode & node, const std::string & group );
    
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
    
    // Get the number of visible symbols on this reel
    int GetVisibleSymbolCount() const;
    
    // Get the cycle result symbol for this spot on the reel strip
    CSymbol2d * GetCycleResultSymbol( int index );
    
    // This replaces the temporary cycle symbols with the ones used for spinning
    void ClearCycleResultSymbs();
    
    // Connect to the spin state signal
    void Connect_SpinState( const SpinStateSignal::slot_type & slot );
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow );
    
private:
    
    // Get the symbol from the reel strip offset
    CSymbol2d * GetSymbol( int stop );
    
    // Init the reel strip with symbols
    void InitReelStrip();
    
    // Inc the reel spin
    void IncSpin( float speed );
    
    // Hard set the final position of the symbols
    void FinalizeSymbPos();
    
private:
    
    // Spin state signal
    SpinStateSignal m_spinStateSignal;
    
    // Reel strip model
    const CSlotStripModel & m_rSlotStripModel;
    
    // Symbol set view
    CSymbolSetView & m_rSymbolSetView;
    
    // The reel id
    const int m_reelId;
    
    // Number of visible symbols on this reel
    int m_visibleSymbCount;
    
    // Number of buffer symbols
    int m_bufferSymbols;
    
    // Spin direction
    NSlotDefs::ESpinDirection m_spinDir;
    
    // Active display symbols
    std::deque<CSymbol2d *> m_symbolDeq;
    
    // Symbol offsets
    std::deque<CPoint<float>> m_symPosDeq;
    
    // stencil mask sprite
    std::unique_ptr<CSprite2D> m_upStencilMaskSprite;
    
    // sprite deque
    std::deque<CSprite2D> m_spriteDeq;
    
    // spin the reel
    bool m_spin;
    
    // spin distance
    float m_spinDistance;
    
    // Velocity of the spin
    float m_velocity;
    
    // Acceleration
    float m_acceleration;
    
    // Spin direction vector
    float m_spinDirVector;
    
    // Spin distance of the size of a symbol
    float m_spinSymbDist;
    
    // Stop for getting then next math symbol to spin
    int m_spinStop;
    
    // Close symbol alignment
    bool m_symbAlign;
    
    // Symbol alignment counter
    int m_symbAlignCounter;
    
    // Spin state
    NSlotDefs::ESpinState m_spinState;
    
    // Spin Profile
    CSpinProfile m_spinProfile;
    
    // spin timer
    CTimer m_spinTimer;
    
    // Flag to disable spin timer
    bool m_disableSpinTimer;
    
    // Pointer to spin stop sound
    CSound * m_pSpinStopSnd;
    
    // Do we allow stop sounds
    bool m_allowStopSounds;
};

#endif  // __reel_strip_view_h__


