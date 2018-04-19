
/************************************************************************
*    FILE NAME:       isprite.cpp
*
*    DESCRIPTION:     iSprite Class
************************************************************************/

// Physical component dependency
#include <common/isprite.h>

/************************************************************************
*    desc:  Is this a 2D sprite?
************************************************************************/  
bool iSprite::IsSprite2D()
{
    return m_parameters.IsSet( NDefs::SPRITE2D );
            
}   // IsSprite2D


/************************************************************************
*    desc:  Is this a 2D actor?
************************************************************************/  
bool iSprite::IsActor2D()
{
    return m_parameters.IsSet( NDefs::ACTOR2D );
            
}   // IsActor2D


/************************************************************************
*    desc:  Is this a 3D sprite?
************************************************************************/  
bool iSprite::IsSprite3D()
{
    return m_parameters.IsSet( NDefs::SPRITE3D );
            
}   // IsSprite3D


/************************************************************************
*    desc:  Is this a 3D actor?
************************************************************************/  
bool iSprite::IsActor3D()
{
    return m_parameters.IsSet( NDefs::ACTOR3D );
            
}   // IsActor3D