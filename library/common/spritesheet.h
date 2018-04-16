
/************************************************************************
*    FILE NAME:       spritesheet.h
*
*    DESCRIPTION:     Class for holding sprite sheet data
************************************************************************/

#ifndef __sprite_sheet_h__
#define __sprite_sheet_h__

// Game lib dependencies
#include <common/spritesheetglyph.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

class CSpriteSheet
{
public:
    
    // Constructor
    CSpriteSheet();
    
    // Set the glyph count
    void SetGlyphCount( uint glyphCount);
    
    // Set the glyph columns
    void SetGlyphColumns( uint columns );
    
    // Build the simple (grid) sprite sheet
    void Build( const CSize<int> & sheetSize );
    
    // Load the glyph data from XML file
    void LoadFromXML( const std::string & filePath );
    
    // Get the glyph
    const CSpriteSheetGlyph & GetGlyph( int index = -1 ) const;
    const CSpriteSheetGlyph & FindGlyph( const std::string & glyphId ) const;
   
    // Get the number of gylphs in this sprite sheet
    size_t GetCount() const;
    
    // Set/Get the default index
    void SetDefaultIndex( uint index );
    uint GetDefaultIndex() const;
    
    // Set/Get the start index for when loading from format code
    void SetFormatCodeOffset( uint index );
    int GetFormatCodeOffset() const;
    
    // Copy over the gylph data
    void CopyTo( CSpriteSheet & rSpriteSheet, const std::vector<std::string> & strIdVec, bool loadAllGlyphs = false ) const;
    
    // Clear out the data
    void Clear();
    
private:

    // Set the gylph data
    void Set( const CSpriteSheetGlyph & rGlyph );
    void Set( CSpriteSheet & rSpriteSheet, const std::string & rGlyphId ) const;
    void Set( const CSpriteSheet & rSpriteSheet, const std::vector<std::string> & rStrIdVec );
    
private:

    // Sprite Sheet default index
    uint m_defaultIndex;
    
    // Sprite Sheet element count
    uint m_glyphCount;

    // Sprite Sheet columns
    uint m_columns;
    
    // Starting offset for loading with a format code
    int m_formatCodeOffset;
    
    // Size of the sprite sheet
    CSize<int> m_size;

    // A vector of all the glyphs built manually that are of the same size
    std::vector< CSpriteSheetGlyph > m_glyphVec;
    
    // A map of all the glyphs
    std::map< std::string, CSpriteSheetGlyph > m_glyphMap;
};

#endif  // __sprite_sheet_h__

