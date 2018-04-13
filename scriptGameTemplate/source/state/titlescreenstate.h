
/************************************************************************
*    FILE NAME:       titlescreenstate.h
*
*    DESCRIPTION:     CRunState Class State
************************************************************************/

#ifndef __title_screen_state_h__
#define __title_screen_state_h__

// Physical component dependency
#include "commonstate.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <3d/sprite3d.h>
#include <slot/slotgame.h>
#include <script/scriptcomponent.h>
#include <common/camera.h>

// Standard lib dependencies
#include <tuple>

// Forward declaration(s)
class CUIMeter;

class CTitleScreenState : public CCommonState
{
public:

    // Constructor
    CTitleScreenState();

    // Destructor
    virtual ~CTitleScreenState(){};
    
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
    
private:

    // The script conponent
    CScriptComponent m_scriptComponent;
    
    // title screen background
    CSprite2D m_background;
    
    // Slot game class
    //CSlotGame m_slotGame;
    
    CSprite3D m_cube;
    
    CCamera m_camera;
};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    //static std::tuple<std::string, std::string> reelGrpCfg("data/objects/2d/slot/wheelgroup.cfg", "wheelgroup");
    //static std::tuple<std::string, std::string> spinProfileCfg("data/objects/2d/slot/spinProfile.cfg", "spinProfile");
    
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NTitleScreenState


#endif  // __title_screen_state_h__


