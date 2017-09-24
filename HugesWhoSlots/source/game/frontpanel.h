
/************************************************************************
*    FILE NAME:       frontpanel.h
*
*    DESCRIPTION:     Game specific class container to manage buttons and meters.
*                     Does NOT own the pointers. Do Not Free
************************************************************************/

#ifndef __front_panel_h__
#define __front_panel_h__

// Physical component dependency
#include <slot/ifrontpanel.h>

// Game lib dependencies
#include <common/defs.h>

// Standard lib dependencies
#include <vector>

// Forward declaration(s)
class CUIButton;
class CUIMeter;
class CUIControl;

class CFrontPanel : public iFrontPanel
{
public:
    
    // Constructor
    CFrontPanel();
    
    // Set the buttons
    void SetButtons( CUIButton * pPlayBtn, std::vector<CUIControl *> & pOtherBtnVec );
    
    // Set the meters
    void SetMeters( CUIMeter * pWinMeter, CUIMeter * pCreditMeter );
    
    // Init a new game
    void InitGame( uint credits ) override;
    
    // Start the bang up
    void StartBangUp( uint credits, uint win ) override;
    
    // Are the meters banging
    bool IsBanging() override;
    
    // Start the fast bang
    void FastBang() override;
    
    // Enable the buttons
    void EnableButtons( bool allowPlay ) override;
    
private:
    
    // Play button
    CUIButton * m_pPlayBtn;
    
    // Menu button
    std::vector<CUIControl *> m_pOtherBtnVec;
    
    // Win meter
    CUIMeter * m_pWinMeter;
    
    // Credit meter
    CUIMeter * m_pCreditMeter;

};

#endif  // __front_panel_h__


