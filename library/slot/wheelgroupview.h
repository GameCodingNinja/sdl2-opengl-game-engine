
/************************************************************************
*    FILE NAME:       wheelgroupview.h
*
*    DESCRIPTION:     Class for wheel group view
************************************************************************/

#ifndef __wheel_group_view_h__
#define __wheel_group_view_h__

// Physical component dependency
#include <slot/slotgroupview.h>

// Game lib dependencies
#include <slot/slotdefs.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <deque>

// Forward declaration(s)
class CSymbolSetView;
class CSlotStripModel;
class CSymbol2d;
class CWheelView;
class CMatrix;

class CWheelGroupView : public CSlotGroupView, boost::noncopyable
{
public:

    // Constructor
    CWheelGroupView( const std::shared_ptr<CSlotGroupModel> & rSlotGroupModel );

    // Destructor
    virtual ~CWheelGroupView();
    
    // Create the view slot strips
    void Create( const XMLNode & node, CSymbolSetView & rSymbolSetView ) override;
    
    // Update the reel group
    void Update() override;
    
    // Transform the reel group
    void Transform() override;
    
    // do the render
    void Render( const CMatrix & matrix ) override;
    
    // Start the reels spinning
    void StartSpin() override;
    
    // Stop the reels spinning
    void StopSpin() override;
    
    // Is the spin state
    bool IsSpinState( NSlotDefs::ESpinState state ) const override;
    
    // Generate the cycle results symbols
    void GenerateCycleResultSymbs() override;
    
    // Get the cycle results symbols
    std::vector<std::vector<CSymbol2d *>> & GetCycleResultSymbs() override;
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow ) override;
    
private:

    // Deque of wheel views
    std::deque<CWheelView> m_wheelViewDeq;
    
    // Cycle result symb vectors
    std::vector<std::vector<CSymbol2d *>> m_cycleResultSymbVec;

};

#endif  // __wheel_group_view_h__


