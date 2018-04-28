
/************************************************************************
*    FILE NAME:       ispritestrategy.cpp
*
*    DESCRIPTION:     Sprite Strategy Interface Class - Represents a layer of sprites
************************************************************************/

// Physical component dependency
#include <spritestrategy/ispritestrategy.h>

int iSpriteStrategy::m_spriteInc = 0;

/************************************************************************
*    DESC:  Constructor
************************************************************************/
iSpriteStrategy::iSpriteStrategy() :
    m_idOffset(0),
    m_idDir(1)
{
}


/************************************************************************
*    DESC:  Transform the sprites
************************************************************************/
void iSpriteStrategy::setCameraId( const std::string & cameraId )
{
    m_cameraId = cameraId;
}


/************************************************************************
*    DESC:  Enable event handling
************************************************************************/
void iSpriteStrategy::clearSpriteCounter()
{
    m_spriteInc = 0;
}


/************************************************************************
*    DESC:  Customize sprite id generation by defining an offset
************************************************************************/
void iSpriteStrategy::setIdOffset( int offset )
{
    m_idOffset = offset;
}


/************************************************************************
*    DESC:  Customize sprite id generation by defining a direction
************************************************************************/
void iSpriteStrategy::setIdDir( int dir )
{
    m_idDir = dir;
}
