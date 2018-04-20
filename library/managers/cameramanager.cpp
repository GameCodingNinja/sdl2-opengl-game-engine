
/************************************************************************
*    FILE NAME:       cameramanager.cpp
*
*    DESCRIPTION:     camera manager class singleton
************************************************************************/

// Physical component dependency
#include <managers/cameramanager.h>

// Game lib dependencies
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CCameraMgr::CCameraMgr() :
    m_pActiveCamera(nullptr)
{
    CreateProjMatrix();
    
}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CCameraMgr::~CCameraMgr()
{
}   // Destructor


/************************************************************************
*    desc:  Create or recreate the projection matrixes for resolution change
************************************************************************/
void CCameraMgr::CreateProjMatrix()
{
    m_perspectiveMatrix.PerspectiveFovRH(
        CSettings::Instance().GetViewAngle(),
        CSettings::Instance().GetScreenAspectRatio().w,
        CSettings::Instance().GetMinZdist(),
        CSettings::Instance().GetMaxZdist() );

    m_orthographicMatrix.OrthographicRH(
        CSettings::Instance().GetDefaultSize().GetW(),
        CSettings::Instance().GetDefaultSize().GetH(),
        CSettings::Instance().GetMinZdist(),
        CSettings::Instance().GetMaxZdist() );
    
    for( auto & iter : m_cameraMap )
        iter.second.RecreateProjMatrix();
    
}   // CreateProjMatrix


/************************************************************************
*    desc:  Get the default projection matrix
************************************************************************/
const CMatrix & CCameraMgr::GetDefaultProjMatrix() const
{
    if( CSettings::Instance().GetProjectionType() == NDefs::EPT_ORTHOGRAPHIC )
        return m_orthographicMatrix;
    else
        return m_perspectiveMatrix;

}   // GetDefaultProjMatrix


/************************************************************************
*    desc:  Get the projection matrix
************************************************************************/
const CMatrix & CCameraMgr::GetProjectionMatrix( NDefs::EProjectionType type ) const
{
    if( type == NDefs::EPT_PERSPECTIVE )
        return m_perspectiveMatrix;
    else
        return m_orthographicMatrix;

}   // GetProjectionMatrix


/************************************************************************
*    desc:  Add an orthographic camera
************************************************************************/
void CCameraMgr::AddOrthographicCamera( const std::string & id, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(minZDist, maxZDist, scale) );
    
    m_pActiveCamera = &iter.first->second;
    
}   // AddOrthographicCamera


/************************************************************************
*    desc:  Add a perspective camera
************************************************************************/
void CCameraMgr::AddPerspectiveCamera( const std::string & id, float angle, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(angle, minZDist, maxZDist, scale) );
    
    m_pActiveCamera = &iter.first->second;
    
}   // AddPerspectiveCamera


/************************************************************************
*    desc:  Set/Get the active camera
************************************************************************/
void CCameraMgr::SetActiveCamera( const std::string & id )
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        m_pActiveCamera = &iter->second;
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Camera id does not exist (%s).") % id ) );
        
}   // SetActiveCamera

CCamera & CCameraMgr::GetActiveCamera()
{
    return *m_pActiveCamera;
    
} // GetActiveCamera


/************************************************************************
*    desc:  Get the camera
************************************************************************/
CCamera & CCameraMgr::GetCamera( const std::string & id )
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        return iter->second;
    
    throw NExcept::CCriticalException("Get Camera Error!", 
        boost::str( boost::format("Camera id does not exist (%s).\n\n%s\nLine: %s")
            % id % __FUNCTION__ % __LINE__ ));
    
    return m_dummyCamera;
    
}   // GetCamera

const CCamera & CCameraMgr::GetCamera( const std::string & id ) const
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        return iter->second;
    
    throw NExcept::CCriticalException("Get Camera Error!", 
        boost::str( boost::format("Camera id does not exist (%s).\n\n%s\nLine: %s")
            % id % __FUNCTION__ % __LINE__ ));
    
    return m_dummyCamera;
    
}   // GetCamera


/************************************************************************
*    desc:  Set the active camera position
************************************************************************/
void CCameraMgr::SetActiveCameraPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetPos(position);
}

void CCameraMgr::SetActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetPos(x,y,z);
}

void CCameraMgr::SetActiveCameraPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetPos(position);
}

void CCameraMgr::SetActiveCameraPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetPos(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera position
************************************************************************/
void CCameraMgr::IncActiveCameraPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncPos(position);
}

void CCameraMgr::IncActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncPos(x,y,z);
}

void CCameraMgr::IncActiveCameraPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncPos(position);
}

void CCameraMgr::IncActiveCameraPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncPos(x,y,z);
}


/************************************************************************
*    desc:  Set the active camera rotation
************************************************************************/
void CCameraMgr::SetActiveCameraRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetRot(position);
}

void CCameraMgr::SetActiveCameraRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetRot(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera rotation
************************************************************************/
void CCameraMgr::IncActiveCameraRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncRot(position);
}

void CCameraMgr::IncActiveCameraRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncRot(x,y,z);
}


/************************************************************************
*    desc:  Set the active camera scale
************************************************************************/
void CCameraMgr::SetActiveCameraScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetScale(position);
}

void CCameraMgr::SetActiveCameraScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->SetScale(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera scale
************************************************************************/
void CCameraMgr::IncActiveCameraScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncScale(position);
}

void CCameraMgr::IncActiveCameraScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->IncScale(x,y,z);
}


/************************************************************************
*    desc:  Get the final matrix
************************************************************************/  
const CMatrix & CCameraMgr::GetActiveCameraMatrix() const
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->GetFinalMatrix();

    return GetDefaultProjMatrix();
    
}   // GetActiveCameraMatrix



/************************************************************************
*    desc:  Get the camera final matrix
************************************************************************/  
const CMatrix & CCameraMgr::GetCameraMatrix( const std::string & id ) const
{
    if( !id.empty() )
    {
        auto iter = m_cameraMap.find( id );
        if( iter != m_cameraMap.end() )
            return iter->second.GetFinalMatrix();

        NGenFunc::PostDebugMsg( boost::str( boost::format("Camera id does not exist (%s).") % id ) );
    }

    return GetDefaultProjMatrix();
    
}   // GetCameraMatrix


/************************************************************************
*    desc:  Transform the active camera
************************************************************************/  
void CCameraMgr::Transform()
{
    for( auto & iter : m_cameraMap )
        iter.second.Transform();
    
}   // TransformActiveCamera

