
/************************************************************************
*    FILE NAME:       basicstagestrategy.h
*
*    DESCRIPTION:     Basic stage strategy
************************************************************************/

#ifndef __basic_stage_strategy_h__
#define __basic_stage_strategy_h__

// Physical component dependency
#include <spritestrategy/ispritestrategy.h>

// Game lib dependencies
#include <spritestrategy/sector.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>
#include <deque>

// Forward Declarations
class CObject2D;
class CMatrix;

class CBasicStageStrategy : public iSpriteStrategy, boost::noncopyable
{
public:

    // Constructor
    CBasicStageStrategy( const std::string & cameraId );
    CBasicStageStrategy();

    // Destructor
    virtual ~CBasicStageStrategy();

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
    virtual void Transform( const CObject2D & object ) override;

    // Render the actors
    void Render() override;
    void Render( const CMatrix & matrix ) override;
    void Render( const CMatrix & matrix, const CMatrix & rotMatrix ) override;
    
    // Find if the sprite exists
    bool Find( iSprite * piSprite );
    
    // Get the default camera position
    CObject & GetDefaultCameraPos();
    
    // Get a reference to the sprite
    template <typename target>
    target & Get( const std::string & spriteName, int index = 0 )
    {
        return *dynamic_cast<target *>(m_sectorDeq.at(index).Get( spriteName ));
    }
    
protected:
    
    // Load the sector data from node
    virtual void LoadFromNode( const struct XMLNode & node );
    
protected:

    // Deque of the sector sprites
    std::deque<CSector> m_sectorDeq;
    
    // default camera position
    CObject m_defaultCameraPos;
};

#endif  // __basic_stage_strategy_h__
