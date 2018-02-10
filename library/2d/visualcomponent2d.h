
/************************************************************************
*    FILE NAME:       visualcomponent2d.h
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#ifndef __visual_component_2d_h__
#define __visual_component_2d_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/color.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/rect.h>
#include <common/size.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <memory>

// Forward declaration(s)
class CObjectVisualData2D;
class CFont;
class CShaderData;
class CFontData;
class CFontProperties;

class CVisualComponent2D : boost::noncopyable
{
public:

    // Constructor
    CVisualComponent2D( const CObjectVisualData2D & visualData );

    // Destructor
    ~CVisualComponent2D();
    
    // Delete the custom VBO for this font
    void DeleteFontVBO();

    // do the render
    void Render( const CMatrix & objMatrix, const CMatrix & matrix );

    // Is this a font sprite
    bool IsFontSprite();

    // Set/Get the color
    void SetColor( const CColor & color );
    void SetRGBA( float r, float g, float b, float a );
    const CColor & GetColor() const;
    void SetDefaultColor();

    // Set/Get the alpha
    void SetAlpha( float alpha, bool allowToExceed = false );
    float GetAlpha() const;

    // Set the frame index
    void SetFrame( uint index );
    
    // Get the current frame index
    uint GetCurrentFrame() const;
    
    // Set the font data
    void SetFontData( const CFontData & fontData );
    
    // Load the font properties from XML node
    void LoadFontPropFromNode( const XMLNode & node );

    // Set the string to display
    void CreateFontString();
    void CreateFontString( const std::string & fontString );

    // Get the displayed font string
    const std::string & GetFontString();
    void SetFontString( const std::string & fontString );
    
    // Get the font size
    const CSize<float> & GetFontSize() const;
    
    // Set the font properties
    void SetFontProperties( const CFontProperties & fontProp );

private:

    // Add the line width to the vector based on horz alignment
    void AddLineWithToVec(
        const CFont & font,
        std::vector<float> & lineWidthOffsetVec,
        const NDefs::EHorzAlignment hAlign,
        float width,
        float firstCharOffset,
        float lastCharOffset );

    // Add up all the character widths
    std::vector<float> CalcLineWidthOffset(
        const CFont & font,
        const std::string & str);
    
    // Is rendering allowed?
    bool AllowRender();

private:
    
    // Shader data pointer - We DON'T own this pointer, don't free
    CShaderData * m_pShaderData;

    // VBO
    GLuint m_vbo;

    // IBO
    GLuint m_ibo;

    // Loaded texture data
    GLuint m_textureID;

    // shader location data
    GLint m_vertexLocation;
    GLint m_uvLocation;
    GLint m_text0Location;
    GLint m_colorLocation;
    GLint m_matrixLocation;
    GLint m_glyphLocation;

    // Generation type
    const NDefs::EGenerationType GENERATION_TYPE;

    // The scale of the quad
    CSize<float> m_quadVertScale;

    // Reference to object visual data
    const CObjectVisualData2D & m_rVisualData;

    // Color
    CColor m_color;

    // IBO count
    uint16_t m_iboCount;

    // OpenGL draw mode
    GLenum m_drawMode;

    // IBO indice type
    GLenum m_indiceType;
    
    // Sprite sheet Glyph UV
    CRect<GLfloat> m_glyphUV;
    
    // Frame index
    uint16_t m_frameIndex;

    ///////////////////////
    //  Font data types
    ///////////////////////

    // Unique pointer for font data
    CFontData * m_pFontData;
    
};

#endif  // __visual_component_2d_h__

