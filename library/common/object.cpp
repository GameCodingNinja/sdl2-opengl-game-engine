/************************************************************************
*    FILE NAME:       object.cpp
*
*    DESCRIPTION:     object class
************************************************************************/

// Physical component dependency
#include <common/object.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CObject::CObject() :
    m_parameters(NDefs::VISIBLE),
    m_scale(1,1,1)
{
}   // Constructor

// Copy constructor
CObject::CObject( const CObject & obj ) :
    m_parameters( obj.m_parameters ),
    m_pos( obj.m_pos ),
    m_rot( obj.m_rot ),
    m_scale( obj.m_scale ),
    m_centerPos( obj.m_centerPos ),
    m_cropOffset( obj.m_cropOffset )
{
}   // Constructor


/************************************************************************
*    desc:  Destructor
************************************************************************/
CObject::~CObject()
{
}   // Destructor


/************************************************************************
*    desc:  Get the object's world position
************************************************************************/
const CPoint<CWorldValue> & CObject::GetPos() const
{
    return m_pos;

}   // GetPos


CPoint<float> CObject::GetPosFloat()
{
    return m_pos;

}   // GetPos


/************************************************************************
*    desc:  Set the object's position
************************************************************************/
void CObject::SetPos( const CPoint<CWorldValue> & position )
{
    m_parameters.Add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos = position;

}   // SetPos

void CObject::SetPos( const CPoint<float> & position )
{
    m_parameters.Add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos = position;

}   // SetPos

void CObject::SetPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    m_parameters.Add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos.Set( x, y, z );

}   // SetPos


/************************************************************************
*    desc:  Inc the object's float position
************************************************************************/
void CObject::IncPos( const CPoint<CWorldValue> & position )
{
    m_parameters.Add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos += position;

}   // IncPos

// For AngelScript
void CObject::IncPos( const CPoint<float> & position )
{
    m_parameters.Add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos += position;

}   // SetPos

void CObject::IncPos( CWorldValue x, CWorldValue y, CWorldValue z )
{
    m_parameters.Add( NDefs::TRANSLATE | NDefs::TRANSFORM );

    m_pos.Inc( x, y, z );

}   // IncPos


/************************************************************************
*    desc:  Set the object's rotation
*           NOTE: Rotation is stored as radians
************************************************************************/
void CObject::SetRot( const CPoint<float> & rotation, bool convertToRadians )
{
    m_parameters.Add( NDefs::ROTATE | NDefs::TRANSFORM );

    m_rot = rotation;
    
    if( convertToRadians )
        m_rot = rotation * defs_DEG_TO_RAD;
    else
        m_rot = rotation;

}   // SetRot

void CObject::SetRot( float x, float y, float z, bool convertToRadians )
{
    m_parameters.Add( NDefs::ROTATE | NDefs::TRANSFORM );
    
    if( convertToRadians )
        m_rot.Set( x * defs_DEG_TO_RAD, y * defs_DEG_TO_RAD, z * defs_DEG_TO_RAD );
    else
        m_rot.Set( x, y, z );

}   // SetRot


/************************************************************************
*    desc:  Inc the pre-translation matrix
*           NOTE: Rotation is stored as radians
************************************************************************/
void CObject::IncRot( const CPoint<float> & rotation, bool convertToRadians )
{
    m_parameters.Add( NDefs::ROTATE | NDefs::TRANSFORM );

    if( convertToRadians )
        m_rot += rotation * defs_DEG_TO_RAD;
    else
        m_rot += rotation;

    m_rot.Cap(360.0f * defs_DEG_TO_RAD);

}   // IncRot

void CObject::IncRot( float x, float y, float z, bool convertToRadians )
{
    m_parameters.Add( NDefs::ROTATE | NDefs::TRANSFORM );
    
    if( convertToRadians )
        m_rot.Inc( x * defs_DEG_TO_RAD, y * defs_DEG_TO_RAD, z * defs_DEG_TO_RAD );
    else
        m_rot.Inc( x, y, z );

}   // IncRot


/************************************************************************
*    desc:  Get the object's rotation in radians
*           NOTE: Rotation is stored as radians
************************************************************************/
const CPoint<float> & CObject::GetRot() const
{
    return m_rot;

}   // GetRot


/************************************************************************
*    desc:  Set the object's scale
************************************************************************/
void CObject::SetScale( const CPoint<float> & scale )
{
    m_parameters.Add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale = scale;

}   // SetScale

void CObject::SetScale( float x, float y, float z )
{
    m_parameters.Add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale.Set( x, y, z );

}   // SetScale


/************************************************************************
*    desc:  Inc the object's scale
************************************************************************/
void CObject::IncScale( const CPoint<float> & scale )
{
    m_parameters.Add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale += scale;

}   // IncScale

void CObject::IncScale( float x, float y, float z )
{
    m_parameters.Add( NDefs::SCALE | NDefs::TRANSFORM );

    m_scale.Inc( x, y, z );

}   // IncScale


/************************************************************************
*    desc:  Get the object's scale
*
*    ret:	const CPoint & - sprite scale
************************************************************************/
const CPoint<float> & CObject::GetScale() const
{
    return m_scale;

}   // GetScale


/************************************************************************
*    desc:  Get the object's center position
************************************************************************/
const CPoint<float> & CObject::GetCenterPos() const
{
    return m_centerPos;

}   // GetCenterPos


/************************************************************************
*    desc:  Set the object's center position
************************************************************************/
void CObject::SetCenterPos( const CPoint<float> & position )
{
    m_parameters.Add( NDefs::CENTER_POINT | NDefs::TRANSFORM );

    m_centerPos = position;

}   // SetCenterPos

void CObject::SetCenterPosXYZ( float x, float y, float z )
{
    m_parameters.Add( NDefs::CENTER_POINT | NDefs::TRANSFORM );

    m_centerPos.Set( x, y, z );

}   // SetCenterPosXYZ


/************************************************************************
*    desc:  Set the object's crop offset
************************************************************************/
void CObject::SetCropOffset( const CSize<int16_t> & offset )
{
    if( !m_centerPos.IsEmpty() || !offset.IsEmpty() )
    {
        m_parameters.Add( NDefs::CROP_OFFSET | NDefs::TRANSFORM );

        m_cropOffset = offset;
    }

}   // SetCropOffset


/************************************************************************
*    desc:  Set the object visible
************************************************************************/
void CObject::SetVisible( bool value )
{
    if( value )
        m_parameters.Add( NDefs::VISIBLE );
    else
        m_parameters.Remove( NDefs::VISIBLE );

}   // SetVisible


/************************************************************************
*    desc:  Is the object visible
************************************************************************/
bool CObject::IsVisible() const
{
    return m_parameters.IsSet( NDefs::VISIBLE );

}   // IsVisible


/************************************************************************
*    desc:  Copy the transform to the passed in object
************************************************************************/
void CObject::CopyTransform( const CObject * pObject )
{
    if( pObject->m_parameters.IsSet( NDefs::TRANSLATE ) )
        SetPos( pObject->m_pos );

    if( pObject->m_parameters.IsSet( NDefs::ROTATE ) )
        SetRot( pObject->m_rot, false );

    if( pObject->m_parameters.IsSet( NDefs::SCALE ) )
        SetScale( pObject->m_scale );
    
    if( pObject->m_parameters.IsSet( NDefs::CENTER_POINT ) )
        SetCenterPos( pObject->m_centerPos );
    
    if( pObject->m_parameters.IsSet( NDefs::CROP_OFFSET ) )
        SetCropOffset( pObject->m_cropOffset );

}   // CopyTransform


/************************************************************************
*    desc:  Load the transform data from node
************************************************************************/
void CObject::LoadTransFromNode( const XMLNode & node )
{
    bool loadedFlag;

    CPoint<CWorldValue> pos = NParseHelper::LoadPosition( node, loadedFlag );
    if( loadedFlag )
        SetPos( pos );

    CPoint<float> rot = NParseHelper::LoadRotation( node, loadedFlag );
    if( loadedFlag )
        SetRot( rot );

    CPoint<float> scale = NParseHelper::LoadScale( node, loadedFlag );
    if( loadedFlag )
        SetScale( scale );
    
    CPoint<float> centerPos = NParseHelper::LoadCenterPos( node, loadedFlag );
    if( loadedFlag )
        SetCenterPos( centerPos );

}   // LoadTransFromNode
