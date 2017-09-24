
/************************************************************************
*    FILE NAME:       spritedata.cpp
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

// Physical component dependency
#include <common/spritedata.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>
#include <common/fontdata.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSpriteData::CSpriteData( 
    const XMLNode & node,
    const std::string & defGroup,
    const std::string & defObjName,
    const std::string & defAIName,
    int defId ) :
        m_group(defGroup),
        m_objectName(defObjName),
        m_aiName(defAIName),
        m_id(defId)
{
    // Get the name of this specific sprite instance
    if( node.isAttributeSet( "name" ) )
        m_name = node.getAttribute( "name" );
    
    // Get the group this sprite belongs to
    if( node.isAttributeSet( "group" ) )
        m_group = node.getAttribute( "group" );
    
    // Get the object data name
    if( node.isAttributeSet( "objectName" ) )
        m_objectName = node.getAttribute( "objectName" );
    
    // Get the sprite's AI name
    if( node.isAttributeSet( "aiName" ) )
        m_aiName = node.getAttribute( "aiName" );
    
    // Get the sprite's unique id number
    if( node.isAttributeSet( "id" ) )
        m_id = std::atoi(node.getAttribute( "id" ));
    
    // Get if the sprite is visible
    if( node.isAttributeSet( "visible" ) )
        SetVisible( std::strcmp( node.getAttribute("visible"), "true" ) == 0 );
    
    // See if any font data is specified
    const XMLNode fontNode = node.getChildNode("font");
    if( !fontNode.isEmpty() )
    {
        m_upFontData.reset( new CFontData );
        m_upFontData->LoadFromNode( node );
    }

    // Load the transform data from node
    LoadTransFromNode( node );
    
}   // constructor

CSpriteData::CSpriteData( const CSpriteData & data ) :
    CObject( data ),
    m_name( data.m_name ),
    m_group( data.m_group ),
    m_objectName( data.m_objectName ),
    m_aiName( data.m_aiName ),
    m_id( data.m_id )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSpriteData::~CSpriteData()
{
}   // destructor


/************************************************************************
*    desc:  Get the name of this specific sprite instance
************************************************************************/
const std::string & CSpriteData::GetName() const
{
    return m_name;

}   // GetName


/************************************************************************
*    desc:  Get the group
************************************************************************/
const std::string & CSpriteData::GetGroup() const
{
    return m_group;

}   // GetGroup


/************************************************************************
*    desc:  Get the name
************************************************************************/
const std::string & CSpriteData::GetObjectName() const
{
    return m_objectName;

}   // GetObjectName


/************************************************************************
*    desc:  Get the ai name
************************************************************************/
const std::string & CSpriteData::GetAIName() const
{
    return m_aiName;
}


/************************************************************************
*    desc:  Get the unique id number
************************************************************************/
int CSpriteData::GetId() const
{
    return m_id;

}   // GetId




/************************************************************************
*    desc:  Get the font data
************************************************************************/
const CFontData * CSpriteData::GetFontData() const
{
    return m_upFontData.get();

}   // GetFontData
