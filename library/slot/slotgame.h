
/************************************************************************
*    FILE NAME:       slotgame.h
*
*    DESCRIPTION:     Class for handling the slot game
************************************************************************/

#ifndef __slot_game_h__
#define __slot_game_h__

// Game lib dependencies
#include <slot/slotgroup.h>
#include <slot/slotdefs.h>
#include <slot/slotresults.h>
#include <script/scriptcomponent.h>
#include <utilities/timer.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <deque>
#include <memory>

// Forward declaration(s)
class CSlotMath;
class CUIMeter;
class CUIControl;
class CMatrix;
class CSymbolSetView;
class iFrontPanel;
class iCycleResults;
union SDL_Event;
struct XMLNode;

class CSlotGame : public boost::noncopyable
{
public:

    // Constructor
    CSlotGame( const std::string & group );

    // Destructor
    virtual ~CSlotGame();
    
    // Create the slot group. Math and video reel strips
    void CreateSlotGroup(
        const NSlotDefs::ESlotDevice slotDevice,
        const std::string & slotStripSetId,
        const std::string & paytableSetId,
        const CSlotMath & rSlotMath,
        const XMLNode & viewReelCfgNode,
        const XMLNode & viewSpinProfileCfgNode,
        CSymbolSetView & rSymbolSetView,
        std::unique_ptr<iCycleResults> upCycleResults );
    
    // Load the slot config file
    void LoadSlotConfig( const std::string & filePath );
    
    // Handle events
    void HandleEvent( const SDL_Event & rEvent );
    
    // Update objects that require them
    void Update();

    // Transform the game objects
    void Transform();

    // Render of game content
    void Render( const CMatrix & matrix );
    
    // Play a game
    void PlayGame(CUIControl *);
    
    // Go through the game state
    void ProcessGameState();
    
    // Set the front panel
    void SetFrontPanel( iFrontPanel * pFrontPanel );
    
    // Get the state
    NSlotDefs::ESlotState GetState();
    
    // Do we play the spin music
    void AllowSpinMusic( bool allow );
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow );
    
protected:
    
    // Game state functions
    void StateIdle();
    void StateWaitCycleResultsStop();
    void StatePlaceWager();
    void StateGenerateStops();
    void StateEvaluate();
    void StatePreSpin();
    void StateSpin();
    void StatePostSpin();
    void StatePreAwardWin();
    void StateBonusDecision();
    void StatePreBonus();
    void StateBonus();
    void StatePostBonus();
    void StatePostAwardWin();
    void StateWaitForAward();
    void StateEnd();
    
    // Is the cycle results animating
    bool IsCycleResultsAnimating();

private:
    
    // Slot results class
    CSlotResults m_slotResults;
    
    // Reel group
    std::deque<CSlotGroup> m_slotGroupDeq;
    
    // slot state
    NSlotDefs::ESlotState m_slotState;
    
    // stop spin music timer
    CTimer m_stopSpinMusicTimer;
    
    // For scripting needs
    CScriptComponent m_scriptComponent;
    
    // Slot group
    const std::string m_group;
    
    // Class for holding interface items
    // Does not own pointer. Do Not Free
    iFrontPanel * m_pFrontPanel;
    
    // bool to control the spotting of the spin music
    bool m_waitForSpinMusicTimer;
    
    // Spin start and stop music function calls
    std::string m_spinMusicStartFunc;
    std::string m_spinMusicStopFunc;
    int m_spinMusicTimeOut;
    
    // Flag to indicate spin music can be played
    bool m_allowSpinMusic;
    
    // Flag to indicate stop sounds can be played
    bool m_allowStopSounds;
    
    // Cycle results flag
    bool m_cycleResultsActive;
};

#endif  // __slot_game_h__


