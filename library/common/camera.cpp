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
    m_orthoHeightAspectRatio(0.f)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CCamera::~CCamera()
{
}   // destructor


/************************************************************************
*    desc:  Generate a custom orthographic projection for this camera
************************************************************************/  
void CCamera::GenerateOrthographicProjection( float scale )
{
    const auto defSize = CSettings::Instance().GetDefaultSize();
    
    // Calc the new width and height
    m_orthoProjSize.Set( defSize.GetW() * scale, defSize.GetH() * scale );
    m_orthoProjSizeHalf = m_orthoProjSize / 2.f;
    
    // Calc the new height aspect ratio
    m_orthoHeightAspectRatio = CSettings::Instance().GetSize().h / m_orthoProjSize.h;

    m_projectionMatrix.OrthographicRH(
        m_orthoProjSize.w,
        m_orthoProjSize.h,
        CSettings::Instance().GetMinZdist(),
        CSettings::Instance().GetMaxZdist() );
    
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

}   // SetPosXYZ


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

}   // IncPosXYZ
