/************************************************************************
 *    FILE NAME:       objectdatamanager.cpp
 *
 *    DESCRIPTION:    Singlton that holds a map of all 2D/3D object data used for
 *					 later loading
 ************************************************************************/

// Physical component dependency
#include <objectdata/objectdatamanager.h>

// Game lib dependencies
#include <common/build_defs.h>
#include <common/spritedata.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdata3d.h>
#include <managers/vertexbuffermanager.h>
#include <managers/texturemanager.h>
#include <managers/meshmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
 *    desc:  Constructor                                                             
 ************************************************************************/
CObjectDataMgr::CObjectDataMgr()
{
}   // Constructor


/************************************************************************
 *    desc:  Destructor                                                             
 ************************************************************************/
CObjectDataMgr::~CObjectDataMgr()
{
}   // Destructor


/************************************************************************
 *    desc:  Load all of the meshes and materials of a specific data group
 *
 *    param: string & group - specified group of meshes and materials to load
 ************************************************************************/
void CObjectDataMgr::LoadGroup2D( const std::string & group, const bool createFromData )
{
    // Check for a hardware extension
    std::string ext;
    if( !m_mobileExt.empty() && NBDefs::IsMobileDevice() )
        if( m_listTableMap.find( group + m_mobileExt ) != m_listTableMap.end() )
            ext = m_mobileExt;
    
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group + ext );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Load Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_objectData2DMapMap.find( group ) == m_objectData2DMapMap.end() )
    {
        // Create a new group map inside of our map
        m_objectData2DMapMap.emplace( group, std::map<const std::string, CObjectData2D>() );

        for( auto & iter : listTableIter->second )
            LoadFromXML2D( group, iter, createFromData );
    }
    else
    {
        throw NExcept::CCriticalException("Obj Data List 2D group load Error!",
            boost::str( boost::format("Object data list group has already been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // LoadGroup2D


/************************************************************************
 *    desc:  Load all object information from an xml
 ************************************************************************/
void CObjectDataMgr::LoadFromXML2D( const std::string & group, const std::string & filePath, const bool createFromData )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "objectDataList2D" );

    // Get an iterator to the group
    auto groupMapIter = m_objectData2DMapMap.find( group );
    

    //////////////////////////////////////////////
    // Load the default data
    //////////////////////////////////////////////

    // Get the node to the default object data
    const XMLNode defaultNode = mainNode.getChildNode( "default" );

    // Class to hold a data list's default data.
    CObjectData2D defaultData;

    // If there's no default node then we just use the defaults set in the data classes
    if( !defaultNode.isEmpty() )
        defaultData.LoadFromNode( defaultNode, "", "" );


    //////////////////////////////////////////////
    // Load the object data
    //////////////////////////////////////////////

    XMLNode objectListNode = mainNode.getChildNode( "objectList" );

    for( int i = 0; i < objectListNode.nChildNode(); ++i )
    {
        // Get the object data node
        const XMLNode objectNode = objectListNode.getChildNode(i);

        // Get the object's name
        const std::string name = objectNode.getAttribute( "name" );

        // Allocate the object data to the map
        auto iter = groupMapIter->second.emplace( name, defaultData );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Object Data Load Group Error!",
                boost::str( boost::format("Duplicate object name (%s - %s).\n\n%s\nLine: %s")
                    % name % group % __FUNCTION__ % __LINE__ ));
        }

        // Load in the object data
        iter.first->second.LoadFromNode( objectNode, group, name );
        
        // Create it from the data
        if( createFromData )
            iter.first->second.CreateFromData( group );
    }

}   // LoadFromXML2D


/************************************************************************
 *    desc:  Create the group's VBO, IBO, textures, etc
 ************************************************************************/
void CObjectDataMgr::CreateFromData2D( const std::string & group )
{
    // Create it from the data
    auto groupMapIter = m_objectData2DMapMap.find( group );
    if( groupMapIter != m_objectData2DMapMap.end() )
    {
        for( auto & iter : groupMapIter->second )
            iter.second.CreateFromData( group );
    }
    else
    {
        throw NExcept::CCriticalException("Object Create From Data Group Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // CreateFromData2D


/************************************************************************
 *    desc:  Free all of the meshes materials and data of a specific group
 ************************************************************************/
void CObjectDataMgr::FreeGroup2D( const std::string & group, const bool freeOpenGLObjects )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Free Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // See if this group is still loaded
    auto groupMapIter = m_objectData2DMapMap.find( group );
    if( groupMapIter != m_objectData2DMapMap.end() )
    {
        if( freeOpenGLObjects )
            FreeOpenGL2D( group );

        // Unload the group data
        m_objectData2DMapMap.erase( groupMapIter );
    }

}   // FreeGroup2D


/************************************************************************
 *    desc:  Free all OpenGL objects created from these groups
 ************************************************************************/
void CObjectDataMgr::FreeOpenGL2D( const std::string & group )
{
    CTextureMgr::Instance().DeleteTextureGroupFor2D( group );
    CVertBufMgr::Instance().DeleteBufferGroupFor2D( group );

}   // FreeOpenGL2D


/************************************************************************
 *    desc:  Get a specific 2D object's data
 *
 *    param: string & group - the group the 2D object belongs to
 *			string & name  - name of the 2D object
 *
 *	 ret:	CObjectData2D & - 2D object
 ************************************************************************/
const CObjectData2D & CObjectDataMgr::GetData2D( const std::string & group, const std::string & name ) const
{
    auto groupMapIter = m_objectData2DMapMap.find( group );
    if( groupMapIter == m_objectData2DMapMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Get Data Error!",
            boost::str( boost::format("Object data list group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    auto dataMapIter = groupMapIter->second.find( name );
    if( dataMapIter == groupMapIter->second.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Get Data Error!",
            boost::str( boost::format("Object data list name can't be found (%s).\n\n%s\nLine: (%s - %s)")
                % group % name % __FUNCTION__ % __LINE__ ));

    return dataMapIter->second;

}   // GetData2D

const CObjectData2D & CObjectDataMgr::GetData2D( const CSpriteData & spriteData ) const
{
    return GetData2D( spriteData.GetGroup(), spriteData.GetObjectName() );
    
}   // GetData2D


/************************************************************************
 *    desc:  Load all of the meshes and materials of a specific data group
 *
 *    param: string & group - specified group of meshes and materials to load
 ************************************************************************/
void CObjectDataMgr::LoadGroup3D( const std::string & group, const bool createFromData )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Load Group Data Error!",
            boost::str( boost::format( "Object data list group name can't be found (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );

    // Load the group data if it doesn't already exist
    if( m_objectData3DMapMap.find( group ) == m_objectData3DMapMap.end() )
    {
        // Create a new group map inside of our map
        m_objectData3DMapMap.emplace( group, std::map<const std::string, CObjectData3D>() );

        for( auto & iter : listTableIter->second )
            LoadFromXML3D( group, iter, createFromData );
    }
    else
    {
        throw NExcept::CCriticalException( "Obj Data List 3D load Error!",
            boost::str( boost::format( "Object data list group has alread been loaded (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );
    }

}   // LoadGroup3D


/************************************************************************
 *    desc:  Create the group's VBO, IBO, textures, etc
 ************************************************************************/
void CObjectDataMgr::CreateFromData3D( const std::string & group )
{
    // Create it from the data
    auto groupMapIter = m_objectData3DMapMap.find( group );
    if( groupMapIter != m_objectData3DMapMap.end() )
    {
        for( auto & iter : groupMapIter->second )
            iter.second.CreateFromData( group );
    }
    else
    {
        throw NExcept::CCriticalException("Object Create From Data Group Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // CreateFromData3D


/************************************************************************
 *    desc:  Load all object information from an xml
 *
 *	 param:	string & filePath - file path of the object data list XML
 ************************************************************************/
void CObjectDataMgr::LoadFromXML3D( const std::string & group, const std::string & filePath, const bool createFromData )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "objectDataList3D" );

    // Get an iterator to the group
    auto groupMapIter = m_objectData3DMapMap.find( group );

    //////////////////////////////////////////////
    // Load the default data
    //////////////////////////////////////////////

    // Get the node to the default object data
    const XMLNode defaultNode = mainNode.getChildNode( "default" );

    // Class to hold a data list's default data.
    CObjectData3D defaultData;

    // If there's no default node then we just use the defaults set in the data classes
    if( !defaultNode.isEmpty() )
        defaultData.LoadFromNode( defaultNode, "", "" );


    //////////////////////////////////////////////
    // Load the object data
    //////////////////////////////////////////////

    XMLNode objectListNode = mainNode.getChildNode( "objectList" );

    for( int i = 0; i < objectListNode.nChildNode(); ++i )
    {
        // Get the object data node
        const XMLNode objectNode = objectListNode.getChildNode( i );

        // Get the object's name
        const std::string name = objectNode.getAttribute( "name" );

        // Allocate the object data to the map
        auto iter = groupMapIter->second.emplace( name, defaultData );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException( "Object Data Load Group Error!",
                boost::str( boost::format( "Duplicate object name (%s - %s).\n\n%s\nLine: %s" )
                    % name % group % __FUNCTION__ % __LINE__ ) );
        }

        // Load in the object data
        iter.first->second.LoadFromNode( objectNode, group, name );

        // Create it from the data
        if( createFromData )
            iter.first->second.CreateFromData( group );
    }

}   // LoadFromXML3D


/************************************************************************
 *    desc:  Free all of the meshes and materials of a specific data group
 *
 *    param: string & group - specified group of meshes and materials to load
 ************************************************************************/
void CObjectDataMgr::FreeGroup3D( const std::string & group, const bool freeOpenGLObjects )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
    {
        throw NExcept::CCriticalException( "Obj Data List 3D Load Group Data Error!",
            boost::str( boost::format( "Object data list group name can't be found (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );
    }

    // See if this group is still loaded
    auto groupMapIter = m_objectData3DMapMap.find( group );
    if( groupMapIter != m_objectData3DMapMap.end() )
    {
        if( freeOpenGLObjects )
            FreeOpenGL3D( group );

        // Unload the group data
        m_objectData3DMapMap.erase( groupMapIter );
    }

}   // FreeGroup3D


/************************************************************************
 *    desc:  Free all OpenGL objects created from these groups
 ************************************************************************/
void CObjectDataMgr::FreeOpenGL3D( const std::string & group )
{
    CTextureMgr::Instance().DeleteTextureGroupFor3D( group );
    CMeshMgr::Instance().DeleteBufferGroup( group );

}   // FreeOpenGL3D


/************************************************************************
 *    desc:  Get a specific 3D object's data
 *
 *    param: string & group - the group the 3D object belongs to
 *			string & name  - name of the 3D object
 *
 *	 ret:	CObjectData3D & - 3D object
 ************************************************************************/
const CObjectData3D & CObjectDataMgr::GetData3D( const std::string & group, const std::string & name ) const
{
    auto groupMapIter = m_objectData3DMapMap.find( group );
    if( groupMapIter == m_objectData3DMapMap.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Get Data Error!",
            boost::str( boost::format( "Object data list group can't be found (%s).\n\n%s\nLine: %s" )
                % group % __FUNCTION__ % __LINE__ ) );

    auto dataMapIter = groupMapIter->second.find( name );
    if( dataMapIter == groupMapIter->second.end() )
        throw NExcept::CCriticalException( "Obj Data List 3D Get Data Error!",
            boost::str( boost::format( "Object data list name can't be found (%s).\n\n%s\nLine: (%s - %s)" )
                % group % name % __FUNCTION__ % __LINE__ ) );

    return dataMapIter->second;

}   // GetData3D
