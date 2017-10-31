
/************************************************************************
*    FILE NAME:       menumanager.h
*
*    DESCRIPTION:     menu manager class singleton
************************************************************************/

#ifndef __menu_manager_h__
#define __menu_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <utilities/matrix.h>
#include <gui/menudefs.h>
#include <gui/menu.h>
#include <utilities/genfunc.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class CMenu;
class CMenuTree;
class CUIControl;
union SDL_Event;

class CMenuManager : public CManagerBase
{
public:
    
    // Flag that indicates the load group doesn't create the VBO, IBO, textures, etc
    static const bool DONT_INIT_GROUP = false;

    // Get the instance of the singleton class
    static CMenuManager & Instance()
    {
        static CMenuManager menuMgr;
        return menuMgr;
    }
    
    // Load the menu action list from XML
    void LoadMenuActionFromXML( const std::string & filePath );
    
    // Load the menu group
    void LoadGroup( const std::string & group, const bool initGroup = true );
    
    // Free the menu group
    void FreeGroup( const std::string & group );
    
    // Init a menu group
    void InitGroup( const std::string & group );
    
    // Clean up a menu group
    void CleanUpGroup( const std::string & group );

    // Activate a tree to be used
    void ActivateTree( const std::string & group, const std::string & treeStr );
    void ActivateTree( const std::string & treeStr );
    
    // Deactivate a tree that's in use
    void DeactivateTree( const std::string & group, const std::string & treeStr );
    void DeactivateTree( const std::string & treeStr );

    // Clear the active trees
    void ClearActiveTrees();

    // Handle input events and dispatch menu events
    void HandleEvent( const SDL_Event & rEvent );

    // Update the menu
    void Update();
    
    // Do the transform
    void TransformMenu();
    void TransformMenu( const CObject2D & object );
    void TransformInterface();
    void TransformInterface( const CObject2D & object );

    // Render menus
    void Render( const CMatrix & matrix );
    
    // Render interface menus  
    void RenderInterface( const CMatrix & matrix );

    // Get reference to the menu in questionn
    CMenu & GetMenu( const std::string & nameStr );
    
    // Get reference to the active menu
    CMenu & GetActiveMenu();

    // Get the reference to the control in question
    template <typename Target>
    Target & GetMenuControl( const std::string & menuName, const std::string & controlName );

    // Get the pointer to the active control - can return null
    template <typename Target>
    Target * GetPtrToActiveControl( const std::string & menuName );

    // Is this menu system active
    bool IsActive();
    
    // Is this standard menu system active?
    bool IsMenuActive();

    // Is a menu item active
    bool IsMenuItemActive();
    
    // Is a interface item active
    bool IsInterfaceItemActive();

    // Reset the transform
    void ResetTransform();

    // Reset the dynamic positions of menus
    void ResetDynamicOffset();
    
    // Allow message processing
    void Allow( bool allow = true );

private:
    
    // Constructor
    CMenuManager();

    // Destructor
    ~CMenuManager();
    
    // Load the menu info from file
    void LoadFromXML( const std::string & group, const std::string & filePath );

    // Load the from node
    void LoadMenusFromNode( const std::string & group, const XMLNode & node );
    
    // Load the trees from node
    void LoadTreesFromNode( const std::string & group, const XMLNode & node );

    // Set the active state
    void SetActiveState();

    // Timer call back function
    static Uint32 ScrollTimerCallbackFunc( Uint32 interval, void *param );
    
    // Handle input events depending on if this is a menu or interface tree
    void HandleEventForTrees( const SDL_Event & rEvent );
    
    // Handle input events depending on if this is a menu or interface tree
    void HandleEventForScrolling( const SDL_Event & rEvent );
    
    // Handle input events for menu scrolling
    bool HandleMenuScrolling( 
        const SDL_Event & rEvent, const std::vector<CMenuTree *> & activeTreeVec );
    
    // Update the menu
    bool UpdateMenu( const std::vector<CMenuTree *> & activeTreeVec );
    
    // Transform the menu
    void Transform( const std::vector<CMenuTree *> & activeTreeVec );
    void Transform( const std::vector<CMenuTree *> & activeTreeVec, const CObject2D & object );
    
    // Get a pointer to the active tree
    CMenuTree * GetActiveTree();

private:

    // Map map of menu trees
    std::map<const std::string, std::map<const std::string, CMenuTree> > m_menuTreeMapMap;

    // Map map of the menus
    std::map<const std::string, std::map<const std::string, CMenu> > m_menuMapMap;

    // Vector of active menu trees
    std::vector<CMenuTree *> m_pActiveMenuTreeVec;
    
    // Vector of active interface trees
    std::vector<CMenuTree *> m_pActiveInterTreeVec;

    // menu manager state
    bool m_active;

    // Actions
    std::string m_backAction;
    std::string m_toggleAction;
    std::string m_escapeAction;
    std::string m_selectAction;
    std::string m_upAction;
    std::string m_downAction;
    std::string m_leftAction;
    std::string m_rightAction;
    std::string m_tabLeft;
    std::string m_tabRight;
    std::string m_defaultTree;

    // scroll timer Id
    SDL_TimerID m_scrollTimerId;
    
    // Allow message processing
    bool m_allow;
    
};

/************************************************************************
*    desc:  Get the reference to the control in question
************************************************************************/
template <typename Target>
Target & CMenuManager::GetMenuControl( const std::string & menuName, const std::string & controlName )
{
    CMenu & rMenu = CMenuManager::Instance().GetMenu(menuName);
    Target * pControl = NGenFunc::DynCast<Target>(rMenu.GetPtrToControl(controlName));
    assert( pControl != nullptr );

    return *pControl;
}

/************************************************************************
*    desc:  Get the pointer to the active control - can return null
************************************************************************/
template <typename Target>
Target * CMenuManager::GetPtrToActiveControl( const std::string & menuName )
{
    CMenu & rMenu = CMenuManager::Instance().GetMenu(menuName);
    Target * pControl = NGenFunc::DynCast<Target>(rMenu.GetPtrToActiveControl());

    return pControl;
}

#endif  // __menu_manager_h__

