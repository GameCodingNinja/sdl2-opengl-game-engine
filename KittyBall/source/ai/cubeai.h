
/************************************************************************
*    FILE NAME:       cubeai.h
*
*    DESCRIPTION:     Class cube AI
************************************************************************/

#ifndef __cube_ai_h__
#define __cube_ai_h__

// Physical component dependency
#include <2d/iaibase2d.h>

// Standard lib dependencies
#include <random>

// Forward declaration(s)
class iSprite3D;
class CSprite3D;

class CCubeAI : public iAIBase2D
{
public:

    // Constructor
    CCubeAI( iSprite3D * pSprite );

    // Destructor
    virtual ~CCubeAI();
    
    // Do any initializing
    void Init() override;
    
    // Update animations, move sprites, etc.
    void Update() override;
    
private:
    
    // Init the ball placement
    void InitPhysics();
    
private:
    
    // Sprite reference
    CSprite3D & m_sprite;
    
    // For ball random position generation
    std::default_random_engine m_generator;
};

#endif  // __cube_ai_h__


