/************************************************************************
*    FILE NAME:       objectdatamanager.h
*
*    DESCRIPTION:     Singlton that holds a map of all 2D/3D object data used 
*					  for later loading
************************************************************************/

#ifndef __object_data_manager_h__
#define __object_data_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Forward declaration(s)
class CObjectData2D;
class CObjectData3D;
class CSpriteData;

class CObjectDataMgr : public CManagerBase
{
public:
    
    // Flag that indicates the load group doesn't create the VBO, IBO, textures, etc
    static const bool DONT_CREATE_FROM_DATA = false;
    static const bool DONT_FREE_OPENGL_OBJECTS = false;

    // Get the instance of the singleton class
    static CObjectDataMgr & Instance()
    {
        static CObjectDataMgr objectDataMgr;
        return objectDataMgr;
    }

    // Get a specific object's data
    const CObjectData2D & GetData2D( const std::string & group, const std::string & name ) const;
    const CObjectData2D & GetData2D( const CSpriteData & spriteData ) const;
    const CObjectData3D & GetData3D( const std::string & group, const std::string & name ) const;

    // Load all of the meshes and materials of a specific data group
    void LoadGroup2D( const std::string & group, const bool createFromData = true );
    void LoadGroup3D( const std::string & group, const bool createFromData = true );
    
    // Create the group's VBO, IBO, textures, etc
    void CreateFromData2D( const std::string & group );
    void CreateFromData3D( const std::string & group );

    // Free all of the meshes and materials of a specific data group
    void FreeGroup2D( const std::string & group, const bool freeOpenGLObjects = true );
    void FreeGroup3D( const std::string & group, const bool freeOpenGLObjects = true );
    
    // Free all OpenGL objects created from these groups
    void FreeOpenGL2D( const std::string & group );
    void FreeOpenGL3D( const std::string & group );

private:

    CObjectDataMgr();
    virtual ~CObjectDataMgr();

    // Load all object information from an xml
    void LoadFromXML2D( const std::string & group, const std::string & filePath, const bool createFromData );
    void LoadFromXML3D( const std::string & group, const std::string & filePath, const bool createFromData );

private:

    // Map in a map of all the objects' data
    std::map<const std::string, std::map<const std::string, CObjectData2D> > m_objectData2DMapMap;
    std::map<const std::string, std::map<const std::string, CObjectData3D> > m_objectData3DMapMap;

};

#endif  // __object_data_list_2d_h__
