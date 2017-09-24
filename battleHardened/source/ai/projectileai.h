
/************************************************************************
*    FILE NAME:       projectileai.h
*
*    DESCRIPTION:     Class projectile AI
************************************************************************/

#ifndef __projectile_ai_h__
#define __projectile_ai_h__

// Physical component dependency
#include <2d/iaibase2d.h>

// Game lib dependencies
#include <common/point.h>

// Forward declaration(s)
class CSprite2D;
class iSprite2D;
class CBasicSpriteStrategy2D;

class CProjectileAI : public iAIBase2D
{
public:

    // Constructor
    CProjectileAI( iSprite2D * pSprite );

    // Destructor
    virtual ~CProjectileAI();
    
    // Do any initalizing
    void Init() override;
    
    // Update animations, move sprites, etc.
    void Update() override;
    
private:
    
    // Type specific sprite
    CSprite2D & m_sprite;
    
    // The velocity and ship momentum of the projectile
    CPoint<float> m_velocity;
    
    // The initial position of the projectile
    CPoint<float> m_initialPos;
    
    // A flag for the first update of this projectile
    bool m_firstUpdate;
    
    // pointer to sprite trategy. We don't won this pointer
    CBasicSpriteStrategy2D & m_rStrategy;

};

#endif  // __projectile_ai_h__


