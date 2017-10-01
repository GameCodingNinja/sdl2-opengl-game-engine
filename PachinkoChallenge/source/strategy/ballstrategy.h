
/************************************************************************
*    FILE NAME:       ballstrategy.h
*
*    DESCRIPTION:     Class template
************************************************************************/

#ifndef __ball_strategy_h__
#define __ball_strategy_h__

// Physical component dependency
#include <2d/basicspritestrategy2d.h>

// Standard lib dependencies
#include <random>

class CBallStrategy : public CBasicSpriteStrategy2D
{
public:

    // Constructor
    CBallStrategy( int idOffset );

    // Destructor
    virtual ~CBallStrategy();
    
    // Create the sprite
    int Create(
        const std::string & name,
        const int id,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ) override;
    
private:

    std::default_random_engine m_generator;
    std::uniform_real_distribution<float> m_angularImpulse;
    std::uniform_real_distribution<float> m_rotation;

};

#endif  // __ball_strategy_h__


