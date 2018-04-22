
/************************************************************************
*    FILE NAME:       dynamicoffset.h
*
*    DESCRIPTION:     Dynamic Offset class
************************************************************************/  

#ifndef __dynamic_offset_h__
#define __dynamic_offset_h__

// Game lib dependencies
#include <common/point.h>
#include <common/size.h>
#include <common/defs.h>
#include <utilities/bitmask.h>

class CDynamicOffset
{
public:

    enum EDynamicOffset
    {
        EDO_NULL=0,
        EDO_LEFT=1,
        EDO_RIGHT=2,
        EDO_HORZ_CENTER=4,
        EDO_TOP=8,
        EDO_BOTTOM=16,
        EDO_VERT_CENTER=32,
    };

    // Add to the bit mask
    void Add( EDynamicOffset value )
    { m_parameters.Add( value ); }

    // Set/Get X
    void SetX( float value )
    { point.x = value; }

    // Set/Get Y
    void SetY( float value )
    { point.y = value; }

    // Is the dynamic offset being used
    bool IsEmpty() const
    { return m_parameters.IsEmpty(); }

    // Get the dynamic position
    CPoint<float> GetPos( CSize<float> defaultHalfSize )
    {
        CPoint<float> pos;
        
        // Strip out any fractional component for correct rendering
        defaultHalfSize.round();

        if( m_parameters.IsSet( CDynamicOffset::EDO_LEFT ) )
            pos.x = -(defaultHalfSize.w - point.x);

        else if( m_parameters.IsSet( CDynamicOffset::EDO_RIGHT ) )
            pos.x = defaultHalfSize.w - point.x;

        else if( m_parameters.IsSet( CDynamicOffset::EDO_HORZ_CENTER ) )
            pos.x = point.x;

        if( m_parameters.IsSet( CDynamicOffset::EDO_TOP ) )
            pos.y = defaultHalfSize.h - point.y;
            
        else if( m_parameters.IsSet( CDynamicOffset::EDO_BOTTOM ) )
            pos.y = -(defaultHalfSize.h - point.y);

        else if( m_parameters.IsSet( EDO_VERT_CENTER ) )
            pos.y = point.y;

        return pos;
    }

private:

    // bit mask parameters
    CBitmask<uint> m_parameters;

    // offset
    CPoint<float> point;

};

#endif  // __dynamic_offset_h__

