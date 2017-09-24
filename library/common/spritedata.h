
/************************************************************************
*    FILE NAME:       spritedata.h
*
*    DESCRIPTION:     Sprite Data Class
************************************************************************/

#ifndef __sprite_data_h__
#define __sprite_data_h__

// Physical component dependency
#include <common/object.h>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
struct XMLNode;
class CFontData;

class CSpriteData : public CObject
{
public:

    // Constructor
    CSpriteData(
        const XMLNode & node,
        const std::string & defGroup = std::string(),
        const std::string & defObjName = std::string(),
        const std::string & defAIName = std::string(),
        int defId = defs_SPRITE_DEFAULT_ID );
    
    CSpriteData( const CSpriteData & data );
    
    // Destructor
    ~CSpriteData();
    
    // Get the group
    const std::string & GetName() const;
    
    // Get the group
    const std::string & GetGroup() const;
    
    // Get the name
    const std::string & GetObjectName() const;

    // Get the AI name
    const std::string & GetAIName() const;
    
    // Get the unique id number
    int GetId() const;
    
    // Get the font data
    const CFontData * GetFontData() const;

private:

    std::string m_name;
    std::string m_group;
    std::string m_objectName;
    std::string m_aiName;
    int m_id;
    std::unique_ptr<CFontData> m_upFontData;
};

#endif  // __sprite_data_h__


