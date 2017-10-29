
/************************************************************************
*    FILE NAME:       icycleresult.h
*
*    DESCRIPTION:     Interface for cycle results animations
************************************************************************/

#ifndef __i_cycle_results_h__
#define __i_cycle_results_h__

// Standard lib dependencies
#include <memory>

// Forward declaration(s)
class CSlotGroupView;
class CPlayResult;

class iCycleResults
{
public:
    
    // Constructor
    iCycleResults();

    // Destructor
    virtual ~iCycleResults();
    
    // Do some inits
    virtual void Init( std::shared_ptr<CSlotGroupView> & spSlotGroupView, CPlayResult * pPlayResult ) = 0;
    
    // Start the cycle results animation
    virtual void StartAnimation() = 0;
    
    // Stop the cycle results animation
    virtual void StopAnimation() = 0;
    
    // Are we still animating
    virtual bool IsAnimating() = 0;
    
    // Update the cycle results
    virtual void Update() = 0;
    
    // Start the cycle results
    virtual void Start();
    
    // Stop the cycle results
    virtual void Stop();
    
    // Did we complete one animation cycle of all the pays?
    bool IsFirstCycleComplete();
    
    // Is the cycle results active?
    bool IsCycleResultsActive();
    
protected:
    
    // Pointer to the play result. NOTE: We don't own this pointer
    CPlayResult * m_pPlayResult;
    
    // Pointer to slot group view.
    std::shared_ptr<CSlotGroupView> m_spSlotGroupView;
    
    // index into pay
    int m_cyclePayCounter;
    
    // Did we complete one animation cycle of all the pays
    bool m_firstCycleComplete;
    
    // Cycle results flag
    bool m_cycleResultsActive;

};

#endif  // __i_cycle_results_h__


