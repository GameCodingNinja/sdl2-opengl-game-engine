
/************************************************************************
*    FILE NAME:       sector2d.h
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

#ifndef __sector_2d_h__
#define __sector_2d_h__

// Physical component dependency
#include <2d/object2d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>

// Forward Declarations
class iSprite2D;

class CSector2D : public CObject2D, boost::noncopyable
{
public:

    // Constructor
    CSector2D();

    // Destructor
    virtual ~CSector2D();
    
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
    void DoTransform();
    void DoTransform( const CObject2D & object );

    // Render the actor
    void Render( const CMatrix & matrix );

    // Is the actor in view
    bool InView();
    
private:
    
    // Check if the sector is within the orthographic view frustum
    bool InOrthographicView();
    
    // Check if the sector is within the perspective view frustum
    bool InPerspectiveView();
    
private:
    
    // sprite allocation vector
    std::vector<iSprite2D *> m_pSpriteVec;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Half of the sector size
    float m_sectorSizeHalf;

};

#endif  // __sector_2d_h__

