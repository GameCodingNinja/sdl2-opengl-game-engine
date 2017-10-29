
/************************************************************************
*    FILE NAME:       slotgroup.h
*
*    DESCRIPTION:     Controller class for the slot group
************************************************************************/

#ifndef __slot_group_h__
#define __slot_group_h__

// Game lib dependencies
#include <slot/slotdefs.h>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CSlotGroupModel;
class CSlotGroupView;
class CSymbolSetView;
class iCycleResults;

class CSlotGroup
{
public:

    // Constructor
    CSlotGroup(
        std::shared_ptr<CSlotGroupModel> spSlotGroupModel,
        std::shared_ptr<CSlotGroupView> spSlotGroupView,
        std::unique_ptr<iCycleResults> upCycleResults );

    // Destructor
    virtual ~CSlotGroup();
    
    // Start the cycle results
    void StartCycleResults();
    
    // Stop the cycle results
    void StopCycleResults();
    
    // Start the cycle results animation
    void StartCycleResultsAnimation();
    
    // Stop the cycle results animation
    void StopCycleResultsAnimation();
    
    // Is the cycle results animating
    bool IsCycleResultsAnimating();
    
    // Update objects that require them
    void Update();
    
    // Get the slot group model
    CSlotGroupModel * GetModel();
    
    // Get the slot group view
    CSlotGroupView * GetView();
    
private:
    
    // Slot group model
    std::shared_ptr<CSlotGroupModel> m_spSlotGroupModel;
    
    // Slot group view
    std::shared_ptr<CSlotGroupView> m_spSlotGroupView;
    
    // Cycle results interface
    std::unique_ptr<iCycleResults> m_upCycleResults;

};

#endif  // __slot_group_h__


