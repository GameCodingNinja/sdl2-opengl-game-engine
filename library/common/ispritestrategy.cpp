
/************************************************************************
*    FILE NAME:       ispritestrategy.cpp
*
*    DESCRIPTION:     Sprite Strategy Interface Class - Represents a layer of sprites
************************************************************************/

// Physical component dependency
#include <common/ispritestrategy.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
iSpriteStrategy::iSpriteStrategy()
{
}   // constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
iSpriteStrategy::~iSpriteStrategy()
{
}   // Destructor


/************************************************************************
*    desc:  Transform the sprites
************************************************************************/
void iSpriteStrategy::SetCameraId( const std::string & cameraId )
{
    m_cameraId = cameraId;

}   // Transform
