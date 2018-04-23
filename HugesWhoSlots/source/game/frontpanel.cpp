
/************************************************************************
*    FILE NAME:       frontpanel.cpp
*
*    DESCRIPTION:     Game specific class container to manage buttons and meters.
*                     Does NOT own the pointers. Do Not Free
************************************************************************/

// Physical component dependency
#include "frontpanel.h"

// Game lib dependencies
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <2d/sprite2d.h>
#include <objectdata/objectdatamanager.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CFrontPanel::CFrontPanel() :
    m_pPlayBtn(nullptr),
    m_pWinMeter(nullptr),
    m_pCreditMeter(nullptr)
{
}   // constructor


/************************************************************************
*    desc:  Set the buttons
************************************************************************/
void CFrontPanel::SetButtons( CUIButton * pPlayBtn, std::vector<CUIControl *> & pOtherBtnVec )
{
    m_pPlayBtn = pPlayBtn;
    m_pOtherBtnVec = pOtherBtnVec;
}


/************************************************************************
*    desc:  Set the meters
************************************************************************/
void CFrontPanel::SetMeters( CUIMeter * pWinMeter, CUIMeter * pCreditMeter )
{
    m_pWinMeter = pWinMeter;
    m_pCreditMeter = pCreditMeter;
    
}   // SetMeters


/************************************************************************
*    desc:  Init a new game
************************************************************************/
void CFrontPanel::InitGame( uint credits )
{
    if( m_pWinMeter != nullptr )
        m_pWinMeter->clear();
    
    if( m_pCreditMeter != nullptr )
        m_pCreditMeter->set( credits );
    
    for( auto iter : m_pOtherBtnVec )
        iter->disableControl();

}   // InitGame


/************************************************************************
*    desc:  Start the bang up
************************************************************************/
void CFrontPanel::StartBangUp( uint win, uint credits )
{
    if( m_pWinMeter != nullptr )
        m_pWinMeter->startBangUp( win );
    
    if( m_pCreditMeter != nullptr )
        m_pCreditMeter->startBangUp( credits );

}   // StartBangUp


/************************************************************************
*    desc:  Are the meters banging
************************************************************************/
bool CFrontPanel::IsBanging()
{
    bool result(false);
    
    if( m_pWinMeter != nullptr )
        result |= m_pWinMeter->isBanging();
    
    if( m_pCreditMeter != nullptr )
        result |= m_pCreditMeter->isBanging();
    
    return result;
    
}   // IsBanging


/************************************************************************
*    desc:  Start the fast bang
************************************************************************/
void CFrontPanel::FastBang()
{
    if( m_pWinMeter != nullptr )
        m_pWinMeter->fastBang();
    
    if( m_pCreditMeter != nullptr )
        m_pCreditMeter->fastBang();
    
}   // FastBang


/************************************************************************
*    desc:  Enable the buttons
************************************************************************/
void CFrontPanel::EnableButtons( bool allowPlay )
{
    for( auto iter : m_pOtherBtnVec )
        iter->enableControl();
    
    if( (m_pPlayBtn != nullptr) && allowPlay )
        m_pPlayBtn->callSpriteScriptFuncKey( "playReady", true );

}   // EnableButtons
