
/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class for 3D
************************************************************************/

#ifndef __object_3d_h__
#define __object_3d_h__

// Physical component dependency
#include <2d/object2d.h>

// Forward declaration(s)
class btTransform;

class CObject3D : public CObject2D
{
public:

    // Constructor
    CObject3D();
    CObject3D( const CObject3D & obj );

    // Destructor
    virtual ~CObject3D();
    
    // Get the object's rotation matrix
    const CMatrix & GetRotMatrix() const;
    
    // Set the translation/rotation from Bullet Physics
    void SetTransform( const btTransform & trans );
    
protected:
    
    // Apply the rotation
    virtual void ApplyRotation( CMatrix & matrix ) override;
    
protected:

    // Matrix for rotations only
    // Basicly used for normal calculations
    CMatrix m_rotMatrix;

};

#endif  // __object_3d_h__
