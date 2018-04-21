
/************************************************************************
*    FILE NAME:       cameramanager.h
*
*    DESCRIPTION:     camera manager class singleton
************************************************************************/  

#ifndef __camera_manager_h__
#define __camera_manager_h__

// Game lib dependencies
#include <common/camera.h>

// Standard lib dependencies
#include <string>
#include <map>

class CCameraMgr
{
public:

    // Get the instance of the singleton class
    static CCameraMgr & Instance()
    {
        static CCameraMgr cameraMgr;
        return cameraMgr;
    }
    
    // Create the projection matrixes
    void CreateProjMatrix();
    
    // Get the default projection matrix
    const CMatrix & GetDefaultProjMatrix() const;
    
    // Get the camera final matrix
    const CMatrix & GetCameraMatrix( const std::string & id ) const;
    
    // Get the projection matrix
    const CMatrix & GetProjectionMatrix( NDefs::EProjectionType type ) const;
    
    // Create an orthographic camera
    CCamera & CreateOrthographic( const std::string & id, float minZDist = 5, float maxZDist = 1000, float scale = 1.f );
    
    // Create a perspective camera
    CCamera & CreatePerspective( const std::string & id, float angle = 45, float minZDist = 5, float maxZDist = 1000, float scale = 1.f );
    
    // Set/Get the active camera
    void SetActiveCamera( const std::string & id );
    CCamera & GetActiveCamera();
    
    // Get the camera
    CCamera & GetCamera( const std::string & id );
    const CCamera & GetCamera( const std::string & id ) const;
    
    // Set the active camera position
    void SetActiveCameraPos( const CPoint<CWorldValue> & position );
    void SetActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z );
    void SetActiveCameraPos( const CPoint<float> & position );
    void SetActiveCameraPos( float x = 0, float y = 0, float z = 0 );
    
    // Inc the active camera position
    void IncActiveCameraPos( const CPoint<CWorldValue> & position );
    void IncActiveCameraPos( CWorldValue x, CWorldValue y, CWorldValue z );
    void IncActiveCameraPos( const CPoint<float> & position );
    void IncActiveCameraPos( float x = 0, float y = 0, float z = 0 );
    
    // Set the active camera rotation
    void SetActiveCameraRot( const CPoint<float> & position );
    void SetActiveCameraRot( float x = 0, float y = 0, float z = 0 );
    
    // Inc the active camera rotation
    void IncActiveCameraRot( const CPoint<float> & position );
    void IncActiveCameraRot( float x = 0, float y = 0, float z = 0 );
    
    // Set the active camera scale
    void SetActiveCameraScale( const CPoint<float> & position );
    void SetActiveCameraScale( float x = 1, float y = 1, float z = 1 );
    
    // Inc the active camera scale
    void IncActiveCameraScale( const CPoint<float> & position );
    void IncActiveCameraScale( float x = 1, float y = 1, float z = 1 );
    
    // Get the final matrix
    const CMatrix & GetActiveCameraMatrix() const;
    
    // Transform all the cameras
    void Transform();
    void TransformCamera( const std::string & id );

private:

    CCameraMgr();
    ~CCameraMgr();

private:

    // map list of cameras
    std::map<const std::string, CCamera> m_cameraMap;
    
    // Default Projection matrixes
    CMatrix m_perspectiveMatrix;
    CMatrix m_orthographicMatrix;
    
    // Default camera
    CCamera m_defaultCamera;
    
    // Active camera pointer
    CCamera * m_pActiveCamera;
};

#endif  // __camera_manager_h__
