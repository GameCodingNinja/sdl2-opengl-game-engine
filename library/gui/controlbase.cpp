
/************************************************************************
*    FILE NAME:       controlbase.cpp
*
*    DESCRIPTION:     Control base class
************************************************************************/

// Physical component dependency
#include <gui/controlbase.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/xmlpreloader.h>
#include <utilities/settings.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CControlBase::CControlBase( const std::string & group ) :
    m_group(group),
    m_type(NUIControl::ECT_NULL)
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CControlBase::~CControlBase()
{
}


/************************************************************************
*    desc:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CControlBase::loadFromNode( const XMLNode & node )
{
    // Set the controls name
    if( node.isAttributeSet( "name" ) )
        m_name = node.getAttribute( "name" );

    // Set the faction name
    if( node.isAttributeSet( "faction" ) )
        m_faction = node.getAttribute( "faction" );

    // Load the transform data
    loadTransFromNode( node );

    // Load the dynamic offset from node
    loadDynamicOffsetFromNode( node );

    // See if we have a list of strings
    XMLNode stringLstNode = node.getChildNode( "fontStringLst" );
    if( !stringLstNode.isEmpty() )
    {
        for( int i = 0; i < stringLstNode.nChildNode(); ++i )
            m_stringVec.push_back( stringLstNode.getChildNode( "string", i ).getAttribute( "text" ) );
    }

    // Load the control specific xml file
    // Get the file path node to the control specific xml code
    XMLNode filePathNode = node.getChildNode( "filePath" );
    if( !filePathNode.isEmpty() )
    {
        // Get the control's file path
        const std::string controlFilePath = filePathNode.getAttribute( "file" );

        // Load xml specific control code
        // Use the preloaded since many controls reuse xml files
        loadControlFromNode( CXMLPreloader::Instance().Load( controlFilePath.c_str(), "UIControl" ) );
    }
}


/************************************************************************
*    desc:  Load the dynamic offset data from node
************************************************************************/
void CControlBase::loadDynamicOffsetFromNode( const XMLNode & node )
{
    // Load the dynamic offset
    m_dynamicOffset = NParseHelper::LoadDynamicOffset( node );

    // Set the dynamic position
    setDynamicPos();
}


/************************************************************************
*    desc:  Set the dynamic position
************************************************************************/
void CControlBase::setDynamicPos()
{
    // Position the menu based on the dynamic offset
    if( !m_dynamicOffset.isEmpty() )
        setPos( m_dynamicOffset.getPos( CSettings::Instance().GetDefaultSizeHalf() ) );
}


/************************************************************************
*    desc:  Get the object group name
************************************************************************/
const std::string & CControlBase::getGroup() const
{
    return m_group;
}


/************************************************************************
*    desc:  Get the control name
************************************************************************/
const std::string & CControlBase::getName() const
{
    return m_name;
}


/************************************************************************
*    desc:  Get the control type
************************************************************************/
NUIControl::EControlType CControlBase::getType() const
{
    return m_type;
}


/************************************************************************
*    desc:  Get the faction name
************************************************************************/
const std::string & CControlBase::getFaction() const
{
    return m_faction;
}
