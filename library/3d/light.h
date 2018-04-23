
/************************************************************************
*    FILE NAME:       light.h
*
*    DESCRIPTION:     light class
************************************************************************/

#ifndef __light_h__
#define __light_h__

// Game lib dependencies
#include <common/point.h>
#include <common/defs.h>

class CLight
{
public:

    // Constructor
    CLight( NDefs::ELightType lightType );

    // Destructor
    virtual ~CLight();
    
    // Enable/disable light
    void enable( bool value = true );
    
    // Set/Get direction
    void setPosDir( CPoint<float> & value );
    const CPoint<float> & getPosDir() const;
    
    // Set/Get color
    void setColor( CPoint<float> & value );
    const CPoint<float> & getColor() const;
    
private:
    
    // Light type
    NDefs::ELightType m_type;

    // Pos/Dir value
    CPoint<float> m_posDir;

    // light color - Light does not use alpha
    CPoint<float> m_color;

    // Radius
    float m_radius;
    
    // is light on or off
    bool m_enable;

};

#endif  // __classtemplate_h__


