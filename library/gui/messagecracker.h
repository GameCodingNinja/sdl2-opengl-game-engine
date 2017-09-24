
/************************************************************************
*    FILE NAME:       messagecracker.h
*
*    DESCRIPTION:     Message crackers
************************************************************************/

#ifndef __message_cracker_h__
#define __message_cracker_h__

// Game lib dependencies
#include <common/defs.h>
#include <common/point.h>

class CSelectMsgCracker
{
public:

    CSelectMsgCracker()
    { m_unionValue.m_uintValue = 0; }

    // Set/Get the press type
    void SetPressType( uint value )
    { m_unionValue.m_packedUint.m_pressType = value; }
    NDefs::EActionPress GetPressType() const
    { return NDefs::EActionPress(m_unionValue.m_packedUint.m_pressType); }

    // Set/Get the device Id
    void SetDeviceId( uint value )
    { m_unionValue.m_packedUint.m_deviceId = value; }
    NDefs::EDeviceId GetDeviceId() const
    { return NDefs::EDeviceId(m_unionValue.m_packedUint.m_deviceId); }

    // Set/Get the x
    void SetX( uint value )
    { m_unionValue.m_packedUint.m_x = value; }
    uint GetX() const
    { return m_unionValue.m_packedUint.m_x; }

    // Set/Get the y
    void SetY( uint value )
    { m_unionValue.m_packedUint.m_y = value; }
    uint GetY() const
    { return m_unionValue.m_packedUint.m_y; }

    // Set/Get the packed uint
    void SetPackedUnit( uint value )
    { m_unionValue.m_uintValue = value; }
    uint GetPackedUnit() const
    { return m_unionValue.m_uintValue; }
    
    // Is this device a mouse?
    bool IsDeviceMouse() const
    { return (GetDeviceId() == NDefs::MOUSE); }
    
    // Is the a press down?
    bool IsPressDown() const
    { return (GetPressType() == NDefs::EAP_DOWN); }
    
    // Is the a press down?
    bool IsPressUp() const
    { return (GetPressType() == NDefs::EAP_UP); }
    
    // Get the mouse position
    CPoint<float> GetPos() const
    { CPoint<float> pos( GetX(), GetY() );
      return pos; }

private:

    union
    {
        struct
        {
            uint m_pressType : 2;
            uint m_deviceId  : 2;
            uint m_x         : 14;
            uint m_y         : 14;
        } m_packedUint;

        uint m_uintValue;

    } m_unionValue;
    
    //CPoint<float> m_pos;
};


#endif  // __message_cracker_h__


