
/************************************************************************
*    FILE NAME:       visualcomponent3d.h
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#ifndef __visual_component_3d_h__
#define __visual_component_3d_h__

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/color.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/fontproperties.h>
#include <common/mesh3d.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>

// Forward declaration(s)
class CObjectVisualData3D;
class CFont;
class CShaderData;

class CVisualComponent3D : boost::noncopyable
{
public:

    // Constructor
    CVisualComponent3D( const CObjectVisualData3D & visualData );

    // Destructor
    ~CVisualComponent3D();

    // do the render
    void Render( const CMatrix & matrix, const CMatrix & normalMatrix );

    // Set/Get the color
    void SetColor( const CColor & color );
    void SetColor( float r, float g, float b, float a );
    const CColor & GetColor() const;
    void SetDefaultColor();

    // Set/Get the alpha
    void SetAlpha( float alpha );
    float GetAlpha() const;

private:

    // Reference to object visual data
    const CObjectVisualData3D & m_visualData;
    
    // Shader data pointer - We DON'T own this pointer, don't free
    CShaderData * m_pShaderData;

    // shader location data
    int32_t m_vertexLocation;
    int32_t m_normalLocation;
    int32_t m_uvLocation;
    int32_t m_text0Location;
    int32_t m_colorLocation;
    int32_t m_matrixLocation;
    int32_t m_normalMatrixLocation;

    // Copy of 3D mesh data
    const CMesh3D & m_mesh3d;

    // Color
    CColor m_color;
    
    // Vertex buffer sizes
    const int8_t m_VERTEX_BUF_SIZE;
};

#endif  // __visual_component_3d_h__

