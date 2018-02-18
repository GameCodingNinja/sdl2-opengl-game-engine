
/************************************************************************
*    FILE NAME:       fontmanager.cpp
*
*    DESCRIPTION:     font manager class singleton
************************************************************************/

// Physical component dependency
#include <managers/fontmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CFontMgr::CFontMgr()
{
}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CFontMgr::~CFontMgr()
{
}   // Destructor


/************************************************************************
*    desc:  Load the material from file path
*  
*    param: string & filePath - path to material file
* 
*    return: bool - false on fail
************************************************************************/
void CFontMgr::LoadFromXML( const std::string & filePath )
{
    // open this file and parse
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "fontLst" );

    // Get the group the textures will be saves as
    const XMLNode listGroupNode = mainNode.getChildNode( "listGroup" );
    m_group = listGroupNode.getAttribute( "name" );

    // Get the list of font info
    XMLNode fontLstNode = mainNode.getChildNode( "fonts" );

    for( int i = 0; i < fontLstNode.nChildNode(); ++i )
    {
        const XMLNode fontNode = fontLstNode.getChildNode( "font", i );

        // Get the name of the font
        const std::string name = fontNode.getAttribute( "name" );

        // Add the font to our list
        auto iter = m_fontMap.emplace( std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple() );
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Font Manager Error!",
                boost::str( boost::format("Font name has already been loaded (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }

        // Load the character info from file
        iter.first->second.LoadFromXML( m_group, std::string(fontNode.getAttribute( "file" )) );
    }

}   // LoadFromFile


/************************************************************************
*    desc:  Get the font 
************************************************************************/
const CFont & CFontMgr::GetFont( const std::string & name ) const
{
    // See if this font is part of the map
    auto iter = m_fontMap.find( name );

    if( iter == m_fontMap.end() )
        throw NExcept::CCriticalException("Font Manager Error!",
            boost::str( boost::format("Font name can't be found (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));

    return iter->second;

}   // GetFont


/************************************************************************
*    desc:  Is the font in the map. throws exception if not
************************************************************************/
void CFontMgr::IsFont( const std::string & name ) const
{
    // See if this character is part of the map
    auto iter = m_fontMap.find( name );

    if( iter == m_fontMap.end() )
        throw NExcept::CCriticalException("Font Manager Error!",
            boost::str( boost::format("Font name can't be found (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));

}   // IsFont


/************************************************************************
*    desc:  Delete a specific mesh
*
*    NOTE: The name of this function is because of windows naming
*          conflict. Can't name it DeleteFont().       
*  
*    param: string & key
************************************************************************/
void CFontMgr::DeleteTheFont( const std::string & key )
{
    // See if this font has already been loaded
    auto iter = m_fontMap.find( key );

    // If it's found, delete from the map
    if( iter != m_fontMap.end() )
        m_fontMap.erase( iter );

}   // DeleteTheFont


/************************************************************************
*    desc:  Get the group name 
************************************************************************/
const std::string & CFontMgr::GetGroup() const
{
    return m_group;

}   // GetGroup
