
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
* 
*           NOTE: The first time the sprite sheet is created, byStrID sets
*           the stage as to how it is stored. If false, it's all loaded
*           in the sprite sheets vector and if true, the map. It's assumed
*           that string Id's are for complex sprite sheets that are shared
*           among many sprites where as no string id is a simple sprite
*           sheet animation used by one sprite.
************************************************************************/
const CSpriteSheet & CSpriteSheetMgr::LoadFromXML( const std::string & filePath )
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
    
}   // LoadFromXML


/************************************************************************
*    desc:  Delete the sprite sheet data
************************************************************************/
void CSpriteSheetMgr::DeleteSpriteSheetData( const std::string & filePath )
{
    // Free the texture group if it exists
    auto mapIter = m_spriteSheetMap.find( filePath );
    if( mapIter != m_spriteSheetMap.end() )
        m_spriteSheetMap.erase( mapIter );

}   // DeleteSpriteSheetData


/************************************************************************
*    desc:  Clear all the sprite sheet data
************************************************************************/
void CSpriteSheetMgr::Clear()
{
    m_spriteSheetMap.clear();
}
