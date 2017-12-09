
/************************************************************************
*    FILE NAME:       simplecycleresults.h
*
*    DESCRIPTION:     Simple cycle results implementation
************************************************************************/

#ifndef __simple_cycle_results_h__
#define __simple_cycle_results_h__

// Physical component dependency
#include <slot/icycleresults.h>

// Game lib dependencies
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

class CSimpleCycleresults : public iCycleResults, boost::noncopyable
{
public:

    // Constructor
    CSimpleCycleresults( CPlayResult * pPlayResult );

    // Destructor
    virtual ~CSimpleCycleresults();
    
    // Do some inits
    void Init( std::shared_ptr<CSlotGroupView> & spSlotGroupView ) override;
    
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
    
private:

    // cycle results timer
    CTimer m_cycleResultsTimer;
};

#endif  // __simple_cycle_results_h__


