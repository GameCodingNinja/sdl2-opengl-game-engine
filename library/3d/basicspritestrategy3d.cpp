
/************************************************************************
*    FILE NAME:       basicspritestrategy3d.cpp
*
*    DESCRIPTION:     Basic sprite strategy 3d class
************************************************************************/

// Physical component dependency
#include <3d/basicspritestrategy3d.h>

// Game lib dependencies
#include <common/spritedata.h>
#include <common/actordata.h>
#include <3d/sprite3d.h>
#include <3d/actorsprite3d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdata3d.h>
#include <objectdata/objectdatamanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBasicSpriteStrategy3D::CBasicSpriteStrategy3D( int idOffset, int idDir ) :
    CBaseStrategy( idOffset, idDir )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CBasicSpriteStrategy3D::~CBasicSpriteStrategy3D()
{
    for( auto & iter : m_spriteMap )
        NDelFunc::Delete( iter.second );
    
}   // destructor


/************************************************************************
 *    desc:  Load the sprite data from node
 ************************************************************************/
void CBasicSpriteStrategy3D::LoadFromFile( const std::string & file )
{
    // open and parse the XML file:
    XMLNode spriteListNode = XMLNode::openFileHelper( file.c_str(), "spriteList" );
    if( !spriteListNode.isEmpty() )
    {
        std::string defGroup, defObjName, defAIName;
        
        // Check for any defaults
        if( spriteListNode.isAttributeSet( "defaultGroup" ) )
            defGroup = spriteListNode.getAttribute( "defaultGroup" );
        
        if( spriteListNode.isAttributeSet( "defaultObjectName" ) )
            defObjName = spriteListNode.getAttribute( "defaultObjectName" );

        if( spriteListNode.isAttributeSet( "defaultAIName" ) )
            defAIName = spriteListNode.getAttribute( "defaultAIName" );

        // Load the sprite data
        for( int i = 0; i < spriteListNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteListNode.getChildNode( i );

            // Get the sprite name
            const std::string name = spriteNode.getAttribute( "name" );
            
            const std::string tag( spriteNode.getName() );
            
            bool duplicate(false);
            
            // Load the sprite data into the map
            if( tag == "sprite" )
                duplicate = !m_dataMap.emplace( name, new CSpriteData( spriteNode, defGroup, defObjName, defAIName ) ).second;
            
            else if( tag == "actor" )
                duplicate = !m_dataMap.emplace( name, new CActorData( spriteNode, defGroup, defObjName, defAIName ) ).second;
            
            else
                throw NExcept::CCriticalException("Sprite Load Error!",
                    boost::str( boost::format("Undefined sprite tag (%s).\n\n%s\nLine: %s")
                        % name % __FUNCTION__ % __LINE__ ));
            
            // Check for duplicate names
            if( duplicate )
            {
                throw NExcept::CCriticalException("Sprite Load Error!",
                    boost::str( boost::format("Duplicate sprite name (%s).\n\n%s\nLine: %s")
                        % name % __FUNCTION__ % __LINE__ ));
            }
        }
    }

}   // LoadDataGroup


/************************************************************************
 *    desc:  Get the sprite data container by name
 ************************************************************************/
CSpriteDataContainer & CBasicSpriteStrategy3D::GetData( const std::string & name )
{
    auto iter = m_dataMap.find( name );
    if( iter == m_dataMap.end() )
        throw NExcept::CCriticalException("Sprite Data Container Error!",
            boost::str( boost::format("Error finding sprite data container (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));

    return iter->second;

}   // GetData


/************************************************************************
*    desc:  create the sprite sprite
*           NOTE: Function assumes radians
************************************************************************/
int CBasicSpriteStrategy3D::Create(
    const std::string & name,
    const int id,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    std::string aiName;
    const CSpriteDataContainer & rSpriteDataContainer = GetData( name );
    
    // If the sprite defined a unique id then use that
    int spriteId( (id + m_idOffset) * m_idDir );
    
    // Create the sprite
    if( rSpriteDataContainer.GetType() == NDefs::SPRITE2D )
    {
        const auto & rData = rSpriteDataContainer.Get<CSpriteData>();
        if( rData.GetId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.GetId();
        
        auto & objectData =
            CObjectDataMgr::Instance().GetData3D( rData.GetGroup(), rData.GetObjectName() );
        
        m_iter = m_spriteMap.emplace( spriteId, new CSprite3D( objectData ) );
        
        // Copy transform data specified in the xml
        m_iter.first->second->CopyTransform( &rData );
        
        aiName = rData.GetAIName();
    }
    else if( rSpriteDataContainer.GetType() == NDefs::ACTOR2D )
    {
        const auto & rData = rSpriteDataContainer.Get<CActorData>();
        if( rData.GetId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.GetId();
        
        m_iter = m_spriteMap.emplace( spriteId, new CActorSprite3D( rData, spriteId ) );
        
        // Copy transform data specified in the xml
        m_iter.first->second->CopyTransform( &rData );
        
        aiName = rData.GetAIName();
    }

    // Check for duplicate id's
    if( !m_iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % name % id % __FUNCTION__ % __LINE__ ));
    }

    // Use passed in transforms if specified
    if( !pos.IsEmpty() )
        m_iter.first->second->SetPos(pos);

    if( !rot.IsEmpty() )
        m_iter.first->second->SetRot(rot, false);

    if( scale != CPoint<float>(1,1,1) )
        m_iter.first->second->SetScale(scale);

    // Init the physics
    m_iter.first->second->InitPhysics();
    
    // Broadcast the signal to create the sprite AI
    if( !aiName.empty() )
        CSignalMgr::Instance().Broadcast( aiName, m_iter.first->second );
    
    return spriteId;

}   // Create


/***************************************************************************
*    desc:  Handle the deleting of any sprites
****************************************************************************/
void CBasicSpriteStrategy3D::CreateObj( const std::string & name )
{
    Create( name, 0, CPoint<float>(), CPoint<float>(), CPoint<float>(1,1,1) );
    
}   // CreateObj


/***************************************************************************
*    desc:  Handle the deleting of any sprites
****************************************************************************/
void CBasicSpriteStrategy3D::DeleteObj( int index )
{
    // Find the sprite, delete it and remove entry from map
    const auto iter = m_spriteMap.find( index );
    if( iter != m_spriteMap.end() )
    {
        // Specificly delete the physics body before deleting the sprite
        iter->second->GetPhysicsComponent().RemoveBody();
        
        NDelFunc::Delete( iter->second );
        m_spriteMap.erase( iter );
    }
    else
    {
        throw NExcept::CCriticalException("Error Deleting Sprite!",
            boost::str( boost::format("Sprite index can't be found (%d).\n\n%s\nLine: %s")
                % index % __FUNCTION__ % __LINE__ ));
    }

}   // DeleteObj


/***************************************************************************
*    desc:  Update the sprites
****************************************************************************/
void CBasicSpriteStrategy3D::Update()
{
    for( auto & iter : m_spriteMap )
    {
        iter.second->Update();
        //iter.second->PhysicsUpdate();
    }

}   // Update


/************************************************************************
*    desc:  Transform the sprites
************************************************************************/
void CBasicSpriteStrategy3D::Transform()
{
    for( auto & iter : m_spriteMap )
        iter.second->Transform();

}   // Transform


/***************************************************************************
*    desc:  Render the spriies
****************************************************************************/
void CBasicSpriteStrategy3D::Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix )
{
    for( auto & iter : m_spriteMap )
        iter.second->Render( projMatrix, cameraMatrix );

}   // Render


/************************************************************************
*    desc:  Get the pointer to the sprite
************************************************************************/
iSprite3D * CBasicSpriteStrategy3D::get( const int id )
{
    // See if this sprite has already been created
    auto iter = m_spriteMap.find( id );
    if( iter == m_spriteMap.end() )
        throw NExcept::CCriticalException("Sprite Request Error!",
            boost::str( boost::format("Requested sprite has not been created! (%d).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;

}   // get
