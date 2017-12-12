
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
    m_parameters.Add( NDefs::ROTATE | NDefs::PHYSICS_TRANSFORM );
    
    // Set the position
    const btVector3 & btVec = trans.getOrigin();
    SetPosXYZ( btVec.x(), btVec.y(), btVec.z() );
    
    // Set the rotation
    const btMatrix3x3 & btMat = trans.getBasis();
    for( int i = 0; i < 3; ++i )
    {
        const btVector3 & vec = btMat.getRow(i);
        m_rotMatrix.SetColumn( i, vec.x(), vec.y(), vec.z() );
    }
    
    // This is an example of how to get the rotation out of bullet physics
    // but it's a lot of extra work to only do the rotation calculation all over again.
    
    // Get the rotation
    //const btMatrix3x3 & btMat = trans.getBasis();
    //btScalar z, y, x;
    //btMat.getEulerYPR( z, y, x );
    
    // Set the rotation
    //SetRotXYZ( x, y, z, false );

}   // SetTransform


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
