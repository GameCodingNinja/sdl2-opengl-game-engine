
/************************************************************************
*    FILE NAME:       uislider.cpp
*
*    DESCRIPTION:     Class for user interface slider
************************************************************************/

// Physical component dependency
#include <gui/uislider.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/uicontroldefs.h>
#include <utilities/genfunc.h>
#include <utilities/xmlParser.h>
#include <utilities/settings.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>
#include <iostream>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CUISlider::CUISlider( const std::string & group ) :
    CUISubControl( group ),
    m_travelDistPixels(0),
    m_orientation(EO_HORIZONTAL),
    m_minValue(0),
    m_maxValue(0),
    m_curValue(0),
    m_incValue(0),
    m_displayValueAsInt(false),
    m_sliderBtnHold(false),
    m_pressType( NDefs::EAP_IDLE)
{
    m_type = NUIControl::ECT_SLIDER;

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CUISlider::~CUISlider()
{
}   // destructor


/************************************************************************
*    desc:  Load the control info from XML node
************************************************************************/
void CUISlider::LoadFromNode( const XMLNode & node )
{
    CUISubControl::LoadFromNode( node );

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "orientation" ) )
            if( std::strcmp(settingsNode.getAttribute( "orientation" ), "VERT") == 0 )
                m_orientation = EO_VERTICAL;

        if( settingsNode.isAttributeSet( "minValue" ) )
            m_minValue = atof(settingsNode.getAttribute( "minValue" ));

        if( settingsNode.isAttributeSet( "maxValue" ) )
            m_maxValue = atof(settingsNode.getAttribute( "maxValue" ));

        if( settingsNode.isAttributeSet( "incValue" ) )
            m_incValue = atof(settingsNode.getAttribute( "incValue" ));

        if( settingsNode.isAttributeSet( "defValue" ) )
            m_curValue = atof(settingsNode.getAttribute( "defValue" ));

        if( settingsNode.isAttributeSet( "displayValueAsInt" ) )
            if( std::strcmp(settingsNode.getAttribute( "displayValueAsInt" ), "true") == 0 )
                m_displayValueAsInt = true;
    }

}   // LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: node - XML node
************************************************************************/
void CUISlider::LoadControlFromNode( const XMLNode & node )
{
    // Have the parent load it's stuff
    CUISubControl::LoadControlFromNode( node );

    // Get the position of the slider button as the default position
    m_defaultPos = GetSubControl()->GetPos();

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "maxTravelDistPixels" ) )
            m_travelDistPixels = atof(settingsNode.getAttribute( "maxTravelDistPixels" ));
    }

}   // LoadControlFromNode


/************************************************************************
*    desc:  Init the control
************************************************************************/
void CUISlider::Init()
{
    CUIControl::Init();
    
    UpdateSlider();
    
}   // Init


/************************************************************************
*    desc:  Handle OnLeftAction message
************************************************************************/
void CUISlider::OnLeftAction( const SDL_Event & rEvent )
{
    // Handle the slider change
    if( rEvent.user.code == NDefs::EAP_DOWN )
        HandleSliderChange( -m_incValue, true );

}   // OnLeftAction

/************************************************************************
*    desc:  Handle OnRightAction message
************************************************************************/
void CUISlider::OnRightAction( const SDL_Event & rEvent )
{
    // Handle the slider change
    if( rEvent.user.code == NDefs::EAP_DOWN )
        HandleSliderChange( m_incValue, true );

}   // OnRightAction

/************************************************************************
*    desc:  Handle OnLeftScroll message
************************************************************************/
void CUISlider::OnLeftScroll( const SDL_Event & rEvent )
{
    HandleSliderChange( -m_incValue );

}   // OnLeftScrol

/************************************************************************
*    desc:  Handle OnRightScroll message
************************************************************************/
void CUISlider::OnRightScroll( const SDL_Event & rEvent )
{
    HandleSliderChange( m_incValue );

}   // OnRightScroll

/************************************************************************
*    desc:  Handle OnMouseMove message
************************************************************************/
bool CUISlider::OnMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::OnMouseMove( rEvent );
    
    if( IsActive() && (m_pressType == NDefs::EAP_DOWN) )
    {
        const float oneOverAspectRatio(1.f / CSettings::Instance().GetOrthoAspectRatio().getH());
        
        if( m_orientation == EO_HORIZONTAL )
            IncSliderMovePos( (float)rEvent.motion.xrel * oneOverAspectRatio );
        else
            IncSliderMovePos( (float)rEvent.motion.yrel * oneOverAspectRatio );

        SmartExecuteAction();
    }
    
    return result;

}   // OnMouseMove


/************************************************************************
*    desc:  Handle the select action
************************************************************************/
bool CUISlider::HandleSelectAction( const CSelectMsgCracker & msgCracker )
{
    bool result( IsActive() && msgCracker.IsDeviceMouse() && IsPointInControl( msgCracker.GetPos() ) );
    if( result && (msgCracker.GetPressType() == GetMouseSelectType()) )
    {
        // Get the press type to know if we need to move the slider 
        // along with the mouse move
        m_pressType = msgCracker.GetPressType();
        
        if( msgCracker.IsPressDown() )
        {
            PrepareControlScriptFunction( NUIControl::ECS_SELECTED );

            CPoint<float> dif =
                (msgCracker.GetPos() - GetSubControl()->GetCollisionPos()) * 
                    (1.f / CSettings::Instance().GetOrthoAspectRatio().getH());

            if( m_orientation == EO_HORIZONTAL )
                IncSliderMovePos( dif.x );
            else
                IncSliderMovePos( dif.y );

            SmartExecuteAction();
        }
    }
    else if( msgCracker.GetPressType() != GetMouseSelectType() )
    {
        m_pressType = NDefs::EAP_IDLE;
    }

    return result;

}   // HandleSelectAction


/************************************************************************
*    desc:  Deactivate the control
************************************************************************/
void CUISlider::DeactivateControl()
{
    CUISubControl::DeactivateControl();

    m_pressType = NDefs::EAP_IDLE;

}   // DeactivateControl


/************************************************************************
*    desc:  Handle the slider change
************************************************************************/
void CUISlider::HandleSliderChange( float value, bool prepareOnSelect )
{
    if( IsActive() )
    {
        if( prepareOnSelect )
            PrepareControlScriptFunction( NUIControl::ECS_SELECTED );

        // Send a message to blink the button
        NGenFunc::DispatchEvent( 
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_SELECTED,
            GetSubControl() );

        IncSlider(value);

        SmartExecuteAction();
    }

}   // HandleSliderChange


/************************************************************************
*    desc:  Set the slider min value
*  
*    param: value
************************************************************************/
void CUISlider::SetMinValue( float value )
{
    m_minValue = value;

}   // SetMinValue


/************************************************************************
*    desc:  Set the slider max value
*  
*    param: value
************************************************************************/
void CUISlider::SetMaxValue( float value )
{
    m_maxValue = value;

}   // SetMaxValue


/************************************************************************
*    desc:  Set the slider inc value
*  
*    param: float sliderPos
************************************************************************/
void CUISlider::SetSlider( float value )
{
    m_curValue = value;

    // Update the slider
    UpdateSlider();

}   // SetSlider


/************************************************************************
*    desc:  Set the slider inc value
*  
*    param: float slider value
************************************************************************/
void CUISlider::IncSlider( float value )
{
    m_curValue += value;

    // Update the slider
    UpdateSlider();

}   // IncSlider


/************************************************************************
*    desc:  Inc the slider based on mouse movement
*  
*    param: float slider value
************************************************************************/
void CUISlider::IncSliderMovePos( float value )
{
    m_curValue += value * ((m_maxValue - m_minValue) / m_travelDistPixels);

    // Update the slider
    UpdateSlider();

}   // IncSliderMovePos


/************************************************************************
*    desc:  Update the slider
************************************************************************/
void CUISlider::UpdateSlider()
{
    // Cap current value to it's range
    CapSliderValue();

    // Set the position of the slider
    SetSliderPos();

    // Set the slider label if there is one
    if( !m_stringVec.empty() )
    {
        // See if we specified a format string
        std::string formatStr = m_stringVec.back();

        // Format for display
        std::string valueStr;

        if( m_displayValueAsInt )
            valueStr = boost::str( boost::format(formatStr) % (int)m_curValue );
        else
            valueStr = boost::str( boost::format(formatStr) % m_curValue );

        CreateFontString( valueStr );
    }

}   // UpdateSlider


/************************************************************************
*    desc:  Cap the slider value
************************************************************************/
void CUISlider::CapSliderValue()
{
    // Cap current value to range
    if( m_curValue < m_minValue )
        m_curValue = m_minValue;

    else if( m_curValue > m_maxValue )
        m_curValue = m_maxValue;

}   // CapSliderValue


/************************************************************************
*    desc:  Set the position of the slider
************************************************************************/
void CUISlider::SetSliderPos()
{
    if( std::fabs(m_maxValue) > 0.001f )
    {
        float startPos = -(m_travelDistPixels / 2);
        float pixelsPerValue = m_travelDistPixels / (m_maxValue - m_minValue);
        float pos = startPos + (pixelsPerValue * (m_curValue - m_minValue));

        if( m_orientation == EO_HORIZONTAL )
            GetSubControl()->SetPos( m_defaultPos + CPoint<float>(pos,0,0) );
        else
            GetSubControl()->SetPos( m_defaultPos + CPoint<float>(0,-pos,0) );
    }

}   // SetSliderPos


/************************************************************************
*    desc:  Cap the slider value
************************************************************************/
float CUISlider::GetValue()
{
    return m_curValue;

}   // GetValue


/************************************************************************
*    desc:  Is the mouse down
************************************************************************/
bool CUISlider::IsMouseDown()
{
    return (m_pressType == NDefs::EAP_DOWN);

}   // IsMouseDown
