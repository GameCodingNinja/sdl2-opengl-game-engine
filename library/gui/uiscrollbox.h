
/************************************************************************
*    FILE NAME:       uiscrollbox.h
*
*    DESCRIPTION:     Class for user interface scroll boxes
************************************************************************/

#ifndef __ui_scroll_box_h__
#define __ui_scroll_box_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>
#include <utilities/bitmask.h>
#include <2d/sprite2d.h>

// Standard lib dependencies
#include <vector>
#include <memory>

// Forward declaration(s)
class CSprite2D;

class CUIScrollBox : public CUISubControl
{
public:

    // Constructor
    CUIScrollBox( const std::string & group );

    // Destructor
    virtual ~CUIScrollBox();
    
    // Load the initial info from XML node
    void LoadFromNode( const XMLNode & node ) override;
    
    // Init control
    void Init() override;
    
    // Do some cleanup
    void CleanUp() override;

    // Update the control
    void Update() override;
    
    // Handle events
    void HandleEvent( const SDL_Event & rEvent ) override;

    // Transform the control
    void Transform( const CObject2D & object ) override;

    // Render the control
    void Render( const CMatrix & matrix ) override;

    // Set the first inactive control to be active
    bool ActivateFirstInactiveControl() override;
    
    // Handle the mouse move
    bool OnMouseMove( const SDL_Event & rEvent ) override;
    
    // Only deactivate sub controls
    void DeactivateControl() override;

    // Get the scroll control vector
    const std::vector<CUIControl *> & GetScrollCtrlVec();
    
    // Set the alpha value of this menu
    void SetAlpha( float alpha ) override;
    
    // Get the pointer to the active control
    CUIControl * GetPtrToActiveControl() override;

protected:

    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & node ) override;

    // Add the scroll control from node
    CUIControl * AddScrollControlFromNode( const XMLNode & node );
    
    // Handle Action messages
    void OnUpAction( const SDL_Event & rEvent ) override;
    void OnDownAction( const SDL_Event & rEvent ) override;
    
    // Handle Scroll messages
    void OnUpScroll( const SDL_Event & rEvent ) override;
    void OnDownScroll( const SDL_Event & rEvent ) override;
    
    // Handle Tab messages
    void OnTabLeft( const SDL_Event & rEvent ) override;
    void OnTabRight( const SDL_Event & rEvent ) override;
    
    // Handle the select action
    bool HandleSelectAction( const CSelectMsgCracker & msgCracker ) override;
    
    // Handle the keyboard/Gamepad scrolling
    void HandleKeyboardGamepadScroll( int scrollVector );
    
    // Handle the page scrolling
    void HandlePageScroll( int scrollVector );
    
    // Handle the sub control mouse move
    bool OnSubControlMouseMove( const SDL_Event & rEvent ) override;
    
    // Get the pointer to the subcontrol if found
    CUIControl * FindSubControl( const std::string & name ) override;
    
    // Find the sub control via is pointer
    CUIControl * FindSubControl( void * pVoid ) override;
    
    // Deactivate the sub control
    void DeactivateSubControl() override;
    
    // Set the start and end positions
    void SetStartEndPos();
    
    // Reposition the scroll controls
    void RepositionScrollControls();

private:
    
    // Handle the time based Scrolling of the contents of the scroll box
    void HandleScrollUpdate();
    
    // Align the scroll box to it's proper stopping point
    void AlignScrollPostion();
    
    // Select the next control
    CBitmask<uint> SelectNextControl( int scrollVector );
    
    // Select the paged control
    void SelectPagedControl( int scrollVector );
    
    // Set the active control to the viewable area
    bool SetActiveCtrlToViewableArea( int scrollVector );
    
    // Scroll to the next control in the viewable area
    CBitmask<uint> ScrollToTheNextCtrlInViewableArea( int scrollVector );
    
    // Is the scroll index in view
    bool InView( int scrollIndex, int scrollVector );
    
    // Init the variables that scroll the contents of the scroll box
    void InitScrolling( int scrollVector, float distance, bool endScrollSelection = true, bool paging = false );
    
    // Get the fractional amount the controls are off within the scroll box
    float GetControlAlignment();
    
    // See if we can activate this scroll control
    bool ActivateScrollCtrl( int scrollControlIndex );
    
    // Do we need to select and reposition the control
    bool SelectAndRepositionCtrl( int scrollVector );
    
private:
    
    const uint IN_VIEWABLE_AREA;
    const uint NEW_ACTIVE_CTRL;

    // Vector list of controls in scroll box
    std::vector<CUIControl *> m_pScrollControlVec;

    // Initial scroll box control offset
    CPoint<float> m_initialOffset;

    // Height to cull
    float m_cullHeight;

    // height of control
    uint m_controlHeight;
    
    // Scroll move counter
    float m_scrollCurPos;

    // Number of controls visible in scroll box
    int m_visibleCount;
    
    // Visible start pos
    int m_visStartPos;
    int m_visEndPos;

    // Max scroll amount
    float m_maxMoveAmount;

    // stencil mask sprite
    std::unique_ptr<CSprite2D> m_upStencilMaskSprite;
    
    // Active scroll control index in this control
    int m_activeScrollCtrl;
    
    // index of first control in scroll box
    int m_firstScrollCtrlIndex;
    
    // Default offsets
    std::vector<float> m_defaultOffsetVec;
    
    // speed members
    float m_scrollSpeed;
    float m_pageSpeed;
    
    // Scroll vector to indicate the control is scrolling
    int m_scrollVector;
    
    // Flag to indicate the control is paging
    bool m_paging;
    
    // Scroll counter
    float m_scrollCounter;

    // Scroll distance
    float m_scrollDistance;
    
    // Flag to indicate scrolling needs to stop
    // but allows the scrolling to finish
    bool m_endScroll;
    
    // Flag to indicate that the scroll message has been sent
    bool m_scrollMsg;
    
    // Flag to allow for end scroll selection
    bool m_endScrollSelection;

};

#endif  // __ui_scroll_box_h__


