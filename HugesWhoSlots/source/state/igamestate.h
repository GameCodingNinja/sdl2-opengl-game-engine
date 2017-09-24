
/************************************************************************
*    FILE NAME:       igamestate.h
*
*    DESCRIPTION:     gamestate interface Class
************************************************************************/

#ifndef __i_game_state_h__
#define __i_game_state_h__

// Game dependencies
#include "statemessage.h"

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
union SDL_Event;

class iGameState : public boost::noncopyable
{
public:

    // Constructor
    iGameState( const NGameDefs::EGameState gameState, const NGameDefs::EGameState nextState )  :
        m_gameState(gameState),
        m_nextState(nextState)
    {};

    // Constructor
    iGameState( const NGameDefs::EGameState gameState, const CStateMessage & stateMsg ) :
        m_gameState(gameState),
        m_nextState(NGameDefs::EGS_GAME_LOAD),
        m_stateMessage(stateMsg)
    {};

    // Destructor
    virtual ~iGameState(){};
    
    // Do any pre-game loop init's
    virtual void Init(){};

    // Allow Music
    virtual void AllowMusic( bool allow ){};
    
    // Allow Stop Sounds
    virtual void AllowStopSounds( bool allow ){};

    // Handle events
    virtual void HandleEvent( const SDL_Event & rEvent ){};

    // Act upon what the user is doing
    virtual void GetUserInput( bool hasFocus ){};
    
    // Handle any misc processing before the real work is started
    virtual void MiscProcess(){};

    // Handle the physics
    virtual void Physics(){};

    // Update objects that require them
    virtual void Update(){};

    // Transform the game objects
    virtual void Transform(){};

    // 2D/3D Render of game content
    virtual void PreRender(){};
    virtual void PostRender(){};

    // Is the state done
    virtual bool DoStateChange()
    { return m_changeState; };

    // Get the currect game state
    NGameDefs::EGameState GetState()
    { return m_gameState; }

    // Get the next game state
    virtual NGameDefs::EGameState GetNextState()
    { return m_nextState; }

    // Get the message
    const CStateMessage & GetStateMessage()
    { return m_stateMessage; }

protected:

    // This objects current game state value
    const NGameDefs::EGameState m_gameState;

    // This objects next game state value
    const NGameDefs::EGameState m_nextState;
    
    // Message to send to next state
    CStateMessage m_stateMessage;
    
    // Flag to indicate state change
    bool m_changeState = false;

};

#endif  // __i_game_state_h__


