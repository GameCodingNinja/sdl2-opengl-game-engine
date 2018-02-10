
/************************************************************************
*    FILE NAME:       object3d.cpp
*
*    DESCRIPTION:     object class for 3D
************************************************************************/

// Physical component dependency
#include <3d/object3d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CObject3D::CObject3D()
{
}   // constructor

CObject3D::CObject3D( const CObject3D & obj ) :
    CObject2D( obj ),
    m_rotMatrix( obj.m_rotMatrix )
{
}   // Constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CObject3D::~CObject3D()
{
}   // destructor


/************************************************************************
*    desc:  Apply the rotation
************************************************************************/
void CObject3D::ApplyRotation( CMatrix & matrix )
{
    // Add in the center point prior to rotation
    if( m_parameters.IsSet( NDefs::CENTER_POINT ) )
        matrix.Translate( m_centerPos );
    
    // Add in the rotation if this is NOT a physics transformation
    if( !m_parameters.IsSet( NDefs::PHYSICS_TRANSFORM ) )
    {
        m_rotMatrix.InitilizeMatrix();
        m_rotMatrix.Rotate( m_rot );
    }
    
    // Since the rotation has already been done, multiply it into the matrix
    matrix.Multiply3x3( m_rotMatrix );
    
    // Subtract the center point after rotation to put back in original position
    if( m_parameters.IsSet( NDefs::CENTER_POINT ) )
        matrix.Translate( -m_centerPos );

}   // ApplyRotation


/************************************************************************
*    desc:  Get the object's rotation matrix
************************************************************************/
const CMatrix & CObject3D::GetRotMatrix() const
{
    return m_rotMatrix;

}   // GetMatrix
