
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
        bool interfaceMenu = false );

    // Destructor
    ~CMenuTree();

    // Handle events
    void HandleEvent( const SDL_Event & rEvent );

    // Update the menu tree
    void Update();

    // Transform the menu tree
    void Transform();
    void Transform( const CObject2D & object );

    // do the render
    void Render( const CMatrix & matrix );

    // Is a menu active
    bool IsActive();

    // Does this tee have a root menu
    bool HasRootMenu();

    // Init the tree for use
    void Init();

    // Get the active menu
    CMenu & GetActiveMenu();

    // Get the scroll param data
    CScrollParam & GetScrollParam( int msg );

    // Is a menu item active
    bool IsMenuItemActive();
    
    // Is this menu an interface
    bool IsMenuInterface() const;
    
    // Get the name of the tree
    std::string & GetName();

private:

    // Transition the menu
    void TransitionMenu();

    // Handle message
    void OnEscape( const SDL_Event & rEvent );
    void OnToggle( const SDL_Event & rEvent );
    void OnBack( const SDL_Event & rEvent );
    void OnToTree( const SDL_Event & rEvent );
    void OnToMenu( const SDL_Event & rEvent );
    void OnTransOut( const SDL_Event & rEvent );
    void OnTransIn( const SDL_Event & rEvent );

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
    
    // Is interface menu?
    const bool m_interfaceMenu;

    // Name of menu we are transitioning to
    std::string m_toMenu;

    // Vector of the path taken through the menu
    // Do not free. We DON'T own the pointers
    std::vector<CMenu *> m_pMenuPathVec;

    // menu tree state
    NMenu::EMenuTreeState m_state;

};

#endif  // __menu_tree_h__


