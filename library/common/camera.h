/************************************************************************
*    FILE NAME:       camera.h
*
*    DESCRIPTION:     class that holds the camera position and rotation
************************************************************************/

#ifndef __camera_h__
#define __camera_h__

// Physical component dependency
#include <3d/object3d.h>

// Game lib dependencies
#include <common/defs.h>
#include <utilities/matrix.h>
#include <common/size.h>

class CCamera : public CObject3D
{
public:

    // Constructor
    CCamera();
    
    // Destructor
    virtual ~CCamera();
    
    // Set/Inc the camera's world position
    void SetPos( const CPoint<CWorldValue> & position );
    void IncPos( const CPoint<CWorldValue> & position );
    void SetPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );
    void IncPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );
    
    // Generate a custom orthographic projection for this camera
    void GenerateOrthographicProjection( float scale );
    
    // Get the projected matrix
    const CMatrix & GetProjectionMatrix() const;
    
    // Height and width screen ratio for render 2D objects
    float GetOrthoHeightAspectRatio() const;
    
    // Get the orthographic projected size
    const CSize<float> & GetOrthoProjSize() const;
    
    // Get the orthographic projected size half
    const CSize<float> & GetOrthoProjSizeHalf() const;
  
private:

    // Custom projection matrix
    CMatrix m_projectionMatrix;
    
    // Precalculated aspect ratios for orthographic projection
    float m_orthoHeightAspectRatio;
    
    // Orthographic projection size
    CSize<float> m_orthoProjSize;
    CSize<float> m_orthoProjSizeHalf;

};

#endif  // __camera_h__
