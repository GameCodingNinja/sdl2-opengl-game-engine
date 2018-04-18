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
}   // constructor

CCamera::CCamera( float minZDist, float maxZDist, float scale ) :
    m_orthoHeightAspectRatio(0.f)
{
    GenerateOrthographicProjection( minZDist, maxZDist, scale ); 
}   // constructor

CCamera::CCamera( float angle, float minZDist, float maxZDist, float scale ) :
    m_orthoHeightAspectRatio(0.f)
{
    GeneratePerspectiveProjection( angle, minZDist, maxZDist, scale );
    
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CCamera::~CCamera()
{
}   // destructor


/************************************************************************
*    desc:  Recreate the projection matrix
************************************************************************/
void CCamera::RecreateProjMatrix()
{
    if( m_projType == NDefs::EPT_PERSPECTIVE )
        GeneratePerspectiveProjection( m_angle, m_minZDist, m_maxZDist, m_scale );
    else
        GenerateOrthographicProjection( m_minZDist, m_maxZDist, m_scale );
    
}   // RecreateProjMatrix


/************************************************************************
*    desc:  Generate a custom perspective projection for this camera
************************************************************************/  
void CCamera::GeneratePerspectiveProjection( float angle, float minZDist, float maxZDist, float scale )
{
    m_projType = NDefs::EPT_PERSPECTIVE;
    m_angle = angle;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    m_scale = scale;
    
    m_projectionMatrix.PerspectiveFovRH(
        angle,
        CSettings::Instance().GetScreenAspectRatio().w * scale,
        minZDist,
        maxZDist );
    
    CalcFinalMatrix();
    
}   // GeneratePerspectiveProjection


/************************************************************************
*    desc:  Generate a custom orthographic projection for this camera
************************************************************************/  
void CCamera::GenerateOrthographicProjection( float minZDist, float maxZDist, float scale )
{
    m_projType = NDefs::EPT_ORTHOGRAPHIC;
    m_minZDist = minZDist;
    m_maxZDist = maxZDist;
    m_scale = scale;
    const auto defSize = CSettings::Instance().GetDefaultSize();
    
    // Calc the new width and height
    m_orthoProjSize.Set( defSize.GetW() * scale, defSize.GetH() * scale );
    m_orthoProjSizeHalf = m_orthoProjSize / 2.f;
    
    // Calc the new height aspect ratio
    m_orthoHeightAspectRatio = CSettings::Instance().GetSize().h / m_orthoProjSize.h;

    m_projectionMatrix.OrthographicRH(
        m_orthoProjSize.w,
        m_orthoProjSize.h,
        minZDist,
        maxZDist );
    
    CalcFinalMatrix();
    
}   // GenerateOrthographicProjection


/************************************************************************
*    desc:  Get the orthographic projected size
************************************************************************/
const CSize<float> & CCamera::GetOrthoProjSize() const
{
    return m_orthoProjSize;
}


/************************************************************************
*    desc:  Get the orthographic projected size half
************************************************************************/
const CSize<float> & CCamera::GetOrthoProjSizeHalf() const
{
    return m_orthoProjSizeHalf;
}


/************************************************************************
*    desc:  Height and width screen ratio for render 2D objects
*           The difference between screen height and the default height
************************************************************************/
float CCamera::GetOrthoHeightAspectRatio() const
{
    return m_orthoHeightAspectRatio;
    
}   // GetOrthoHeightAspectRatio


/************************************************************************
*    desc:  Get the projected matrix
************************************************************************/  
const CMatrix & CCamera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
    
}   // GetProjectionMatrix


/************************************************************************
*    desc:  Set the camera's world position
************************************************************************/  
void CCamera::SetPos( const CPoint<CWorldValue> & position )
{
    CObject::SetPos( -position );

}   // SetPos

void CCamera::SetPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObject::SetPos( -x, -y, -z );

}   // SetPos


/************************************************************************
*    desc:  Increment the camera's world position
************************************************************************/  
void CCamera::IncPos( const CPoint<CWorldValue> & position )
{
    CObject::IncPos( -position );

}   // IncPos

void CCamera::IncPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    CObject::IncPos( -x, -y, -z );

}   // IncPos


/************************************************************************
*    desc:  Transform
************************************************************************/
void CCamera::Transform()
{
    const bool wasTransformed( m_parameters.IsSet( NDefs::TRANSFORM ) );
    
    CObject3D::Transform();
    
    if( wasTransformed )
        CalcFinalMatrix();

}   // Transform


/************************************************************************
*    desc:  Calculate the final matrix
************************************************************************/
void CCamera::CalcFinalMatrix()
{
    m_finalMatrix.InitilizeMatrix();
    m_finalMatrix.MergeMatrix( m_matrix );
    m_finalMatrix.MergeMatrix( m_projectionMatrix );
    
}   // CalcFinalMatrix


/************************************************************************
*    desc:  Get the final matrix
************************************************************************/  
const CMatrix & CCamera::GetFinalMatrix() const
{
    return m_finalMatrix;
    
}   // GetFinalMatrix
