
/************************************************************************
*    FILE NAME:       slotgame.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <slot/slotgame.h>

// Game lib dependencies
#include <slot/slotmathmanager.h>
#include <slot/betmanager.h>
#include <slot/ifrontpanel.h>
#include <slot/slotgroupview.h>
#include <slot/slotgroupmodel.h>
#include <slot/icycleresults.h>
#include <gui/uimeter.h>
#include <utilities/xmlParser.h>
#include <utilities/matrix.h>

// Boost lib dependencies
#include <boost/bind.hpp>

// SDL/OpenGL lib dependencies
#include <SDL.h>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotGame::CSlotGame( const std::string & group ) :
    m_slotState( NSlotDefs::ESLOT_IDLE ),
    m_group( group ),
    m_pFrontPanel( nullptr ),
    m_waitForSpinMusicTimer(false),
    m_spinMusicTimeOut(0),
    m_allowSpinMusic(true),
    m_allowStopSounds(true),
    m_cycleResultsActive(false)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotGame::~CSlotGame()
{
}   // destructor


/************************************************************************
*    desc:  Create the slot group. Math and video reel strips
************************************************************************/
void CSlotGame::CreateSlotGroup(
    const NSlotDefs::ESlotDevice slotDevice,
    const std::string & slotStripSetId,
    const std::string & paytableSetId,
    const CSlotMath & rSlotMath,
    const XMLNode & viewReelCfgNode,
    const XMLNode & viewSpinProfileCfgNode,
    CSymbolSetView & rSymbolSetView,
    std::unique_ptr<iCycleResults> upCycleResults )
{
    m_slotGroupDeq.emplace_back( rSlotMath, m_slotResults.Create() );
    
    m_slotGroupDeq.back().Create(
        slotDevice,
        slotStripSetId,
        paytableSetId,
        viewReelCfgNode,
        viewSpinProfileCfgNode,
        rSymbolSetView,
        std::move(upCycleResults) );
    
}   // CreateReelGroup


/***************************************************************************
*    desc:  Load the slot config file
****************************************************************************/
void CSlotGame::LoadSlotConfig( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "slot" );
    
    const XMLNode spinMusicScriptFunNode = node.getChildNode( "spinMusicScriptFun" );
    if( !spinMusicScriptFunNode.isEmpty() )
    {
        m_spinMusicStartFunc = spinMusicScriptFunNode.getAttribute( "startMusic" );
        m_spinMusicStopFunc = spinMusicScriptFunNode.getAttribute( "stopMusic" );
        m_spinMusicTimeOut = std::atoi(spinMusicScriptFunNode.getAttribute( "timeOut" ));
    }
    
}   // LoadSlotConfig


/************************************************************************
*    desc:  Do we play the spin music
************************************************************************/
void CSlotGame::AllowSpinMusic( bool allow )
{
    m_allowSpinMusic = allow;
    
}   // AllowSpinMusic


/************************************************************************
*    desc:  Do we allow the stop sounds?
************************************************************************/
void CSlotGame::AllowStopSounds( bool allow )
{
    for( auto & iter : m_slotGroupDeq )
        iter.GetView()->AllowStopSounds( allow );
    
}   // AllowStopSounds


/***************************************************************************
*    desc:  Go through the game state
****************************************************************************/
void CSlotGame::ProcessGameState()
{
    switch( m_slotState )
    {
        case NSlotDefs::ESLOT_IDLE:                     StateIdle();                  break;
        case NSlotDefs::ESLOT_WAIT_CYCLE_RESULTS_STOP:  StateWaitCycleResultsStop();  break;
        case NSlotDefs::ESLOT_PLACE_WAGER:              StatePlaceWager();            break;
        case NSlotDefs::ESLOT_GENERATE_STOPS:           StateGenerateStops();         break;
        case NSlotDefs::ESLOT_EVALUATE:                 StateEvaluate();              break;
        case NSlotDefs::ESLOT_PRE_SPIN:                 StatePreSpin();               break;
        case NSlotDefs::ESLOT_SPIN:                     StateSpin();                  break;
        case NSlotDefs::ESLOT_POST_SPIN:                StatePostSpin();              break;
        case NSlotDefs::ESLOT_PRE_AWARD_WIN:            StatePreAwardWin();           break;
        case NSlotDefs::ESLOT_BONUS_DECISION:           StateBonusDecision();         break;
        case NSlotDefs::ESLOT_PRE_BONUS:                StatePreBonus();              break;
        case NSlotDefs::ESLOT_BONUS:                    StateBonus();                 break;
        case NSlotDefs::ESLOT_POST_BONUS:               StatePostBonus();             break;
        case NSlotDefs::ESLOT_POST_AWARD_WIN:           StatePostAwardWin();          break;
        case NSlotDefs::ESLOT_WAIT_FOR_AWARD:           StateWaitForAward();          break;
        case NSlotDefs::ESLOT_END:                      StateEnd();                   break;
    };
    
}   // ProcessGameState


/***************************************************************************
*    desc: State Idle slot
****************************************************************************/
void CSlotGame::StateIdle()
{
    // Fade down the music if the player is not spinning
    if( m_allowSpinMusic && m_waitForSpinMusicTimer && !m_spinMusicStopFunc.empty() )
    {
        if( m_stopSpinMusicTimer.Expired() )
        {
            m_scriptComponent.Prepare( m_group, m_spinMusicStopFunc );
            m_waitForSpinMusicTimer = false;
        }
    }
    
}   // StateIdle


/***************************************************************************
*    desc:  State Wait for the cycle results to stop
****************************************************************************/
void CSlotGame::StateWaitCycleResultsStop()
{
    if( !IsCycleResultsAnimating() )
    {
        m_cycleResultsActive = false;
        
        for( auto & iter : m_slotGroupDeq )
            iter.StopCycleResults();
        
        m_slotState = NSlotDefs::ESLOT_PLACE_WAGER;
    }
    
}   // StateWaitCycleResultsStop


/***************************************************************************
*    desc:  State Place Wager
****************************************************************************/
void CSlotGame::StatePlaceWager()
{
    CBetMgr::Instance().DeductBet();
    
    if( m_pFrontPanel != nullptr )
        m_pFrontPanel->InitGame( CBetMgr::Instance().GetCredits() );
    
    m_slotResults.Clear();
    
    if( m_allowSpinMusic && !m_spinMusicStartFunc.empty() )
    {
        m_scriptComponent.StopAndRecycle( m_spinMusicStopFunc );
        m_scriptComponent.Prepare( m_group, m_spinMusicStartFunc );
    }
    
    m_slotState = NSlotDefs::ESLOT_GENERATE_STOPS;
    
}   // StatePlaceWager


/***************************************************************************
*    desc:  State Generate Stops
****************************************************************************/
void CSlotGame::StateGenerateStops()
{
    for( auto & iter : m_slotGroupDeq )
        iter.GetModel().GenerateStops();
    
    m_slotState = NSlotDefs::ESLOT_EVALUATE;
    
}   // StateGenerateStops


/***************************************************************************
*    desc:  State Evaluate
****************************************************************************/
void CSlotGame::StateEvaluate()
{
    for( auto & iter : m_slotGroupDeq )
        iter.GetModel().Evaluate();
    
    m_slotResults.SortPays();
    m_slotResults.AddUpWin();
    
    m_slotState = NSlotDefs::ESLOT_PRE_SPIN;
    
}   // StateEvaluate


/***************************************************************************
*    desc:  State Pre Spin
****************************************************************************/
void CSlotGame::StatePreSpin()
{
    for( auto & iter : m_slotGroupDeq )
        iter.GetView()->StartSpin();
    
    m_slotState = NSlotDefs::ESLOT_SPIN;
    
}   // StatePreSpin


/***************************************************************************
*    desc:  State Spin
****************************************************************************/
void CSlotGame::StateSpin()
{
    bool stopped(true);
    for( auto & iter : m_slotGroupDeq )
    {
        if( !iter.GetView()->IsSpinState( NSlotDefs::ESS_STOPPED ) )
        {
            stopped = false;
            break;
        }
    }
    
    if( stopped )
        m_slotState = NSlotDefs::ESLOT_POST_SPIN;
    
}   // StatePreSpin


/***************************************************************************
*    desc:  State Post Spin
****************************************************************************/
void CSlotGame::StatePostSpin()
{
    m_slotState = NSlotDefs::ESLOT_PRE_AWARD_WIN;
    
}   // StatePostSpin


/***************************************************************************
*    desc:  State Pre Award Win
****************************************************************************/
void CSlotGame::StatePreAwardWin()
{
    m_slotState = NSlotDefs::ESLOT_BONUS_DECISION;
    
}   // StatePreAwardWin


/***************************************************************************
*    desc:  State Bonus Decision
****************************************************************************/
void CSlotGame::StateBonusDecision()
{
    m_slotState = NSlotDefs::ESLOT_PRE_BONUS;
    
}   // StateBonusDecision


/***************************************************************************
*    desc:  State Pre Bonus
****************************************************************************/
void CSlotGame::StatePreBonus()
{
    m_slotState = NSlotDefs::ESLOT_BONUS;
    
}   // StatePreBonus


/***************************************************************************
*    desc:  State Bonus
****************************************************************************/
void CSlotGame::StateBonus()
{
    m_slotState = NSlotDefs::ESLOT_POST_BONUS;
    
}   // StateBonus


/***************************************************************************
*    desc:  State Post Bonus
****************************************************************************/
void CSlotGame::StatePostBonus()
{
    m_slotState = NSlotDefs::ESLOT_POST_AWARD_WIN;
    
}   // StatePostBonus


/***************************************************************************
*    desc:  State Post Award Win
****************************************************************************/
void CSlotGame::StatePostAwardWin()
{
    if( m_slotResults.IsWin() )
    {
        CBetMgr::Instance().AddAward( m_slotResults.GetTotalWin() );
        
        if( m_pFrontPanel != nullptr )
            m_pFrontPanel->StartBangUp( 
                m_slotResults.GetTotalWin(), CBetMgr::Instance().GetCredits() );
        
        // Start the cycle results
        for( auto & iter : m_slotGroupDeq )
            iter.StartCycleResults();
        
        m_cycleResultsActive = true;
    }
    
    m_slotState = NSlotDefs::ESLOT_WAIT_FOR_AWARD;
    
}   // StatePostAwardWin


/***************************************************************************
*    desc:  State wait for the award to finish it's display
****************************************************************************/
void CSlotGame::StateWaitForAward()
{
    if( m_pFrontPanel != nullptr )
    {
        if( !m_pFrontPanel->IsBanging() )
            m_slotState = NSlotDefs::ESLOT_END;
    }
    else
    {
        m_slotState = NSlotDefs::ESLOT_END;
    }
    
}   // StateWaitForAward


/***************************************************************************
*    desc:  State End
****************************************************************************/
void CSlotGame::StateEnd()
{
    if( m_pFrontPanel != nullptr )
        m_pFrontPanel->EnableButtons( CBetMgr::Instance().AllowPlay() );
    
    // Set the timer that waits to see if the music should time out
    m_stopSpinMusicTimer.Set( m_spinMusicTimeOut );
    m_waitForSpinMusicTimer = true;
    
    m_slotState = NSlotDefs::ESLOT_IDLE;
    
}   // StateEnd


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CSlotGame::HandleEvent( const SDL_Event & rEvent )
{
}   // HandleEvent


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CSlotGame::Update()
{
    for( auto & iter : m_slotGroupDeq )
        iter.Update();
    
    // Start the cycle results animation if not currently animating
    if( m_cycleResultsActive )
    {
        if( !IsCycleResultsAnimating() )
            for( auto & iter : m_slotGroupDeq )
                iter.StartCycleResultsAnimation();
    }
    
    m_scriptComponent.Update();

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CSlotGame::Transform()
{
    for( auto & iter : m_slotGroupDeq )
        iter.GetView()->Transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CSlotGame::Render( const CMatrix & matrix )
{
    for( auto & iter : m_slotGroupDeq )
        iter.GetView()->Render( matrix );

}   // Render


/***************************************************************************
*    desc:  Play a game
****************************************************************************/
void CSlotGame::PlayGame(CUIControl *)
{
    if( m_slotState == NSlotDefs::ESLOT_IDLE )
    {
        if( CBetMgr::Instance().AllowPlay() )
        {
            if( m_cycleResultsActive )
            {
                // Stop the cycle results
                for( auto & iter : m_slotGroupDeq )
                    iter.StopCycleResultsAnimation();
                
                m_slotState = NSlotDefs::ESLOT_WAIT_CYCLE_RESULTS_STOP;
            }
            else
                m_slotState = NSlotDefs::ESLOT_PLACE_WAGER;
        }
    }
    else if( m_slotState == NSlotDefs::ESLOT_SPIN )
    {
        for( auto & iter : m_slotGroupDeq )
            iter.GetView()->StopSpin();
    }
    else if( m_slotState == NSlotDefs::ESLOT_WAIT_FOR_AWARD )
    {
        if( m_pFrontPanel != nullptr )
            m_pFrontPanel->FastBang();
    }
    
}   // PlayGame


/***************************************************************************
*    desc:  Set the front panel
****************************************************************************/
void CSlotGame::SetFrontPanel( iFrontPanel * pFrontPanel )
{
    m_pFrontPanel = pFrontPanel;
}


/***************************************************************************
*    desc:  Get the state
****************************************************************************/
NSlotDefs::ESlotState CSlotGame::GetState()
{
    return m_slotState;
    
}   // GetState


/************************************************************************
*    desc:  Is the cycle results animating
************************************************************************/
bool CSlotGame::IsCycleResultsAnimating()
{
    bool animating = false;

    for( auto & iter : m_slotGroupDeq )
        animating |= iter.IsCycleResultsAnimating();

    return animating;
    
}   // IsCycleResultsAnimating
