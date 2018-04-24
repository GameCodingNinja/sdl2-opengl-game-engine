
/************************************************************************
*    FILE NAME:       ispritestrategy.cpp
*
*    DESCRIPTION:     Sprite Strategy Interface Class - Represents a layer of sprites
************************************************************************/

// Physical component dependency
#include <spritestrategy/ispritestrategy.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
iSpriteStrategy::iSpriteStrategy(const std::string & cameraId) :
    m_cameraId(cameraId)
{
}

iSpriteStrategy::iSpriteStrategy()
{
}


/************************************************************************
*    desc:  Destructor
************************************************************************/
iSpriteStrategy::~iSpriteStrategy()
{
}


/************************************************************************
*    desc:  Transform the sprites
************************************************************************/
void iSpriteStrategy::setCameraId( const std::string & cameraId )
{
    m_cameraId = cameraId;
}
