
/************************************************************************
*    FILE NAME:       basicstagestrategy2d.h
*
*    DESCRIPTION:     Basic 2D stage strategy
************************************************************************/

#ifndef __basic_stage_strategy_2d_h__
#define __basic_stage_strategy_2d_h__

// Physical component dependency
#include <common/ispritestrategy.h>

// Game lib dependencies
#include <2d/sector2d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

//add "basestrategy" class

// Standard lib dependencies
#include <vector>
#include <deque>

class CBasicStageStrategy2D : public iSpriteStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicStageStrategy2D();

    // Destructor
    virtual ~CBasicStageStrategy2D();

    // Load the sector data from file
    virtual void LoadFromFile( const std::string & file ) override;
    
    // Do any pre-game loop init's
    virtual void Init() override;
    
    // Do some cleanup
    virtual void CleanUp() override;

    // Update the actors
    virtual void Update() override;

    // Transform the actor
    virtual void Transform() override;
    virtual void Transform( const class CObject2D & object ) override;

    // Render the actors
    virtual void Render( const class CMatrix & matrix ) override;
    
    // Get the default camera position
    CObject & GetDefaultCameraPos();
    
protected:
    
    // Load the sector data from node
    virtual void LoadFromNode( const struct XMLNode & node );
    
protected:

    // Deque of the sector 2d sprites
    std::deque<CSector2D> m_sectorDeq;
    
    // default camera position
    CObject m_defaultCameraPos;
};

#endif  // __basic_stage_strategy_2d_h__
