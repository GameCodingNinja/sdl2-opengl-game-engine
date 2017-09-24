
/************************************************************************
*    FILE NAME:       statemessage.h
*
*    DESCRIPTION:     This class is custom per game project
************************************************************************/

#ifndef __state_message_h__
#define __state_message_h__

// Game dependencies
#include "gamedefs.h"

class CStateMessage
{
public:
    
    NGameDefs::EGameState GetLoadState()
    { return m_loadState; }
    
    NGameDefs::EGameState GetUnloadState()
    { return m_unloadState; }
    
    void SetMsg( NGameDefs::EGameState loadState, NGameDefs::EGameState unloadState )
    {
        m_loadState = loadState;
        m_unloadState = unloadState;
    }
    
private:

    NGameDefs::EGameState m_loadState   : 16;
    NGameDefs::EGameState m_unloadState : 16;
};

#endif  // __state_message_h__


