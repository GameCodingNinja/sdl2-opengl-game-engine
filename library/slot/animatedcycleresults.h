
/************************************************************************
*    FILE NAME:       animatedcycleresults.h
*
*    DESCRIPTION:     Animated cycle results implementation
************************************************************************/

#ifndef __animated_cycle_results_h__
#define __animated_cycle_results_h__

// Physical component dependency
#include <slot/icycleresults.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CAnimatedCycleResults : public iCycleResults, boost::noncopyable
{
public:

    // Constructor
    CAnimatedCycleResults( CPlayResult * pPlayResult );

    // Destructor
    virtual ~CAnimatedCycleResults();
    
    // Do some inits
    void Init( std::shared_ptr<CSlotGroupView> & spSlotGroupView ) override;
    
    // Update the cycle results
    void Update() override;
    
    // Transform
    void Transform( const CMatrix & matrix, bool tranformWorldPos ) override;
    
    // Activate the cycle results
    void Activate() override;
    
    // Deactivate the cycle results
    void Deactivate() override;
    
    // Start the cycle results animation
    void StartAnimation() override;
    
    // Stop the cycle results animation
    void StopAnimation() override;
    
    // Animate the cycle results
    bool IsAnimating() override;

    // Do the render
    void Render( const CMatrix & matrix ) override;
};

#endif  // __simple_cycle_results_h__


