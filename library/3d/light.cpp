
/************************************************************************
*    FILE NAME:       light.cpp
*
*    DESCRIPTION:     light class
************************************************************************/

// Physical component dependency
#include <3d/light.h>

// Game dependencies

// Game lib dependencies

// Boost lib dependencies

// Standard lib dependencies


/************************************************************************
*    desc:  Constructor
************************************************************************/
CLight::CLight( NDefs::ELightType lightType ) :
    m_type(lightType),
    m_radius(0.f),
    m_enable(false)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CLight::~CLight()
{
}   // destructor


/************************************************************************
*    desc:  Enable/disable light
************************************************************************/
void CLight::Enable( bool value )
{
    m_enable = value;
}


/************************************************************************
*    desc:  Set/Get the position / direction
************************************************************************/
void CLight::SetPosDir( CPoint<float> & value )
{
    m_posDir = value;
}

const CPoint<float> & CLight::GetPosDir() const
{
    return m_posDir;
}


/************************************************************************
*    desc:  Set/Get the color
************************************************************************/
void CLight::SetColor( CPoint<float> & value )
{
    m_color = value;
}

const CPoint<float> & CLight::GetColor() const
{
    return m_color;
}

