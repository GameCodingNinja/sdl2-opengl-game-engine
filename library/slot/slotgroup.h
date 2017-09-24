
/************************************************************************
*    FILE NAME:       slotgroup.h
*
*    DESCRIPTION:     Controller class for the slot group
************************************************************************/

#ifndef __slot_group_h__
#define __slot_group_h__

// Game lib dependencies
#include <slot/slotgroupmodel.h>
#include <slot/slotdefs.h>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CSlotGroupView;
class CPlayResult;
class CSymbolSetView;
class iCycleResults;

class CSlotGroup
{
public:

    // Constructor
    CSlotGroup( const CSlotMath & rSlotMath, CPlayResult & rPlayResult );

    // Destructor
    virtual ~CSlotGroup();
    
    // Create the slot group. Math and video slot strips
    void Create(
        const NSlotDefs::ESlotDevice slotDevice,
        const std::string & slotStripSetId,
        const std::string & paytableSetId,
        const XMLNode & viewSlotCfgNode,
        const XMLNode & viewSpinProfileCfgNode,
        CSymbolSetView & rSymbolSetView,
        std::unique_ptr<iCycleResults> upCycleResults );
    
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
    CSlotGroupModel & GetModel();
    
    // Get the slot group view
    CSlotGroupView * GetView();
    
private:
    
    // Slot group model
    CSlotGroupModel m_slotGroupModel;
    
    // Slot group view
    std::unique_ptr<CSlotGroupView> m_upSlotGroupView;
    
    // Cycle results interface
    std::unique_ptr<iCycleResults> m_upCycleResults;
    
    // The slot group has it's own copy of the play result reference
    CPlayResult & m_rPlayResult;

};

#endif  // __slot_group_h__


