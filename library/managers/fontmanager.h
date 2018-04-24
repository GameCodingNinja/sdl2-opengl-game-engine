
/************************************************************************
*    FILE NAME:       fontmanager.h
*
*    DESCRIPTION:     font manager class singleton
************************************************************************/  

#ifndef __font_manager_h__
#define __font_manager_h__

// Game lib dependencies
#include <2d/font.h>

// Standard lib dependencies
#include <string>
#include <map>

class CFontMgr
{
public:

    // Get the instance of the singleton class
    static CFontMgr & Instance()
    {
        static CFontMgr fontMgr;
        return fontMgr;
    }

    // Load the material from file path
    void loadFromXML( const std::string & filePath, const bool createFromData = true );
    
    // Create the font texture from data
    void createFromData();

    // Get the font
    const CFont & getFont( const std::string & name ) const;

    // Is the font in the map. throws exception if not
    void isFont( const std::string & name ) const;

    // Delete a specific mesh
    void deleteTheFont( const std::string & key );

    // Get the group name
    const std::string & getGroup() const;

private:

    CFontMgr();
    ~CFontMgr();

private:

    // map list of fonts
    std::map<const std::string, CFont> m_fontMap;

    // Group name
    std::string m_group;

};

#endif  // __font_manager_h__
