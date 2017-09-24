
/************************************************************************
*    FILE NAME:       slotresults.h
*
*    DESCRIPTION:     Class of slot result's
************************************************************************/

#ifndef __slot_results_h__
#define __slot_results_h__

// Game lib dependencies
#include <slot/playresult.h>

// Standard lib dependencies
#include <vector>

class CSlotResults
{
public:

    // Constructor
    CSlotResults();
    
    // Create a new play results entry into the vector
    CPlayResult & Create();
    
    // Clear the pays
    void Clear();
    
    // Sort the pays
    void SortPays();
    
    // Add up the win
    void AddUpWin();
    
    // Get the total win
    uint GetTotalWin();
    
    // Do we have a win
    bool IsWin();
    
private:
    
    // Play result vector
    std::vector<CPlayResult> m_playResultVec;
    
    // Total win amount
    uint m_totalWinAmount;

};

#endif  // __slot_results_h__


