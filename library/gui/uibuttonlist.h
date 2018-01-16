
/************************************************************************
*    FILE NAME:       uibuttonlist.h
*
*    DESCRIPTION:     Class for user interface buttons
************************************************************************/

#ifndef __ui_button_list_h__
#define __ui_button_list_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

// Game lib dependencies
#include <utilities/bitmask.h>

class CUIButtonList : public CUISubControl
{
public:

    // Constructor
    CUIButtonList( const std::string & group );

    // Destructor
    virtual ~CUIButtonList();
    
    // Load the initial info from XML node
    void LoadFromNode( const XMLNode & node ) override;

    // Set the first inactive control to be active
    bool ActivateFirstInactiveControl() override;

    // Update the display
    void UpdateDisplay( int index );

    // Get the active index
    int GetActiveIndex() const;

protected:

    // Load the control specific info from XML node
    void LoadControlFromNode( const XMLNode & node ) override;

    // Handle message
    void OnStateChange( const SDL_Event & rEvent ) override;

private:
    
    enum EActionResponse
    {
        EAR_UP=1,
        EAR_DOWN=2,
        EAR_LEFT=4,
        EAR_RIGHT=8
    };
    
    // Inc/Dec control
    void Inc();
    void Dec();

    // Handle Action messages
    virtual void OnLeftAction( const SDL_Event & rEvent ) override;
    virtual void OnRightAction( const SDL_Event & rEvent ) override;
    virtual void OnUpAction( const SDL_Event & rEvent ) override;
    virtual void OnDownAction( const SDL_Event & rEvent ) override;

    // Handle Scroll messages
    virtual void OnLeftScroll( const SDL_Event & rEvent ) override;
    virtual void OnRightScroll( const SDL_Event & rEvent ) override;
    virtual void OnUpScroll( const SDL_Event & rEvent ) override;
    virtual void OnDownScroll( const SDL_Event & rEvent ) override;

    // Inc/Dec the list
    void IncList();
    void DecList();

private:

    // Active index into the list
    int m_activeIndex;

    // Index of the image list
    int m_imageLstIndex;
    
    // Indicates if the control responds to up, down, left or right
    CBitmask<uint> m_ActionMask;

};

#endif  // __ui_button_list_h__


