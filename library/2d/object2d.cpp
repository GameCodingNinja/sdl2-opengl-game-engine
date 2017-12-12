/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class
************************************************************************/

// Physical component dependency
#include <2d/object2d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CObject2D::CObject2D()
{
}   // Constructor

CObject2D::CObject2D( const CObject2D & obj ) :
    CObject( obj ),
    m_transPos( obj.m_transPos ),
    m_matrix( obj.m_matrix )
{
}   // Constructor


/************************************************************************
*    desc:  Destructor
************************************************************************/
CObject2D::~CObject2D()
{
}   // Destructor


/************************************************************************
*    desc:  Get the object's translated position
************************************************************************/
const CPoint<float> & CObject2D::GetTransPos() const
{
    return m_transPos;

}   // GetTransPos


/************************************************************************
*    desc:  Transform the object in local space
************************************************************************/
void CObject2D::TransformLocal( CMatrix & matrix )
{
    // Reset the matrices
    matrix.InitilizeMatrix();

    // Apply the crop offset
    if( m_parameters.IsSet( NDefs::CROP_OFFSET ) )
        matrix.Translate( m_cropOffset );

    // Apply the scale
    if( m_parameters.IsSet( NDefs::SCALE ) )
        ApplyScale( matrix );

    // Apply the rotation
    if( m_parameters.IsSet( NDefs::ROTATE ) )
        ApplyRotation( matrix );

    // Apply the translation
    if( m_parameters.IsSet( NDefs::TRANSLATE ) )
        matrix.Translate( m_pos );

    // Clear the check parameter
    m_parameters.Remove( NDefs::TRANSFORM | NDefs::PHYSICS_TRANSFORM );

    // Indicate that translation was done
    m_parameters.Add( NDefs::WAS_TRANSFORMED );

}   // TransformLocal


/************************************************************************
*    desc:  Transform
************************************************************************/
void CObject2D::Transform()
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
    {
        TransformLocal( m_matrix );
    
        m_transPos = m_pos;
    }

}   // Transform

void CObject2D::Transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) || tranformWorldPos )
    {
        CMatrix localMatrix;
    
        TransformLocal( localMatrix );
    
        m_matrix = localMatrix * matrix;

        m_matrix.Transform( m_transPos, CPoint<float>() );
    }

}   // Transform

void CObject2D::Transform( const CObject2D & object )
{
    CObject2D::Transform( object.GetMatrix(), object.WasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  Apply the scale
************************************************************************/
void CObject2D::ApplyScale( CMatrix & matrix )
{
    matrix.SetScale( m_scale );

}   // ApplyScale


/************************************************************************
*    desc:  Apply the rotation
************************************************************************/
void CObject2D::ApplyRotation( CMatrix & matrix )
{
    // Add in the center point prior to rotation
    if( m_parameters.IsSet( NDefs::CENTER_POINT ) )
        matrix.Translate( m_centerPos );

    matrix.Rotate( m_rot );

    // Subtract the center point after rotation to put back in original position
    if( m_parameters.IsSet( NDefs::CENTER_POINT ) )
        matrix.Translate( -m_centerPos );

}   // ApplyRotation


/************************************************************************
*    desc:  Get the object's matrix
************************************************************************/
const CMatrix & CObject2D::GetMatrix() const
{
    return m_matrix;

}   // GetMatrix


/************************************************************************
*    desc:  Was the world position transformed?
************************************************************************/
bool CObject2D::WasWorldPosTranformed() const
{
    return m_parameters.IsSet( NDefs::WAS_TRANSFORMED );

}   // WasWorldPosTranformed


/************************************************************************
*    desc:  Force a transform from this point all the way up the line
************************************************************************/
void CObject2D::ForceTransform()
{
    m_parameters.Add( NDefs::TRANSFORM );

}   // ForceTransform
