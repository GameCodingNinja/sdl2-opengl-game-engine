
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
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CUISlider::~CUISlider()
{
}


/************************************************************************
*    desc:  Load the control info from XML node
************************************************************************/
void CUISlider::loadFromNode( const XMLNode & node )
{
    CUISubControl::loadFromNode( node );

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
}


/************************************************************************
*    desc:  Load the control specific info from XML node
*
*    param: node - XML node
************************************************************************/
void CUISlider::loadControlFromNode( const XMLNode & node )
{
    // Have the parent load it's stuff
    CUISubControl::loadControlFromNode( node );

    // Get the position of the slider button as the default position
    m_defaultPos = getSubControl()->getPos();

    // Get the slider specific settings
    XMLNode settingsNode = node.getChildNode( "settings" );
    if( !settingsNode.isEmpty() )
    {
        if( settingsNode.isAttributeSet( "maxTravelDistPixels" ) )
            m_travelDistPixels = atof(settingsNode.getAttribute( "maxTravelDistPixels" ));
    }
}


/************************************************************************
*    desc:  Init the control
************************************************************************/
void CUISlider::init()
{
    CUIControl::init();

    updateSlider();
}


/************************************************************************
*    desc:  Handle OnLeftAction message
************************************************************************/
void CUISlider::onLeftAction( const SDL_Event & rEvent )
{
    // Handle the slider change
    if( rEvent.user.code == NDefs::EAP_DOWN )
        handleSliderChange( -m_incValue, true );
}

/************************************************************************
*    desc:  Handle OnRightAction message
************************************************************************/
void CUISlider::onRightAction( const SDL_Event & rEvent )
{
    // Handle the slider change
    if( rEvent.user.code == NDefs::EAP_DOWN )
        handleSliderChange( m_incValue, true );
}

/************************************************************************
*    desc:  Handle OnLeftScroll message
************************************************************************/
void CUISlider::onLeftScroll( const SDL_Event & rEvent )
{
    handleSliderChange( -m_incValue );
}

/************************************************************************
*    desc:  Handle OnRightScroll message
************************************************************************/
void CUISlider::onRightScroll( const SDL_Event & rEvent )
{
    handleSliderChange( m_incValue );
}

/************************************************************************
*    desc:  Handle OnMouseMove message
************************************************************************/
bool CUISlider::onMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::onMouseMove( rEvent );

    if( isActive() && (m_pressType == NDefs::EAP_DOWN) )
    {
        const float oneOverAspectRatio(1.f / CSettings::Instance().getOrthoAspectRatio().getH());

        if( m_orientation == EO_HORIZONTAL )
            incSliderMovePos( (float)rEvent.motion.xrel * oneOverAspectRatio );
        else
            incSliderMovePos( (float)rEvent.motion.yrel * oneOverAspectRatio );

        smartExecuteAction();
    }

    return result;
}


/************************************************************************
*    desc:  Handle the select action
************************************************************************/
bool CUISlider::handleSelectAction( const CSelectMsgCracker & msgCracker )
{
    bool result( isActive() && msgCracker.isDeviceMouse() && isPointInControl( msgCracker.getPos() ) );
    if( result && (msgCracker.getPressType() == getMouseSelectType()) )
    {
        // Get the press type to know if we need to move the slider
        // along with the mouse move
        m_pressType = msgCracker.getPressType();

        if( msgCracker.isPressDown() )
        {
            prepareControlScriptFunction( NUIControl::ECS_SELECTED );

            CPoint<float> dif =
                (msgCracker.getPos() - getSubControl()->getCollisionPos()) *
                    (1.f / CSettings::Instance().getOrthoAspectRatio().getH());

            if( m_orientation == EO_HORIZONTAL )
                incSliderMovePos( dif.x );
            else
                incSliderMovePos( dif.y );

            smartExecuteAction();
        }
    }
    else if( msgCracker.getPressType() != getMouseSelectType() )
    {
        m_pressType = NDefs::EAP_IDLE;
    }

    return result;
}


/************************************************************************
*    desc:  Deactivate the control
************************************************************************/
void CUISlider::deactivateControl()
{
    CUISubControl::deactivateControl();

    m_pressType = NDefs::EAP_IDLE;
}


/************************************************************************
*    desc:  Handle the slider change
************************************************************************/
void CUISlider::handleSliderChange( float value, bool prepareOnSelect )
{
    if( isActive() )
    {
        if( prepareOnSelect )
            prepareControlScriptFunction( NUIControl::ECS_SELECTED );

        // Send a message to blink the button
        NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_SELECTED,
            getSubControl() );

        incSlider(value);

        smartExecuteAction();
    }
}


/************************************************************************
*    desc:  Set the slider min value
************************************************************************/
void CUISlider::setMinValue( float value )
{
    m_minValue = value;
}


/************************************************************************
*    desc:  Set the slider max value
************************************************************************/
void CUISlider::setMaxValue( float value )
{
    m_maxValue = value;
}


/************************************************************************
*    desc:  Set the slider inc value
************************************************************************/
void CUISlider::setSlider( float value )
{
    m_curValue = value;

    // Update the slider
    updateSlider();
}


/************************************************************************
*    desc:  Set the slider inc value
************************************************************************/
void CUISlider::incSlider( float value )
{
    m_curValue += value;

    // Update the slider
    updateSlider();
}


/************************************************************************
*    desc:  Inc the slider based on mouse movement
************************************************************************/
void CUISlider::incSliderMovePos( float value )
{
    m_curValue += value * ((m_maxValue - m_minValue) / m_travelDistPixels);

    // Update the slider
    updateSlider();
}


/************************************************************************
*    desc:  Update the slider
************************************************************************/
void CUISlider::updateSlider()
{
    // Cap current value to it's range
    capSliderValue();

    // Set the position of the slider
    setSliderPos();

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

        createFontString( valueStr );
    }
}


/************************************************************************
*    desc:  Cap the slider value
************************************************************************/
void CUISlider::capSliderValue()
{
    // Cap current value to range
    if( m_curValue < m_minValue )
        m_curValue = m_minValue;

    else if( m_curValue > m_maxValue )
        m_curValue = m_maxValue;
}


/************************************************************************
*    desc:  Set the position of the slider
************************************************************************/
void CUISlider::setSliderPos()
{
    if( std::fabs(m_maxValue) > 0.001f )
    {
        float startPos = -(m_travelDistPixels / 2);
        float pixelsPerValue = m_travelDistPixels / (m_maxValue - m_minValue);
        float pos = startPos + (pixelsPerValue * (m_curValue - m_minValue));

        if( m_orientation == EO_HORIZONTAL )
            getSubControl()->setPos( m_defaultPos + CPoint<float>(pos,0,0) );
        else
            getSubControl()->setPos( m_defaultPos + CPoint<float>(0,-pos,0) );
    }
}


/************************************************************************
*    desc:  Cap the slider value
************************************************************************/
float CUISlider::getValue()
{
    return m_curValue;
}


/************************************************************************
*    desc:  Is the mouse down
************************************************************************/
bool CUISlider::isMouseDown()
{
    return (m_pressType == NDefs::EAP_DOWN);
}
