
/************************************************************************
*    FILE NAME:       slotgame.h
*
*    DESCRIPTION:     Class for handling the slot game
************************************************************************/

#ifndef __slot_game_h__
#define __slot_game_h__

// Game lib dependencies
#include <slot/slotdefs.h>
#include <slot/slotresults.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <memory>

// Forward declaration(s)
class CSlotGroup;
class CSlotMath;
class CUIMeter;
class CUIControl;
class CMatrix;
class CSymbolSetView;
class iFrontPanel;
class iGameMusic;
class iCycleResults;
class CPlayResult;
union SDL_Event;
struct XMLNode;

class CSlotGame : public boost::noncopyable
{
public:

    // Constructor
    CSlotGame();

    // Destructor
    virtual ~CSlotGame();
    
    // Add the slot group
    void AddSlotGroup( std::unique_ptr<CSlotGroup> slotGroup );
    
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
    
    // Set the game music
    void SetGameMusic( iGameMusic * pGameMusic );
    
    // Get the state
    NSlotDefs::ESlotState GetState();
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow );
    
    // Create a play result for a slot group
    CPlayResult & CreatePlayResult();
    
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
    std::vector<std::unique_ptr<CSlotGroup>> m_slotGroupVec;
    
    // slot state
    NSlotDefs::ESlotState m_slotState;
    
    // Class for holding interface items
    // Does not own pointer. Do Not Free
    iFrontPanel * m_pFrontPanel;
    
    // Class for holding interface items
    // Does not own pointer. Do Not Free
    iGameMusic * m_pGameMusic;
    
    // Cycle results flag
    bool m_cycleResultsActive;
};

#endif  // __slot_game_h__


