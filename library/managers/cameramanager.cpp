
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
    createProjMatrix();

}


/************************************************************************
*    desc:  Destructor
************************************************************************/
CCameraMgr::~CCameraMgr()
{
}


/************************************************************************
*    desc:  Create or recreate the projection matrixes for resolution change
************************************************************************/
void CCameraMgr::createProjMatrix()
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
}


/************************************************************************
*    desc:  Get the default projection matrix
************************************************************************/
const CMatrix & CCameraMgr::getDefaultProjMatrix() const
{
    if( CSettings::Instance().GetProjectionType() == NDefs::EPT_ORTHOGRAPHIC )
        return m_orthographicMatrix;
    else
        return m_perspectiveMatrix;
}


/************************************************************************
*    desc:  Get the projection matrix
************************************************************************/
const CMatrix & CCameraMgr::getProjectionMatrix( NDefs::EProjectionType type ) const
{
    if( type == NDefs::EPT_PERSPECTIVE )
        return m_perspectiveMatrix;
    else
        return m_orthographicMatrix;
}


/************************************************************************
*    desc:  Create an orthographic camera
************************************************************************/
CCamera & CCameraMgr::createOrthographic( const std::string & id, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(minZDist, maxZDist, scale) );

    m_pActiveCamera = &iter.first->second;

    return iter.first->second;
}


/************************************************************************
*    desc:  Create a perspective camera
************************************************************************/
CCamera & CCameraMgr::createPerspective( const std::string & id, float angle, float minZDist, float maxZDist, float scale )
{
    auto iter = m_cameraMap.emplace(
        std::piecewise_construct, std::forward_as_tuple(id), std::forward_as_tuple(angle, minZDist, maxZDist, scale) );

    m_pActiveCamera = &iter.first->second;

    return iter.first->second;
}


/************************************************************************
*    desc:  Set/Get the active camera
************************************************************************/
void CCameraMgr::setActiveCamera( const std::string & id )
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        m_pActiveCamera = &iter->second;
    else
        NGenFunc::PostDebugMsg( boost::str( boost::format("Camera id does not exist (%s).") % id ) );
}

CCamera & CCameraMgr::getActiveCamera()
{
    return *m_pActiveCamera;
}


/************************************************************************
*    desc:  Get the camera
************************************************************************/
CCamera & CCameraMgr::getCamera( const std::string & id )
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        return iter->second;

    return m_defaultCamera;
}

const CCamera & CCameraMgr::getCamera( const std::string & id ) const
{
    auto iter = m_cameraMap.find( id );
    if( iter != m_cameraMap.end() )
        return iter->second;

    return m_defaultCamera;
}


/************************************************************************
*    desc:  Set the active camera position
************************************************************************/
void CCameraMgr::setActiveCameraPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(position);
}

void CCameraMgr::setActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(x,y,z);
}

void CCameraMgr::setActiveCameraPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(position);
}

void CCameraMgr::setActiveCameraPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setPos(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera position
************************************************************************/
void CCameraMgr::incActiveCameraPos( const CPoint<CWorldValue> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(position);
}

void CCameraMgr::incActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(x,y,z);
}

void CCameraMgr::incActiveCameraPos( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(position);
}

void CCameraMgr::incActiveCameraPos( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incPos(x,y,z);
}


/************************************************************************
*    desc:  Set the active camera rotation
************************************************************************/
void CCameraMgr::setActiveCameraRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setRot(position);
}

void CCameraMgr::setActiveCameraRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setRot(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera rotation
************************************************************************/
void CCameraMgr::incActiveCameraRot( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incRot(position);
}

void CCameraMgr::incActiveCameraRot( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incRot(x,y,z);
}


/************************************************************************
*    desc:  Set the active camera scale
************************************************************************/
void CCameraMgr::setActiveCameraScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setScale(position);
}

void CCameraMgr::setActiveCameraScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->setScale(x,y,z);
}


/************************************************************************
*    desc:  Inc the active camera scale
************************************************************************/
void CCameraMgr::incActiveCameraScale( const CPoint<float> & position )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incScale(position);
}

void CCameraMgr::incActiveCameraScale( float x, float y, float z )
{
    if( m_pActiveCamera != nullptr )
        m_pActiveCamera->incScale(x,y,z);
}


/************************************************************************
*    desc:  Get the final matrix
************************************************************************/
const CMatrix & CCameraMgr::getActiveCameraMatrix() const
{
    if( m_pActiveCamera != nullptr )
        return m_pActiveCamera->getFinalMatrix();

    return getDefaultProjMatrix();
}



/************************************************************************
*    desc:  Get the camera final matrix
************************************************************************/
const CMatrix & CCameraMgr::getCameraMatrix( const std::string & id ) const
{
    if( !id.empty() )
    {
        auto iter = m_cameraMap.find( id );
        if( iter != m_cameraMap.end() )
            return iter->second.getFinalMatrix();

        NGenFunc::PostDebugMsg( boost::str( boost::format("Camera id does not exist (%s).") % id ) );
    }

    return getDefaultProjMatrix();
}


/************************************************************************
*    desc:  Transform the all cameras
************************************************************************/
void CCameraMgr::transform()
{
    for( auto & iter : m_cameraMap )
        iter.second.transform();
}


/************************************************************************
*    desc:  Transform the camera
************************************************************************/
void CCameraMgr::transformCamera( const std::string & id )
{
    if( !id.empty() )
    {
        auto iter = m_cameraMap.find( id );
        if( iter != m_cameraMap.end() )
            return iter->second.transform();
    }
}
