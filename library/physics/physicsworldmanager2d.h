
/************************************************************************
*    FILE NAME:       physicsworld2d.h
*
*    DESCRIPTION:     Box2D b2World wrapper class singleton
************************************************************************/

#ifndef __physics_world_manager_2d_h__
#define __physics_world_manager_2d_h__

// Physical component dependency
#include <managers/managerbase.h>

// Standard lib dependencies
#include <string>
#include <map>

// Forward declaration(s)
class CPhysicsWorld2D;

class CPhysicsWorldManager2D : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CPhysicsWorldManager2D & Instance()
    {
        static CPhysicsWorldManager2D worldManager;
        return worldManager;
    }

    // Create the physics world
    void CreateWorld( const std::string & group );
    
    // Destroy the physics world
    void DestroyWorld( const std::string & group );

    // Get the physics world
    CPhysicsWorld2D & GetWorld( const std::string & group );
    
    // Delete all worlds
    void Clear();

private:

    // Constructor
    CPhysicsWorldManager2D();

    // Destructor
    ~CPhysicsWorldManager2D();

    // Load the physics worlds from an XML
    void LoadFromXML( const std::string & group, const std::string & LoadFromXML );

private:

    // Map of physics worlds
    std::map<const std::string, CPhysicsWorld2D> m_pWorld2dMap;

};

#endif  // __physics_world_manager_2d_h__