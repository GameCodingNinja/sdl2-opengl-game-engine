
/************************************************************************
*    FILE NAME:       menuinterfacestrategy.cpp
*
*    DESCRIPTION:     Menu interface strategy
************************************************************************/

// Physical component dependency
#include <spritestrategy/menuinterfacestrategy.h>

// Game lib dependencies
#include <managers/cameramanager.h>
#include <gui/menumanager.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenuInterfaceStrategy::CMenuInterfaceStrategy()
{
}


/***************************************************************************
*    DESC:  Update the menu
****************************************************************************/
void CMenuInterfaceStrategy::update()
{
    CMenuMgr::Instance().updateInterface();
}


/************************************************************************
*    DESC:  Transform the menu
************************************************************************/
void CMenuInterfaceStrategy::transform()
{
    CMenuMgr::Instance().transformInterface();
}


/***************************************************************************
*    DESC:  Render the menu
****************************************************************************/
void CMenuInterfaceStrategy::render()
{
    const auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    CMenuMgr::Instance().renderInterface( camera.getFinalMatrix() );
}
