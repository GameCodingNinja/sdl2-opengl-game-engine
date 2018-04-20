
/************************************************************************
*    FILE NAME:       basicstagestrategy3d.cpp
*
*    DESCRIPTION:     Basic 3D stage strategy
************************************************************************/

// Physical component dependency
#include <3d/basicstagestrategy3d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBasicStageStrategy3D::CBasicStageStrategy3D()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CBasicStageStrategy3D::~CBasicStageStrategy3D()
{
}   // destructor


/************************************************************************
*    desc:  Load the sector data from file
************************************************************************/
void CBasicStageStrategy3D::LoadFromFile( const std::string & file )
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
void CBasicStageStrategy3D::LoadFromNode( const XMLNode & node )
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
void CBasicStageStrategy3D::Init()
{
    for( auto & iter : m_sectorDeq )
        iter.Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CBasicStageStrategy3D::CleanUp()
{
    for( auto & iter : m_sectorDeq )
        iter.CleanUp();
    
}   // CleanUp


/***************************************************************************
*    desc:  Update the sector
****************************************************************************/
void CBasicStageStrategy3D::Update()
{
    for( auto & iter : m_sectorDeq )
        iter.Update();

}   // Update3D


/************************************************************************
*    desc:  Transform the sector
************************************************************************/
void CBasicStageStrategy3D::Transform()
{
    for( auto & iter : m_sectorDeq )
        iter.Transform();

}   // Transform

//void CBasicStageStrategy3D::Transform( const CObject3D & object )
//{
//    for( auto & iter : m_sectorDeq )
//        iter.Transform( object );
//
//}   // Transform


/***************************************************************************
*    desc:  Render the sector
****************************************************************************/
void CBasicStageStrategy3D::Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix )
{
    for( auto & iter : m_sectorDeq )
        iter.Render( projMatrix, cameraMatrix );

}   // Render


/************************************************************************
*    desc:  Get the default camera position
************************************************************************/
CObject & CBasicStageStrategy3D::GetDefaultCameraPos()
{
    return m_defaultCameraPos;
    
}   // GetDefaultCameraPos
