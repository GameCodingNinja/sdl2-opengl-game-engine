
/************************************************************************
*    FILE NAME:       uislider.h
*
*    DESCRIPTION:     Class for user interface slider
************************************************************************/

#ifndef __ui_slider_h__
#define __ui_slider_h__

// Physical component dependency
#include <gui/uisubcontrol.h>

class CUISlider : public CUISubControl
{
public:

    enum EOrientation
    {
        EO_HORIZONTAL,
        EO_VERTICAL
    };
    
    // Constructor
    CUISlider( const std::string & group );

    // Destructor
    virtual ~CUISlider();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node ) override;
    
    // Init control
    virtual void Init() override;

    // Set the slider min value
    void SetMinValue( float value );

    // Set the slider max value
    void SetMaxValue( float value );

    // Set the slider position
    void SetSlider( float value = 0 );

    // Set the slider inc value
    void IncSlider( float value = 0 );

    // Get the current value
    float GetValue();
    
    // Is the mouse down
    bool IsMouseDown();
    
    // Deactivate the control
    virtual void DeactivateControl() override;
    
    // Handle the mouse move
    virtual bool OnMouseMove( const SDL_Event & rEvent ) override;

protected:

    // Load the control info from XML node
    virtual void LoadControlFromNode( const XMLNode & node ) override;

    // Handle the select action
    bool HandleSelectAction( const CSelectMsgCracker & msgCracker );

private:

    // Update the slider
    void UpdateSlider();

    // Cap the slider value
    void CapSliderValue();

    // Set the position of the slider
    void SetSliderPos();

    // Inc/Set the slider based on mouse movement
    void IncSliderMovePos( float value );

    // Handle Action messages
    virtual void OnLeftAction( const SDL_Event & rEvent ) override;
    virtual void OnRightAction( const SDL_Event & rEvent ) override;

    // Handle Scroll messages
    virtual void OnLeftScroll( const SDL_Event & rEvent ) override;
    virtual void OnRightScroll( const SDL_Event & rEvent ) override;

    // Handle the slider change
    void HandleSliderChange( float value, bool prepareOnSelect = false );

private:

    // Slider travel distance in pixels
    float m_travelDistPixels;

    // Slider Orientation
    EOrientation m_orientation;

    // Min value
    float m_minValue;

    // Max value
    float m_maxValue;

    // Current value
    float m_curValue;

    // inc value
    float m_incValue;

    // Flag to indicate to display the value as an int
    bool m_displayValueAsInt;

    // Default position of the slider button
    CPoint<float> m_defaultPos;

    // slider button hold flag
    bool m_sliderBtnHold;

    // The current press type
    NDefs::EActionPress m_pressType;

};

#endif  // __ui_slider_h__


