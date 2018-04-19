
/************************************************************************
*    FILE NAME:       signalmanager.h
*
*    DESCRIPTION:     Class for handling messaging
************************************************************************/

#ifndef __message_manager_h__
#define __message_manager_h__

// Boost lib dependencies
#include <boost/signals2.hpp>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class CUIControl;
class CMenu;
class iSprite;

class CSignalMgr
{
public:

    // Define the boost signals
    typedef boost::signals2::signal<void (CUIControl *)> SmartGuiControlSignal;
    typedef boost::signals2::signal<void (CMenu *)> SmartMenuSignal;
    typedef boost::signals2::signal<void (const std::string &, iSprite *)> AICreateSignal;
    typedef boost::signals2::signal<void ()> BasicFunction;

    // Get the instance of the singleton class
    static CSignalMgr & Instance()
    {
        static CSignalMgr signalMgr;
        return signalMgr;
    }

    ////////////////////////////////////////////////////////
    // Note: Boost::Bind does not produce unique types
    //       so function overloading does not work which
    //       which is why each connect has it's own function
    ////////////////////////////////////////////////////////

    // Connect/Disconnect to the smart gui signal
    void Connect_SmartGui( const SmartGuiControlSignal::slot_type & slot );
    void Disconnect_SmartGui();

    // Connect to the smart menu signal
    void Connect_SmartMenu( const SmartMenuSignal::slot_type & slot );
    void Disconnect_SmartMenu();
    
    // Connect to the Ai Sprite create signal
    void Connect_AICreate( const AICreateSignal::slot_type & slot );
    void Disconnect_AICreate();
    
    // Connect/Disconnect to the load signal
    void Connect_Load( const BasicFunction::slot_type & slot );
    void Disconnect_Load();
    
    // Connect/Disconnect to the resolution change signal
    void Connect_ResolutionChange( const BasicFunction::slot_type & slot );
    void Disconnect_ResolutionChange();

    // Broadcast smart gui control signal
    void Broadcast( CUIControl * pControl );

    // Broadcast smart menu signal
    void Broadcast( CMenu * pMenu );
    
    // Broadcast AI Sprite create signal
    void Broadcast( const std::string & aiName, iSprite * pSprite );
    
    // Broadcast the load signal
    void Broadcast_LoadSignal();
    
    // Broadcast the resolution change
    void Broadcast_ResolutionChange();

private:

    // Constructor
    CSignalMgr();

    // Destructor
    ~CSignalMgr();

private:

    // Boost signals
    SmartGuiControlSignal m_smartGuiControlSignal;
    SmartMenuSignal m_smartMenuSignal;
    AICreateSignal m_aiCreateSignal;
    BasicFunction m_loadSignal;
    BasicFunction m_resolutionChange;

};

#endif  // __message_manager_h__


