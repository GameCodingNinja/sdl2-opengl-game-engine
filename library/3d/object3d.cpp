
/************************************************************************
*    FILE NAME:       object3d.cpp
*
*    DESCRIPTION:     object class for 3D
************************************************************************/

// Physical component dependency
#include <3d/object3d.h>

// Bullet Physics lib dependencies
#include <btBulletCollisionCommon.h>

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
*    desc:  Set the translation/rotation from Bullet Physics
************************************************************************/
void CObject3D::SetTransform( const btTransform & trans )
{
    //m_parameters.Add( NDefs::MATRIX_ROTATION | NDefs::TRANSFORM );
    m_parameters.Add( NDefs::TRANSFORM );

    // Set the position
    const btVector3 & btVec = trans.getOrigin();
    SetPosXYZ( btVec.x(), btVec.y(), btVec.z() );
    
    // Get the rotation
    const btMatrix3x3 & btMat = trans.getBasis();
    btScalar z, y, x;
    btMat.getEulerYPR( z, y, x );
    
    // Set the rotation
    SetRotXYZ( x, y, z, false );

}   // SetTransform


/************************************************************************
*    desc:  Apply the rotation
************************************************************************/
void CObject3D::ApplyRotation( CMatrix & matrix )
{
    CObject2D::ApplyRotation( matrix );
    
    m_rotMatrix.InitilizeMatrix();
    m_rotMatrix.Rotate( m_rot );

}   // ApplyRotation


/************************************************************************
*    desc:  Get the object's rotation matrix
************************************************************************/
const CMatrix & CObject3D::GetRotMatrix() const
{
    return m_rotMatrix;

}   // GetMatrix
