
/************************************************************************
*    FILE NAME:       basicstagestrategy3d.h
*
*    DESCRIPTION:     Basic 3D stage strategy
************************************************************************/

#ifndef __basic_stage_strategy_3d_h__
#define __basic_stage_strategy_3d_h__

// Physical component dependency
#include <common/ispritestrategy.h>

// Game lib dependencies
#include <3d/sector3d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

//add "basestrategy" class

// Standard lib dependencies
#include <vector>
#include <deque>

class CBasicStageStrategy3D : public iSpriteStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicStageStrategy3D();

    // Destructor
    virtual ~CBasicStageStrategy3D();

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
    //virtual void Transform( const class CObject3D & object ) override;

    // Render the actors
    virtual void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ) override;
    
    // Get the default camera position
    CObject & GetDefaultCameraPos();
    
protected:
    
    // Load the sector data from node
    virtual void LoadFromNode( const struct XMLNode & node );
    
protected:

    // Deque of the sector 3d sprites
    std::deque<CSector3D> m_sectorDeq;
    
    // default camera position
    CObject m_defaultCameraPos;
};

#endif  // __basic_stage_strategy_3d_h__
