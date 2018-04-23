
/************************************************************************
*    FILE NAME:       menu.h
*
*    DESCRIPTION:     Class for user interface menu
************************************************************************/

#ifndef __menu_h__
#define __menu_h__

// Physical component dependency
#include <2d/object2d.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/uicontrolnavnode.h>
#include <gui/uicontrol.h>
#include <gui/scrollparam.h>
#include <script/scriptcomponent.h>
#include <common/dynamicoffset.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <memory>

// Forward declaration(s)
class CSprite2D;

class CMenu : public CObject2D, boost::noncopyable
{
public:

    // Define the navigation helper map
    typedef std::map<const std::string, CUIControlNavNode *> NavHelperMap;

    CMenu( const std::string & name, const std::string & group );
    virtual ~CMenu();

    // Load the menu info from XML file
    void loadFromXML( const std::string & filePath );
    
    // Init the menu
    void init();
    
    // Cleanup the menu controls. Currently only for font usage
    void cleanUp();

    // Activate this menu because it's probably a root menu
    void activateMenu();

    // Update the menu
    void update();

    // Transform the menu
    void transform() override;
    void transform( const CObject2D & object ) override;

    // Get the name of the menu
    const std::string & getName() const;

    // do the render
    void render( const CMatrix & matrix );

    // Handle events
    void handleEvent( const SDL_Event & rEvent );

    // Reset all controls
    void reset();

    // Get the pointer to the control in question
    CUIControl * getPtrToControl( const std::string & nameStr );

    // Get the pointer to the active control
    CUIControl * getPtrToActiveControl();

    // Load the dynamic offset data from node
    void loadDynamicOffsetFromNode( const XMLNode & node );

    // Set the dynamic position
    void resetDynamicPos();

    // Does this menu use dynamic offsets
    bool isDynamicOffset();

    // Get the scroll params
    CScrollParam & getScrollParam( int msg );
    
    // Set the smart menu pointer. This class owns the pointer
    void setSmartGui( class CSmartGuiMenu * pSmartGuiMenu );
    
    // Do any smart create
    void smartCreate();
    
    // Is the menu idle
    bool isIdle();
    
    // Is the menu active
    bool isActive();
    
    // Set/Get the alpha value of this menu
    void setAlpha( float alpha );
    float getAlpha();

private:
    
    // Set the dynamic position
    void setDynamicPos();
    
    // Init the script functions
    void initScriptFunctions( const XMLNode & node );

    // Load a static sprite from an XML node
    void loadStaticSpriteFromNode( const XMLNode & node );

    // Load static controls from an XML node
    void loadStaticControlFromNode( XMLNode & node );

    // Load mouse only controls from an XML node
    void loadMouseOnlyControlFromNode( const XMLNode & node );

    // Load a control from an XML node
    void loadControlFromNode( XMLNode & node, NavHelperMap & navNodeMap );

    // Find the reference nodes
    void findNodes(
        const XMLNode & node,
        int nodeIndex,
        NavHelperMap & navNodeMap );

    // Find the reference nodes
    void setNodes(
        const XMLNode & node,
        int nodeIndex,
        std::string attr,
        CUIControlNavNode::ENavNode navNode,
        NavHelperMap & navNodeMap );

    // Navigate the menu
    void navigateMenu( CUIControlNavNode::ENavNode navNodeAction );

    // Message handlers
    void onUpAction( const SDL_Event & rEvent );
    void onDownAction( const SDL_Event & rEvent );
    void onLeftAction( const SDL_Event & rEvent );
    void onRightAction( const SDL_Event & rEvent );
    void onSelectAction( const SDL_Event & rEvent );
    void onSetActiveControl( const SDL_Event & rEvent );
    void onReactivate( const SDL_Event & rEvent );
    void onTransIn( const SDL_Event & rEvent );
    void onTransOut( const SDL_Event & rEvent );
    void onMouseMove( const SDL_Event & rEvent );
    
    // Prepare the script function to run
    void prepare( const std::string & scriptFuncId );

    // Set the first inactive control to be active
    void activateFirstInactiveControl();
    
    // Do any smart event handling
    void smartHandleEvent( const SDL_Event & rEvent );

private:

    // This menu's name
    std::string m_name;

    // Group name
    std::string m_group;

    // Deque of menu static sprites
    std::deque<CSprite2D> m_spriteDeq;

    // Vector list of static controls
    std::vector<CUIControl *> m_pStaticControlVec;

    // Vector list of mouse only controls
    std::vector<CUIControl *> m_pMouseOnlyControlVec;

    // Vector list of controls
    std::vector<CUIControl *> m_pControlVec;

    // Vector list of navigation nodes
    std::vector<CUIControlNavNode *> m_pControlNodeVec;

    // Map container of controls for easy name access
    // NOTE: This container does not own it's pointers.
    std::map<const std::string, CUIControl *> m_pControlMap;

    // Current active node
    CUIControlNavNode * m_pActiveNode;

    // menu state
    NMenu::EMenuState m_state;

    // Dynamic offset
    CDynamicOffset m_dynamicOffset;

    // Scrolling parameters
    CScrollParam m_scrollParam;
    
    // Base smart Gui control scoped pointer
    std::unique_ptr<class CSmartGuiMenu> m_upSmartGui;
    
    // The script conponent
    CScriptComponent m_scriptComponent;
    
    // Script function map. Tie events to script functions
    std::map<const std::string, std::string> m_scriptFunctionMap;
    
    // menu alpha value
    float m_alpha;

};

#endif  // __menu_h__


