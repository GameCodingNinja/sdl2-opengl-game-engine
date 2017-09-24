
/************************************************************************
*    FILE NAME:       reelgroupview.h
*
*    DESCRIPTION:     View class for the reel group
************************************************************************/

#ifndef __reel_group_view_h__
#define __reel_group_view_h__

// Physical component dependency
#include <slot/slotgroupview.h>

// Game lib dependencies
#include <slot/slotdefs.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <deque>
#include <vector>

// Forward declaration(s)
class CSymbolSetView;
class CSlotStripModel;
class CSymbol2d;
class CMatrix;
class CReelStripView;

class CReelGroupView : public CSlotGroupView, boost::noncopyable
{
public:

    // Constructor
    CReelGroupView( const CSlotGroupModel & rSlotGroupModel );

    // Destructor
    virtual ~CReelGroupView();
    
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
    
    // Clear the cycle results symbols
    void ClearCycleResultSymbs() override;
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow ) override;
    
private:
    
    // Free the memory allocated for the cycle result symbols
    void DeleteCycleResultSymbs();
    
private:
    
    // Deque of reel strip views
    std::deque<CReelStripView> m_reelStripViewDeq;
    
    // Cycle result symb vectors
    std::vector<std::vector<CSymbol2d *>> m_cycleResultSymbVec;
};

#endif  // __reel_group_view_h__


