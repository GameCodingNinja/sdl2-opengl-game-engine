
/************************************************************************
*    FILE NAME:       menumanager.cpp
*
*    DESCRIPTION:     menu manager class singleton
************************************************************************/

// Physical component dependency
#include <gui/menumanager.h>

// Game lib dependencies
#include <common/build_defs.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <gui/menutree.h>
#include <gui/menu.h>
#include <gui/scrollparam.h>
#include <gui/messagecracker.h>
#include <managers/actionmanager.h>
#include <managers/signalmanager.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CMenuManager::CMenuManager() :
    m_active(false),
    m_scrollTimerId(0),
    m_allow(false)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CMenuManager::~CMenuManager()
{
    SDL_RemoveTimer( m_scrollTimerId );

}   // destructor


/************************************************************************
 *    desc:  Load the menu group
 ************************************************************************/
void CMenuManager::LoadGroup( const std::string & group, const bool initGroup )
{
    // Check for a hardware extension
    std::string ext;
    if( !m_mobileExt.empty() && NBDefs::IsMobileDevice() )
        if( m_listTableMap.find( group + m_mobileExt ) != m_listTableMap.end() )
            ext = m_mobileExt;
    
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group + ext );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Menu Load Group Error!",
            boost::str( boost::format("Menu group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_menuMapMap.find( group ) == m_menuMapMap.end() )
    {
        // Create a new group map inside of our maps
        m_menuMapMap.emplace( group, std::map<const std::string, CMenu>() );
        m_menuTreeMapMap.emplace( group, std::map<const std::string, CMenuTree>() );

        for( auto & iter : listTableIter->second )
            LoadFromXML( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Menu Group load Error!",
            boost::str( boost::format("Menu group has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }
    
    if( initGroup )
        InitGroup( group );
    
}   // LoadGroup


/************************************************************************
 *    desc:  Free the menu group
 ************************************************************************/
void CMenuManager::FreeGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Menu Free Group Error!",
            boost::str( boost::format("Menu group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Free the tree group
    auto treeMapIter = m_menuTreeMapMap.find( group );
    if( treeMapIter != m_menuTreeMapMap.end() )
    {
        // Remove it from the tree vectors if it is there
        for( auto & treeIter : treeMapIter->second )
        {
            if( treeIter.second.IsMenuInterface() )
            {
                auto interIter = std::find( m_pActiveInterTreeVec.begin(), m_pActiveInterTreeVec.end(), &treeIter.second );
                if( interIter != m_pActiveInterTreeVec.end() )
                    m_pActiveInterTreeVec.erase( interIter );
            }
            else
            {
                auto menuIter = std::find( m_pActiveMenuTreeVec.begin(), m_pActiveMenuTreeVec.end(), &treeIter.second );
                if( menuIter != m_pActiveMenuTreeVec.end() )
                    m_pActiveMenuTreeVec.erase( menuIter );
            }
        }
        
        m_menuTreeMapMap.erase( treeMapIter );
    }
    
    // Free the menu group
    auto menuMapIter = m_menuMapMap.find( group );
    if( menuMapIter != m_menuMapMap.end() )
        m_menuMapMap.erase( menuMapIter );

    // See if we are active
    SetActiveState();
    
}   // FreeGroup


/************************************************************************
 *    desc:  Init a menu group
 *           This allows certain actions to be done after the group load
 ************************************************************************/
void CMenuManager::InitGroup( const std::string & group )
{
    auto menuMapIter = m_menuMapMap.find( group );
    if( menuMapIter != m_menuMapMap.end() )
    {
        for( auto & iter : menuMapIter->second )
            iter.second.Init();
    }
    else
    {
        throw NExcept::CCriticalException("Menu Init Group Error!",
            boost::str( boost::format("Menu group name can't be found to clean up (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // InitGroup


/************************************************************************
 *    desc:  Clean up a menu group
 *           This allows certain actions to be done after the group load
 ************************************************************************/
void CMenuManager::CleanUpGroup( const std::string & group )
{
    auto menuMapIter = m_menuMapMap.find( group );
    if( menuMapIter != m_menuMapMap.end() )
    {
        for( auto & iter : menuMapIter->second )
            iter.second.CleanUp();
    }
    else
    {
        throw NExcept::CCriticalException("Menu Init Group Error!",
            boost::str( boost::format("Menu group name can't be found to init (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // CleanUpGroup


/************************************************************************
*    desc:  Load the menu/tree info from file
************************************************************************/
void CMenuManager::LoadFromXML( const std::string & group, const std::string & filePath )
{
    // open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "menuTreeList" );
    
    // Load the menus from node
    LoadMenusFromNode( group, node );
    
    // Load the trees from node
    LoadTreesFromNode( group, node );
    
}   // LoadFromXML
 

/************************************************************************
*    desc:  Load the menus from node
************************************************************************/
void CMenuManager::LoadMenusFromNode( const std::string & group, const XMLNode & node )
{
    // Get an iterator to the menu group
    auto menuMapIter = m_menuMapMap.find( group );
    
    // Get the node to the default object data
    const XMLNode menuLstNode = node.getChildNode( "menuList" );
    
    for( int i = 0; i < menuLstNode.nChildNode(); ++i )
    {
        // Menu name and file path 
        const XMLNode menuNode = menuLstNode.getChildNode(i);

        // Get the name of the menu
        const std::string name = menuNode.getAttribute( "name" );

        // Insert the menu into the menu map
        auto iter = menuMapIter->second.emplace( std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, group) );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Menu Load Error!",
                boost::str( boost::format("Duplicate menu name (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }

        // Load the transform from node
        iter.first->second.LoadTransFromNode( menuNode );

        // Load the dynamic offset from node
        iter.first->second.LoadDynamicOffsetFromNode( menuNode );

        // Have the menu load it's share
        iter.first->second.LoadFromXML( menuNode.getAttribute( "file" ) );
        
        // Broadcast signal to let the game handle smart menu inits
        CSignalMgr::Instance().Broadcast( &iter.first->second );
        
        // Handle any smart menu creates
        iter.first->second.SmartCreate();
    }

}   // LoadMenusFromNode


/************************************************************************
*    desc:  Load the trees from node
************************************************************************/
void CMenuManager::LoadTreesFromNode( const std::string & group, const XMLNode & node )
{
    // Get an iterator to the menu group
    auto menuMapIter = m_menuMapMap.find( group );
    
    // Get an iterator to the tree group
    auto treeMapIter = m_menuTreeMapMap.find( group );
    
    // Get the node to the default object data
    const XMLNode treeLstNode = node.getChildNode( "treeList" );
    
    for( int i = 0; i < treeLstNode.nChildNode(); ++i )
    {
        // Get the tree node
        const XMLNode treeNode = treeLstNode.getChildNode("tree", i);

        // Get the name
        const std::string name = treeNode.getAttribute( "name" );

        // Get the root menu
        std::string rootMenu;
        if( treeNode.isAttributeSet("root") )
            rootMenu = treeNode.getAttribute( "root" );

        // Get the default menu
        std::string defaultMenu;
        if( treeNode.isAttributeSet("default") )
            defaultMenu = treeNode.getAttribute( "default" );
        
        // Is this menu an interface menu?
        bool interfaceMenu(false);
        if( treeNode.isAttributeSet("interfaceMenu") )
            interfaceMenu = ( std::strcmp( treeNode.getAttribute("interfaceMenu"), "true" ) == 0 );

        // Add the tree data to the map
        auto iter = treeMapIter->second.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(name, menuMapIter->second, rootMenu, defaultMenu, interfaceMenu) );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Menu Tree Load Error!",
                boost::str( boost::format("Duplicate tree name (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }

        // Check that the root menu exists
        if( !rootMenu.empty() )
        {
            if( menuMapIter->second.find(rootMenu) == menuMapIter->second.end() )
            {
                throw NExcept::CCriticalException("Menu Load Error!",
                    boost::str( boost::format("Root menu doesn't exist (%s).\n\n%s\nLine: %s")
                        % rootMenu % __FUNCTION__ % __LINE__ ));
            }
        }
        else if( !defaultMenu.empty() )
        {
            if( menuMapIter->second.find(defaultMenu) == menuMapIter->second.end() )
            {
                throw NExcept::CCriticalException("Menu Load Error!",
                    boost::str( boost::format("Default menu doesn't exist (%s).\n\n%s\nLine: %s")
                        % defaultMenu % __FUNCTION__ % __LINE__ ));
            }
        }
    }
    
}   // LoadTreesFromNode


/************************************************************************
*    desc:  Load the menu action list from XML
************************************************************************/
void CMenuManager::LoadMenuActionFromXML( const std::string & filePath )
{
    // open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "menuActionList" );
    
    m_backAction = node.getChildNode( "backAction" ).getText();
    m_toggleAction = node.getChildNode( "toggleAction" ).getText();
    m_escapeAction = node.getChildNode( "escapeAction" ).getText();
    m_selectAction = node.getChildNode( "selectAction" ).getText();
    m_upAction = node.getChildNode( "upAction" ).getText();
    m_downAction = node.getChildNode( "downAction" ).getText();
    m_leftAction = node.getChildNode( "leftAction" ).getText();
    m_rightAction = node.getChildNode( "rightAction" ).getText();
    m_tabLeft = node.getChildNode( "tabLeft" ).getText();
    m_tabRight = node.getChildNode( "tabRight" ).getText();
    m_defaultTree = node.getChildNode( "defaultTree" ).getText();
    
}   // LoadMenuActionFromXML


/************************************************************************
*    desc:  Activate a tree to be used by tree name only
*           NOTE: Assumes unique tree names
************************************************************************/
void CMenuManager::ActivateTree( const std::string & treeStr )
{
    for( auto & groupIter : m_menuTreeMapMap )
    {
        for( auto & treeIter : groupIter.second )
        {
            if( treeIter.first == treeStr )
            {
                ActivateTree( groupIter.first, treeIter.first );
                return;
            }
        }
    }
    
    // If you got this far, it's a problem
    throw NExcept::CCriticalException("Menu Tree Acticate Error!",
        boost::str( boost::format("Menu tree doesn't exist (%s).\n\n%s\nLine: %s")
            % treeStr % __FUNCTION__ % __LINE__ ));
    
}   // ActivateTree

/************************************************************************
*    desc:  Activate a tree to be used
************************************************************************/
void CMenuManager::ActivateTree( const std::string & group, const std::string & treeStr )
{
    auto groupIter = m_menuTreeMapMap.find( group );
    if( groupIter != m_menuTreeMapMap.end() )
    {
        // Find the tree in the map
        auto treeIter = groupIter->second.find( treeStr );
        if( treeIter != groupIter->second.end() )
        {
            if( treeIter->second.IsMenuInterface() )
            {
                if( std::find( m_pActiveInterTreeVec.begin(), m_pActiveInterTreeVec.end(), &treeIter->second ) != m_pActiveInterTreeVec.end() )
                    throw NExcept::CCriticalException("Menu Tree Acticate Error!",
                        boost::str( boost::format("Menu tree already active (%s - %s).\n\n%s\nLine: %s")
                            % group % treeStr % __FUNCTION__ % __LINE__ ));
                
                m_pActiveInterTreeVec.push_back( &treeIter->second );
            }
            else
            {
                if( std::find( m_pActiveMenuTreeVec.begin(), m_pActiveMenuTreeVec.end(), &treeIter->second ) != m_pActiveMenuTreeVec.end() )
                    throw NExcept::CCriticalException("Menu Tree Acticate Error!",
                        boost::str( boost::format("Menu tree already active (%s - %s).\n\n%s\nLine: %s")
                            % group % treeStr % __FUNCTION__ % __LINE__ ));
                
                m_pActiveMenuTreeVec.push_back( &treeIter->second );
            }
            
            // Init the tree for use
            treeIter->second.Init();
        }
        else
        {
            throw NExcept::CCriticalException("Menu Tree Acticate Error!",
                boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
                    % group % treeStr % __FUNCTION__ % __LINE__ ));
        }
    }
    else
    {
        throw NExcept::CCriticalException("Menu Tree Acticate Error!",
            boost::str( boost::format("Menu tree group doesn't exist (%s - %s).\n\n%s\nLine: %s")
                % group % treeStr % __FUNCTION__ % __LINE__ ));
    }
    
    // See if we are active
    SetActiveState();
    
}   // ActivateTree


/************************************************************************
*    desc:  Deactivate a tree to be used
*           NOTE: Assumes unique tree names
************************************************************************/
void CMenuManager::DeactivateTree( const std::string & treeStr )
{
    for( auto & groupIter : m_menuTreeMapMap )
    {
        for( auto & treeIter : groupIter.second )
        {
            if( treeIter.first == treeStr )
            {
                DeactivateTree( groupIter.first, treeIter.first );
                return;
            }
        }
    }
    
    // If you got this far, it's a problem
    throw NExcept::CCriticalException("Menu Tree Deacticate Error!",
        boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
            % treeStr % __FUNCTION__ % __LINE__ ));
    
}   // DeactivateTree

/************************************************************************
*    desc:  Deactivate a tree that's in use
************************************************************************/
void CMenuManager::DeactivateTree( const std::string & group, const std::string & treeStr )
{
    auto groupIter = m_menuTreeMapMap.find( group );
    if( groupIter != m_menuTreeMapMap.end() )
    {
        // Find the tree in the map
        auto treeIter = groupIter->second.find( treeStr );
        if( treeIter != groupIter->second.end() )
        {
            // Remove the tree from the vector
            if( treeIter->second.IsMenuInterface() )
            {
                auto iter = std::find( m_pActiveInterTreeVec.begin(), m_pActiveInterTreeVec.end(), &treeIter->second );
                if( iter != m_pActiveInterTreeVec.end() )
                    m_pActiveInterTreeVec.erase( iter );
            }
            else
            {
                auto iter = std::find( m_pActiveMenuTreeVec.begin(), m_pActiveMenuTreeVec.end(), &treeIter->second );
                if( iter != m_pActiveMenuTreeVec.end() )
                    m_pActiveMenuTreeVec.erase( iter );
            }
        }
        else
        {
            throw NExcept::CCriticalException("Menu Tree Deacticate Error!",
                boost::str( boost::format("Menu tree doesn't exist (%s - %s).\n\n%s\nLine: %s")
                    % group % treeStr % __FUNCTION__ % __LINE__ ));
        }
    }
    else
    {
        throw NExcept::CCriticalException("Menu Tree Deacticate Error!",
            boost::str( boost::format("Menu tree group doesn't exist (%s - %s).\n\n%s\nLine: %s")
                % group % treeStr % __FUNCTION__ % __LINE__ ));
    }
    
    // See if we are still active
    SetActiveState();
    
}   // DeactivateTree


/************************************************************************
*    desc:  Clear the active trees
************************************************************************/
void CMenuManager::ClearActiveTrees()
{
    m_active = false;
    
    SDL_RemoveTimer( m_scrollTimerId );

    m_pActiveMenuTreeVec.clear();
    m_pActiveInterTreeVec.clear();

}   // ClearActiveTrees


/************************************************************************
*    desc:  Handle input events and dispatch menu events
************************************************************************/
void CMenuManager::HandleEvent( const SDL_Event & rEvent )
{
    if( m_allow )
    {
        // Convert keyboard, mouse and controller messages in action type messages
        if( (rEvent.type > SDL_SYSWMEVENT) && (rEvent.type < SDL_CLIPBOARDUPDATE) )
        {
            // Message TESTING code
            /*if( rEvent.type == SDL_CONTROLLERBUTTONDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Controller Button: %d") % ((int)rEvent.cbutton.button) ) );
            else if( rEvent.type == SDL_JOYBUTTONDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Joystick Button: %d") % ((int)rEvent.cbutton.button) ) );
            else if( rEvent.type == SDL_MOUSEMOTION )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Mouse Motion: x = %d, y = %d") % ((int)rEvent.motion.x) % ((int)rEvent.motion.y) ) );
            else if( rEvent.type == SDL_MOUSEBUTTONDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Mouse Button Down: %d") % ((int)rEvent.button.button) ) );
            else if( rEvent.type == SDL_MOUSEBUTTONUP )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Mouse Button Up: %d") % ((int)rEvent.button.button) ) );
            else if( rEvent.type == SDL_KEYDOWN )
                NGenFunc::PostDebugMsg( boost::str( boost::format("Key Button: %d") % ((int)rEvent.key.keysym.sym) ) );

            else if( rEvent.type == SDL_CONTROLLERAXISMOTION )
            {
                if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Left: x = %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Left: y = %d") % ((int)rEvent.caxis.value) ) );
                if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Right: x = %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CAxis Right: y = %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CTrigger Left: %d") % ((int)rEvent.caxis.value) ) );
                else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("CTrigger Right: %d") % ((int)rEvent.caxis.value) ) );
            }
            else if( rEvent.type == SDL_JOYAXISMOTION )
            {
                if( rEvent.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("Joystick Left: x = %d") % ((int)rEvent.jaxis.value) ) );
                else if( rEvent.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
                    NGenFunc::PostDebugMsg( boost::str( boost::format("Joystick Left: y = %d") % ((int)rEvent.jaxis.value) ) );
            }*/

            // Only the default tree can execute an escape or toggle when none are active.
            if( CActionMgr::Instance().WasAction( rEvent, m_escapeAction, NDefs::EAP_DOWN ) )
            {
                CMenuTree * pTree = GetActiveTree();
                
                if( pTree == nullptr )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION, 0, &m_defaultTree );
                else
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_ESCAPE_ACTION, 0, &pTree->GetName() );
            }
            else if( CActionMgr::Instance().WasAction( rEvent, m_toggleAction, NDefs::EAP_DOWN ) )
            {
                CMenuTree * pTree = GetActiveTree();
                
                if( pTree == nullptr )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_TOGGLE_ACTION, 0, &m_defaultTree );
                else
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_TOGGLE_ACTION, 0, &pTree->GetName() );
            }
            else if( IsActive() )
            {
                NDefs::EActionPress pressType;

                // common and can result in many messages which is why it's specifically defined here
                if( rEvent.type == SDL_MOUSEMOTION )
                {
                    // Allow the mouse move message to get eaten when action handling is disabled.
                    HandleEventForTrees( rEvent );
                }
                // Need to pack multiple data items into one 32-bit int for this message
                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_selectAction )) > NDefs::EAP_IDLE )
                {
                    // Need to pack a lot of information into one 32 bit int
                    CSelectMsgCracker msgCracker;
                    msgCracker.SetPressType( pressType );
                    msgCracker.SetDeviceId( CActionMgr::Instance().GetLastDeviceUsed() );

                    if( msgCracker.GetDeviceId() == NDefs::MOUSE )
                    {
                        msgCracker.SetX( rEvent.button.x );
                        msgCracker.SetY( rEvent.button.y );
                    }

                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_SELECT_ACTION, msgCracker.GetPackedUnit() );
                }
                else if( CActionMgr::Instance().WasAction( rEvent, m_backAction, NDefs::EAP_DOWN ) )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_BACK_ACTION );

                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_upAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_UP_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_downAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_DOWN_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_leftAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_LEFT_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_rightAction )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_RIGHT_ACTION, pressType );

                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_tabLeft )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_TAB_LEFT, pressType );

                else if( (pressType = CActionMgr::Instance().WasAction( rEvent, m_tabRight )) > NDefs::EAP_IDLE )
                    NGenFunc::DispatchEvent( NMenu::EGE_MENU_TAB_RIGHT, pressType );

                // If none of the predefined actions have been hit, just send the message for processing
                else
                {
                    HandleEventForTrees( rEvent );
                }
            }
        }
        else if( rEvent.type > SDL_USEREVENT )
        {
            // Are we doing menu actions? May need to do some scrolling
            if( (rEvent.type >= NMenu::EGE_MENU_UP_ACTION) && (rEvent.type <= NMenu::EGE_MENU_RIGHT_ACTION) )
            {
                // Free a timer if one happens to be running
                SDL_RemoveTimer(m_scrollTimerId);
                m_scrollTimerId = 0;

                if( rEvent.user.code == NDefs::EAP_DOWN )
                    HandleEventForScrolling( rEvent );
            }

            HandleEventForTrees( rEvent );

            // Set the active state
            SetActiveState();
        }
    }

}   // HandleEvent


/************************************************************************
*    desc:  Handle input events depending on if this is a menu or interface tree
************************************************************************/
void CMenuManager::HandleEventForTrees( const SDL_Event & rEvent )
{
    bool menuActive(false);
    
    for( auto iter : m_pActiveMenuTreeVec )
    {
        // See if there's an active tree
        menuActive |= iter->IsActive();
        
        // Even if a menu tree is not active, it needs to receive events to become active
        iter->HandleEvent( rEvent );
    }

    // Only allow event handling for interface menus when regular menus are not active
    if( !menuActive )
    {
        for( auto iter : m_pActiveInterTreeVec )
        {
            if( iter->IsActive() )
                iter->HandleEvent( rEvent );
        }
    }
    
}   // HandleEventForTrees


/************************************************************************
*    desc:  Handle input events depending on if this is a menu or interface tree
************************************************************************/
void CMenuManager::HandleEventForScrolling( const SDL_Event & rEvent )
{
    if( m_active )
    {
        if( !HandleMenuScrolling( rEvent, m_pActiveMenuTreeVec ) )
        {
            // Only allow event handling for interface menus when regular menus are not active
            HandleMenuScrolling( rEvent, m_pActiveInterTreeVec );
        }
    }
    
}   // HandleEventForScrolling


/************************************************************************
*    desc:  Handle input events for menu scrolling
************************************************************************/
bool CMenuManager::HandleMenuScrolling( 
    const SDL_Event & rEvent, const std::vector<CMenuTree *> & activeTreeVec )
{
    bool menuActive(false);
    
    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->IsActive() )
        {
            menuActive = true;
            
            CScrollParam & rScrollParam = iter->GetScrollParam( rEvent.type );
            
            // If scrolling is allowed, start the timer
            if( rScrollParam.CanScroll( rEvent.type ) )
            {
                m_scrollTimerId = SDL_AddTimer(rScrollParam.GetStartDelay(), ScrollTimerCallbackFunc, &rScrollParam);
                break;
            }
        }
    }
    
    return menuActive;
    
}   // HandleMenuScrolling


/************************************************************************
*    desc:  Update the menu
************************************************************************/
void CMenuManager::Update()
{
    if( m_active )
    {
        if( !UpdateMenu( m_pActiveMenuTreeVec ) )
        {
            // Only allow Updating for interface menus when regular menus are not active
            UpdateMenu( m_pActiveInterTreeVec );
        }
    }

}   // Update


/************************************************************************
*    desc:  Update the menu
************************************************************************/
bool CMenuManager::UpdateMenu( const std::vector<CMenuTree *> & activeTreeVec )
{
    bool menuActive(false);
    
    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->IsActive() )
        {
            menuActive = true;
            iter->Update();
        }
    }
    
    return menuActive;

}   // UpdateMenu


/************************************************************************
*    desc:  Transform the menu
************************************************************************/
void CMenuManager::TransformMenu()
{
    if( m_active )
        Transform( m_pActiveMenuTreeVec );

}   // TransformMenu

void CMenuManager::TransformMenu( const CObject2D & object )
{
    if( m_active )
        Transform( m_pActiveMenuTreeVec, object );

}   // TransformMenu


/************************************************************************
*    desc:  Transform the menu
************************************************************************/
void CMenuManager::TransformInterface()
{
    if( m_active )
        Transform( m_pActiveInterTreeVec );

}   // TransformInterface

void CMenuManager::TransformInterface( const CObject2D & object )
{
    if( m_active )
        Transform( m_pActiveInterTreeVec, object );

}   // TransformInterface


/************************************************************************
*    desc:  Transform the menu
************************************************************************/
void CMenuManager::Transform( const std::vector<CMenuTree *> & activeTreeVec )
{
    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->IsActive() )
            iter->Transform();
    }

}   // Transform

void CMenuManager::Transform( const std::vector<CMenuTree *> & activeTreeVec, const CObject2D & object )
{
    for( auto iter : activeTreeVec )
    {
        // See if there's an active menu
        if( iter->IsActive() )
            iter->Transform( object );
    }

}   // Transform


/************************************************************************
*    desc:  Render menus
************************************************************************/
void CMenuManager::Render( const CMatrix & matrix )
{
    if( m_active )
    {
        for( auto iter : m_pActiveMenuTreeVec )
            if( iter->IsActive() )
                iter->Render( matrix );
    }

}   // Render


/************************************************************************
*    desc:  Render interface menus
************************************************************************/
void CMenuManager::RenderInterface( const CMatrix & matrix )
{
    if( m_active )
    {
        for( auto iter : m_pActiveInterTreeVec )
            if( iter->IsActive() )
                iter->Render( matrix );
    }

}   // RenderInterface


/************************************************************************
*    desc:  Is this menu system active?
************************************************************************/
bool CMenuManager::IsActive()
{
    return m_active;

}   // IsActive


/************************************************************************
*    desc:  Is this standard menu system active?
************************************************************************/
bool CMenuManager::IsMenuActive()
{
    if( m_active )
        for( auto iter : m_pActiveMenuTreeVec )
            if( iter->IsActive() )
                return true;
    
    return false;

}   // IsActive


/************************************************************************
*    desc:  Is a menu item active
************************************************************************/
bool CMenuManager::IsMenuItemActive()
{
    bool result(false);

    if( m_active )
    {
        for( auto iter : m_pActiveMenuTreeVec )
        {
            if( iter->IsActive() )
            {
                result = iter->IsMenuItemActive();

                break;
            }
        }
    }

    return result;

}   // IsMenuItemActive


/************************************************************************
*    desc:  Is a interface item active
************************************************************************/
bool CMenuManager::IsInterfaceItemActive()
{
    bool result(false);

    if( m_active )
    {
        for( auto iter : m_pActiveInterTreeVec )
        {
            if( iter->IsActive() )
            {
                result = iter->IsMenuItemActive();

                break;
            }
        }
    }

    return result;

}   // IsInterfaceItemActive


/************************************************************************
*    desc:  Set the active state
************************************************************************/
void CMenuManager::SetActiveState()
{
    m_active = false;

    for( auto iter : m_pActiveMenuTreeVec )
    {
        if( iter->IsActive() )
        {
            m_active = true;
            break;
        }
    }
    
    if( !m_active )
    {
        for( auto iter : m_pActiveInterTreeVec )
        {
            if( iter->IsActive() )
            {
                m_active = true;
                break;
            }
        }
    }

}   // SetActiveState


/************************************************************************
*    desc:  Get reference to the menu in question
************************************************************************/
CMenu & CMenuManager::GetMenu( const std::string & nameStr )
{
    bool found(false);
    std::map<const std::string, CMenu>::iterator menuIter;
    
    for( auto & iter : m_menuMapMap )
    {
        menuIter = iter.second.find( nameStr );
        if( menuIter != iter.second.end() )
        {
            found = true;
            break;
        }
    }

    if( !found )
    {
        throw NExcept::CCriticalException("UIMenu Error!",
            boost::str( boost::format("Menu being asked for is missing (%s).\n\n%s\nLine: %s")
                % nameStr.c_str() % __FUNCTION__ % __LINE__ ));
    }

    return menuIter->second;

}   // GetMenu


/************************************************************************
*    desc:  Get reference to the first active menu.
*           NOTE: Only call this function if you are certain it will not fail
************************************************************************/
CMenu & CMenuManager::GetActiveMenu()
{
    CMenu * pMenu(nullptr);
    
    for( auto iter : m_pActiveMenuTreeVec )
    {
        if( iter->IsActive() )
        {
            pMenu = &iter->GetActiveMenu();
            break;
        }
    }
    
    if( pMenu == nullptr )
        throw NExcept::CCriticalException("Menu Error!",
            boost::str( boost::format("There is no active menu.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    
    return *pMenu;
    
}   // GetActiveMenu


/************************************************************************
*    desc:  Get a pointer to the active tree
************************************************************************/
CMenuTree * CMenuManager::GetActiveTree()
{
    CMenuTree * pTree(nullptr);
    
    for( auto iter : m_pActiveMenuTreeVec )
    {
        if( iter->IsActive() )
        {
            pTree = iter;
            break;
        }
    }
    
    return pTree;
    
}   // GetActiveMenu


/************************************************************************
*    desc:  Reset the transform
************************************************************************/
void CMenuManager::ResetTransform()
{
    for( auto & grpIter : m_menuMapMap )
        for( auto & iter : grpIter.second )
            iter.second.ForceTransform();

}   // ResetTransform


/************************************************************************
*    desc:  Reset the dynamic positions of menus
************************************************************************/
void CMenuManager::ResetDynamicOffset()
{
    for( auto & grpIter : m_menuMapMap )
        for( auto & iter : grpIter.second )
            iter.second.ResetDynamicPos();

}   // ResetDynamicOffset


/************************************************************************
*    desc:  Timer call back function for 
************************************************************************/
Uint32 CMenuManager::ScrollTimerCallbackFunc( Uint32 interval, void *param )
{
    CScrollParam * pScrollParam = static_cast<CScrollParam *>(param);

    NGenFunc::DispatchEvent( pScrollParam->GetMsg() );

    return pScrollParam->GetScrollDelay();

}   // ScrollTimerCallbackFunc


/************************************************************************
*    desc:  Allow message processing
************************************************************************/
void CMenuManager::Allow( bool allow )
{
    m_allow = allow;
    
}   // Allow
