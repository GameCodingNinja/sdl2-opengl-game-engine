
/************************************************************************
*    FILE NAME:       basicstagestrategy.cpp
*
*    DESCRIPTION:     Basic stage strategy
************************************************************************/

// Physical component dependency
#include <spritestrategy/basicstagestrategy.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <managers/cameramanager.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBasicStageStrategy::CBasicStageStrategy( const std::string & cameraId ) :
    iSpriteStrategy(cameraId)
{
}   // constructor

CBasicStageStrategy::CBasicStageStrategy() :
    iSpriteStrategy()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CBasicStageStrategy::~CBasicStageStrategy()
{
}   // destructor


/************************************************************************
*    desc:  Load the sector data from file
************************************************************************/
void CBasicStageStrategy::LoadFromFile( const std::string & file )
{
    // open and parse the XML file:
    XMLNode node = XMLNode::openFileHelper( file.c_str(), "stage" );
    
    XMLNode cameraNode = node.getChildNode( "cameraPosition" );
    if( !cameraNode.isEmpty() )
        m_defaultCameraPos.loadTransFromNode( cameraNode );

    // Load thes sector data from node
    LoadFromNode( node );

}   // LoadFromFile


/************************************************************************
*    desc:  Load thes sector data from node
************************************************************************/
void CBasicStageStrategy::LoadFromNode( const XMLNode & node )
{
    XMLNode sectorsNode = node.getChildNode( "sectors" );
    if( !sectorsNode.isEmpty() )
    {
        const XMLNode sectorsNode = node.getChildNode( "sectors" );
        
        for( int i = 0; i < sectorsNode.nChildNode(); ++i )
        {
            XMLNode sectorNode = sectorsNode.getChildNode( i );
            
            m_sectorDeq.emplace_back();
            m_sectorDeq.back().LoadFromNode( sectorNode );
            m_sectorDeq.back().loadTransFromNode( sectorNode );
        }
    }
    
}   // LoadFromNode


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CBasicStageStrategy::Init()
{
    for( auto & iter : m_sectorDeq )
        iter.Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CBasicStageStrategy::CleanUp()
{
    for( auto & iter : m_sectorDeq )
        iter.CleanUp();
    
}   // CleanUp


/***************************************************************************
*    desc:  Update the sector
****************************************************************************/
void CBasicStageStrategy::Update()
{
    for( auto & iter : m_sectorDeq )
        iter.Update();

}   // Update2D


/************************************************************************
*    desc:  Transform the sector
************************************************************************/
void CBasicStageStrategy::Transform()
{
    for( auto & iter : m_sectorDeq )
        iter.transform();

}   // Transform

void CBasicStageStrategy::Transform( const CObject2D & object )
{
    for( auto & iter : m_sectorDeq )
        iter.transform( object );

}   // Transform


/***************************************************************************
*    desc:  Render the sector
****************************************************************************/
void CBasicStageStrategy::Render( const CMatrix & matrix )
{
    for( auto & iter : m_sectorDeq )
        iter.Render( matrix );

}   // Render

void CBasicStageStrategy::Render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    for( auto & iter : m_sectorDeq )
        iter.Render( matrix, rotMatrix );

}   // Render

void CBasicStageStrategy::Render()
{
    const auto & camera = CCameraMgr::Instance().GetCamera( m_cameraId );

    for( auto & iter : m_sectorDeq )
        iter.Render( camera );

}   // Render


/************************************************************************
*    desc:  Get the default camera position
************************************************************************/
CObject & CBasicStageStrategy::GetDefaultCameraPos()
{
    return m_defaultCameraPos;
    
}   // GetDefaultCameraPos


/************************************************************************
 *    desc:  Find if the sprite exists
 ************************************************************************/
bool CBasicStageStrategy::Find( iSprite * piSprite )
{
    for( auto & iter : m_sectorDeq )
    {
        if( iter.Find( piSprite ) )
            return true;
    }

    return false;
}
