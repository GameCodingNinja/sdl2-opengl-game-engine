
/************************************************************************
*    FILE NAME:       actionmanager.h
*
*    DESCRIPTION:     Class for handling action mapping
************************************************************************/

#ifndef __action_manager_h__
#define __action_manager_h__

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <common/keycodeaction.h>
#include <common/defs.h>
#include <common/point.h>
#include <gui/menudefs.h>

// Boost lib dependencies
#include <boost/bimap.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <array>
#include <map>

class CActionMgr
{
public:

    // Get the instance of the singleton class
    static CActionMgr & Instance()
    {
        static CActionMgr actionMgr;
        return actionMgr;
    }

    // Load the action mappings from xml
    void LoadActionFromXML( const std::string & filePath );

    // Was this an action
    bool WasAction( const SDL_Event & rEvent, const std::string & actionStr, NDefs::EActionPress );
	NDefs::EActionPress WasAction( const SDL_Event & rEvent, const std::string & actionStr );

    // What was the last devic
    bool WasLastDeviceGamepad();
    bool WasLastDeviceKeyboard();
    bool WasLastDeviceMouse();
    NDefs::EDeviceId GetLastDeviceUsed();

    // Reset the last used device
    void ResetLastUsedDevice();

    // Get the last mouse position
    const CPoint<float> & GetLastMousePos() const;
    
    // Get the action/component strings for the give device id
    bool GetDeviceActionStr( 
        NDefs::EDeviceId deviceId,
        const std::string & actionNameStr,
        std::string & componetIdStr,
        bool & configurable );
    
    // Reset the action
    NDefs::EDeviceId ResetAction(
        const SDL_Event & rEvent,
        const std::string & actionNameStr,
        std::string & componetIdStr );
    
    // Enable/Disable action handling
    void EnableAction( bool value );
    
    // Is action handling allowed
    bool IsAction();
    
    // Save the xml to file
    void SaveToFile();
    
    // Reset all the key bindings to their default settings
    void ResetKeyBindingsToDefault();

private:

    // Constructor
    CActionMgr();

    // Destructor
    ~CActionMgr();
    
    // map types
    typedef boost::bimap< std::string, int > keyCodeMapType;
    typedef std::map< const std::string, CKeyCodeAction > actionMapType;

    // Load action data from xml node
    void LoadActionFromNode(
        const XMLNode & node, 
        keyCodeMapType & keyCodeMap,
        actionMapType & actionMap );

    // Load the keyboard/mouse/gamepad mapping from node
    void LoadAction();
    void LoadKeyboardMappingFromNode( const XMLNode & node );
    void LoadMouseMappingFromNode( const XMLNode & node );
    void LoadGamepadMappingFromNode( const XMLNode & node );

    // Was this an action
    bool WasAction( 
        const int Id,
        const std::string & actionStr,
        const actionMapType & actionMap );
    
    // Get the action/component strings for the keyboard device id
    int GetActionStr(
        const XMLNode & playerVisibleNode, 
        const std::string & actionNameStr, 
        std::string & componetIdStr, 
        bool & configurable );
    
    // Get the string associated with the key code
    bool GetKeyCodeStr(
        keyCodeMapType & keyCodeMap,
        const int keyCode,
        std::string & componetIdStr );
    
    // Get the key code associated with the string
    int GetKeyCode(
        keyCodeMapType & keyCodeMap,
        const std::string & componetIdStr );

private:

    enum
    {
        ANALOG1_UP = SDL_CONTROLLER_BUTTON_MAX,
        ANALOG1_DOWN,
        ANALOG1_LEFT,
        ANALOG1_RIGHT,
        ANALOG2_UP,
        ANALOG2_DOWN,
        ANALOG2_LEFT,
        ANALOG2_RIGHT,
        ANALOG_STICK_MSG_MAX = 20000,
    };
    
    // SDL only defines 2 additional mouse buttons
    // Using this enum to define up to 8 additional mouse buttons
    enum
    {
        MOUSE_BTN_X1 = SDL_BUTTON_X1,
        MOUSE_BTN_X2,
        MOUSE_BTN_X3,
        MOUSE_BTN_X4,
        MOUSE_BTN_X5,
        MOUSE_BTN_X6,
        MOUSE_BTN_X7,
        MOUSE_BTN_X8
    };

    // Maps for parsing codes
    keyCodeMapType m_keyboardKeyCodeMap;
    keyCodeMapType m_mouseKeyCodeMap;
    keyCodeMapType m_gamepadKeyCodeMap;

    // Action maps
    actionMapType m_keyboardActionMap;
    actionMapType m_mouseActionMap;
    actionMapType m_gamepadActionMap;

    // xml node
    XMLNode m_mainNode;

    // Last device used
    NDefs::EDeviceId m_lastDeviceUsed;

    // Last mouse pos
    CPoint<float> m_lastMousePos;

    // Flag to indicate analog state as button
    std::array<NDefs::EActionPress,16> m_analogLXButtonStateAry;
    std::array<NDefs::EActionPress,16> m_analogLYButtonStateAry;
    std::array<NDefs::EActionPress,16> m_analogRXButtonStateAry;
    std::array<NDefs::EActionPress,16> m_analogRYButtonStateAry;
    
    // The save file path
    std::string m_saveFilePath;
    
    // Flag to allow action handling
    bool m_allowAction;
    
    // Unbound key string
    const std::string UNBOUND_KEYCODE_STR_ID;
    const int UNBOUND_KEYCODE_ID;
    const int UNDEFINED_ACTION;
    
    // flag for xml action changes
    bool m_xmlActionChange;

};

#endif  // __action_manager_h__


