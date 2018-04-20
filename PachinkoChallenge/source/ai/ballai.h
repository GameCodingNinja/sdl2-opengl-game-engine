
/************************************************************************
*    FILE NAME:       ballai.h
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

#ifndef __ball_ai_h__
#define __ball_ai_h__

// Physical component dependency
#include <common/iaibase.h>

// Game lib dependencies
#include <common/point.h>

// Standard lib dependencies
#include <random>

// Forward declaration(s)
class iSprite;
class CSprite2D;
class CBasicSpriteStrategy;

class CBallAI : public iAIBase
{
public:

    // Constructor
    CBallAI( iSprite * pSprite );

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
    CBasicSpriteStrategy & m_rStrategy;
    
    std::default_random_engine m_generator;
    std::uniform_real_distribution<float> m_angularImpulse;
    std::uniform_real_distribution<float> m_rotation;
};

#endif  // __projectile_ai_h__


