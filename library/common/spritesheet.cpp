
/************************************************************************
*    FILE NAME:       spritesheet.cpp
*
*    DESCRIPTION:     Class for holding sprite sheet data
************************************************************************/

// Physical component dependency
#include <common/spritesheet.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <limits>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSpriteSheet::CSpriteSheet() :
    m_defaultIndex(0),
    m_glyphCount(0),
    m_columns(0)
{
}


/************************************************************************
*    desc:  Set the glyph count
************************************************************************/
void CSpriteSheet::SetGlyphCount( uint glyphCount)
{
    m_glyphCount = glyphCount;
    
}   // InitBuild


/************************************************************************
*    desc:  Set the glyph columns
************************************************************************/
void CSpriteSheet::SetGlyphColumns( uint columns )
{
    m_columns = columns;
    
}   // SetGlyphColumns


/************************************************************************
*    desc:  Build the simple (grid) sprite sheet data
************************************************************************/
void CSpriteSheet::Build( const CSize<int> & sheetSize )
{
    if( (m_glyphCount != 0) && (m_columns != 0) )
    {
        m_size = sheetSize;
        
        int rows = m_glyphCount / m_columns;

        if( (m_glyphCount % m_columns) > 0 )
            ++rows;

        // Reserve the number of glyphs
        m_glyphVec.reserve( m_glyphCount );

        // Calculate the size of the individual glyph. They are all the same size
        const CSize<float> size( sheetSize.w / m_columns, sheetSize.h / rows );

        for( int i = 0; i < rows; ++i )
        {
            for( uint j = 0; j < m_columns; ++j )
            {
                m_glyphVec.emplace_back(
                    size,
                    CRect<float>(
                        ((float)j * size.w) / (float)sheetSize.w,
                        ((float)i * size.h) / (float)sheetSize.h,
                        size.w / (float)sheetSize.w,
                        size.h / (float)sheetSize.h ) );

                // Break out after all the gylphs have been defined
                if( m_glyphVec.size() == (size_t)m_glyphCount )
                    break;
            }
        }
    }
    
}   // Build


/************************************************************************
*    desc:  Load the glyph data from XML file
* 
*           NOTE: The first time the sprite sheet is created, byStrID sets
*           the stage as to how it is stored. If false, it's all loaded
*           in the sprite sheets vector and if true, the map. It's assumed
*           that string Id's are for complex sprite sheets that are shared
*           among many sprites where as no string id is a simple sprite
*           sheet animation used by maybe one sprite.
************************************************************************/
void CSpriteSheet::LoadFromXML( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "spriteSheet" );
    if( !node.isEmpty() )
    {
        m_size = NParseHelper::LoadSizeFromChild( node );
        
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            const XMLNode rectNode = node.getChildNode(i);
            
            auto rect = NParseHelper::LoadRectFromChild( rectNode );
            
            const CSize<float> glyphSize( rect.x2, rect.y2 );
            
            const CRect<float> uv(
                (float)rect.x1 / (float)m_size.w,
                (float)rect.y1 / (float)m_size.h,
                (float)rect.x2 / (float)m_size.w,
                (float)rect.y2 / (float)m_size.h );
            
            const CSize<int> cropOffset(
                std::atoi( rectNode.getAttribute( "cx" ) ),
                std::atoi( rectNode.getAttribute( "cy" ) ) );
            
            // cropOffset only uses bytes so check that the limits on sloppy image cropping
            if( cropOffset.w < std::numeric_limits<int8_t>::min() || cropOffset.w > std::numeric_limits<int8_t>::max() ||
                cropOffset.h < std::numeric_limits<int8_t>::min() || cropOffset.h > std::numeric_limits<int8_t>::max() )
            {
                throw NExcept::CCriticalException("Crop offsets out of range!",
                    boost::str( boost::format("Crop offsets exceed byte values.\n\n%s\nLine: %s")
                        % __FUNCTION__ % __LINE__ ));
            }
            
            // Add the gylph to the map
            std::string strId = rectNode.getAttribute( "name" );
            m_glyphMap.emplace( std::piecewise_construct, std::forward_as_tuple(strId), std::forward_as_tuple(glyphSize, uv, cropOffset) );
        }
    }
    
}   // LoadFromXML


/************************************************************************
*    desc:  Get the glyph
************************************************************************/
const CSpriteSheetGlyph & CSpriteSheet::GetGlyph( int index ) const
{
    if( index > -1 )
        return m_glyphVec.at( index );
    
    else
        return m_glyphVec.at( m_defaultIndex );
            
}   // GetGlyph

const CSpriteSheetGlyph & CSpriteSheet::FindGlyph( const std::string & glyphId ) const
{
    auto mapIter = m_glyphMap.find( glyphId );
    if( mapIter == m_glyphMap.end() )
    {
        throw NExcept::CCriticalException("Sprite Sheet Error!",
            boost::str( boost::format("Glyph name is missing (%s).\n\n%s\nLine: %s")
                % glyphId % __FUNCTION__ % __LINE__ ));
    }
         
    return mapIter->second;
    
}   // GetGlyph


/************************************************************************
*    desc:  Get the number of gylphs in this sprite sheet
************************************************************************/
size_t CSpriteSheet::GetCount() const
{
    size_t count = m_glyphMap.size();
    const size_t vecCount = m_glyphVec.size();
    
    if( count < vecCount )
        count = vecCount;
    
    if( count < m_glyphCount )
        return m_glyphCount;
    
    return count;
    
}   // GetCount


/************************************************************************
*    desc:  Set the default index
************************************************************************/
void CSpriteSheet::SetDefaultIndex( uint defaultIndex )
{
    m_defaultIndex = defaultIndex;
}

uint CSpriteSheet::GetDefaultIndex() const
{
    return m_defaultIndex;
}


/************************************************************************
*    desc:  Set the gylph data
************************************************************************/
void CSpriteSheet::Set( const CSpriteSheetGlyph & rGlyph )
{
    m_glyphVec.push_back( rGlyph );
    
}   // Set

void CSpriteSheet::Set( CSpriteSheet & rSpriteSheet, const std::string & glyphId ) const
{
    auto mapIter = m_glyphMap.find( glyphId );
    if( mapIter != m_glyphMap.end() )
    {
        rSpriteSheet.Set( mapIter->second );
    }
    else
    {
        throw NExcept::CCriticalException("Sprite Sheet Error!",
            boost::str( boost::format("Glyph name is missing (%s).\n\n%s\nLine: %s")
                % glyphId % __FUNCTION__ % __LINE__ ));
    }
}   // Set


/************************************************************************
*    desc:  Copy over the gylph data
************************************************************************/
void CSpriteSheet::CopyTo( CSpriteSheet & rSpriteSheet, const std::vector<std::string> & strIdVec ) const
{
    if( strIdVec.empty() )
    {
        for( auto & iter: m_glyphVec )
            rSpriteSheet.Set( iter );
    }
    else if( !m_glyphMap.empty() )
    {
        // Init the sprite sheet class when each glyph is defined in the object data
        if( rSpriteSheet.GetCount() == 0 )
        {
            for( auto & iter: strIdVec )
                Set( rSpriteSheet, iter );
        }
        // Init the sprite sheet class with an animation that is one glyph defined with a format code
        else
        {
            // Should only be one entry
            if( strIdVec.size() == 1 )
            {
                for( uint i = 0; i < rSpriteSheet.GetCount(); ++i )
                {
                    std::string glyphStr = boost::str( boost::format(strIdVec.back()) % i );
                    Set( rSpriteSheet, glyphStr );
                }
            }
            else
            {
                throw NExcept::CCriticalException("Sprite Sheet Error!",
                    boost::str( boost::format("Incorrect configuration.\n\n%s\nLine: %s")
                        % __FUNCTION__ % __LINE__ ));
            }
        }
    }
    
}   // CopyTo
