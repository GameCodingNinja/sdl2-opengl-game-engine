
/************************************************************************
*    FILE NAME:       basicstagestrategy2d.cpp
*
*    DESCRIPTION:     Basic 2D stage strategy
************************************************************************/

// Physical component dependency
#include <2d/basicstagestrategy2d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBasicStageStrategy2D::CBasicStageStrategy2D()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CBasicStageStrategy2D::~CBasicStageStrategy2D()
{
}   // destructor


/************************************************************************
*    desc:  Load the sector data from file
************************************************************************/
void CBasicStageStrategy2D::LoadFromFile( const std::string & file )
{
    // open and parse the XML file:
    XMLNode node = XMLNode::openFileHelper( file.c_str(), "stage" );
    
    XMLNode cameraNode = node.getChildNode( "cameraPosition" );
    if( !cameraNode.isEmpty() )
        m_defaultCameraPos.LoadTransFromNode( cameraNode );

    // Load thes sector data from node
    LoadFromNode( node );

}   // LoadFromFile


/************************************************************************
*    desc:  Load thes sector data from node
************************************************************************/
void CBasicStageStrategy2D::LoadFromNode( const XMLNode & node )
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
            m_sectorDeq.back().LoadTransFromNode( sectorNode );
        }
    }
    
}   // LoadFromNode


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CBasicStageStrategy2D::Init()
{
    for( auto & iter : m_sectorDeq )
        iter.Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CBasicStageStrategy2D::CleanUp()
{
    for( auto & iter : m_sectorDeq )
        iter.CleanUp();
    
}   // CleanUp


/***************************************************************************
*    desc:  Update the sector
****************************************************************************/
void CBasicStageStrategy2D::Update()
{
    for( auto & iter : m_sectorDeq )
        iter.Update();

}   // Update2D


/************************************************************************
*    desc:  Transform the sector
************************************************************************/
void CBasicStageStrategy2D::Transform()
{
    for( auto & iter : m_sectorDeq )
        iter.Transform();

}   // Transform

void CBasicStageStrategy2D::Transform( const CObject2D & object )
{
    for( auto & iter : m_sectorDeq )
        iter.Transform( object );

}   // Transform


/***************************************************************************
*    desc:  Render the sector
****************************************************************************/
void CBasicStageStrategy2D::Render( const CMatrix & matrix )
{
    for( auto & iter : m_sectorDeq )
        iter.Render( matrix );

}   // Render


/************************************************************************
*    desc:  Get the default camera position
************************************************************************/
CObject & CBasicStageStrategy2D::GetDefaultCameraPos()
{
    return m_defaultCameraPos;
    
}   // GetDefaultCameraPos
