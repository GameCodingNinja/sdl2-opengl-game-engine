
/************************************************************************
*    FILE NAME:       bigcashbackstate.h
*
*    DESCRIPTION:     The Big Cash Back game state
************************************************************************/

#ifndef __big_cash_back_state_h__
#define __big_cash_back_state_h__

// Physical component dependency
#include "commonstate.h"

// Game dependencies
#include "../game/frontpanel.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <3d/sprite3d.h>
#include <slot/slotgame.h>
#include <slot/basegamemusic.h>
#include <script/scriptcomponent.h>

// Standard lib dependencies
#include <string>
#include <mutex>
#include <tuple>

class CBigCashBackState : public CCommonState
{
public:

    // Constructor
    CBigCashBackState( const std::string & group );

    // Destructor
    virtual ~CBigCashBackState(){};
    
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
    
    // Do we allow the stop sounds?
    void AllowStopSounds( bool allow ) override;
    
private:
    
    void TotalBetCallBack(CUIControl *);
    
private:
    
    // Group name
    const std::string m_stateGroup;
    
    // The script conponent
    CScriptComponent m_scriptComponent;

    // Slot game class
    CSlotGame m_slotGame;
    
    // title screen background
    CSprite2D m_background;

    CSprite2D m_pig;
    
    // Front panel
    CFrontPanel m_frontPanel;
    
    // Base game music
    CBaseGameMusic m_baseGameMusic;

};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NBigCashBack
{
    static std::tuple<std::string, std::string> reelGrpCfg("data/objects/2d/slot/games/bigCashBack/reelgroup.cfg", "reelgroup");
    static std::tuple<std::string, std::string> spinProfileCfg("data/objects/2d/slot/games/bigCashBack/spinProfile.cfg", "spinProfile");
    
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NBigCashBack

#endif  // __big_cash_back_state_h__


