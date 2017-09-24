
/************************************************************************
*    FILE NAME:       startupstate.h
*
*    DESCRIPTION:     CStartUp Class State
************************************************************************/

#ifndef __start_up_state_h__
#define __start_up_state_h__

// Physical component dependenc
#include "igamestate.h"

// Game lib dependencies
#include <2d/sprite2d.h>

// Standard lib dependencies
#include <deque>

// Forward declaration(s)
class CColor;

class CStartUpState : public iGameState
{
public:

    // Constructor
    CStartUpState();

    // Destructor
    virtual ~CStartUpState();
    
    // Do any pre-load init's
    void Init() override;

    // Is the state done
    bool DoStateChange() override;

private:
    
    // Load the assets
    void AssetLoad();

    // Fade to color
    void Fade(
        float time,
        const CColor & cur,
        const CColor & finalColor );
    
private:
    
    // Deque of sprites
    std::deque<CSprite2D> m_SpriteDeque;

};

#endif  // __start_up_state_h__


