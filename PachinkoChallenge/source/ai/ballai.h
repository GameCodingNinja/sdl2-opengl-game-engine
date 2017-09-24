
/************************************************************************
*    FILE NAME:       ballai.h
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

#ifndef __ball_ai_h__
#define __ball_ai_h__

// Physical component dependency
#include <2d/iaibase2d.h>

// Game lib dependencies
#include <common/point.h>

// Forward declaration(s)
class iSprite2D;
class CSprite2D;
class CBasicSpriteStrategy2D;

class CBallAI : public iAIBase2D
{
public:

    // Constructor
    CBallAI( iSprite2D * pSprite );

    // Destructor
    virtual ~CBallAI();
    
    // Do any initializing
    void Init() override;
    
    // Update animations, move sprites, etc.
    void Update() override;
    
private:
    
    // Sprite reference
    CSprite2D & m_sprite;
    
    // Reference to sprite strategy
    CBasicSpriteStrategy2D & m_rStrategy;
};

#endif  // __projectile_ai_h__

