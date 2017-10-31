
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
    virtual void Init() override;
    
    // Do some cleanup
    virtual void CleanUp() override;

    // Update the control
    virtual void Update() override;

    // Transform the control
    virtual void Transform( const CObject2D & object ) override;

    // Render the control
    virtual void Render( const CMatrix & matrix ) override;

    // Handle events
    virtual void HandleEvent( const SDL_Event & rEvent ) override;

    // Reset the control to inactive if its not disabled
    virtual void Reset( bool complete = false ) override;

    // Handle the select action
    virtual bool HandleSelectAction( const CSelectMsgCracker & msgCracker ) override;

    // Set the first inactive control to be active
    virtual bool ActivateFirstInactiveControl() override;
    
    // Deactivate the control
    virtual void DeactivateControl() override;

    // Get the pointer to the control if found
    virtual CUIControl * FindControl( const std::string & name ) override;
    virtual CUIControl * FindControl( void * pVoid ) override;
    
    // Get the pointer to the subcontrol if found
    virtual CUIControl * FindSubControl( const std::string & name );
    virtual CUIControl * FindSubControl( void * pVoid );

    // Get the sub control via index
    virtual CUIControl * GetSubControl( uint index = 0 );
    
    // Handle the mouse move
    virtual bool OnMouseMove( const SDL_Event & rEvent ) override;
    
    // Check if control is a sub control
    virtual bool IsSubControl() const override;
    
    // Disable the control
    virtual void DisableControl() override;
    
    // Enable the control to the inactive state
    virtual void EnableControl() override;
    
    // Set the alpha value of this menu
    virtual void SetAlpha( float alpha ) override;
    
    // Get the pointer to the active control
    virtual CUIControl * GetPtrToActiveControl() override;

protected:

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & controlNode ) override;

    // Handle message
    virtual void OnStateChange( const SDL_Event & rEvent ) override;
    
    // Handle the sub control mouse move
    virtual bool OnSubControlMouseMove( const SDL_Event & rEvent );

    // Handle Action messages
    virtual void OnUpAction( const SDL_Event & rEvent );
    virtual void OnDownAction( const SDL_Event & rEvent );
    virtual void OnLeftAction( const SDL_Event & rEvent );
    virtual void OnRightAction( const SDL_Event & rEvent );

    // Handle Scroll messages
    virtual void OnUpScroll( const SDL_Event & rEvent );
    virtual void OnDownScroll( const SDL_Event & rEvent );
    virtual void OnLeftScroll( const SDL_Event & rEvent );
    virtual void OnRightScroll( const SDL_Event & rEvent );
    
    // Handle Tab messages
    virtual void OnTabLeft( const SDL_Event & rEvent );
    virtual void OnTabRight( const SDL_Event & rEvent );
    
    // Deactivate the sub control
    virtual void DeactivateSubControl();

private:

    // Find the reference nodes
    void FindNodes(
        const XMLNode & node,
        int nodeIndex,
        NavHelperMap & navNodeMap );

    // Find the reference nodes
    void SetNodes(
        const XMLNode & node,
        int nodeIndex,
        std::string attr,
        CUIControlNavNode::ENavNode navNode,
        NavHelperMap & navNodeMap );

private:

    // Navigate the menu
    void NavigateMenu( CUIControlNavNode::ENavNode navNode );

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


