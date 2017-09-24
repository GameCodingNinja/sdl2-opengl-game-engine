
/************************************************************************
*    FILE NAME:       simplecycleresults.h
*
*    DESCRIPTION:     Simple cycle results implementation
************************************************************************/

#ifndef __simplecycleresults_h__
#define __simplecycleresults_h__

// Physical component dependency
#include <slot/icycleresults.h>

// Game lib dependencies
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies

// Forward declaration(s)


class CSimpleCycleresults : public iCycleResults, boost::noncopyable
{
public:

    // Constructor
    CSimpleCycleresults();

    // Destructor
    virtual ~CSimpleCycleresults();
    
    // Do some inits
    void Init( CSlotGroupView * pSlotGroupView, CPlayResult * pPlayResult ) override;
    
    // Update the cycle results
    void Update() override;
    
    // Start the cycle results
    void Start() override;
    
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

#endif  // __simplecycleresults_h__


