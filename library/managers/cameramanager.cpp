
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
        CSettings::Instance().GetDefaultSize().getW(),
        CSettings::Instance().GetDefaultSize().getH(),
        CSettings::Instance().GetMinZdist(),
        CSettings::Instance().GetMaxZdist() );
    
    for( auto & iter : m_cameraMap )
        iter.second.recreateProjMatrix();
    
    // Generate the default camera
    if( CSettings::Instance().GetProjectionType() == NDefs::EPT_ORTHOGRAPHIC )
        m_defaultCamera.generateOrthographicProjection(
            CSettings::Instance().GetMinZdist(),
            CSettings::Instance().GetMaxZdist(),
            CSettings::Instance().GetProjectionScale() );
    else
        m_defaultCamera.generatePerspectiveProjection(
            CSettings::Instance().GetViewAngle(),
            CSettings::Instance().GetMinZdist(),
            CSettings::Instance().GetMaxZdist(),
            CSettings::Instance().GetProjectionScale() );
    
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
*    desc:  Create an orthographic camera
************************************************************************/
CCamera & CCameraMgr::CreateOrthographic( const std::string & id, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(minZDist, maxZDist, scale) );
    
    m_pActiveCamera = &iter.first->second;
    
    return iter.first->second;
    
}   // CreateOrthographic


/************************************************************************
*    desc:  Create a perspective camera
************************************************************************/
CCamera & CCameraMgr::CreatePerspective( const std::string & id, float angle, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(angle, minZDist, maxZDist, scale) );
    
    m_pActiveCamera = &iter.first->second;
    
    return iter.first->second;
    
}   // CreatePerspective


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
    
    return m_defaultCamera;
    
}   // GetCamera

const CCamera & CCameraMgr::GetCamera( const std::string & id ) const
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        return iter->second;
    
    return m_defaultCamera;
    
}   // GetCamera


/************************************************************************
*    desc:  Set the active camera position
************************************************************************/
void CCameraMgr::SetActiveCameraPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(position);
}

void CCameraMgr::SetActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(x,y,z);
}

void CCameraMgr::SetActiveCameraPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(position);
}

void CCameraMgr::SetActiveCameraPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera position
************************************************************************/
void CCameraMgr::IncActiveCameraPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(position);
}

void CCameraMgr::IncActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(x,y,z);
}

void CCameraMgr::IncActiveCameraPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(position);
}

void CCameraMgr::IncActiveCameraPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(x,y,z);
}


/************************************************************************
*    desc:  Set the active camera rotation
************************************************************************/
void CCameraMgr::SetActiveCameraRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setRot(position);
}

void CCameraMgr::SetActiveCameraRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setRot(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera rotation
************************************************************************/
void CCameraMgr::IncActiveCameraRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incRot(position);
}

void CCameraMgr::IncActiveCameraRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incRot(x,y,z);
}


/************************************************************************
*    desc:  Set the active camera scale
************************************************************************/
void CCameraMgr::SetActiveCameraScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setScale(position);
}

void CCameraMgr::SetActiveCameraScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setScale(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera scale
************************************************************************/
void CCameraMgr::IncActiveCameraScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incScale(position);
}

void CCameraMgr::IncActiveCameraScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incScale(x,y,z);
}


/************************************************************************
*    desc:  Get the final matrix
************************************************************************/  
const CMatrix & CCameraMgr::GetActiveCameraMatrix() const
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->getFinalMatrix();

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
            return iter->second.getFinalMatrix();

        NGenFunc::PostDebugMsg( boost::str( boost::format("Camera id does not exist (%s).") % id ) );
    }

    return GetDefaultProjMatrix();
    
}   // GetCameraMatrix


/************************************************************************
*    desc:  Transform the all cameras
************************************************************************/  
void CCameraMgr::Transform()
{
    for( auto & iter : m_cameraMap )
        iter.second.transform();
    
}   // Transform


/************************************************************************
*    desc:  Transform the camera
************************************************************************/  
void CCameraMgr::TransformCamera( const std::string & id )
{
    if( !id.empty() )
    {
        auto iter = m_cameraMap.find( id );
        if( iter != m_cameraMap.end() )
            return iter->second.transform();
    }
    
}   // Transform

