
/************************************************************************
*    FILE NAME:       physicsworldmanager3d.h
*
*    DESCRIPTION:     Physics manager class singleton
************************************************************************/

#ifndef __physics_world_manager_h__
#define __physics_world_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <map>

// Forward declaration(s)
class CPhysicsWorld3D;

class CPhysicsWorldManager3D : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CPhysicsWorldManager3D & Instance()
    {
        static CPhysicsWorldManager3D worldManager;
        return worldManager;
    }

    // Create the physics world
    void CreateWorld( const std::string & group );
    
    // Destroy the physics world
    void DestroyWorld( const std::string & group );

    // Get the physics world
    CPhysicsWorld3D & GetWorld( const std::string & group );
    
    // Delete all worlds
    void Clear();

private:

    // Constructor
    CPhysicsWorldManager3D();

    // Destructor
    ~CPhysicsWorldManager3D();

    // Load the physics worlds from an XML
    void LoadFromXML( const std::string & group, const std::string & LoadFromXML );

private:

    // Map of physics worlds
    std::map<const std::string, CPhysicsWorld3D> m_pWorld3dMap;

};

#endif  // __physics_world_manager_h__