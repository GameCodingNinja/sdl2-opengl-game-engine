
/************************************************************************
*    FILE NAME:       spritesheetmanager.cpp
*
*    DESCRIPTION:     Temporary container for loading sprite sheet data
*                     so that the same large complex xml is not reloaded.
************************************************************************/

// Physical component dependency
#include <managers/spritesheetmanager.h>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CSpriteSheetMgr::CSpriteSheetMgr()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSpriteSheetMgr::~CSpriteSheetMgr()
{
}   // destructor


/************************************************************************
*    desc:  Load the glyph data from XML file
************************************************************************/
const CSpriteSheet & CSpriteSheetMgr::Load( const std::string & filePath )
{
    auto mapIter = m_spriteSheetMap.find( filePath );
    if( mapIter == m_spriteSheetMap.end() )
    {
        // Add a new entry to the map
        mapIter = m_spriteSheetMap.emplace( filePath, CSpriteSheet() ).first;
        
        // Load the glyph data from XML file
        mapIter->second.LoadFromXML( filePath );
    }

    return mapIter->second;
    
}   // Load


/************************************************************************
*    desc:  Clear all the sprite sheet data
************************************************************************/
void CSpriteSheetMgr::Clear()
{
    m_spriteSheetMap.clear();
}
