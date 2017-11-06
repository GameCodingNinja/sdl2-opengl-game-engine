
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
*    desc:  Constructer
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

    if( m_pUIControl->GetName() == "exit_btn" )
    {
        actionType = NUIControl::ECAT_QUIT_GAME;
        conformationMsg = "Do you want to|quit the game?";
    }
    else if( m_pUIControl->GetName() == "home_btn" )
    {
        conformationMsg = "Are you sure you|want to go back to|the lobby?";
        executionAction = "lobby_state";
        actionType = NUIControl::ECAT_GAME_STATE_CHANGE;
    }
    else if( m_pUIControl->GetName() == "big_pay_back_btn" )
    {
        conformationMsg = "Are you sure you|want to load|The Big Pay Back game?";
        executionAction = "big_pay_back_state";
        actionType = NUIControl::ECAT_GAME_STATE_CHANGE;
    }
    else if( m_pUIControl->GetName() == "Key_Binding_reset_btn" )
    {
        pSmartGuiCtrl = new CSmartResetKeyBindBtn( pYesBtn );
        conformationMsg = "Reset all key bindings|to their default settings?";
    }
    /*else if( m_pUIControl->GetName().find(".game_save") != string::npos )
    {
        // Find the file name save no. deliminator
        size_t dashPos = m_pUIControl->GetName().find("_");

        // Get the save no.
        std::string saveNo = pUIControl->GetName().substr(0, dashPos);

        pSmartGuiCtrl = new CSmartNewGameSaveBtn( pYesBtn );
        conformationMsg = "Do you want to|overwrite Save " + saveNo + " with|the new game save file?";

        // Set the control name which is the file name to overwrite.
        executionAction = pUIControl->GetName();

        // Set the action type to kill the menu
        actionType = NUIControl::ECAT_DEACTIVATE;
    }*/

    // Set the conformation menu
    pYesBtn->SetSmartGui( pSmartGuiCtrl );
    pYesBtn->SetActionType( actionType );
    pYesBtn->SetExecutionAction( executionAction );
    pMegLbl->CreateFontString( conformationMsg );

}   // Execute







