
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
*    desc:  Constructor
************************************************************************/
CSpriteDataContainer::CSpriteDataContainer( CSpriteData * pData ) :
    m_type(NDefs::SPRITE2D),
    m_data(pData)
{
}   // constructor

CSpriteDataContainer::CSpriteDataContainer( CActorData * pData ) :
    m_type(NDefs::ACTOR2D),
    m_data(pData)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSpriteDataContainer::~CSpriteDataContainer()
{
    if( m_type == NDefs::SPRITE2D )
        NDelFunc::Delete( m_data.m_pSpriteData );
    
    else if( m_type == NDefs::ACTOR2D )
        NDelFunc::Delete( m_data.m_pActorData );
        
}   // destructor


/************************************************************************
*    desc:  Get the data type                                                             
************************************************************************/
int CSpriteDataContainer::GetType() const
{
    return m_type;
}
