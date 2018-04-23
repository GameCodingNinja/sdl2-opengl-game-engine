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
    void loadTransFromNode( const XMLNode & node );

    // Set/Get the object's position
    void setPos( const CPoint<CWorldValue> & position );
    void setPos( const CPoint<float> & position );
    void setPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );
    const CPoint<CWorldValue> & getPos() const;
    CPoint<float> getPosFloat();

    // Increment the object's position
    void incPos( const CPoint<CWorldValue> & position );
    void incPos( const CPoint<float> & position );
    void incPos( CWorldValue x = 0, CWorldValue y = 0, CWorldValue z = 0 );

    // Set/Get the object's rotation
    // NOTE: Rotation is stored as radians
    void setRot( const CPoint<float> & rotation, bool convertToRadians = true );
    void setRot( float x = 0, float y = 0, float z = 0, bool convertToRadians = true );
    const CPoint<float> & getRot() const;

    // Increment the object's rotation
    // NOTE: Rotation is stored as radians
    void incRot( const CPoint<float> & rotation, bool convertToRadians = true );
    void incRot( float x = 0, float y = 0, float z = 0, bool convertToRadians = true );

    // Set the object's scale
    void setScale( const CPoint<float> & scale );
    void setScale( float x = 1, float y = 1, float z = 1 );

    // Increment the object's scale
    void incScale( const CPoint<float> & scale );
    void incScale( float x = 1, float y = 1, float z = 1 );

    // Get the object's scale
    const CPoint<float> & getScale() const;
    
    // Set/Get the object's position
    void setCenterPos( const CPoint<float> & position );
    void setCenterPos( float x = 0, float y = 0, float z = 0 );
    const CPoint<float> & getCenterPos() const;
    
    // Set the object's crop offset
    void setCropOffset( const CSize<int16_t> & offset );

    // Set the object's visibility
    void setVisible( bool value = true );

    // Is the object visible
    bool isVisible() const;
    
    // Copy the transform to the passed in object
    void copyTransform( const CObject * pObject );

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
