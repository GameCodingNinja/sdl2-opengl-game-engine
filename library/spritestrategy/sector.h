
/************************************************************************
*    FILE NAME:       sector2d.h
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

#ifndef __sector_h__
#define __sector_h__

// Physical component dependency
#include <3d/object3d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>
#include <map>

// Forward Declarations
class iSprite;
class CCamera;

class CSector : public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CSector();

    // Destructor
    virtual ~CSector();
    
    // Load the sector data from node
    void LoadFromNode( const struct XMLNode & node );
    
    // Do any pre-game loop init's
    void Init();
    
    // Do some cleanup
    void CleanUp();
    
    // Destroy this sector
    void Destroy();

    // Update the actor
    void Update();

    // Transform the actor
    void Transform() override;
    void Transform( const CObject2D & object ) override;

    // Render the actor
    void Render( const CCamera & camera );
    void Render( const CMatrix & matrix );
    void Render( const CMatrix & matrix, const CMatrix & rotMatrix );

    // Is the actor in view
    bool InView();
    
    // Find if the sprite exists
    bool Find( iSprite * piSprite );
    
    // Get the pointer to the sprite
    iSprite * Get( const std::string & spriteName );
    
private:
    
    // Check if the sector is within the orthographic view frustum
    bool InOrthographicView();
    
    // Check if the sector is within the perspective view frustum
    bool InPerspectiveView();
    
private:
    
    // sprite allocation vector
    std::vector<iSprite *> m_pSpriteVec;
    
    // sprites with names
    std::map<const std::string, iSprite *> m_pSpriteMap;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Half of the sector size
    float m_sectorSizeHalf;

};

#endif  // __sector_h__


