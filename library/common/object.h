/************************************************************************
*    FILE NAME:       object.h
*
*    DESCRIPTION:     object class
************************************************************************/

#ifndef __object_h__
#define __object_h__

// Game lib dependencies
#include <common/size.h>
#include <common/point.h>
#include <common/worldvalue.h>
#include <utilities/bitmask.h>

// Standard lib dependencies
#include <cstdint>

// Forward declaration(s)
struct XMLNode;

class CObject
{
public:
    
    CObject();
    CObject( const CObject & obj );
    virtual ~CObject();

    // Load the transform data from node
    void LoadTransFromNode( const XMLNode & node );

    // Set/Get the object's position
    void SetPos( const CPoint<CWorldValue> & position );
    void SetPos( const CPoint<float> & position );
    void SetPos( float x = 0, float y = 0, float z = 0 );
    const CPoint<CWorldValue> & GetPos() const;
    CPoint<float> GetPosFloat();

    // Increment the object's position
    void IncPos( const CPoint<CWorldValue> & position );
    void IncPos( const CPoint<float> & position );
    void IncPos( float x = 0, float y = 0, float z = 0 );

    // Set/Get the object's rotation
    // NOTE: Rotation is stored as radians
    void SetRot( const CPoint<float> & rotation, bool convertToRadians = true );
    void SetRot( float x = 0, float y = 0, float z = 0, bool convertToRadians = true );
    const CPoint<float> & GetRot() const;

    // Increment the object's rotation
    // NOTE: Rotation is stored as radians
    void IncRot( const CPoint<float> & rotation, bool convertToRadians = true );
    void IncRot( float x = 0, float y = 0, float z = 0, bool convertToRadians = true );

    // Set the object's scale
    void SetScale( const CPoint<float> & scale );
    void SetScale( float x = 1, float y = 1, float z = 1 );

    // Increment the object's scale
    void IncScale( const CPoint<float> & scale );
    void IncScale( float x = 1, float y = 1, float z = 1 );

    // Get the object's scale
    const CPoint<float> & GetScale() const;
    
    // Set/Get the object's position
    void SetCenterPos( const CPoint<float> & position );
    void SetCenterPosXYZ( float x = 0, float y = 0, float z = 0 );
    const CPoint<float> & GetCenterPos() const;
    
    // Set the object's crop offset
    void SetCropOffset( const CSize<int16_t> & offset );

    // Set the object's visibility
    void SetVisible( bool value = true );

    // Is the object visible
    bool IsVisible() const;
    
    // Copy the transform to the passed in object
    void CopyTransform( const CObject * pObject );

protected:
    
    // Bitmask settings to record if the object needs to be transformed
    CBitmask<int16_t> m_parameters;

    // Local position
    CPoint<CWorldValue> m_pos;

    // Local Rotation stored in radians
    CPoint<float> m_rot;

    // Local scale
    CPoint<float> m_scale;
    
    // The center point. Point of rotation
    // This is used for defining a different center point
    CPoint<float> m_centerPos;
    
    // Offset due to a sprite sheet crop.
    CSize<int16_t> m_cropOffset;
};

#endif  // __object_h__
