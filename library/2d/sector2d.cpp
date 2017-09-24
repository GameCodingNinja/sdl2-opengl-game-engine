
/************************************************************************
*    FILE NAME:       sector2d.cpp
*
*    DESCRIPTION:     Class the creates & renders all the sector sprites
************************************************************************/

// Physical component dependency
#include <2d/sector2d.h>

// Game lib dependencies
#include <2d/isprite2d.h>
#include <2d/sprite2d.h>
#include <2d/actorsprite2d.h>
#include <utilities/settings.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <managers/signalmanager.h>
#include <common/spritedata.h>
#include <common/actordata.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSector2D::CSector2D() :
    m_projectionType(CSettings::Instance().GetProjectionType()),
    m_sectorSizeHalf(CSettings::Instance().GetSectorSizeHalf())
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSector2D::~CSector2D()
{
}   // destructor


/************************************************************************
*    desc:  Load the sector data from node
************************************************************************/
void CSector2D::LoadFromNode( const XMLNode & node )
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
                m_pSpriteVec.push_back( new CSprite2D( CObjectDataMgr::Instance().GetData2D( data.GetGroup(), data.GetObjectName() ), data.GetId() ) );
                
                aiName = data.GetAIName();
            }
            else if( tag == "actor" )
            {
                CActorData data( spriteNode, defGroup, defObjName, defAIName, defId );
                m_pSpriteVec.push_back( new CActorSprite2D( data, data.GetId() ) );
                
                aiName = data.GetAIName();
            }
            
            // Load the transform data from node
            m_pSpriteVec.back()->LoadTransFromNode( spriteNode );
            
            // Init the physics
            m_pSpriteVec.back()->InitPhysics();
            
            // Broadcast the signal to create the sprite AI
            if( !aiName.empty() )
                CSignalMgr::Instance().Broadcast( aiName, m_pSpriteVec.back() );
            
            // See if this sprite is used for rendering a font string
            const XMLNode fontNode = spriteNode.getChildNode( "font" );
            if( !fontNode.isEmpty() && m_pSpriteVec.back()->IsSprite2D() )
            {
                // Load the font properties from XML node
                dynamic_cast<CSprite2D *>(m_pSpriteVec.back())->GetVisualComponent().LoadFontPropFromNode( fontNode );
                
                if( fontNode.isAttributeSet( "string" ) )
                {
                    // Set the font string to be created later
                    dynamic_cast<CSprite2D *>(m_pSpriteVec.back())->GetVisualComponent().SetFontString( fontNode.getAttribute( "string" ) );
                }
            }
        }
    }
}   // LoadFromNode


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CSector2D::Init()
{
    // Create any font strings
    // This allows for delayed VBO create so that the fonts can be allocated during the load screen
    for( auto iter : m_pSpriteVec )
    {
        if( iter->IsSprite2D() )
        {
            if( dynamic_cast<CSprite2D *>(iter)->GetVisualComponent().IsFontSprite() )
            {
                CSignalMgr::Instance().Broadcast_LoadSignal();

                dynamic_cast<CSprite2D *>(iter)->GetVisualComponent().CreateFontString();
            }
        }
    }
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CSector2D::CleanUp()
{
    // Free the font VBO
    // This allows for early VBO delete so that the font can be freed from the load screen
    for( auto iter : m_pSpriteVec )
    {
        if( iter->IsSprite2D() )
        {
            if( dynamic_cast<CSprite2D *>(iter)->GetVisualComponent().IsFontSprite() )
            {
                CSignalMgr::Instance().Broadcast_LoadSignal();

                dynamic_cast<CSprite2D *>(iter)->GetVisualComponent().DeleteFontVBO();
            }
        }
    }
    
}   // CleanUp


/************************************************************************
*    desc:  Destroy this sector
************************************************************************/
void CSector2D::Destroy()
{
    m_pSpriteVec.clear();
    
}   // Destroy


/************************************************************************
*    desc:  Update the actor
************************************************************************/
void CSector2D::Update()
{
    for( auto & iter : m_pSpriteVec )
        iter->Update();
    
}   // Update


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CSector2D::DoTransform()
{
    Transform();

    for( auto iter : m_pSpriteVec )
        iter->Transform( GetMatrix(), WasWorldPosTranformed() );
    
}   // Transform

void CSector2D::DoTransform( const CObject2D & object )
{
    Transform( object.GetMatrix(), object.WasWorldPosTranformed() );

    for( auto iter : m_pSpriteVec )
        iter->Transform( GetMatrix(), WasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  Render the actor
************************************************************************/
void CSector2D::Render( const CMatrix & matrix )
{
    // Render in reverse order
    if( InView() )
    {
        for( auto it = m_pSpriteVec.rbegin(); it != m_pSpriteVec.rend(); ++it )
            (*it)->Render( matrix );
    }
    
}   // Render


/************************************************************************
*    desc:  Render the actor
************************************************************************/
bool CSector2D::InView()
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
bool CSector2D::InOrthographicView()
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
bool CSector2D::InPerspectiveView()
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