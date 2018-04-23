
/************************************************************************
*    FILE NAME:       light.cpp
*
*    DESCRIPTION:     light class
************************************************************************/

// Physical component dependency
#include <3d/light.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLight::CLight( NDefs::ELightType lightType ) :
    m_type(lightType),
    m_radius(0.f),
    m_enable(false)
{
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CLight::~CLight()
{
}


/************************************************************************
*    desc:  Enable/disable light
************************************************************************/
void CLight::enable( bool value )
{
    m_enable = value;
}


/************************************************************************
*    desc:  Set/Get the position / direction
************************************************************************/
void CLight::setPosDir( CPoint<float> & value )
{
    m_posDir = value;
}

const CPoint<float> & CLight::getPosDir() const
{
    return m_posDir;
}


/************************************************************************
*    desc:  Set/Get the color
************************************************************************/
void CLight::setColor( CPoint<float> & value )
{
    m_color = value;
}

const CPoint<float> & CLight::getColor() const
{
    return m_color;
}
