
/************************************************************************
*    FILE NAME:       basicspritestrategy.cpp
*
*    DESCRIPTION:     Basic sprite strategy 2d class
************************************************************************/

// Physical component dependency
#include <spritestrategy/basicspritestrategy.h>

// Game lib dependencies
#include <common/spritedata.h>
#include <common/actordata.h>
#include <2d/sprite2d.h>
#include <2d/actorsprite2d.h>
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>
#include <managers/cameramanager.h>
#include <managers/signalmanager.h>
#include <objectdata/objectdata2d.h>
#include <objectdata/objectdatamanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBasicSpriteStrategy::CBasicSpriteStrategy( const std::string & cameraId, int idOffset, int idDir ) :
    CBaseStrategy( cameraId, idOffset, idDir )
{
}   // constructor

CBasicSpriteStrategy::CBasicSpriteStrategy( int idOffset, int idDir ) :
    CBaseStrategy( idOffset, idDir )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CBasicSpriteStrategy::~CBasicSpriteStrategy()
{
    for( auto & iter : m_spriteMap )
        NDelFunc::Delete( iter.second );
    
}   // destructor


/************************************************************************
 *    desc:  Load the sprite data from node
 ************************************************************************/
void CBasicSpriteStrategy::LoadFromFile( const std::string & file )
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
            if( tag == "sprite2d" )
                duplicate = !m_dataMap.emplace( 
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(new CSpriteData(spriteNode, defGroup, defObjName, defAIName), NDefs::SPRITE2D) ).second;
            
            else if( tag == "sprite3d" )
                duplicate = !m_dataMap.emplace( 
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(new CSpriteData(spriteNode, defGroup, defObjName, defAIName), NDefs::SPRITE3D) ).second;
            
            else if( tag == "actor2d" )
                duplicate = !m_dataMap.emplace( 
                    std::piecewise_construct,
                    std::forward_as_tuple(name),
                    std::forward_as_tuple(new CActorData(spriteNode, defGroup, defObjName, defAIName), NDefs::ACTOR2D) ).second;
            
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
CSpriteDataContainer & CBasicSpriteStrategy::GetData( const std::string & name )
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
iSprite * CBasicSpriteStrategy::Create(
    const std::string & dataName,
    const int id,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    std::string aiName;
    const CSpriteDataContainer & rSpriteDataContainer = GetData( dataName );
    
    // If the sprite defined a unique id then use that
    int spriteId( (id + m_idOffset) * m_idDir );
    
    std::pair<std::map<const int, iSprite *>::iterator, bool> m_iter;
    
    // Create the sprite
    if( rSpriteDataContainer.GetType() == NDefs::SPRITE2D )
    {
        const auto & rData = rSpriteDataContainer.Get<CSpriteData>();
        if( rData.GetId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.GetId();
        
        // Allocate the sprite
        m_iter = m_spriteMap.emplace( spriteId, new CSprite2D( CObjectDataMgr::Instance().GetData2D( rData ), spriteId ) );
        
        // Load the rest from sprite data
        dynamic_cast<CSprite2D *>(m_iter.first->second)->Load( rData );
        
        aiName = rData.GetAIName();
    }
    else if( rSpriteDataContainer.GetType() == NDefs::ACTOR2D )
    {
        const auto & rData = rSpriteDataContainer.Get<CActorData>();
        if( rData.GetId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.GetId();
        
        // Allocate the actor sprite
        m_iter = m_spriteMap.emplace( spriteId, new CActorSprite2D( rData, spriteId ) );
        
        aiName = rData.GetAIName();
    }

    // Check for duplicate id's
    if( !m_iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % dataName % id % __FUNCTION__ % __LINE__ ));
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
    
    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( m_iter.first->second );
    
    return m_iter.first->second;

}   // Create

iSprite * CBasicSpriteStrategy::Create(
    const std::string & dataName,
    const int id )
{
    std::string aiName;
    const CSpriteDataContainer & rSpriteDataContainer = GetData( dataName );
    
    // If the sprite defined a unique id then use that
    int spriteId( (id + m_idOffset) * m_idDir );
    
    std::pair<std::map<const int, iSprite *>::iterator, bool> m_iter;
    
    // Create the sprite
    if( rSpriteDataContainer.GetType() == NDefs::SPRITE2D )
    {
        const auto & rData = rSpriteDataContainer.Get<CSpriteData>();
        if( rData.GetId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.GetId();
        
        // Allocate the sprite
        m_iter = m_spriteMap.emplace( spriteId, new CSprite2D( CObjectDataMgr::Instance().GetData2D( rData ), spriteId ) );
        
        // Load the rest from sprite data
        dynamic_cast<CSprite2D *>(m_iter.first->second)->Load( rData );
        
        aiName = rData.GetAIName();
    }
    else if( rSpriteDataContainer.GetType() == NDefs::ACTOR2D )
    {
        const auto & rData = rSpriteDataContainer.Get<CActorData>();
        if( rData.GetId() != defs_SPRITE_DEFAULT_ID )
            spriteId = rData.GetId();
        
        // Allocate the actor sprite
        m_iter = m_spriteMap.emplace( spriteId, new CActorSprite2D( rData, spriteId ) );
        
        aiName = rData.GetAIName();
    }
    
    // Init the physics
    m_iter.first->second->InitPhysics();

    // Check for duplicate id's
    if( !m_iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % dataName % id % __FUNCTION__ % __LINE__ ));
    }
    
    // Broadcast the signal to create the sprite AI
    if( !aiName.empty() )
        CSignalMgr::Instance().Broadcast( aiName, m_iter.first->second );
    
    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( m_iter.first->second );
    
    return m_iter.first->second;

}   // Create

iSprite * CBasicSpriteStrategy::Create(
    const std::string & group,
    const std::string & name,
    const int id,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // If the sprite defined a unique id then use that
    int spriteId( (id + m_idOffset) * m_idDir );
    
    std::pair<std::map<const int, iSprite *>::iterator, bool> m_iter;
    
    // Allocate the sprite
    auto & objData = CObjectDataMgr::Instance().GetData2D( group, name );
    m_iter = m_spriteMap.emplace( spriteId, new CSprite2D( objData, spriteId ) );
        
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
    
    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( m_iter.first->second );
    
    return m_iter.first->second;

}   // Create

iSprite * CBasicSpriteStrategy::Create(
    const std::string & group,
    const std::string & name,
    const int id )
{
    // If the sprite defined a unique id then use that
    int spriteId( (id + m_idOffset) * m_idDir );
    
    // Allocate the sprite
    auto & objData = CObjectDataMgr::Instance().GetData2D( group, name );
    auto m_iter = m_spriteMap.emplace( spriteId, new CSprite2D( objData, spriteId ) );
        
    // Check for duplicate id's
    if( !m_iter.second )
    {
        throw NExcept::CCriticalException("Sprite Create Error!",
            boost::str( boost::format("Duplicate sprite id (%s - %d).\n\n%s\nLine: %s")
                % name % id % __FUNCTION__ % __LINE__ ));
    }
    
    // Init the physics
    m_iter.first->second->InitPhysics();
    
    // Add the sprite pointer to the vector for rendering
    m_pSpriteVec.push_back( m_iter.first->second );
    
    return m_iter.first->second;

}   // Create


/***************************************************************************
*    desc:  Handle the deleting of any sprites
****************************************************************************/
void CBasicSpriteStrategy::CreateObj( const std::string & name )
{
    Create( name, 0, CPoint<float>(), CPoint<float>(), CPoint<float>(1,1,1) );
    
}   // CreateObj


/***************************************************************************
*    desc:  Handle the deleting of any sprites
*           NOTE: Do not call from a destructor!
****************************************************************************/
void CBasicSpriteStrategy::DeleteObj( int index )
{
    // Find the sprite, delete it and remove entry from map
    const auto iter = m_spriteMap.find( index );
    if( iter != m_spriteMap.end() )
    {
        // specifically delete the physics body before deleting the sprite
        // Deleting the physics always needs to be done externally and
        // under the right conditions. NEVER call from destructor
        iter->second->CleanUp();
        
        auto spriteIter = std::find( m_pSpriteVec.begin(), m_pSpriteVec.end(), iter->second );
        if( spriteIter != m_pSpriteVec.end() )
            m_pSpriteVec.erase( spriteIter );
        
        NDelFunc::Delete( iter->second );
        m_spriteMap.erase( iter );
    }
    else
    {
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sprite index can't be found (%s).\n\n%s\nLine: %s") 
            % index % __FUNCTION__ % __LINE__ ) );
    }

}   // DeleteObj


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CBasicSpriteStrategy::Init()
{
    for( auto iter : m_pSpriteVec )
        iter->Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CBasicSpriteStrategy::CleanUp()
{
    for( auto iter : m_pSpriteVec )
        iter->CleanUp();

}   // CleanUp


/***************************************************************************
*    desc:  Update the sprites
****************************************************************************/
void CBasicSpriteStrategy::Update()
{
    for( auto iter : m_pSpriteVec )
    {
        iter->Update();
        iter->PhysicsUpdate();
    }

}   // Update


/************************************************************************
*    desc:  Transform the sprites
************************************************************************/
void CBasicSpriteStrategy::Transform()
{
    for( auto iter : m_pSpriteVec )
        iter->Transform();

}   // Transform


/***************************************************************************
*    desc:  Render the sprites
****************************************************************************/
void CBasicSpriteStrategy::Render( const CMatrix & matrix )
{
    for( auto iter : m_pSpriteVec )
        iter->Render( matrix );

}   // Render

void CBasicSpriteStrategy::Render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    for( auto iter : m_pSpriteVec )
        iter->Render( matrix, rotMatrix );

}   // Render

void CBasicSpriteStrategy::Render()
{
    const auto & camera = CCameraMgr::Instance().GetCamera( m_cameraId );

    for( auto iter : m_pSpriteVec )
        iter->Render( camera );

}   // Render


/************************************************************************
*    desc:  Get the pointer to the sprite
************************************************************************/
iSprite * CBasicSpriteStrategy::get( const int id )
{
    // See if this sprite has already been created
    auto iter = m_spriteMap.find( id );
    if( iter == m_spriteMap.end() )
        throw NExcept::CCriticalException("Sprite Request Error!",
            boost::str( boost::format("Requested sprite has not been created! (%d).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));

    return iter->second;

}   // get


/************************************************************************
 *    desc:  Find if the sprite exists
 ************************************************************************/
bool CBasicSpriteStrategy::Find( iSprite * piSprite )
{
    // See if this sprite has already been created
    auto iter = m_spriteMap.find( piSprite->GetId() );
    if( iter != m_spriteMap.end() && (iter->second == piSprite) )
        return true;
        
    return false;
}