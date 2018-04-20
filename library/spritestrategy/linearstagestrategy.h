
/************************************************************************
*    FILE NAME:       linearstagestrategy.h
*
*    DESCRIPTION:     Linear stage strategy
*                     Strategy is optimized to move in a linear fashion
************************************************************************/

#ifndef __linear_stage_strategy_h__
#define __linear_stage_strategy_h__

// Physical component dependency
#include <spritestrategy/basicstagestrategy.h>

// Game lib dependencies
#include <spritestrategy/sector.h>

// Standard lib dependencies
#include <vector>
#include <deque>

// Forward declaration(s)
class CObject2D;
class CMatrix;

class CLinearStageStrategy : public CBasicStageStrategy
{
public:

    // Constructor
    CLinearStageStrategy();

    // Destructor
    virtual ~CLinearStageStrategy();

    // Update the actors
    void Update() override;

    // Transform the actor
    void Transform() override;
    void Transform( const CObject2D & object ) override;

    // Render the actors
    void Render() override;
    void Render( const CMatrix & matrix ) override;
    
protected:
    
    // Load thes actor data from node
    virtual void LoadFromNode( const XMLNode & node ) override;
    
    // Init the range of sectors to check
    void InitRange();

private:
    
    // Set the range based on the sector's visibility
    bool SetRange( const size_t index );
    
protected:
    
    enum EStageDirection
    {
        ESD_NULL,
        ESD_FORWARD,
        ESD_BACKWARD,
    };
    
    // Start index
    size_t m_startIndex;
    
    // First sector that is in view
    size_t m_firstIndex;
    
    // Last sector not in view
    size_t m_lastIndex;
    
    // Loop type
    EStageDirection m_dirType;
};

#endif  // __linear_stage_strategy_h__
