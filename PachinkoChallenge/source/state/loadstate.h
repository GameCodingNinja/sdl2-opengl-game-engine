
/************************************************************************
*    FILE NAME:       loadstate.h
*
*    DESCRIPTION:     All this state does is unload/load and
*                     does a state change.
************************************************************************/

#ifndef __load_state_h__
#define __load_state_h__

// Physical component dependency
#include "igamestate.h"

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CSprite2D;

class CLoadState : public iGameState
{
public:

    // Constructor
    CLoadState( const CStateMessage & stateMsg );

    // Destructor
    virtual ~CLoadState();
    
    // Do any pre-load init's
    void Init() override;

    // Is the state done
    bool DoStateChange() override;

    // Get the next state to load
    NGameDefs::EGameState GetNextState() override;

private:

    // Load during the startup screen
    void Animate();

    // Load the assets
    void ObjectDataLoad();
    void AssetsLoad();
    void CriticalLoad();
    void CriticalInit();
    
private:
    
    // Load animation
    std::unique_ptr<CSprite2D> m_upSprite;
    
    // Flag to indicate the thread load is finished
    bool m_threadActive;
    
    // Animation members
    float m_time;
    int m_frame;
    
    // Exception text
    std::string m_errorTitle;
    std::string m_errorMsg;

};

#endif  // __game_load_state_h__


