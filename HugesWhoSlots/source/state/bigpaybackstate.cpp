
/************************************************************************
*    FILE NAME:       bigpaybackstate.cpp
*
*    DESCRIPTION:     The Big Pay Back game state
************************************************************************/

// Physical component dependency
#include "bigpaybackstate.h"

// Game dependencies
#include "../game/gamesave.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/xmlpreloader.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/settings.h>
#include <gui/menumanager.h>
#include <gui/uilabel.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <gui/uiprogressbar.h>
#include <gui/uibuttonlist.h>
#include <slot/slotmathmanager.h>
#include <slot/symbolsetviewmanager.h>
#include <slot/betmanager.h>
#include <slot/slotgroup.h>
#include <slot/animatedcycleresults.h>
#include <slot/slotgroupfactory.h>
#include <slot/playresult.h>
#include <script/scriptmanager.h>
#include <managers/soundmanager.h>
#include <managers/spritesheetmanager.h>
#include <managers/cameramanager.h>
#include <common/build_defs.h>

// Boost lib dependencies
#include <boost/bind.hpp>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CBigPayBackState::CBigPayBackState( const std::string & group ) :
    CCommonState( NGameDefs::EGS_BIG_CASH_BACK, NGameDefs::EGS_GAME_LOAD ),
        m_stateGroup( group ),
        m_background( CObjectDataMgr::Instance().getData2D( group, "background" ) ),
	m_pig( CObjectDataMgr::Instance().getData2D( group, "Payback Pig" ) ),
        m_baseGameMusic( group, "SlotGame_StartSpinMusic", "SlotGame_StopSpinMusic", "SlotGame_FastStopSpinMusic", 7000 )
{
}   // Constructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CBigPayBackState::Init()
{
    CCommonState::Init();
    
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().allow();
    CMenuManager::Instance().activateTree( "menu_tree");
    CMenuManager::Instance().activateTree( "confirmation_tree");
    CMenuManager::Instance().activateTree( "buy_tree");
    CMenuManager::Instance().activateTree( "base_game_tree");
    
    // Hook the Play button to the reel group
    CUIButton & rPlayBtn = CMenuManager::Instance().getMenuControl<CUIButton>( "base_game_menu", "play_btn" );
    rPlayBtn.connect_executionAction( boost::bind(&CSlotGame::PlayGame, &m_slotGame, _1) );

    // Hook the total bet call back function to the total bet meter
    auto & rTotalBetMeter = CMenuManager::Instance().getMenuControl<CUIButtonList>( "base_game_menu", "total_bet_meter" );
    auto & rTotalBetDecBtn = *rTotalBetMeter.findSubControl( std::string("total_bet_dec_btn") );
    auto & rTotalBetIncBtn = *rTotalBetMeter.findSubControl( std::string("total_bet_inc_btn") );
    rTotalBetDecBtn.connect_executionAction( boost::bind(&CBigPayBackState::TotalBetCallBack, this, _1) );
    rTotalBetIncBtn.connect_executionAction( boost::bind(&CBigPayBackState::TotalBetCallBack, this, _1) );
    
    // Create a play result
    auto & rPlayResult = m_slotGame.CreatePlayResult();
    
    // Create the slot group
    m_slotGame.AddSlotGroup(
        NSlotGroupFactory::Create(
            NSlotDefs::ED_REEL,
            "main_reel_strip",
            "main_paytable",
            CSlotMathMgr::Instance().GetSlotMath( m_stateGroup, "slot" ),
            CXMLPreloader::Instance().getNode( std::get<0>(NBigPayBack::reelGrpCfg) ),
            CXMLPreloader::Instance().getNode( std::get<0>(NBigPayBack::spinProfileCfg) ),
            CSymbolSetViewMgr::Instance().Get( m_stateGroup, "base_game" ),
            rPlayResult,
            std::move(std::unique_ptr<iCycleResults>(new CAnimatedCycleResults( &rPlayResult ))) ) );
    
    // Init the front panel
    std::vector<CUIControl *> btnVec = {
        &CMenuManager::Instance().getMenuControl<CUIControl>( "base_game_menu", "home_btn" ),
        &CMenuManager::Instance().getMenuControl<CUIControl>( "base_game_menu", "menu_btn" ),
        &CMenuManager::Instance().getMenuControl<CUIControl>( "base_game_menu", "buy_btn" ),
        &rTotalBetMeter };
    m_frontPanel.SetButtons( &rPlayBtn, btnVec );
    
    m_frontPanel.SetMeters(
        &CMenuManager::Instance().getMenuControl<CUIMeter>( "base_game_menu", "win_meter" ),
        &CMenuManager::Instance().getMenuControl<CUIMeter>( "base_game_menu", "credit_meter" ) );
    
    // Add slot game component
    m_slotGame.SetFrontPanel( &m_frontPanel );
    m_slotGame.SetGameMusic( &m_baseGameMusic );
    
    m_pig.setPos( CPoint<float>(-875,-200,0) );
    
    // Init the credit meter
    CMenuManager::Instance().getMenuControl<CUIMeter>( "base_game_menu", "credit_meter" ).set( CBetMgr::Instance().GetCredits()  );
    
    // Clear any preloaded XML files
    CXMLPreloader::Instance().clear();
    
    // Prepare the script to fade in the screen
    m_scriptComponent.prepare( m_stateGroup, "Screen_FadeIn" );
    
    AllowMusic( CGameSave::Instance().GetPlaySpinMusic() );
    AllowStopSounds( CGameSave::Instance().GetPlayStopSounds() );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
    
}   // Init


/************************************************************************
*    desc:  Do we play the spin music
************************************************************************/
void CBigPayBackState::AllowMusic( bool allow )
{
    if( !allow )
        m_baseGameMusic.FastFadeDown();
    
    m_baseGameMusic.AllowMusic( allow );
    
}   // AllowSpinMusic


/************************************************************************
*    desc:  Do we allow the stop sounds?
************************************************************************/
void CBigPayBackState::AllowStopSounds( bool allow )
{
    m_slotGame.AllowStopSounds( allow );
    
}   // AllowStopSounds


/***************************************************************************
*    desc:  Call back for when the total be is changed
****************************************************************************/
void CBigPayBackState::TotalBetCallBack(CUIControl *)
{
    const CUIButtonList & rTotalBetMeter = CMenuManager::Instance().getMenuControl<CUIButtonList>( "base_game_menu", "total_bet_meter" );
    CBetMgr::Instance().SetLineBet( rTotalBetMeter.getActiveIndex() + 1 );
    
}   // TotalBetCallBack


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CBigPayBackState::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN )
        {
            m_scriptComponent.prepare( m_stateGroup, "Screen_FadeOut" );
            m_baseGameMusic.FastFadeDown();
        }
    }
    else if( rEvent.type == SDL_APP_WILLENTERBACKGROUND )
    {
        if( m_slotGame.GetState() == NSlotDefs::ESLOT_IDLE )
            CSoundMgr::Instance().stopMusic();
    }

}   // HandleEvent


/***************************************************************************
*    desc:  Handle any misc processing before the real work is started
****************************************************************************/
void CBigPayBackState::MiscProcess()
{
    m_slotGame.ProcessGameState();
    
}   // MiscProcess


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CBigPayBackState::Update()
{
    CCommonState::Update();
    
    CScriptManager::Instance().update();
    
    m_scriptComponent.update();
    
    m_baseGameMusic.Update();
    
    if( !CMenuManager::Instance().isMenuActive() )
        m_slotGame.Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CBigPayBackState::Transform()
{
    CCommonState::Transform();

    CMenuManager::Instance().transformInterface();
    m_background.transform();
    m_pig.transform();
    m_slotGame.Transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CBigPayBackState::PreRender()
{
    const CMatrix & orthoMatrix = CCameraMgr::Instance().getDefaultProjMatrix();
    m_background.render( orthoMatrix );
    m_slotGame.Render( orthoMatrix );
    m_pig.render( orthoMatrix );
    
    CCommonState::PreRender();

}   // PreRender


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NBigPayBack
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().loadGroup2D( "(big_pay_back)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().createFromData2D( "(big_pay_back)" );
    }
    
    void Load()
    {
        // Load the state specific menu group
        CMenuManager::Instance().loadGroup("(big_pay_back)", CMenuManager::DONT_INIT_GROUP);
        
        // Load sound resources for the game
        CSoundMgr::Instance().loadGroup("(big_pay_back)");
        
        // Load state specific AngelScript functions
        CScriptManager::Instance().loadGroup("(big_pay_back)");
        
        // Load the slot group stuff
        CSymbolSetViewMgr::Instance().LoadGroup( "(big_pay_back)" );
        CSlotMathMgr::Instance().LoadGroup( "(big_pay_back)" );
        CSlotMathMgr::Instance().LoadPaylineSetFromFile( "data/objects/2d/slot/payline_4x5.cfg" );
        
        // Set the line bet and the total numvber of lines bet
        CBetMgr::Instance().SetLineBet(1);
        CBetMgr::Instance().SetTotalLines( CSlotMathMgr::Instance().GetPaylineSet("40_4x5").GetLineData().size() );
        
        // Free the sprite sheet data manager because it's no longer needed
        CSpriteSheetMgr::Instance().clear();
        
        // Preload some needed XML files
        CXMLPreloader::Instance().clear();
        CXMLPreloader::Instance().load( std::get<0>(reelGrpCfg), std::get<1>(reelGrpCfg) );
        CXMLPreloader::Instance().load( std::get<0>(spinProfileCfg), std::get<1>(spinProfileCfg) );

    }   // Load
    
    void CriticalInit()
    {
        // Creates the font strings, run init scripts
        CMenuManager::Instance().initGroup("(big_pay_back)");
    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void CriticalUnload()
    {
        CMenuManager::Instance().cleanUpGroup("(big_pay_back)");
        CObjectDataMgr::Instance().freeGroup2D( "(big_pay_back)" );
        
        // Unload the slot group stuff
        CSymbolSetViewMgr::Instance().Clear();
    }
    
    void Unload()
    {
        // Unload the state specific menu group
        CMenuManager::Instance().freeGroup("(big_pay_back)");
        
        // Unload the slot group stuff
        CSlotMathMgr::Instance().Clear();
        
        // Unload state specific AngelScript functions
        CScriptManager::Instance().freeGroup("(big_pay_back)");
        
        // Unload sound resources for the game
        CSoundMgr::Instance().freeGroup("(big_pay_back)");

    }   // Unload

}   // NBigPayBack