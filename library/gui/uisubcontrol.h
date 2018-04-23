
/************************************************************************
*    FILE NAME:       uisubcontrol.h
*
*    DESCRIPTION:     Class for user interface controls with sub-controls
************************************************************************/

#ifndef __ui_sub_control_h__
#define __ui_sub_control_h__

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <gui/uicontrolnavnode.h>

// Standard lib dependencies
#include <vector>
#include <map>

class CUISubControl : public CUIControl
{
public:

    // Define the navigation helper map
    typedef std::map<const std::string, CUIControlNavNode *> NavHelperMap;

    // Constructor
    CUISubControl( const std::string & group );

    // Destructor
    virtual ~CUISubControl();
    
    // Init control
    virtual void init() override;
    
    // Do some cleanup
    virtual void cleanUp() override;

    // Update the control
    virtual void update() override;

    // Transform the control
    virtual void transform( const CObject2D & object ) override;

    // Render the control
    virtual void render( const CMatrix & matrix ) override;

    // Handle events
    virtual void handleEvent( const SDL_Event & rEvent ) override;

    // Reset the control to inactive if its not disabled
    virtual void reset( bool complete = false ) override;

    // Handle the select action
    virtual bool handleSelectAction( const CSelectMsgCracker & msgCracker ) override;

    // Set the first inactive control to be active
    virtual bool activateFirstInactiveControl() override;
    
    // Deactivate the control
    virtual void deactivateControl() override;

    // Get the pointer to the control if found
    virtual CUIControl * findControl( const std::string & name ) override;
    virtual CUIControl * findControl( void * pVoid ) override;
    
    // Get the pointer to the subcontrol if found
    virtual CUIControl * findSubControl( const std::string & name );
    virtual CUIControl * findSubControl( void * pVoid );

    // Get the sub control via index
    virtual CUIControl * getSubControl( uint index = 0 );
    
    // Handle the mouse move
    virtual bool onMouseMove( const SDL_Event & rEvent ) override;
    
    // Check if control is a sub control
    virtual bool isSubControl() const override;
    
    // Disable the control
    virtual void disableControl() override;
    
    // Enable the control to the inactive state
    virtual void enableControl() override;
    
    // Set the alpha value of this menu
    virtual void setAlpha( float alpha ) override;
    
    // Get the pointer to the active control
    virtual CUIControl * getPtrToActiveControl() override;

protected:

    // Load the control specific info from XML node
    virtual void loadControlFromNode( const XMLNode & controlNode ) override;

    // Handle message
    virtual void onStateChange( const SDL_Event & rEvent ) override;
    
    // Handle the sub control mouse move
    virtual bool onSubControlMouseMove( const SDL_Event & rEvent );

    // Handle Action messages
    virtual void onUpAction( const SDL_Event & rEvent );
    virtual void onDownAction( const SDL_Event & rEvent );
    virtual void onLeftAction( const SDL_Event & rEvent );
    virtual void onRightAction( const SDL_Event & rEvent );

    // Handle Scroll messages
    virtual void onUpScroll( const SDL_Event & rEvent );
    virtual void onDownScroll( const SDL_Event & rEvent );
    virtual void onLeftScroll( const SDL_Event & rEvent );
    virtual void onRightScroll( const SDL_Event & rEvent );
    
    // Handle Tab messages
    virtual void onTabLeft( const SDL_Event & rEvent );
    virtual void onTabRight( const SDL_Event & rEvent );
    
    // Deactivate the sub control
    virtual void deactivateSubControl();

private:

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

private:

    // Navigate the menu
    void navigateMenu( CUIControlNavNode::ENavNode navNode );

protected:

    // Vector list of sub-controls
    std::vector<CUIControl *> m_pSubControlVec;

    // Vector list of navigation nodes
    std::vector<CUIControlNavNode *> m_pControlNodeVec;

    // Current active node
    // NOTE: This variable does not own it's pointers.
    CUIControlNavNode * m_pActiveNode;

    // A sub control is a container for other controls so normally
    // it doesn't respont to select messages. There can be a case
    // where this control needs to respond.
    bool m_respondsToSelectMsg;

};

#endif  // __ui_sub_control_h__


