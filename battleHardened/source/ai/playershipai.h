
/************************************************************************
*    FILE NAME:       playershipai.h
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

#ifndef __player_ship_ai_h__
#define __player_ship_ai_h__

// Physical component dependency
#include <2d/iaibase2d.h>

// Game lib dependencies
#include <common/point.h>
#include <utilities/timer.h>

// Forward declaration(s)
class CActorSprite2D;
class CSprite2D;
class iSprite2D;

class CPlayerShipAI : public iAIBase2D
{
public:

    // Constructor
    CPlayerShipAI( iSprite2D * pSprite );

    // Destructor
    virtual ~CPlayerShipAI();
    
    // Do any initializing
    void Init() override;
    
    // Handle player related messages
    void HandleEvent( const SDL_Event & rEvent ) override;
    
    // Update animations, move sprites, etc.
    void Update() override;
    
private:
    
    // Handle the ship's shooting 
    void HandleShooting();
    
private:
    
    // Type specific sprite
    CActorSprite2D & m_sprite;
    
    // Fire tail sprite
    CSprite2D & m_fireTailSprite;
    
    // Gun sprite
    CSprite2D & m_gunSprite;
    
    // Gun rotation in radians
    float m_gunRotation;
    
    // Gun xy value to calculate rotation
    CPoint<float> m_gunXY;
    
    // Player shoot timer
    CTimer m_playerShootTimer;
};

#endif  // __player_ship_ai_h__


