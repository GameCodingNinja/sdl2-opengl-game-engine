
/************************************************************************
*    FILE NAME:       menuinterfacestrategy.cpp
*
*    DESCRIPTION:     Menu interface strategy
************************************************************************/

// Physical component dependency
#include <spritestrategy/menustrategy.h>

// Game lib dependencies
#include <managers/cameramanager.h>
#include <gui/menumanager.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenuStrategy::CMenuStrategy( const std::string & cameraId ) :
    iSpriteStrategy(cameraId)
{
}

CMenuStrategy::CMenuStrategy() :
    iSpriteStrategy()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CMenuStrategy::~CMenuStrategy()
{
}


/***************************************************************************
*    DESC:  Update the menu
****************************************************************************/
void CMenuStrategy::update()
{
    CMenuMgr::Instance().updateInterface();
}


/************************************************************************
*    DESC:  Transform the menu
************************************************************************/
void CMenuStrategy::transform()
{
    CMenuMgr::Instance().transformInterface();
}


/***************************************************************************
*    DESC:  Render the menu
****************************************************************************/
void CMenuStrategy::render()
{
    const auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    CMenuMgr::Instance().renderInterface( camera.getFinalMatrix() );
}
