
/************************************************************************
*    FILE NAME:       playresult.h
*
*    DESCRIPTION:     Class for holding pays
************************************************************************/

#ifndef __play_result_h__
#define __play_result_h__

// Game lib dependencies
#include <slot/pay.h>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class CPayCombo;

class CPlayResult
{
public:

    // Constructor
    CPlayResult();
    
    // Add a slot pay
    void AddPay(
        const NSlotDefs::EPayType payType,
        const CPayCombo & rCombo,
        const int multiplier,
        const int winLine,
        const std::vector<CSymbPos> & symbPos );
    
    // Sort the pays
    void SortPays();
    
    // Add up the win
    uint AddUpWin();
    
    // Get the pay
    const CPay & GetPay( int index ) const;
    
    // Get the number of pays
    uint GetPayCount() const;
    
    // Clear the pays
    void Clear();
    
private:
    
    // Vector of pays
    std::vector<CPay> m_payVec;

    // Total win amount
    uint m_totalWinAmount;
};

#endif  // __play_result_h__


