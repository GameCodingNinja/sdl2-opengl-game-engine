
/************************************************************************
*    FILE NAME:       actionmanager.cpp
*
*    DESCRIPTION:     Class for handling action mapping
************************************************************************/

// Physical component dependency
#include <managers/actionmanager.h>

// Game lib dependencies
#include <system/device.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CActionMgr::CActionMgr() :
    m_lastDeviceUsed(NDefs::DEVICE_NULL),
    m_allowAction(true),
    UNBOUND_KEYCODE_STR_ID("---"),
    UNBOUND_KEYCODE_ID(-1),
    UNDEFINED_ACTION(-1),
    m_xmlActionChange(false)
{
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(UNBOUND_KEYCODE_STR_ID, UNBOUND_KEYCODE_ID) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RETURN",  SDLK_RETURN) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ESCAPE",  SDLK_ESCAPE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("AC_BACK",  SDLK_AC_BACK) );
    
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW UP",    SDLK_UP) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW DOWN",  SDLK_DOWN) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW LEFT",  SDLK_LEFT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("ARROW RIGHT", SDLK_RIGHT) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("A",  SDLK_a) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("B",  SDLK_b) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("C",  SDLK_c) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("D",  SDLK_d) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("E",  SDLK_e) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F",  SDLK_f) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("G",  SDLK_g) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("H",  SDLK_h) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("I",  SDLK_i) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("J",  SDLK_j) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("K",  SDLK_k) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("L",  SDLK_l) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("M",  SDLK_m) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("N",  SDLK_n) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("O",  SDLK_o) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("P",  SDLK_p) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("Q",  SDLK_q) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("R",  SDLK_r) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("S",  SDLK_s) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("T",  SDLK_t) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("U",  SDLK_u) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("V",  SDLK_v) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("W",  SDLK_w) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("X",  SDLK_x) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("Y",  SDLK_y) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("Z",  SDLK_z) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F1",  SDLK_F1) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F2",  SDLK_F2) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F3",  SDLK_F3) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F4",  SDLK_F4) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F5",  SDLK_F5) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F6",  SDLK_F6) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F7",  SDLK_F7) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F8",  SDLK_F8) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F9",  SDLK_F9) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F10", SDLK_F10) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F11", SDLK_F11) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("F12", SDLK_F12) );
    
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("0",  SDLK_0) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("1",  SDLK_1) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("2",  SDLK_2) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("3",  SDLK_3) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("4",  SDLK_4) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("5",  SDLK_5) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("6",  SDLK_6) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("7",  SDLK_7) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("8",  SDLK_8) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("9",  SDLK_9) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 1", SDLK_KP_1) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 2", SDLK_KP_2) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 3", SDLK_KP_3) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 4", SDLK_KP_4) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 5", SDLK_KP_5) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 6", SDLK_KP_6) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 7", SDLK_KP_7) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 8", SDLK_KP_8) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 9", SDLK_KP_9) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD 0", SDLK_KP_0) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUM LOCK",     SDLK_NUMLOCKCLEAR) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD /",     SDLK_KP_DIVIDE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD *",     SDLK_KP_MULTIPLY) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD -",     SDLK_KP_MINUS) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD +",     SDLK_KP_PLUS) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD ENTER", SDLK_KP_ENTER) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("NUMPAD .",     SDLK_KP_PERIOD) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("LEFT CTRL",   SDLK_LCTRL) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT CTRL",  SDLK_RCTRL) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("LEFT SHIFT",  SDLK_LSHIFT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT SHIFT", SDLK_RSHIFT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("LEFT ALT",    SDLK_LALT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT ALT",   SDLK_RALT) );
    
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PRINT SCREEN", SDLK_PRINTSCREEN) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("SCROLL LOCK",  SDLK_SCROLLLOCK) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PAUSE",        SDLK_PAUSE) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("END",       SDLK_END) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("INSERT",    SDLK_INSERT) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("DELETE",    SDLK_DELETE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("HOME",      SDLK_HOME) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PAGE UP",   SDLK_PAGEUP) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("PAGE DOWN", SDLK_PAGEDOWN) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("BACKSPACE", SDLK_BACKSPACE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("TAB",       SDLK_TAB) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("SPACE",     SDLK_SPACE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(",",         SDLK_COMMA) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("-",         SDLK_MINUS) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(".",         SDLK_PERIOD) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("/",         SDLK_SLASH) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("=",         SDLK_EQUALS) );

    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type(";",  SDLK_SEMICOLON) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("[",  SDLK_LEFTBRACKET) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("\\", SDLK_BACKSLASH) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("]",  SDLK_RIGHTBRACKET) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("`",  SDLK_BACKQUOTE) );
    m_keyboardKeyCodeMap.insert( keyCodeMapType::value_type("'",  SDLK_QUOTE) );

    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type(UNBOUND_KEYCODE_STR_ID, UNBOUND_KEYCODE_ID) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("LEFT MOUSE",           SDL_BUTTON_LEFT) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MIDDLE MOUSE",         SDL_BUTTON_MIDDLE) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT MOUSE",          SDL_BUTTON_RIGHT) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 1",              MOUSE_BTN_X1) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 2",              MOUSE_BTN_X2) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 3",              MOUSE_BTN_X3) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 4",              MOUSE_BTN_X4) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 5",              MOUSE_BTN_X5) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 6",              MOUSE_BTN_X6) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 7",              MOUSE_BTN_X7) );
    m_mouseKeyCodeMap.insert( keyCodeMapType::value_type("MOUSE 8",              MOUSE_BTN_X8) );

    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type(UNBOUND_KEYCODE_STR_ID, UNBOUND_KEYCODE_ID) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("A",                    SDL_CONTROLLER_BUTTON_A) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("B",                    SDL_CONTROLLER_BUTTON_B) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("X",                    SDL_CONTROLLER_BUTTON_X) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("Y",                    SDL_CONTROLLER_BUTTON_Y) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("BACK",                 SDL_CONTROLLER_BUTTON_BACK) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("GUIDE",                SDL_CONTROLLER_BUTTON_GUIDE) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("START",                SDL_CONTROLLER_BUTTON_START) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("UP",                   SDL_CONTROLLER_BUTTON_DPAD_UP) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("RIGHT",                SDL_CONTROLLER_BUTTON_DPAD_RIGHT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("DOWN",                 SDL_CONTROLLER_BUTTON_DPAD_DOWN) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("LEFT",                 SDL_CONTROLLER_BUTTON_DPAD_LEFT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK",              SDL_CONTROLLER_BUTTON_LEFTSTICK) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK",              SDL_CONTROLLER_BUTTON_RIGHTSTICK) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L BUMPER",             SDL_CONTROLLER_BUTTON_LEFTSHOULDER) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R BUMPER",             SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) );

    // Key codes to use analog sticks as buttons
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK UP",    ANALOG1_UP) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK DOWN",  ANALOG1_DOWN) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK LEFT",  ANALOG1_LEFT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("L STICK RIGHT", ANALOG1_RIGHT) );
    
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK UP",    ANALOG2_UP) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK DOWN",  ANALOG2_DOWN) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK LEFT",  ANALOG2_LEFT) );
    m_gamepadKeyCodeMap.insert( keyCodeMapType::value_type("R STICK RIGHT", ANALOG2_RIGHT) );

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CActionMgr::~CActionMgr()
{
}   // destructor


/************************************************************************
*    desc:  Load the action mappings from xml
*  
*    param: std::string filePath - path to file
************************************************************************/
void CActionMgr::LoadActionFromXML( const std::string & filePath )
{
    m_saveFilePath = filePath;
    m_mainNode = XMLNode::openFileHelper( filePath.c_str(), "controllerMapping" );

    // Load the keyboard/mouse/gamepad mapping
    LoadAction();

}   // LoadActionFromXML


/************************************************************************
*    desc:  Load the keyboard/mouse/gamepad mapping
************************************************************************/
void CActionMgr::LoadAction()
{
    // Load the keyboard/mouse/gamepad mapping
    LoadKeyboardMappingFromNode( m_mainNode.getChildNode( "keyboardMapping" ) );
    LoadMouseMappingFromNode( m_mainNode.getChildNode( "mouseMapping" ) );
    LoadGamepadMappingFromNode( m_mainNode.getChildNode( "gamepadMapping" ) );

}   // LoadAction


/************************************************************************
*    desc:  Load the keyboard mapping from node
************************************************************************/
void CActionMgr::LoadKeyboardMappingFromNode( const XMLNode & node )
{
    // Load the player hidden controls
    LoadActionFromNode( node.getChildNode("playerHidden"), m_keyboardKeyCodeMap, m_keyboardActionMap );

    // Load the player visible controls
    LoadActionFromNode( node.getChildNode("playerVisible"), m_keyboardKeyCodeMap, m_keyboardActionMap );

}   // LoadKeyboardMappingFromNode


/************************************************************************
*    desc:  Load the mouse mapping from node
************************************************************************/
void CActionMgr::LoadMouseMappingFromNode( const XMLNode & node )
{
    // Load the player hidden controls
    LoadActionFromNode( node.getChildNode("playerHidden"), m_mouseKeyCodeMap, m_mouseActionMap );

    // Load the player visible controls
    LoadActionFromNode( node.getChildNode("playerVisible"), m_mouseKeyCodeMap, m_mouseActionMap );

}   // LoadMouseMappingFromNode


/************************************************************************
*    desc:  Load the gamepad mapping from node
************************************************************************/
void CActionMgr::LoadGamepadMappingFromNode( const XMLNode & node )
{
    // Load the player hidden controls
    LoadActionFromNode( node.getChildNode("playerHidden"), m_gamepadKeyCodeMap, m_gamepadActionMap );

    // Load the player visible controls
    LoadActionFromNode( node.getChildNode("playerVisible"), m_gamepadKeyCodeMap, m_gamepadActionMap );

}   // LoadGamepadMappingFromNode


/************************************************************************
*    desc:  Load action data from xml node
************************************************************************/
void CActionMgr::LoadActionFromNode( 
    const XMLNode & node, 
    keyCodeMapType & keyCodeMap,
    actionMapType & actionMap )
{
    if( !node.isEmpty() )
    {
        for( int i = 0; i < node.nChildNode(); ++i )
        {
            XMLNode actionNode = node.getChildNode( "actionMap", i );

            // See if we can find the string that represents the key code id
            std::string componentIdStr = actionNode.getAttribute( "componetId" );
            auto keyCodeIter = keyCodeMap.left.find( componentIdStr );

            // Add it in if we found it
            if( keyCodeIter != keyCodeMap.left.end() )
            {
                std::string actionStr = actionNode.getAttribute( "action" );

                // See if the controller action string has already been added
                auto iter = actionMap.find( actionStr );

                if( iter != actionMap.end() )
                {
                    // If it's found, add another id to this map
                    iter->second.SetId( keyCodeIter->second );
                }
                else
                {
                    // Add new action to the map
                    actionMap.emplace( actionStr, keyCodeIter->second );
                }
            }
        }
    }
}   // LoadActionFromXMLNode


/************************************************************************
*    desc:  Was this an action
************************************************************************/
bool CActionMgr::WasAction( const SDL_Event & rEvent, const std::string & actionStr, NDefs::EActionPress actionPress )
{
    if( WasAction( rEvent, actionStr ) == actionPress )
        return true;

    return false;

}   // WasAction


/************************************************************************
*    desc:  Was this an action
************************************************************************/
NDefs::EActionPress CActionMgr::WasAction( const SDL_Event & rEvent, const std::string & actionStr )
{
	NDefs::EActionPress result( NDefs::EAP_IDLE);
    
    if( m_allowAction )
    {
        if( (rEvent.type == SDL_CONTROLLERBUTTONDOWN) || (rEvent.type == SDL_CONTROLLERBUTTONUP) )
        {
            m_lastDeviceUsed = NDefs::GAMEPAD;

            if( WasAction( rEvent.cbutton.button, actionStr, m_gamepadActionMap ) )
            {
                result = NDefs::EAP_UP;

                if( rEvent.type == SDL_CONTROLLERBUTTONDOWN )
                {
                    result = NDefs::EAP_DOWN;
                }
            }
        }
        else if( ((rEvent.type == SDL_KEYDOWN) || (rEvent.type == SDL_KEYUP)) && (rEvent.key.repeat == 0) )
        {
            m_lastDeviceUsed = NDefs::KEYBOARD;

            if( WasAction( rEvent.key.keysym.sym, actionStr, m_keyboardActionMap ) )
            {
                result = NDefs::EAP_UP;

                if( rEvent.type == SDL_KEYDOWN )
                {
                    result = NDefs::EAP_DOWN;
                }
            }
        }
        else if( (rEvent.type == SDL_MOUSEBUTTONDOWN) || (rEvent.type == SDL_MOUSEBUTTONUP) )
        {
            m_lastDeviceUsed = NDefs::MOUSE;

            if( WasAction( rEvent.button.button, actionStr, m_mouseActionMap ) )
            {
                result = NDefs::EAP_UP;

                m_lastMousePos.x = rEvent.button.x;
                m_lastMousePos.y = rEvent.button.y;

                if( rEvent.type == SDL_MOUSEBUTTONDOWN )
                {
                    result = NDefs::EAP_DOWN;
                }
            }
        }
        else if( rEvent.type == SDL_CONTROLLERAXISMOTION )
        {
            if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
            {
                if( m_analogLXButtonStateAry[rEvent.caxis.which] == NDefs::EAP_IDLE )
                {
                    if( (rEvent.caxis.value < -ANALOG_STICK_MSG_MAX) && WasAction( ANALOG1_LEFT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;

                    else if( (rEvent.caxis.value > ANALOG_STICK_MSG_MAX) && WasAction( ANALOG1_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;
                }
                else if( m_analogLXButtonStateAry[rEvent.caxis.which] == NDefs::EAP_DOWN )
                {
                    if( (rEvent.caxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value < 0) && WasAction( ANALOG1_LEFT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;

                    else if( (rEvent.caxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value > 0) && WasAction( ANALOG1_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;
                }

                if( result != NDefs::EAP_IDLE )
                {
                    if( result == NDefs::EAP_DOWN )
                        m_analogLXButtonStateAry[rEvent.caxis.which] = NDefs::EAP_DOWN;
                    else
                        m_analogLXButtonStateAry[rEvent.caxis.which] = NDefs::EAP_IDLE;

                    m_lastDeviceUsed = NDefs::GAMEPAD;
                }
            }
            else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
            {
                if( m_analogLYButtonStateAry[rEvent.caxis.which] == NDefs::EAP_IDLE )
                {
                    if( (rEvent.caxis.value < -ANALOG_STICK_MSG_MAX) && WasAction( ANALOG1_UP, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;

                    else if( (rEvent.caxis.value > ANALOG_STICK_MSG_MAX) && WasAction( ANALOG1_DOWN, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;
                }
                else if( m_analogLYButtonStateAry[rEvent.caxis.which] == NDefs::EAP_DOWN )
                {
                    if( (rEvent.caxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value < 0) && WasAction( ANALOG1_UP, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;

                    else if( (rEvent.caxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value > 0) && WasAction( ANALOG1_DOWN, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;
                }

                if( result != NDefs::EAP_IDLE )
                {
                    if( result == NDefs::EAP_DOWN )
                        m_analogLYButtonStateAry[rEvent.caxis.which] = NDefs::EAP_DOWN;
                    else
                        m_analogLYButtonStateAry[rEvent.caxis.which] = NDefs::EAP_IDLE;

                    m_lastDeviceUsed = NDefs::GAMEPAD;
                }
            }
            else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX )
            {
                if( m_analogRXButtonStateAry[rEvent.caxis.which] == NDefs::EAP_IDLE )
                {
                    if( (rEvent.caxis.value < -ANALOG_STICK_MSG_MAX) && WasAction( ANALOG2_LEFT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;

                    else if( (rEvent.caxis.value > ANALOG_STICK_MSG_MAX) && WasAction( ANALOG2_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;
                }
                else if( m_analogRXButtonStateAry[rEvent.caxis.which] == NDefs::EAP_DOWN )
                {
                    if( (rEvent.caxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value < 0) && WasAction( ANALOG2_LEFT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;

                    else if( (rEvent.caxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value > 0) && WasAction( ANALOG2_RIGHT, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;
                }

                if( result != NDefs::EAP_IDLE )
                {
                    if( result == NDefs::EAP_DOWN )
                        m_analogRXButtonStateAry[rEvent.caxis.which] = NDefs::EAP_DOWN;
                    else
                        m_analogRXButtonStateAry[rEvent.caxis.which] = NDefs::EAP_IDLE;

                    m_lastDeviceUsed = NDefs::GAMEPAD;
                }
            }
            else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY )
            {
                if( m_analogRYButtonStateAry[rEvent.caxis.which] == NDefs::EAP_IDLE )
                {
                    if( (rEvent.caxis.value < -ANALOG_STICK_MSG_MAX) && WasAction( ANALOG2_UP, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;

                    else if( (rEvent.caxis.value > ANALOG_STICK_MSG_MAX) && WasAction( ANALOG2_DOWN, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_DOWN;
                }
                else if( m_analogRYButtonStateAry[rEvent.caxis.which] == NDefs::EAP_DOWN )
                {
                    if( (rEvent.caxis.value > -ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value < 0) && WasAction( ANALOG2_UP, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;

                    else if( (rEvent.caxis.value < ANALOG_STICK_MSG_MAX) && (rEvent.caxis.value > 0) && WasAction( ANALOG2_DOWN, actionStr, m_gamepadActionMap ) )
                        result = NDefs::EAP_UP;
                }

                if( result != NDefs::EAP_IDLE )
                {
                    if( result == NDefs::EAP_DOWN )
                        m_analogRYButtonStateAry[rEvent.caxis.which] = NDefs::EAP_DOWN;
                    else
                        m_analogRYButtonStateAry[rEvent.caxis.which] = NDefs::EAP_IDLE;

                    m_lastDeviceUsed = NDefs::GAMEPAD;
                }
            }
        }
    }

    return result;

}   // WasAction


/************************************************************************
*    desc:  Was this an action
************************************************************************/
bool CActionMgr::WasAction( 
    const int Id,
    const std::string & actionStr,
    const actionMapType & actionMap )
{
    bool result(false);

    // See if the action has already been added
    auto iter = actionMap.find( actionStr );

    // If it's found, see if this is the correct action
    if( iter != actionMap.end() )
    {
        result = iter->second.WasAction( Id );
    }

    return result;

}   // WasAction


/************************************************************************
*    desc:  What was the last device
************************************************************************/
bool CActionMgr::WasLastDeviceGamepad()
{
    return (m_lastDeviceUsed == NDefs::GAMEPAD);
}

bool CActionMgr::WasLastDeviceKeyboard()
{
    return (m_lastDeviceUsed == NDefs::KEYBOARD);
}

bool CActionMgr::WasLastDeviceMouse()
{
    return (m_lastDeviceUsed == NDefs::MOUSE);
}

NDefs::EDeviceId CActionMgr::GetLastDeviceUsed()
{
    return m_lastDeviceUsed;
}


/************************************************************************
*    desc:  Reset the last used device
************************************************************************/
void CActionMgr::ResetLastUsedDevice()
{
    m_lastDeviceUsed = NDefs::DEVICE_NULL;
}


/************************************************************************
*    desc:  Get the last mouse position
************************************************************************/
const CPoint<float> & CActionMgr::GetLastMousePos() const
{
    return m_lastMousePos;
}


/************************************************************************
*    desc:  Get the action/component strings for the give device id
************************************************************************/
bool CActionMgr::GetDeviceActionStr( 
    NDefs::EDeviceId deviceId,
    const std::string & actionNameStr,
    std::string & componetIdStr,
    bool & configurable )
{
    std::string mappingName("keyboardMapping");

    if( deviceId == NDefs::MOUSE )
    {
        mappingName = "mouseMapping";
    }
    else if( deviceId == NDefs::GAMEPAD )
    {
        mappingName = "gamepadMapping";
    }
    
    XMLNode playerVisibleNode = m_mainNode.getChildNode( mappingName.c_str() ).getChildNode( "playerVisible" );
    return (GetActionStr( playerVisibleNode, actionNameStr, componetIdStr, configurable ) > UNDEFINED_ACTION);
    
}   // GetDeviceActionStr


/************************************************************************
*    desc:  Get the action/component strings for the keyboard device id
************************************************************************/
int CActionMgr::GetActionStr(
    const XMLNode & playerVisibleNode, 
    const std::string & actionNameStr, 
    std::string & componetIdStr, 
    bool & configurable )
{
    for( int i = 0; i < playerVisibleNode.nChildNode(); ++i )
    {
        XMLNode actionMapNode = playerVisibleNode.getChildNode( "actionMap", i );

        if( actionNameStr == actionMapNode.getAttribute( "action" ) )
        {
            componetIdStr = actionMapNode.getAttribute( "componetId" );

            configurable = false;

            if( actionMapNode.isAttributeSet( "configurable" ) )
                configurable = ( std::strcmp( actionMapNode.getAttribute("configurable"), "true" ) == 0 );

            return i;
        }
    }
    
    return UNDEFINED_ACTION;

}   // GetActionStr


/************************************************************************
*    desc:  Enable/Disable action handling
************************************************************************/
void CActionMgr::EnableAction( bool value )
{
    m_allowAction = value;
    
}   // EnableAction


/************************************************************************
*    desc:  Is action handling allowed
************************************************************************/
bool CActionMgr::IsAction()
{
    return m_allowAction;
    
}   // EnableAction


/************************************************************************
*    desc:  Reset the action
************************************************************************/
NDefs::EDeviceId CActionMgr::ResetAction(
    const SDL_Event & rEvent,
    const std::string & actionNameStr,
    std::string & componetIdStr )
{
    NDefs::EDeviceId deviceId(NDefs::DEVICE_NULL);
    std::string mappingName;
    int newKeyCodeId;
    keyCodeMapType * pKeyCodeMap;
    actionMapType * pActionMap;

    if( rEvent.type == SDL_KEYUP )
    {
        deviceId = NDefs::KEYBOARD;
        mappingName = "keyboardMapping";
        newKeyCodeId = rEvent.key.keysym.sym;
        pKeyCodeMap = &m_keyboardKeyCodeMap;
        pActionMap = &m_keyboardActionMap;
    }
    else if( rEvent.type == SDL_MOUSEBUTTONUP )
    {
        mappingName = "mouseMapping";
        deviceId = NDefs::MOUSE;
        newKeyCodeId = rEvent.button.button;
        pKeyCodeMap = &m_mouseKeyCodeMap;
        pActionMap = &m_mouseActionMap;
    }
    else if( rEvent.type == SDL_CONTROLLERBUTTONUP )
    {
        mappingName = "gamepadMapping";
        deviceId = NDefs::GAMEPAD;
        newKeyCodeId = rEvent.cbutton.button;
        pKeyCodeMap = &m_gamepadKeyCodeMap;
        pActionMap = &m_gamepadActionMap;
    }
    
    bool configurable;
    std::string oldIdStr;
    
    // If this action ID can be found and is configurable, reset it
    XMLNode playerVisibleNode = m_mainNode.getChildNode( mappingName.c_str() ).getChildNode( "playerVisible" );
    int xmlNodeIndex = GetActionStr( playerVisibleNode, actionNameStr, oldIdStr, configurable );
  
    if( (xmlNodeIndex > UNDEFINED_ACTION) && configurable )
    {
        std::string newKeyCodeStr;
        if( GetKeyCodeStr( *pKeyCodeMap, newKeyCodeId, newKeyCodeStr ) )
        {
            if( newKeyCodeStr != oldIdStr )
                componetIdStr = newKeyCodeStr;
            else
                componetIdStr = UNBOUND_KEYCODE_STR_ID;
            
            int oldKeyCodeId = GetKeyCode( *pKeyCodeMap, oldIdStr );

            // Check for the action to remove the old key code
            auto iter = pActionMap->find( actionNameStr );
            if( iter != pActionMap->end() )
            {
                // Remove the old key code Id
                iter->second.RemoveId( oldKeyCodeId );
                
                // Add the new key code Id
                iter->second.SetId( newKeyCodeId );
                
                // Update the XML node with the change
                XMLNode node = playerVisibleNode.getChildNode( "actionMap", xmlNodeIndex );
                node.updateAttribute(componetIdStr.c_str(), "componetId", "componetId");
                
                m_xmlActionChange = true;
                
                return deviceId;
            }
        }
    }
    
    return NDefs::DEVICE_NULL;
    
}   // ResetAction


/************************************************************************
*    desc:  Get the string associated with the key code
************************************************************************/
bool CActionMgr::GetKeyCodeStr( keyCodeMapType & keyCodeMap, const int keyCode, std::string & componetIdStr )
{
    // See if we can find the string that represents the key code id
    auto keyCodeIter = keyCodeMap.right.find( keyCode );

    // Add it in if we found it
    if( keyCodeIter != keyCodeMap.right.end() )
    {
        componetIdStr = keyCodeIter->second;
        return true;
    }
    
    return false;

}   // GetKeyCodeStr


/************************************************************************
*    desc:  Get the key code associated with the string
************************************************************************/
int CActionMgr::GetKeyCode( keyCodeMapType & keyCodeMap, const std::string & componetIdStr )
{
    // See if we can find the string that represents the key code id
    auto keyCodeIter = keyCodeMap.left.find( componetIdStr );

    // Add it in if we found it
    if( keyCodeIter != keyCodeMap.left.end() )
        return keyCodeIter->second;
    
    return UNBOUND_KEYCODE_ID;

}   // GetKeyCode


/************************************************************************
*    desc:  Save the xml to file
************************************************************************/
void CActionMgr::SaveToFile()
{
    // Save it to file
    if( m_xmlActionChange )
        m_mainNode.writeToFile(m_saveFilePath.c_str(), "utf-8");

    m_xmlActionChange = false;

}   // SaveToFile


/************************************************************************
*    desc:  Reset all the key bindings to their default settings
************************************************************************/
void CActionMgr::ResetKeyBindingsToDefault()
{
    std::vector<std::string> stringPathsVec = {
        "keyboardMapping/playerVisible",
        "mouseMapping/playerVisible",
        "gamepadMapping/playerVisible" };
    
    for( auto & iter : stringPathsVec )
    {
        XMLNode playerVisibleNode = m_mainNode.getChildNodeByPath( iter.c_str() );
        
        for( int i = 0; i < playerVisibleNode.nChildNode("actionMap"); ++i )
        {
            XMLNode node = playerVisibleNode.getChildNode( "actionMap", i );

            if( node.isAttributeSet( "configurable" ) &&
                ( std::strcmp( node.getAttribute("configurable"), "true" ) == 0 ) &&
                node.isAttributeSet( "defaultId" ) )
            {
                node.updateAttribute( node.getAttribute( "defaultId"), "componetId", "componetId");
            }
        }
    }
    
    // Clear out all the actions
    m_keyboardActionMap.clear();
    m_mouseActionMap.clear();
    m_gamepadActionMap.clear();
    
    // Load the keyboard/mouse/gamepad mapping from node
    LoadAction();
    
    // Save the xml to file
    m_mainNode.writeToFile(m_saveFilePath.c_str(), "utf-8");

}   // ResetKeyBindingsToDefault
