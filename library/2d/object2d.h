/************************************************************************
*    FILE NAME:       object2d.h
*
*    DESCRIPTION:     2D object class
************************************************************************/

#ifndef __object_2d_h__
#define __object_2d_h__

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <utilities/matrix.h>

// Forward declaration(s)
class CMatrix;

class CObject2D : public CObject
{
public:

    CObject2D();
    CObject2D( const CObject2D & obj );
    virtual ~CObject2D();

    // Transform - One call for those objects that don't have parents
    virtual void transform();
    virtual void transform( const CObject2D & object );
    virtual void transform( const CMatrix & matrix, bool tranformWorldPos = false );

    // Get the object's matrix
    const CMatrix & getMatrix() const;

    // Was the world position transformed?
    bool wasWorldPosTranformed() const;

    // Force the world transform
    void forceTransform();
    
    // Get the object's translated position
    const CPoint<float> & getTransPos() const;

protected:

    // Transform the object in local space
    void transformLocal( CMatrix & matrix );
    
    // Apply the scale
    virtual void applyScale( CMatrix & matrix );

    // Apply the rotation
    virtual void applyRotation( CMatrix & matrix );

protected:

    // Translated position
    CPoint<float> m_transPos;

    // local matrix
    CMatrix m_matrix;
};

#endif  // __object_2d_h__
