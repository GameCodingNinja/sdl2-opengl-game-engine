
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
#include <script/scriptcomponent.h>

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
};


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    void ObjectDataLoad();
    void CriticalLoad();
    void CriticalUnload();
    void Load();
    void Unload();
    void CriticalInit();
    
}   // NTitleScreenState


#endif  // __title_screen_state_h__


