
/************************************************************************
*    FILE NAME:       smartconfirmbtn.cpp
*
*    DESCRIPTION:     Class CSmartExitBtn
************************************************************************/

// Physical component dependency
#include "smartconfirmbtn.h"

// Game dependencies
#include "smartresetkeybind.h"
//#include "smartnewgamesavebtn.h"

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
}   // constructor


/***************************************************************************
*    decs:  Called when the control is executed
****************************************************************************/
void CSmartConfirmBtn::Execute()
{
    CMenu & rMenu = CMenuManager::Instance().GetMenu("confirmation_menu");
    CUIControl * pYesBtn = rMenu.GetPtrToControl("yes_btn");
    CUIControl * pMegLbl = rMenu.GetPtrToControl("message_lbl");

    CSmartGuiControl * pSmartGuiCtrl(NULL);
    std::string conformationMsg;
    std::string executionAction;
    NUIControl::EControlActionType actionType(NUIControl::ECAT_BACK);

    if( m_pUIControl->GetName() == "main_menu_btn" )
    {
        conformationMsg = "Are you sure you|want to go back to|the main menu?";
        actionType = NUIControl::ECAT_GAME_STATE_CHANGE;
        executionAction = "title_screen_state";
    }

    // Set the conformation menu
    pYesBtn->SetSmartGui( pSmartGuiCtrl );
    pYesBtn->SetActionType( actionType );
    pYesBtn->SetExecutionAction( executionAction );
    pMegLbl->CreateFontString( conformationMsg );

}   // Execute







