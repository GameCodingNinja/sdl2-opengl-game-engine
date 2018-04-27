
/************************************************************************
*    FILE NAME:       objectdata2d.cpp
*
*    DESCRIPTION:     Class that holds a 2D object data
************************************************************************/

// Physical component dependency
#include <objectdata/objectdata2d.h>

// Game lib dependencies
#include <utilities/xmlparsehelper.h>

// Standard lib dependencies
#include <math.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CObjectData2D::CObjectData2D() :
    m_radius(0),
    m_radiusSquared(0)
{
}


/************************************************************************
*    DESC:  Copy Constructor
************************************************************************/
CObjectData2D::CObjectData2D( const CObjectData2D & obj )
{
    *this = obj;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CObjectData2D::~CObjectData2D()
{
}


/************************************************************************
*    DESC:  Load the object data from the passed in node
************************************************************************/
void CObjectData2D::loadFromNode( const XMLNode & node, const std::string & group, const std::string & name )
{
    m_name = name;
    m_group = group;

    // Load the size
    m_size = NParseHelper::LoadSize( node );

    // Load the visual data
    m_visualData.loadFromNode( node );

    // Load the image data from file
    m_visualData.loadImage( group );

    // Load the physics data
    m_physicsData.loadFromNode( node );
}


/************************************************************************
*    DESC:  Create the objects from data
************************************************************************/
void CObjectData2D::createFromData( const std::string & group )
{
    // Create the visuals
    m_visualData.createFromData( group, m_size );

    // Calculate the radii
    m_radiusSquared = pow((float)m_size.w / 2, 2) + pow((float)m_size.h / 2, 2);
    m_radius = sqrt( m_radiusSquared );
}


/************************************************************************
*    DESC:  Access functions for the visual data
************************************************************************/
const CObjectVisualData2D & CObjectData2D::getVisualData() const
{
    return m_visualData;
}


/************************************************************************
*    DESC:  Access functions for the physics data
************************************************************************/
const CObjectPhysicsData2D & CObjectData2D::getPhysicsData() const
{
    return m_physicsData;
}


/************************************************************************
*    DESC:  Access functions for the data name
************************************************************************/
const std::string & CObjectData2D::getName() const
{
    return m_name;
}


/************************************************************************
*    DESC:  Access functions for the data group
************************************************************************/
const std::string & CObjectData2D::getGroup() const
{
    return m_group;
}


/************************************************************************
*    DESC:  Access functions for the size
************************************************************************/
const CSize<int> & CObjectData2D::getSize() const
{
    return m_size;
}


/************************************************************************
*    DESC:  Access functions for the radius
************************************************************************/
float CObjectData2D::getRadius() const
{
    return m_radius;
}


/************************************************************************
*    DESC:  Access functions for the radius squared
************************************************************************/
float CObjectData2D::getRadiusSquared() const
{
    return m_radiusSquared;
}


/************************************************************************
*    DESC:  Is the generation type font
************************************************************************/
bool CObjectData2D::isGenTypeFont() const
{
    return m_visualData.getGenerationType() == NDefs::EGT_FONT;
}
