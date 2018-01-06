
/************************************************************************
*    FILE NAME:       sector3d.cpp
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

// Physical component dependency
#include <3d/sector3d.h>

// Game lib dependencies
#include <3d/isprite3d.h>
#include <3d/sprite3d.h>
#include <3d/actorsprite3d.h>
#include <utilities/settings.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata3d.h>
#include <managers/signalmanager.h>
#include <common/spritedata.h>
#include <common/actordata.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSector3D::CSector3D() :
    m_projectionType(CSettings::Instance().GetProjectionType()),
    m_sectorSizeHalf(CSettings::Instance().GetSectorSizeHalf())
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSector3D::~CSector3D()
{
}   // destructor


/************************************************************************
*    desc:  Load the sector data from node
************************************************************************/
void CSector3D::LoadFromNode( const XMLNode & node )
{
    // open and parse the XML file:
    const std::string filePath = node.getAttribute( "file" );
    const XMLNode spriteListNode = XMLNode::openFileHelper( filePath.c_str(), "spriteList" );
    if( !spriteListNode.isEmpty() )
    {
        std::string defObjName, defGroup, defAIName;
        int defId(-1);

        // Check for any defaults
        if( spriteListNode.isAttributeSet( "defaultGroup" ) )
            defGroup = spriteListNode.getAttribute( "defaultGroup" );

        if( spriteListNode.isAttributeSet( "defaultObjectName" ) )
            defObjName = spriteListNode.getAttribute( "defaultObjectName" );
        
        if( spriteListNode.isAttributeSet( "defaultAIName" ) )
            defAIName = spriteListNode.getAttribute( "defaultAIName" );

        if( spriteListNode.isAttributeSet( "defaultId" ) )
            defId = std::atoi(spriteListNode.getAttribute( "defaultId" ));

        // Create the sprites
        for( int i = 0; i < spriteListNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteListNode.getChildNode( i );
            
            const std::string tag( spriteNode.getName() );
            
            std::string aiName;

            // Allocate the sprite and add it to the vector
            if( tag == "sprite" )
            {
                CSpriteData data( spriteNode, defGroup, defObjName, defAIName, defId );
                m_pSpriteVec.push_back( new CSprite3D( CObjectDataMgr::Instance().GetData3D( data.GetGroup(), data.GetObjectName() )) );
                
                aiName = data.GetAIName();
            }
            else if( tag == "actor" )
            {
                CActorData data( spriteNode, defGroup, defObjName, defAIName, defId );
                m_pSpriteVec.push_back( new CActorSprite3D( data, data.GetId() ) );
                
                aiName = data.GetAIName();
            }
            
            // Load the transform data from node
            m_pSpriteVec.back()->LoadTransFromNode( spriteNode );
            
            // Init the physics
            m_pSpriteVec.back()->InitPhysics();
            
            // Broadcast the signal to create the sprite AI
            if( !aiName.empty() )
                CSignalMgr::Instance().Broadcast( aiName, m_pSpriteVec.back() );
        }
    }
}   // LoadFromNode


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CSector3D::Init()
{

}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CSector3D::CleanUp()
{
    
}   // CleanUp


/************************************************************************
*    desc:  Destroy this sector
************************************************************************/
void CSector3D::Destroy()
{
    m_pSpriteVec.clear();
    
}   // Destroy


/************************************************************************
*    desc:  Update the actor
************************************************************************/
void CSector3D::Update()
{
    for( auto & iter : m_pSpriteVec )
        iter->Update();
    
}   // Update


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CSector3D::Transform()
{
    CObject3D::Transform();

    for( auto iter : m_pSpriteVec )
        iter->Transform( GetMatrix(), WasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  Render the actor
************************************************************************/
void CSector3D::Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix )
{
    // Render in reverse order
    if( InView() )
    {
        for( auto it = m_pSpriteVec.rbegin(); it != m_pSpriteVec.rend(); ++it )
            (*it)->Render( projMatrix, cameraMatrix );
    }
    
}   // Render


/************************************************************************
*    desc:  Render the actor
************************************************************************/
bool CSector3D::InView()
{
    if( m_projectionType == NDefs::EPT_ORTHOGRAPHIC )
        return InOrthographicView();
    
    else if( m_projectionType == NDefs::EPT_PERSPECTIVE )
        return InPerspectiveView();
    
    return true;

}   // InView


/************************************************************************
 *    desc:  Check the sector object is within the orthographic view frustum
 ************************************************************************/
bool CSector3D::InOrthographicView()
{
    const CSize<float> & defaultSizeHalf = CSettings::Instance().GetDefaultSizeHalf();
    
    // Check against the right side of the screen
    if( std::fabs(m_transPos.x) > (defaultSizeHalf.w + m_sectorSizeHalf) )
        return false;

    // Check against the top of the screen
    if( std::fabs(m_transPos.y) > (defaultSizeHalf.h + m_sectorSizeHalf) )
        return false;

    // If we made it this far, the object is in view
    return true;

}   // InOrthographicView


/************************************************************************
 *    desc:  Check the sector object is within the perspective view frustum
 ************************************************************************/
bool CSector3D::InPerspectiveView()
{
    const CSize<float> & aspectRatio = CSettings::Instance().GetScreenAspectRatio();

    // Check the right and left sides of the screen
    if( std::fabs(m_transPos.x) > ((std::fabs(GetPos().z) * aspectRatio.w) + m_sectorSizeHalf) )
        return false;

    // Check the top and bottom sides of the screen
    if( std::fabs(m_transPos.y) > ((std::fabs(GetPos().z) * aspectRatio.h) + m_sectorSizeHalf) )
        return false;

    // if we made it this far, the object was not culled
    return true;

}   // InPerspectiveView
