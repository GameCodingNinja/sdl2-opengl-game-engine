
/************************************************************************
*    FILE NAME:       menutree.cpp
*
*    DESCRIPTION:     Class that hold a tree of menus
************************************************************************/

#ifndef __menu_tree_h__
#define __menu_tree_h__

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Game lib dependencies
#include <common/defs.h>
#include <utilities/matrix.h>
#include <gui/menudefs.h>
#include <gui/scrollparam.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
struct XMLNode;
class CObject2D;
class CMenu;

class CMenuTree : public boost::noncopyable
{
public:

    // Constructor
    CMenuTree(
        const std::string & name,
        std::map<const std::string, CMenu> & rMenuMap, 
        const std::string & rootMenu,
        const std::string & defaultMenu,
        bool interfaceTree = false );

    // Destructor
    ~CMenuTree();

    // Handle events
    void handleEvent( const SDL_Event & rEvent );
    
    // Activate a menu
    void activateMenu( const std::string & menuName );

    // Update the menu tree
    void update();

    // Transform the menu tree
    void transform();
    void transform( const CObject2D & object );

    // do the render
    void render( const CMatrix & matrix );

    // Is a menu active
    bool isActive();

    // Does this tee have a root menu
    bool hasRootMenu();

    // Init the tree for use
    void init();

    // Get the active menu
    CMenu & getActiveMenu();

    // Get the scroll param data
    CScrollParam & getScrollParam( int msg );

    // Is a menu item active
    bool isMenuItemActive();
    
    // Is this an interface tree
    bool isInterfaceTree() const;
    
    // Get the name of the tree
    std::string & getName();

private:

    // Transition the menu
    void transitionMenu();

    // Handle message
    void onEscape( const SDL_Event & rEvent );
    void onToggle( const SDL_Event & rEvent );
    void onBack( const SDL_Event & rEvent );
    void onToTree( const SDL_Event & rEvent );
    void onToMenu( const SDL_Event & rEvent );
    void onTransOut( const SDL_Event & rEvent );
    void onTransIn( const SDL_Event & rEvent );

private:
    
    // Name of the tree
    std::string m_name;

    // Map of the menus
    std::map<const std::string, CMenu> & m_rMenuMap;
    
    // Pointer to the root menu
    // Do not free. We DON'T own the pointer
    CMenu * m_pRootMenu;

    // Pointer to the default menu
    // Do not free. We DON'T own the pointer
    CMenu * m_pDefaultMenu;
    
    // Is interface tree?
    const bool m_interfaceTree;

    // Name of menu we are transitioning to
    std::string m_toMenu;

    // Vector of the path taken through the menu
    // Do not free. We DON'T own the pointers
    std::vector<CMenu *> m_pMenuPathVec;

    // menu tree state
    NMenu::EMenuTreeState m_state;

};

#endif  // __menu_tree_h__


