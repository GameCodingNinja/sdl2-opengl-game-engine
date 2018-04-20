
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
}   // constructor

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
