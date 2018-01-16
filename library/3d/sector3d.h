
/************************************************************************
*    FILE NAME:       sector3d.h
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

#ifndef __sector_3d_h__
#define __sector_3d_h__

// Physical component dependency
#include <3d/object3d.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>

// Forward Declarations
class iSprite3D;

class CSector3D : public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CSector3D();

    // Destructor
    virtual ~CSector3D();
    
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
    //void Transform( const CMatrix & matrix, bool tranformWorldPos = false ) override;

    // Render the actor
    void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix );

    // Is the actor in view
    bool InView();
    
private:
    
    // Check if the sector is within the orthographic view frustum
    bool InOrthographicView();
    
    // Check if the sector is within the perspective view frustum
    bool InPerspectiveView();
    
private:
    
    // sprite allocation vector
    std::vector<iSprite3D *> m_pSpriteVec;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Half of the sector size
    float m_sectorSizeHalf;

};

#endif  // __sector_3d_h__


