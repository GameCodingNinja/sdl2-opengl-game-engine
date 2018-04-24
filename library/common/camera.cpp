/************************************************************************
*    FILE NAME:       camera.cpp
*
*    DESCRIPTION:     Class that holds the camera position and rotation
************************************************************************/

// Physical component dependency
#include <common/camera.h>

// Game lib dependencies
#include <utilities/settings.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CCamera::CCamera() :
    m_orthoHeightAspectRatio(0.f),
    m_projType(NDefs::EPT_NULL),
    m_angle(0),
    m_minZDist(0),
    m_maxZDist(0),
    m_scale(0)
{
}

CCamera::CCamera( float minZDist, float maxZDist, float scale ) :
    m_orthoHeightAspectRatio(0.f)
{
    generateOrthographicProjection( minZDist, maxZDist, scale ); 
}

CCamera::CCamera( float angle, float minZDist, float maxZDist, float scale ) :
    m_orthoHeightAspectRatio(0.f)
{
    generatePerspectiveProjection( angle, minZDist, maxZDist, scale );
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CCamera::~CCamera()
{
}


/************************************************************************
*    desc:  Recreate the projection matrix
************************************************************************/
void CCamera::recreateProjMatrix()
{
    if( m_projType == NDefs::EPT_PERSPECTIVE )
        generatePerspectiveProjection( m_angle, m_minZDist, m_maxZDist, m_scale );
    else
        generateOrthographicProjection( m_minZDist, m_maxZDist, m_scale );
}


/************************************************************************
*    desc:  Generate a custom perspective projection for this camera
************************************************************************/  
void CCamera::generatePerspectiveProjection( float angle, float minZDist, float maxZDist, float scale )
{
    m_projType = NDefs::EPT_PERSPECTIVE;
    m_angle = angle;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    m_scale = scale;
    
    m_projectionMatrix.perspectiveFovRH(
        angle,
        CSettings::Instance().getScreenAspectRatio().w * scale,
        minZDist,
        maxZDist );
    
    calcFinalMatrix();
}


/************************************************************************
*    desc:  Generate a custom orthographic projection for this camera
************************************************************************/  
void CCamera::generateOrthographicProjection( float minZDist, float maxZDist, float scale )
{
    m_projType = NDefs::EPT_ORTHOGRAPHIC;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    m_scale = scale;
    const auto defSize = CSettings::Instance().getDefaultSize();
    
    // Calc the new width and height
    m_orthoProjSize.set( defSize.getW() * scale, defSize.getH() * scale );
    m_orthoProjSizeHalf = m_orthoProjSize / 2.f;
    
    // Calc the new height aspect ratio
    m_orthoHeightAspectRatio = CSettings::Instance().getSize().h / m_orthoProjSize.h;

    m_projectionMatrix.orthographicRH(
        m_orthoProjSize.w,
        m_orthoProjSize.h,
        minZDist,
        maxZDist );
    
    calcFinalMatrix();
}


/************************************************************************
*    desc:  Get the orthographic projected size
************************************************************************/
const CSize<float> & CCamera::getOrthoProjSize() const
{
    return m_orthoProjSize;
}


/************************************************************************
*    desc:  Get the orthographic projected size half
************************************************************************/
const CSize<float> & CCamera::getOrthoProjSizeHalf() const
{
    return m_orthoProjSizeHalf;
}


/************************************************************************
*    desc:  Height and width screen ratio for render 2D objects
*           The difference between screen height and the default height
************************************************************************/
float CCamera::getOrthoHeightAspectRatio() const
{
    return m_orthoHeightAspectRatio; 
}


/************************************************************************
*    desc:  Get the projected matrix
************************************************************************/  
const CMatrix & CCamera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}


/************************************************************************
*    desc:  Set the camera's world position
************************************************************************/  
void CCamera::setPos( const CPoint<CWorldValue> & position )
{
    CObject3D::setPos( -position );
}

void CCamera::setPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObject3D::setPos( -x, -y, -z );
}


/************************************************************************
*    desc:  Increment the camera's world position
************************************************************************/  
void CCamera::incPos( const CPoint<CWorldValue> & position )
{
    CObject3D::incPos( -position );
}

void CCamera::incPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObject3D::incPos( -x, -y, -z );
}


/************************************************************************
*    desc:  Transform
************************************************************************/
void CCamera::transform()
{
    const bool wasTransformed( m_parameters.isSet( NDefs::TRANSFORM ) );
    
    CObject3D::transform();
    
    if( wasTransformed )
        calcFinalMatrix();
}


/************************************************************************
*    desc:  Calculate the final matrix
************************************************************************/
void CCamera::calcFinalMatrix()
{
    m_finalMatrix.initilizeMatrix();
    m_finalMatrix.mergeMatrix( m_matrix );
    m_finalMatrix.mergeMatrix( m_projectionMatrix );
}


/************************************************************************
*    desc:  Get the final matrix
************************************************************************/  
const CMatrix & CCamera::getFinalMatrix() const
{
    return m_finalMatrix;
}
