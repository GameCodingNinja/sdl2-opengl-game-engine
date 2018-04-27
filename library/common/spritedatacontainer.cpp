
/************************************************************************
*    FILE NAME:       spritedatacontainer.cpp
*
*    DESCRIPTION:     Sprite Data Container Class
************************************************************************/

// Physical component dependency
#include <common/spritedatacontainer.h>

// Game lib dependencies
#include <common/spritedata.h>
#include <common/actordata.h>
#include <utilities/deletefuncs.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteDataContainer::CSpriteDataContainer( CSpriteData * pData, int type ) :
    m_type(type),
    m_data(pData)
{
}

CSpriteDataContainer::CSpriteDataContainer( CActorData * pData, int type ) :
    m_type(type),
    m_data(pData)
{
}


/************************************************************************
*    DESC:  destructor                                                             
************************************************************************/
CSpriteDataContainer::~CSpriteDataContainer()
{
    if( m_type == NDefs::SPRITE2D )
        NDelFunc::Delete( m_data.m_pSpriteData );
    
    else if( m_type == NDefs::ACTOR2D )
        NDelFunc::Delete( m_data.m_pActorData );
}


/************************************************************************
*    DESC:  Get the data type                                                             
************************************************************************/
int CSpriteDataContainer::getType() const
{
    return m_type;
}
