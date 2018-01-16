
/************************************************************************
*    FILE NAME:       signalmanager.cpp
*
*    DESCRIPTION:     Class for handling messaging
************************************************************************/

// Physical component dependency
#include <managers/signalmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <common/defs.h>
#include <gui/uicontrol.h>
#include <gui/menu.h>
#include <2d/isprite2d.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSignalMgr::CSignalMgr()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSignalMgr::~CSignalMgr()
{
}   // destructor


/************************************************************************
*    desc:  Connect/Disconnect to the smart gui control signal
************************************************************************/
void CSignalMgr::Connect_SmartGui( const SmartGuiControlSignal::slot_type & slot )
{
    m_smartGuiControlSignal.connect(slot);

}   // Connect_SmartGui

void CSignalMgr::Disconnect_SmartGui()
{
    m_smartGuiControlSignal.disconnect_all_slots();

}   // Disconnect_SmartGui


/************************************************************************
*    desc:  Connect/Disconnect to the smart menu signal
************************************************************************/
void CSignalMgr::Connect_SmartMenu( const SmartMenuSignal::slot_type & slot )
{
    m_smartMenuSignal.connect(slot);

}   // Connect_SmartMenu

void CSignalMgr::Disconnect_SmartMenu()
{
    m_smartMenuSignal.disconnect_all_slots();

}   // Disconnect_SmartMenu


/************************************************************************
*    desc:  Connect/Disconnect to the Ai create signal
************************************************************************/
void CSignalMgr::Connect_AICreate( const AICreateSignal::slot_type & slot )
{
    m_aiCreateSignal.connect(slot);

}   // Connect_AISpriteCreate

void CSignalMgr::Connect_AICreate3D( const AICreateSignal3D::slot_type & slot )
{
    m_aiCreateSignal3d.connect( slot );

}   // Connect_AISpriteCreate

void CSignalMgr::Disconnect_AICreate()
{
    m_aiCreateSignal.disconnect_all_slots();

}   // Disconnect_AISpriteCreate


/************************************************************************
*    desc:  Connect/Disconnect to the load signal
************************************************************************/
void CSignalMgr::Connect_Load( const BasicFunction::slot_type & slot )
{
    m_loadSignal.connect(slot);

}   // Connect_Load

void CSignalMgr::Disconnect_Load()
{
    m_loadSignal.disconnect_all_slots();

}   // Disconnect_Load


/************************************************************************
*    desc:  Connect/Disconnect to the resolution change signal
************************************************************************/
void CSignalMgr::Connect_ResolutionChange( const BasicFunction::slot_type & slot )
{
    m_resolutionChange.connect(slot);

}   // Connect_ResolutionChange

void CSignalMgr::Disconnect_ResolutionChange()
{
    m_resolutionChange.disconnect_all_slots();

}   // Disconnect_ResolutionChange


/************************************************************************
*    desc:  Broadcast smart gui control signal
************************************************************************/
void CSignalMgr::Broadcast( CUIControl * pControl )
{
    m_smartGuiControlSignal(pControl);

}   // Broadcast

/************************************************************************
*    desc:  Broadcast smart menu signal
************************************************************************/
void CSignalMgr::Broadcast( CMenu * pMenu )
{
    m_smartMenuSignal(pMenu);

}   // Broadcast

/************************************************************************
*    desc:  Broadcast AI Sprite create signal
************************************************************************/
void CSignalMgr::Broadcast( const std::string & aiName, iSprite2D * pSprite )
{
    m_aiCreateSignal(aiName, pSprite);

}   // Broadcast

void CSignalMgr::Broadcast( const std::string & aiName, iSprite3D * pSprite )
{
    m_aiCreateSignal3d( aiName, pSprite );

}   // Broadcast

/************************************************************************
*    desc:  Broadcast the load signal
************************************************************************/
void CSignalMgr::Broadcast_LoadSignal()
{
    m_loadSignal();

}   // Broadcast_LoadSignal

/************************************************************************
*    desc:  Broadcast the resolution change
************************************************************************/
void CSignalMgr::Broadcast_ResolutionChange()
{
    m_resolutionChange();

}   // Broadcast_ResolutionChange
