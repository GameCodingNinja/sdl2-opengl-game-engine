
/************************************************************************
*    FILE NAME:       slotgame.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <slot/slotgame.h>

// Game lib dependencies
#include <slot/slotgroup.h>
#include <slot/slotmathmanager.h>
#include <slot/betmanager.h>
#include <slot/slotgroupview.h>
#include <slot/slotgroupmodel.h>
#include <slot/ifrontpanel.h>
#include <slot/icycleresults.h>
#include <slot/igamemusic.h>
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
CSlotGame::CSlotGame() :
    m_slotState( NSlotDefs::ESLOT_IDLE ),
    m_pFrontPanel( nullptr ),
    m_pGameMusic( nullptr )
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotGame::~CSlotGame()
{
}


/************************************************************************
*    desc:  Add the slot group
************************************************************************/
void CSlotGame::addSlotGroup( std::unique_ptr<CSlotGroup> slotGroup )
{
    m_slotGroupVec.emplace_back( std::move(slotGroup) );
}


/************************************************************************
*    desc:  Do we allow the stop sounds?
************************************************************************/
void CSlotGame::allowStopSounds( bool allow )
{
    for( auto & iter : m_slotGroupVec )
        iter->getView()->allowStopSounds( allow );
}


/***************************************************************************
*    desc:  Go through the game state
****************************************************************************/
void CSlotGame::processGameState()
{
    switch( m_slotState )
    {
        case NSlotDefs::ESLOT_IDLE:                stateIdle();              break;
        case NSlotDefs::ESLOT_KILL_CYCLE_RESULTS:  stateKillCycleResults();  break;
        case NSlotDefs::ESLOT_PLACE_WAGER:         statePlaceWager();        break;
        case NSlotDefs::ESLOT_GENERATE_STOPS:      stateGenerateStops();     break;
        case NSlotDefs::ESLOT_EVALUATE:            stateEvaluate();          break;
        case NSlotDefs::ESLOT_PRE_SPIN:            statePreSpin();           break;
        case NSlotDefs::ESLOT_SPIN:                stateSpin();              break;
        case NSlotDefs::ESLOT_POST_SPIN:           statePostSpin();          break;
        case NSlotDefs::ESLOT_PRE_AWARD_WIN:       statePreAwardWin();       break;
        case NSlotDefs::ESLOT_BONUS_DECISION:      stateBonusDecision();     break;
        case NSlotDefs::ESLOT_PRE_BONUS:           statePreBonus();          break;
        case NSlotDefs::ESLOT_BONUS:               stateBonus();             break;
        case NSlotDefs::ESLOT_POST_BONUS:          statePostBonus();         break;
        case NSlotDefs::ESLOT_POST_AWARD_WIN:      statePostAwardWin();      break;
        case NSlotDefs::ESLOT_WAIT_FOR_AWARD:      stateWaitForAward();      break;
        case NSlotDefs::ESLOT_END:                 stateEnd();               break;
    };
}


/***************************************************************************
*    desc: State Idle slot
****************************************************************************/
void CSlotGame::stateIdle()
{
}


/***************************************************************************
*    desc:  State Wait for the cycle results to stop
****************************************************************************/
void CSlotGame::stateKillCycleResults()
{
    killCycleResults();

    m_slotState = NSlotDefs::ESLOT_PLACE_WAGER;
}


/***************************************************************************
*    desc:  State Place Wager
****************************************************************************/
void CSlotGame::statePlaceWager()
{
    CBetMgr::Instance().deductBet();

    if( m_pFrontPanel != nullptr )
        m_pFrontPanel->initGame( CBetMgr::Instance().getCredits() );

    m_slotResults.clear();

    if( m_pGameMusic )
        m_pGameMusic->startMusic();

    m_slotState = NSlotDefs::ESLOT_GENERATE_STOPS;
}


/***************************************************************************
*    desc:  State Generate Stops
****************************************************************************/
void CSlotGame::stateGenerateStops()
{
    for( auto & iter : m_slotGroupVec )
        iter->getModel()->generateStops();

    m_slotState = NSlotDefs::ESLOT_EVALUATE;
}


/***************************************************************************
*    desc:  State Evaluate
****************************************************************************/
void CSlotGame::stateEvaluate()
{
    for( auto & iter : m_slotGroupVec )
        iter->getModel()->evaluate();

    m_slotResults.sortPays();
    m_slotResults.addUpWin();

    m_slotState = NSlotDefs::ESLOT_PRE_SPIN;
}


/***************************************************************************
*    desc:  State Pre Spin
****************************************************************************/
void CSlotGame::statePreSpin()
{
    for( auto & iter : m_slotGroupVec )
        iter->getView()->startSpin();

    m_slotState = NSlotDefs::ESLOT_SPIN;
}


/***************************************************************************
*    desc:  State Spin
****************************************************************************/
void CSlotGame::stateSpin()
{
    bool stopped(true);
    for( auto & iter : m_slotGroupVec )
    {
        if( !iter->getView()->isSpinState( NSlotDefs::ESS_STOPPED ) )
        {
            stopped = false;
            break;
        }
    }

    if( stopped )
        m_slotState = NSlotDefs::ESLOT_POST_SPIN;
}


/***************************************************************************
*    desc:  State Post Spin
****************************************************************************/
void CSlotGame::statePostSpin()
{
    m_slotState = NSlotDefs::ESLOT_PRE_AWARD_WIN;
}


/***************************************************************************
*    desc:  State Pre Award Win
****************************************************************************/
void CSlotGame::statePreAwardWin()
{
    m_slotState = NSlotDefs::ESLOT_BONUS_DECISION;
}


/***************************************************************************
*    desc:  State Bonus Decision
****************************************************************************/
void CSlotGame::stateBonusDecision()
{
    m_slotState = NSlotDefs::ESLOT_PRE_BONUS;
}


/***************************************************************************
*    desc:  State Pre Bonus
****************************************************************************/
void CSlotGame::statePreBonus()
{
    m_slotState = NSlotDefs::ESLOT_BONUS;
}


/***************************************************************************
*    desc:  State Bonus
****************************************************************************/
void CSlotGame::stateBonus()
{
    m_slotState = NSlotDefs::ESLOT_POST_BONUS;
}


/***************************************************************************
*    desc:  State Post Bonus
****************************************************************************/
void CSlotGame::statePostBonus()
{
    m_slotState = NSlotDefs::ESLOT_POST_AWARD_WIN;
}


/***************************************************************************
*    desc:  State Post Award Win
****************************************************************************/
void CSlotGame::statePostAwardWin()
{
    if( m_slotResults.isWin() )
    {
        CBetMgr::Instance().addAward( m_slotResults.getTotalWin() );

        if( m_pFrontPanel != nullptr )
            m_pFrontPanel->startBangUp(
                m_slotResults.getTotalWin(), CBetMgr::Instance().getCredits() );

        // Start the cycle results
        for( auto & iter : m_slotGroupVec )
            iter->getView()->activateCycleResults();
    }

    m_slotState = NSlotDefs::ESLOT_WAIT_FOR_AWARD;
}


/***************************************************************************
*    desc:  State wait for the award to finish it's display
****************************************************************************/
void CSlotGame::stateWaitForAward()
{
    if( m_pFrontPanel != nullptr )
    {
        if( !m_pFrontPanel->isBanging() )
            m_slotState = NSlotDefs::ESLOT_END;
    }
    else
    {
        m_slotState = NSlotDefs::ESLOT_END;
    }
}


/***************************************************************************
*    desc:  State End
****************************************************************************/
void CSlotGame::stateEnd()
{
    if( m_pFrontPanel != nullptr )
        m_pFrontPanel->enableButtons( CBetMgr::Instance().allowPlay() );

    if( m_pGameMusic )
        m_pGameMusic->setTimeOut();

    m_slotState = NSlotDefs::ESLOT_IDLE;
}


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CSlotGame::handleEvent( const SDL_Event & rEvent )
{
}


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CSlotGame::update()
{
    for( auto & iter : m_slotGroupVec )
        iter->getView()->update();

    // Start the cycle results animation if not currently animating
    if( isCycleResultsActive() && !isCycleResultsAnimating() )
    {
        for( auto & iter : m_slotGroupVec )
            iter->getView()->startCycleResultsAnimation();
    }
}


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CSlotGame::transform()
{
    for( auto & iter : m_slotGroupVec )
        iter->getView()->transform();
}


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CSlotGame::render( const CMatrix & matrix )
{
    for( auto & iter : m_slotGroupVec )
        iter->getView()->render( matrix );
}


/***************************************************************************
*    desc:  Play a game
****************************************************************************/
void CSlotGame::playGame(CUIControl *)
{
    if( m_slotState == NSlotDefs::ESLOT_IDLE )
    {
        if( CBetMgr::Instance().allowPlay() )
        {
            if( isCycleResultsActive() )
                m_slotState = NSlotDefs::ESLOT_KILL_CYCLE_RESULTS;
            else
                m_slotState = NSlotDefs::ESLOT_PLACE_WAGER;
        }
    }
    else if( m_slotState == NSlotDefs::ESLOT_SPIN )
    {
        for( auto & iter : m_slotGroupVec )
            iter->getView()->stopSpin();
    }
    else if( m_slotState == NSlotDefs::ESLOT_WAIT_FOR_AWARD )
    {
        if( m_pFrontPanel != nullptr )
            m_pFrontPanel->fastBang();
    }
}


/***************************************************************************
*    desc:  Set the front panel
****************************************************************************/
void CSlotGame::setFrontPanel( iFrontPanel * pFrontPanel )
{
    m_pFrontPanel = pFrontPanel;
}


/***************************************************************************
*    desc:  Set the game music
****************************************************************************/
void CSlotGame::setGameMusic( iGameMusic * pGameMusic )
{
    m_pGameMusic = pGameMusic;
}


/***************************************************************************
*    desc:  Get the state
****************************************************************************/
NSlotDefs::ESlotState CSlotGame::getState()
{
    return m_slotState;
}


/************************************************************************
*    desc:  Is the cycle results animating
************************************************************************/
bool CSlotGame::isCycleResultsAnimating()
{
    for( auto & iter : m_slotGroupVec )
        if( iter->getView()->isCycleResultsAnimating() )
            return true;

    return false;
}


/************************************************************************
*    desc:  Is the cycle results active
************************************************************************/
bool CSlotGame::isCycleResultsActive()
{
    for( auto & iter : m_slotGroupVec )
        if( iter->getView()->isCycleResultsActive() )
            return true;

    return false;
}


/************************************************************************
*    desc:  Create a play result for a slot group
************************************************************************/
CPlayResult & CSlotGame::createPlayResult()
{
    return m_slotResults.create();
}


/************************************************************************
*    desc:  Kill the cycle results
************************************************************************/
void CSlotGame::killCycleResults()
{
    for( auto & iter : m_slotGroupVec )
    {
        iter->getView()->stopCycleResultsAnimation();
        iter->getView()->deactivateCycleResults();
    }
}
