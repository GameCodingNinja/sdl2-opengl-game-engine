
/************************************************************************
*    FILE NAME:       fontproperties.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <common/fontproperties.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <common/defs.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CFontProperties::CFontProperties()
{
}

CFontProperties::CFontProperties( const std::string fontName ) :
    m_fontName(fontName)
{
}

CFontProperties::CFontProperties( const std::string fontName, NDefs::EHorzAlignment hAlign, NDefs::EVertAlignment vAlign ) :
    m_fontName(fontName),
    m_hAlign(hAlign),
    m_vAlign(vAlign)
{
}

CFontProperties::CFontProperties( const std::string fontName, NDefs::EHorzAlignment hAlign, NDefs::EVertAlignment vAlign, float kerning ) :
    m_fontName(fontName),
    m_hAlign(hAlign),
    m_vAlign(vAlign),
    m_kerning(kerning)
{
}


/************************************************************************
*    desc:  Copy the data
************************************************************************/ 
void CFontProperties::Copy( const CFontProperties & obj )
{
    m_fontName = obj.m_fontName;
    m_hAlign = obj.m_hAlign;
    m_vAlign = obj.m_vAlign;
    m_kerning = obj.m_kerning;
    m_spaceCharKerning = obj.m_spaceCharKerning;
    m_lineWrapWidth = obj.m_lineWrapWidth;
    m_lineWrapHeight = obj.m_lineWrapHeight;
    
}   // Copy


/************************************************************************
*    desc:  Load the font properties from XML node
************************************************************************/
void CFontProperties::LoadFromNode( const XMLNode & node )
{
    // Get the must have font name
    m_fontName = node.getAttribute( "fontName" );

    // Get the attributes node
    const XMLNode attrNode = node.getChildNode( "attributes" );
    if( !attrNode.isEmpty() )
    {
        if( attrNode.isAttributeSet( "kerning" ) )
            m_kerning = std::atof( attrNode.getAttribute( "kerning" ) );
        
        if( attrNode.isAttributeSet( "spaceCharKerning" ) )
            m_spaceCharKerning = std::atof( attrNode.getAttribute( "spaceCharKerning" ) );

        if( attrNode.isAttributeSet( "lineWrapWidth" ) )
            m_lineWrapWidth = std::atof( attrNode.getAttribute( "lineWrapWidth" ) );

        if( attrNode.isAttributeSet( "lineWrapHeight" ) )
            m_lineWrapHeight = std::atof( attrNode.getAttribute( "lineWrapHeight" ) );
    }

    // Get the alignment node
    const XMLNode alignmentNode = node.getChildNode( "alignment" );
    if( !alignmentNode.isEmpty() )
    {
        // Set the default alignment
        m_hAlign = NParseHelper::LoadHorzAlignment( alignmentNode, NDefs::EHA_HORZ_CENTER );
        m_vAlign = NParseHelper::LoadVertAlignment( alignmentNode, NDefs::EVA_VERT_CENTER );
    }
    
}   // LoadFromNode
