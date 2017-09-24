
/************************************************************************
*    FILE NAME:       loopstagestrategy2d.h
*
*    DESCRIPTION:     Loop 2D stage strategy
*                     Strategy is optimized to move in a looping fassion
************************************************************************/

#ifndef __loop_stage_strategy_2d_h__
#define __loop_stage_strategy_2d_h__

// Physical component dependency
#include <2d/linearstagestrategy2d.h>

// Game lib dependencies
#include <common/worldvalue.h>
#include <common/point.h>
#include <2d/object2d.h>

class CLoopStageStrategy2D : public CLinearStageStrategy2D
{
public:

    // Constructor
    CLoopStageStrategy2D();

    // Destructor
    virtual ~CLoopStageStrategy2D();
    
    // Load thes actor data from node
    void LoadFromNode( const XMLNode & node ) override;
    
    // Transform the actor
    void Transform() override;

private:

    // First sector to start the loop
    CPoint<CWorldValue> m_loopStart;
    
    // Last sector to loop back
    CPoint<CWorldValue> m_loopEnd;
    
    // loop inc
    CPoint<CWorldValue> m_loopInc;
    
    // loop offset
    CObject2D m_loopOffset;
};

#endif  // __loop_stage_strategy_2d_h__
