
/************************************************************************
*    FILE NAME:       lobbystate.h
*
*    DESCRIPTION:     The lobby state
************************************************************************/

#ifndef __lobby_state_h__
#define __lobby_state_h__

// Physical component dependency
#include "commonstate.h"

// Game dependencies
#include "../game/frontpanel.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <3d/sprite3d.h>
#include <slot/slotgame.h>
#include <script/scriptcomponent.h>

// Standard lib dependencies
#include <string>
#include <mutex>
#include <tuple>

// Forward declaration(s)
class CUIMeter;

class CLobbyState : public CCommonState
{
public:

    // Constructor
    CLobbyState( const std::string & group );

    // Destructor
    virtual ~CLobbyState(){};
    
    // Do any pre-game loop init's
    void Init() override;
    
    // Handle events
    void HandleEvent( const SDL_Event & rEvent ) override;
    
    // Handle any misc processing before the real work is started
    void MiscProcess() override;
    
    // Update objects that require them
    void Update() override;

    // Transform the game objects
    void Transform() override;

    // 2D/3D Render of game content
    void PreRender() override;
    
    // Do we allow lobby music?
    void AllowMusic( bool allow ) override;
    
private:
    
    // Group name
    const std::string m_group;
    
    // The script conponent
    CScriptComponent m_scriptComponent;
    
    // title screen background
    CSprite2D m_background;

};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NLobby
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NLobby

#endif  // __lobby_state_h__


