
/************************************************************************
*    FILE NAME:       smartconfirmbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartconfirmbtn.h"

// Game lib dependencies
#include <gui/uicontrol.h>
#include <gui/menumanager.h>
#include <gui/menu.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSmartConfirmBtn::CSmartConfirmBtn( CUIControl * pUIControl ) :
    CSmartGuiControl( pUIControl )
{
}


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartConfirmBtn::execute()
{
    CMenu & rMenu = CMenuManager::Instance().getMenu("confirmation_menu");
    CUIControl * pYesBtn = rMenu.getPtrToControl("yes_btn");
    CUIControl * pMegLbl = rMenu.getPtrToControl("message_lbl");

    CSmartGuiControl * pSmartGuiCtrl(NULL);
    std::string conformationMsg;
    std::string executionAction;
    NUIControl::EControlActionType actionType(NUIControl::ECAT_BACK);

    if( m_pUIControl->getName() == "main_menu_btn" )
    {
        conformationMsg = "Are you sure you|want to go back to|the main menu?";
        actionType = NUIControl::ECAT_GAME_STATE_CHANGE;
        executionAction = "title_screen_state";
    }

    // Set the conformation menu
    pYesBtn->setSmartGui( pSmartGuiCtrl );
    pYesBtn->setActionType( actionType );
    pYesBtn->setExecutionAction( executionAction );
    pMegLbl->createFontString( conformationMsg );
}
