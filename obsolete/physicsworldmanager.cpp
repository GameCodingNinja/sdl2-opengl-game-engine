
/************************************************************************
*    FILE NAME:       physicsworldmanager.cpp
*
*    DESCRIPTION:     Physics manager class singleton
************************************************************************/

// Physical component dependency
#include <physics/physicsworldmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <physics/physicsworld2d.h>
#include <physics/physicsworld3d.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPhysicsWorldManager::CPhysicsWorldManager()
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CPhysicsWorldManager::~CPhysicsWorldManager()
{
}	// destructor


/************************************************************************
*    desc:  Create the physics world
************************************************************************/
void CPhysicsWorldManager::CreateWorld2D( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Load Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s") 
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_pWorld2dMap.find( group ) == m_pWorld2dMap.end() )
    {
        LoadFromXML2D( group, listTableIter->second.back() );
    }
    else
    {
        throw NExcept::CCriticalException("Physics world 2D load Error!",
            boost::str( boost::format("Physics world 2D has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // CreateWorld2D

void CPhysicsWorldManager::CreateWorld3D( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 3D Load Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s") 
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_pWorld3dMap.find( group ) == m_pWorld3dMap.end() )
    {
        LoadFromXML3D( group, listTableIter->second.back() );
    }
    else
    {
        throw NExcept::CCriticalException("Physics world 3D load Error!",
            boost::str( boost::format("Physics world 3D has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // CreateWorld2D


/************************************************************************
*    desc:  Load the physics worlds from an XML
*
*	 param:	const string & filePath - file to load
************************************************************************/
void CPhysicsWorldManager::LoadFromXML2D( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "physics2d" );

    // Create the world and add it to the map
    auto iter = m_pWorld2dMap.emplace( std::piecewise_construct, std::forward_as_tuple(group), std::forward_as_tuple() );
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Physics World 2D Manager Error!",
            boost::str( boost::format("World name has already been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    // Have the physics world load the rest of its data
    iter.first->second.LoadFromNode( mainNode );

}   // LoadFromXML

void CPhysicsWorldManager::LoadFromXML3D( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "physics3d" );

    // Create the world and add it to the map
    auto iter = m_pWorld3dMap.emplace( std::piecewise_construct, std::forward_as_tuple(group), std::forward_as_tuple() );
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Physics World 3D Manager Error!",
            boost::str( boost::format("World name has already been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    // Have the physics world load the rest of its data
    iter.first->second.LoadFromNode( mainNode );

}   // LoadFromXML


/************************************************************************
*    desc:  Get the physics world
************************************************************************/
CPhysicsWorld2D & CPhysicsWorldManager::GetWorld2D( const std::string & group )
{
    // Find the world
    auto iter = m_pWorld2dMap.find( group );

    // If we found it, return it. If not, we have a problem
    if( iter == m_pWorld2dMap.end() )
        throw NExcept::CCriticalException("Physics World 2D Manager Error!",
            boost::str( boost::format("Physics World 2D doesn't exist (%s).\n\n%s\nLine: %s") 
                % group.c_str() % __FUNCTION__ % __LINE__ ));

    return iter->second;

}   // GetWorld

CPhysicsWorld3D & CPhysicsWorldManager::GetWorld3D( const std::string & group )
{
    // Find the world
    auto iter = m_pWorld3dMap.find( group );

    // If we found it, return it. If not, we have a problem
    if( iter == m_pWorld3dMap.end() )
        throw NExcept::CCriticalException("Physics World 3D Manager Error!",
            boost::str( boost::format("Physics World 3D  doesn't exist (%s).\n\n%s\nLine: %s") 
                % group.c_str() % __FUNCTION__ % __LINE__ ));

    return iter->second;

}   // GetWorld


/************************************************************************
*    desc:  Destroy the physics world
************************************************************************/
void CPhysicsWorldManager::DestroyWorld2D( const std::string & group )
{
    // Find the world
    auto iter = m_pWorld2dMap.find( group );

    // If we find it, delete it.
    if( iter != m_pWorld2dMap.end() )
        m_pWorld2dMap.erase( iter );

}   // DestroyWorld2D

void CPhysicsWorldManager::DestroyWorld3D( const std::string & group )
{
    // Find the world
    auto iter = m_pWorld3dMap.find( group );

    // If we find it, delete it. If not, we have a problem
    if( iter != m_pWorld3dMap.end() )
        m_pWorld3dMap.erase( iter );

}   // DestroyWorld3D


/************************************************************************
*    desc:  Delete all worlds
************************************************************************/
void CPhysicsWorldManager::Clear()
{
    m_pWorld2dMap.clear();
    m_pWorld3dMap.clear();
}
