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
}

CObject2D::CObject2D( const CObject2D & obj ) :
    CObject( obj ),
    m_transPos( obj.m_transPos ),
    m_matrix( obj.m_matrix )
{
}


/************************************************************************
*    desc:  Destructor
************************************************************************/
CObject2D::~CObject2D()
{
}


/************************************************************************
*    desc:  Get the object's translated position
************************************************************************/
const CPoint<float> & CObject2D::getTransPos() const
{
    return m_transPos;
}


/************************************************************************
*    desc:  Transform the object in local space
************************************************************************/
void CObject2D::transformLocal( CMatrix & matrix )
{
    // Reset the matrices
    matrix.InitilizeMatrix();

    // Apply the crop offset
    if( m_parameters.IsSet( NDefs::CROP_OFFSET ) )
        matrix.Translate( m_cropOffset );

    // Apply the scale
    if( m_parameters.IsSet( NDefs::SCALE ) )
        applyScale( matrix );

    // Apply the rotation
    if( m_parameters.IsSet( NDefs::ROTATE ) )
        applyRotation( matrix );

    // Apply the translation
    if( m_parameters.IsSet( NDefs::TRANSLATE ) )
        matrix.Translate( m_pos );

    // Clear the check parameter
    m_parameters.Remove( NDefs::TRANSFORM | NDefs::PHYSICS_TRANSFORM );

    // Indicate that translation was done
    m_parameters.Add( NDefs::WAS_TRANSFORMED );
}


/************************************************************************
*    desc:  Transform
************************************************************************/
void CObject2D::transform()
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
    {
        transformLocal( m_matrix );
    
        m_transPos = m_pos;
    }
}

void CObject2D::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) || tranformWorldPos )
    {
        CMatrix localMatrix;
    
        transformLocal( localMatrix );
    
        m_matrix = localMatrix * matrix;

        m_matrix.Transform( m_transPos, CPoint<float>() );
    }
}

void CObject2D::transform( const CObject2D & object )
{
    CObject2D::transform( object.getMatrix(), object.wasWorldPosTranformed() );
}


/************************************************************************
*    desc:  Apply the scale
************************************************************************/
void CObject2D::applyScale( CMatrix & matrix )
{
    matrix.SetScale( m_scale );
}


/************************************************************************
*    desc:  Apply the rotation
************************************************************************/
void CObject2D::applyRotation( CMatrix & matrix )
{
    // Add in the center point prior to rotation
    if( m_parameters.IsSet( NDefs::CENTER_POINT ) )
        matrix.Translate( m_centerPos );

    matrix.Rotate( m_rot );

    // Subtract the center point after rotation to put back in original position
    if( m_parameters.IsSet( NDefs::CENTER_POINT ) )
        matrix.Translate( -m_centerPos );
}


/************************************************************************
*    desc:  Get the object's matrix
************************************************************************/
const CMatrix & CObject2D::getMatrix() const
{
    return m_matrix;
}


/************************************************************************
*    desc:  Was the world position transformed?
************************************************************************/
bool CObject2D::wasWorldPosTranformed() const
{
    return m_parameters.IsSet( NDefs::WAS_TRANSFORMED );
}


/************************************************************************
*    desc:  Force a transform from this point all the way up the line
************************************************************************/
void CObject2D::forceTransform()
{
    m_parameters.Add( NDefs::TRANSFORM );
}
