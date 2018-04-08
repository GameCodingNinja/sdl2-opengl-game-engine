
/************************************************************************
*    FILE NAME:       visualcomponent2d.cpp
*
*    DESCRIPTION:     Class for handling the visual part of the sprite
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <2d/visualcomponent2d.h>

// Game lib dependencies
#include <objectdata/objectvisualdata2d.h>
#include <managers/shadermanager.h>
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/fontmanager.h>
#include <common/quad2d.h>
#include <common/shaderdata.h>
#include <common/fontdata.h>
#include <system/device.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>
#include <utilities/deletefuncs.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <memory>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CVisualComponent2D::CVisualComponent2D( const CObjectVisualData2D & visualData ) :
    m_pShaderData(nullptr),
    m_vbo( visualData.GetVBO() ),
    m_ibo( visualData.GetIBO() ),
    m_textureID( visualData.GetTextureID() ),
    m_vertexLocation(-1),
    m_uvLocation(-1),
    m_text0Location(-1),
    m_colorLocation(-1),
    m_matrixLocation(-1),
    m_glyphLocation(-1),
    GENERATION_TYPE( visualData.GetGenerationType() ),
    m_quadVertScale( visualData.GetVertexScale() ),
    m_rVisualData( visualData ),
    m_color( visualData.GetColor() ),
    m_iboCount( visualData.GetIBOCount() ),
    m_drawMode( (visualData.GetGenerationType() == NDefs::EGT_QUAD || visualData.GetGenerationType() == NDefs::EGT_SPRITE_SHEET) ? GL_TRIANGLE_FAN : GL_TRIANGLES ),
    m_indiceType( (visualData.GetGenerationType() == NDefs::EGT_FONT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE ),
    m_frameIndex(0),
    m_pFontData(nullptr)
{
    if( visualData.IsActive() )
    {
        m_pShaderData = &CShaderMgr::Instance().GetShaderData( visualData.GetShaderID() );

        // Common shader members
        m_vertexLocation = m_pShaderData->GetAttributeLocation( "in_position" );
        m_matrixLocation = m_pShaderData->GetUniformLocation( "cameraViewProjMatrix" );
        m_colorLocation = m_pShaderData->GetUniformLocation( "color" );
        
        // Do we have a texture? This could be a solid rect
        if( (m_textureID > 0) || (GENERATION_TYPE == NDefs::EGT_FONT) )
        {
            m_uvLocation = m_pShaderData->GetAttributeLocation( "in_uv" );
            m_text0Location = m_pShaderData->GetUniformLocation( "text0" );
        }

        // Is this a sprite sheet? Get the glyph rect position
        if( GENERATION_TYPE == NDefs::EGT_SPRITE_SHEET )
        {
            m_glyphLocation = m_pShaderData->GetUniformLocation( "glyphRect" );
            
            m_glyphUV = visualData.GetSpriteSheet().GetGlyph().GetUV();
            m_frameIndex = visualData.GetSpriteSheet().GetDefaultIndex();
        }
        
        // Allocate the storage for the font if this is a font sprite
        // Using a normal pointer to keep the memory foot print as small as possible
        if( GENERATION_TYPE == NDefs::EGT_FONT )
            m_pFontData = new CFontData;
    }

}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CVisualComponent2D::~CVisualComponent2D()
{
    NDelFunc::Delete(m_pFontData);
    
    // Delete the VBO if this is a font
    DeleteFontVBO();

    // The IBO for the font is managed by the vertex buffer manager.
    // Font IBO are all the same with the only difference being
    // length of the character string.

}   // destructor


/************************************************************************
*    desc:  Delete the custom VBO for this font
*                
*           Added a VBO delete function so that deleting the VBO can be
*           handled separately so that the sprite can be allocated and 
*           freed from a thread
************************************************************************/
void CVisualComponent2D::DeleteFontVBO()
{
    // Delete the VBO if this is a font
    if( (GENERATION_TYPE == NDefs::EGT_FONT) && (m_vbo > 0) )
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    
    // The IBO for the font is managed by the vertex buffer manager.
    // Font IBO are all the same with the only difference being
    // length of the character string.
    
}   // DeleteFontVBO


/************************************************************************
*    desc:  do the render
************************************************************************/
void CVisualComponent2D::Render( const CMatrix & objMatrix, const CMatrix & matrix )
{
    if( AllowRender() )
    {
        const int32_t VERTEX_BUF_SIZE( sizeof(CVertex2D) );
            
        // Increment our stat counter to keep track of what is going on.
        CStatCounter::Instance().IncDisplayCounter();
        
        // Bind the VBO and IBO
        CVertBufMgr::Instance().Bind( m_vbo, m_ibo );

        // Bind the shader. This must be done before glVertexAttribPointer
        CShaderMgr::Instance().Bind( m_pShaderData );
        
        // Setup the vertex attribute shader data
        glVertexAttribPointer( m_vertexLocation, 3, GL_FLOAT, GL_FALSE, VERTEX_BUF_SIZE, (GLvoid*)0 );

        // Are we rendering with a texture?
        if( m_textureID > 0 )
        {
            const int8_t UV_OFFSET( sizeof(CPoint<float>) );
            
            // Bind the texture
            CTextureMgr::Instance().Bind( m_textureID );
            glUniform1i( m_text0Location, 0); // 0 = TEXTURE0

            // Setup the UV attribute shade data
            glVertexAttribPointer( m_uvLocation, 2, GL_FLOAT, GL_FALSE, VERTEX_BUF_SIZE, (GLvoid*)UV_OFFSET );
        }

        // Send the color to the shader
        glUniform4fv( m_colorLocation, 1, (float*)&m_color );

        // If this is a quad, we need to take into account the vertex scale
        if( GENERATION_TYPE == NDefs::EGT_QUAD )
        {
            // Calculate the final matrix
            CMatrix finalMatrix;
            finalMatrix.SetScale( m_quadVertScale );
            finalMatrix *= objMatrix;
            finalMatrix *= matrix;

            // Send the final matrix to the shader
            glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, finalMatrix() );
        }
        // If this is a sprite sheet, we need to take into account the vertex scale and glyph rect
        else if( GENERATION_TYPE == NDefs::EGT_SPRITE_SHEET )
        {
            // Calculate the final matrix
            CMatrix finalMatrix;
            finalMatrix.SetScale( m_quadVertScale );
            finalMatrix *= objMatrix;
            finalMatrix *= matrix;

            // Send the final matrix to the shader
            glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, finalMatrix() );
            
            // Send the glyph rect
            glUniform4fv( m_glyphLocation, 1, (float*)&m_glyphUV );
        }
        // this is for scaled frame and font rendering
        else
        {
            // Calculate the final matrix
            CMatrix finalMatrix;
            finalMatrix *= objMatrix;
            finalMatrix *= matrix;
            
            glUniformMatrix4fv( m_matrixLocation, 1, GL_FALSE, finalMatrix() );
        }

        // Render it
        glDrawElements( m_drawMode, m_iboCount, m_indiceType, nullptr );
    }

}   // Render


/************************************************************************
*    desc:  Load the font properties from XML node
************************************************************************/
void CVisualComponent2D::LoadFontPropFromNode( const XMLNode & node )
{
    if(m_pFontData)
        m_pFontData->LoadFromNode( node );
    
}   // LoadFontPropFromNode


/************************************************************************
*    desc:  Set the font data
************************************************************************/
void CVisualComponent2D::SetFontData( const CFontData & fontData )
{
    if(m_pFontData)
        m_pFontData->Copy( fontData );
    
}   // SetFontData


/************************************************************************
*    desc:  Set the font properties
************************************************************************/
void CVisualComponent2D::SetFontProperties( const CFontProperties & fontProp )
{
    if(m_pFontData)
        m_pFontData->m_fontProp.Copy( fontProp );
    
}   // SetFontProperties


/************************************************************************
*    desc:  Create the font string
************************************************************************/
void CVisualComponent2D::CreateFontString()
{
    if( m_pFontData )
        CreateFontString( m_pFontData->m_fontString );

}   // CreateFontString


/************************************************************************
*    desc:  Create the font string
*
*    NOTE: Line wrap feature only supported for horizontal left
************************************************************************/
void CVisualComponent2D::CreateFontString( const std::string & fontString )
{
    // Qualify if we want to build the font string
    if( m_pFontData &&
        !fontString.empty() &&
        !m_pFontData->m_fontProp.m_fontName.empty() &&
        ((fontString != m_pFontData->m_fontString) || (m_vbo == 0)) )
    {
        m_pFontData->m_fontStrSize.Reset();
        float lastCharDif(0.f);
        
        const CFont & font = CFontMgr::Instance().GetFont( m_pFontData->m_fontProp.m_fontName );

        m_textureID = font.GetTextureID();

        m_pFontData->m_fontString = fontString;

        // count up the number of space characters
        const int spaceCharCount = NGenFunc::CountStrOccurrence( m_pFontData->m_fontString, " " );

        // count up the number of bar | characters
        const int barCharCount = NGenFunc::CountStrOccurrence( m_pFontData->m_fontString, "|" );

        // Size of the allocation
        int charCount = m_pFontData->m_fontString.size() - spaceCharCount - barCharCount;
        m_iboCount = charCount * 6;
        
        // Set a flag to indicate if the IBO should be built
        const bool BUILD_FONT_IBO = (m_iboCount > CVertBufMgr::Instance().GetCurrentMaxFontIndices());

        // Allocate the quad array
        std::unique_ptr<CQuad2D[]> upQuadBuf( new CQuad2D[charCount] );

        // Create a buffer to hold the indices
        std::unique_ptr<uint16_t[]> upIndxBuf;
        
        // Should we build or rebuild the font IBO
        if( BUILD_FONT_IBO )
            upIndxBuf.reset( new uint16_t[m_iboCount] );

        float xOffset = 0.f;
        float width = 0.f;
        float lineHeightOffset = 0.f;
        float lineHeightWrap = font.GetLineHeight() + font.GetVertPadding() + m_pFontData->m_fontProp.m_lineWrapHeight;
        float initialHeightOffset = font.GetBaselineOffset() + font.GetVertPadding();
        float lineSpace = font.GetLineHeight() - font.GetBaselineOffset();

        uint counter = 0;
        int lineCount = 0;

        // Get the size of the texture
        CSize<float> textureSize = font.GetTextureSize();

        // Handle the horizontal alignment
        std::vector<float> lineWidthOffsetVec = CalcLineWidthOffset( font, m_pFontData->m_fontString );

        // Set the initial line offset
        xOffset = lineWidthOffsetVec[lineCount++];

        // Handle the vertical alignment
        if( m_pFontData->m_fontProp.m_vAlign == NDefs::EVA_VERT_TOP )
            lineHeightOffset = -initialHeightOffset;

        if( m_pFontData->m_fontProp.m_vAlign == NDefs::EVA_VERT_CENTER )
        {
            lineHeightOffset = -(initialHeightOffset - ((font.GetBaselineOffset()-lineSpace) / 2.f) - font.GetVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset = ((lineHeightWrap * lineWidthOffsetVec.size()) / 2.f) - font.GetBaselineOffset();
        }

        else if( m_pFontData->m_fontProp.m_vAlign == NDefs::EVA_VERT_BOTTOM )
        {
            lineHeightOffset = -(initialHeightOffset - font.GetBaselineOffset() - font.GetVertPadding());

            if( lineWidthOffsetVec.size() > 1 )
                lineHeightOffset += (lineHeightWrap * (lineWidthOffsetVec.size()-1));
        }

        // Remove any fractional component of the line height offset
        lineHeightOffset = (int)lineHeightOffset;

        // Setup each character in the vertex buffer
        for( size_t i = 0; i < m_pFontData->m_fontString.size(); ++i )
        {
            char id = m_pFontData->m_fontString[i];

            // Line wrap if '|' character was used
            if( id == '|' )
            {
                xOffset = lineWidthOffsetVec[lineCount];
                width = 0.f;

                lineHeightOffset += -lineHeightWrap;
                ++lineCount;
            }
            else
            {
                // See if we can find the character
                const CCharData & charData = font.GetCharData(id);

                // Ignore space characters
                if( id != ' ' )
                {
                    CRect<float> rect = charData.rect;

                    float yOffset = (font.GetLineHeight() - rect.y2 - charData.offset.h) + lineHeightOffset;

                    // Check if the width or height is odd. If so, we offset
                    // by 0.5 for proper orthographic rendering
                    float additionalOffsetX = 0;
                    if( (int)rect.x2 % 2 != 0 )
                        additionalOffsetX = 0.5f;

                    float additionalOffsetY = 0;
                    if( (int)rect.y2 % 2 != 0 )
                        additionalOffsetY = 0.5f;

                    auto & quadBuf = upQuadBuf[counter];
                    
                    // Calculate the first vertex of the first face
                    quadBuf.vert[0].vert.x = xOffset + charData.offset.w + additionalOffsetX;
                    quadBuf.vert[0].vert.y = yOffset + additionalOffsetY;
                    quadBuf.vert[0].uv.u = rect.x1 / textureSize.w;
                    quadBuf.vert[0].uv.v = (rect.y1 + rect.y2) / textureSize.h;

                    // Calculate the second vertex of the first face
                    quadBuf.vert[1].vert.x = xOffset + rect.x2 + charData.offset.w + additionalOffsetX;
                    quadBuf.vert[1].vert.y = yOffset + rect.y2 + additionalOffsetY;
                    quadBuf.vert[1].uv.u = (rect.x1 + rect.x2) / textureSize.w;
                    quadBuf.vert[1].uv.v = rect.y1 / textureSize.h;

                    // Calculate the third vertex of the first face
                    quadBuf.vert[2].vert.x = quadBuf.vert[0].vert.x;
                    quadBuf.vert[2].vert.y = quadBuf.vert[1].vert.y;
                    quadBuf.vert[2].uv.u = quadBuf.vert[0].uv.u;
                    quadBuf.vert[2].uv.v = quadBuf.vert[1].uv.v;

                    // Calculate the second vertex of the second face
                    quadBuf.vert[3].vert.x = quadBuf.vert[1].vert.x;
                    quadBuf.vert[3].vert.y = quadBuf.vert[0].vert.y;
                    quadBuf.vert[3].uv.u = quadBuf.vert[1].uv.u;
                    quadBuf.vert[3].uv.v = quadBuf.vert[0].uv.v;

                    // Should we build or rebuild the font IBO
                    if( BUILD_FONT_IBO )
                    {
                        // Create the indices into the VBO
                        int arrayIndex = counter * 6;
                        int vertIndex = counter * 4;

                        upIndxBuf[arrayIndex]   = vertIndex;
                        upIndxBuf[arrayIndex+1] = vertIndex+1;
                        upIndxBuf[arrayIndex+2] = vertIndex+2;

                        upIndxBuf[arrayIndex+3] = vertIndex;
                        upIndxBuf[arrayIndex+4] = vertIndex+3;
                        upIndxBuf[arrayIndex+5] = vertIndex+1;
                    }

                    ++counter;
                }

                // Inc the font position
                float inc = charData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.GetHorzPadding();

                // Add in any additional spacing for the space character
                if( id == ' ' )
                    inc += m_pFontData->m_fontProp.m_spaceCharKerning;

                width += inc;
                xOffset += inc;
                
                // Get the longest width of this font string
                if( m_pFontData->m_fontStrSize.w < width )
                {
                    m_pFontData->m_fontStrSize.w = width;
                    
                    // This is the space between this character and the next.
                    // Save this difference so that it can be subtracted at the end
                    lastCharDif = inc - charData.rect.x2;
                }

                // Wrap to another line
                if( (id == ' ') && (m_pFontData->m_fontProp.m_lineWrapWidth > 0.f) )
                {
                    float nextWord = 0.f;

                    // Get the length of the next word to see if if should wrap
                    for( size_t j = i+1; j < m_pFontData->m_fontString.size(); ++j )
                    {
                        id = m_pFontData->m_fontString[j];

                        if( id != '|' )
                        {
                            // See if we can find the character
                            const CCharData & anotherCharData = font.GetCharData(id);

                            // Break here when space is found
                            // Don't add the space to the size of the next word
                            if( id == ' ' )
                                break;

                            // Don't count the
                            nextWord += anotherCharData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.GetHorzPadding();
                        }
                    }

                    if( width + nextWord >= m_pFontData->m_fontProp.m_lineWrapWidth )
                    {
                        xOffset = lineWidthOffsetVec[lineCount++];
                        width = 0.f;

                        lineHeightOffset += -lineHeightWrap;
                    }
                }
            }
        }
        
        // Subtract the extra space after the last character
        m_pFontData->m_fontStrSize.w -= lastCharDif;
        m_pFontData->m_fontStrSize.h = font.GetLineHeight();

        // Save the data
        // If one doesn't exist, create the VBO and IBO for this font
        if( m_vbo == 0 )
            glGenBuffers( 1, &m_vbo );

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(CQuad2D) * charCount, upQuadBuf.get(), GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        // All fonts share the same IBO because it's always the same and the only difference is it's length
        // This updates the current IBO if it exceeds the current max
        m_ibo = CVertBufMgr::Instance().CreateDynamicFontIBO( CFontMgr::Instance().GetGroup(), "dynamic_font_ibo", upIndxBuf.get(), m_iboCount );
    }
    else if( m_pFontData &&
             fontString.empty() &&
             (fontString != m_pFontData->m_fontString) &&
             (m_vbo > 0) )
    {
        m_pFontData->m_fontString.clear();
    }

}   // CreateFontString


/************************************************************************
*    desc:  Add up all the character widths
************************************************************************/
std::vector<float> CVisualComponent2D::CalcLineWidthOffset(
    const CFont & font,
    const std::string & str )
{
    float firstCharOffset = 0;
    float lastCharOffset = 0;
    float spaceWidth = 0;
    float width = 0;
    int counter = 0;
    std::vector<float> lineWidthOffsetVec;

    for( size_t i = 0; i < str.size(); ++i )
    {
        char id = str[i];

        // Line wrap if '|' character was used
        if( id == '|' )
        {
            // Add the line width to the vector based on horz alignment
            AddLineWithToVec( font, lineWidthOffsetVec, m_pFontData->m_fontProp.m_hAlign, width, firstCharOffset, lastCharOffset );

            counter = 0;
            width = 0;
        }
        else
        {
            // Get the next character
            const CCharData & charData = font.GetCharData( id );

            if(counter == 0)
                firstCharOffset = charData.offset.w;

            spaceWidth = charData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.GetHorzPadding();

            // Add in any additional spacing for the space character
            if( id == ' ' )
                spaceWidth += m_pFontData->m_fontProp.m_spaceCharKerning;

            width += spaceWidth;

            if( id != ' ')
                lastCharOffset = charData.offset.w;

            ++counter;
        }

        // Wrap to another line
        if( (id == ' ') && (m_pFontData->m_fontProp.m_lineWrapWidth > 0.f) )
        {
            float nextWord = 0.f;

            // Get the length of the next word to see if if should wrap
            for( size_t j = i+1; j < str.size(); ++j )
            {
                id = str[j];

                if( id != '|' )
                {
                    // See if we can find the character
                    const CCharData & charData = font.GetCharData(id);

                    // Break here when space is found
                    // Don't add the space to the size of the next word
                    if( id == ' ' )
                        break;

                    // Don't count the
                    nextWord += charData.xAdvance + m_pFontData->m_fontProp.m_kerning + font.GetHorzPadding();
                }
            }

            if( width + nextWord >= m_pFontData->m_fontProp.m_lineWrapWidth )
            {
                // Add the line width to the vector based on horz alignment
                AddLineWithToVec( font, lineWidthOffsetVec, m_pFontData->m_fontProp.m_hAlign, width-spaceWidth, firstCharOffset, lastCharOffset );

                counter = 0;
                width = 0;
            }
        }
    }

    // Add the line width to the vector based on horz alignment
    AddLineWithToVec( font, lineWidthOffsetVec, m_pFontData->m_fontProp.m_hAlign, width, firstCharOffset, lastCharOffset );

    return lineWidthOffsetVec;

}   // CalcLineWidthOffset


/************************************************************************
*    desc:  Add the line width to the vector based on horz alignment
************************************************************************/
void CVisualComponent2D::AddLineWithToVec(
    const CFont & font,
    std::vector<float> & lineWidthOffsetVec,
    const NDefs::EHorzAlignment hAlign,
    float width,
    float firstCharOffset,
    float lastCharOffset )
{
    if( hAlign == NDefs::EHA_HORZ_LEFT )
        lineWidthOffsetVec.push_back(-(firstCharOffset + font.GetHorzPadding()));

    else if( hAlign == NDefs::EHA_HORZ_CENTER )
        lineWidthOffsetVec.push_back(-((width - font.GetHorzPadding()) / 2.f));

    else if( hAlign == NDefs::EHA_HORZ_RIGHT )
        lineWidthOffsetVec.push_back(-(width - lastCharOffset - font.GetHorzPadding()));

    // Remove any fractional component of the last index
    lineWidthOffsetVec.back() = (int)lineWidthOffsetVec.back();

}   // AddLineWithToVec


/************************************************************************
*    desc:  Get/Set the displayed font string
************************************************************************/
const std::string & CVisualComponent2D::GetFontString()
{
    if( !m_pFontData )
    {
        throw NExcept::CCriticalException("Not a sprite font!",
            boost::str( boost::format("Can't ask for the font string from a sprite that is not a sprite font.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }
    
    return m_pFontData->m_fontString;

}   // GetFontString

void CVisualComponent2D::SetFontString( const std::string & fontString )
{
    if(m_pFontData)
        m_pFontData->m_fontString = fontString;

}   // SetFontString


/************************************************************************
*    desc:  Set/Get the color
************************************************************************/
void CVisualComponent2D::SetColor( const CColor & color )
{
    m_color = color;

}   // SetColor

void CVisualComponent2D::SetRGBA( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_color.Set( r, g, b, a );

}   // SetRGBA

const CColor & CVisualComponent2D::GetColor() const
{
    return m_color;

}   // GetColor


/************************************************************************
*    desc:  Set/Get the alpha
************************************************************************/
void CVisualComponent2D::SetAlpha( float alpha, bool allowToExceed )
{
    if( allowToExceed || (alpha < m_rVisualData.GetColor().a) )
        m_color.a = alpha;
    else
        alpha = m_rVisualData.GetColor().a;

}   // SetAlpha

float CVisualComponent2D::GetAlpha() const
{
    return m_color.a;

}   // GetAlpha


/************************************************************************
*    desc:  Set the frame ID from index
************************************************************************/
void CVisualComponent2D::SetFrame( uint index )
{
    if( GENERATION_TYPE == NDefs::EGT_SPRITE_SHEET )
    {
        auto rGlyph = m_rVisualData.GetSpriteSheet().GetGlyph( index );
        m_glyphUV = rGlyph.GetUV();
        m_quadVertScale = rGlyph.GetSize() * m_rVisualData.GetDefaultUniformScale();
    }
    else
        m_textureID = m_rVisualData.GetTextureID( index );

    m_frameIndex = index;

}   // SetFrame


/************************************************************************
*    desc:  Get the current frame index
************************************************************************/
uint CVisualComponent2D::GetCurrentFrame() const 
{
    return m_frameIndex;

}   // GetFrameCount


/************************************************************************
*    desc:  Set the default color
************************************************************************/
void CVisualComponent2D::SetDefaultColor()
{
    m_color = m_rVisualData.GetColor();

}   // SetDefaultColor


/************************************************************************
*    desc:  Is rendering allowed?
************************************************************************/
bool CVisualComponent2D::AllowRender()
{
    return ((GENERATION_TYPE > NDefs::EGT_NULL) && (GENERATION_TYPE < NDefs::EGT_FONT)) ||
           ((GENERATION_TYPE == NDefs::EGT_FONT) && !m_pFontData->m_fontString.empty() && (m_vbo > 0));

}   // AllowRender


/************************************************************************
*    desc:  Is this a font sprite
************************************************************************/
bool CVisualComponent2D::IsFontSprite()
{
    return (GENERATION_TYPE == NDefs::EGT_FONT);

}   // IsFontSprite


/************************************************************************
*    desc:  Get the font size
************************************************************************/
const CSize<float> & CVisualComponent2D::GetFontSize() const
{
    if( !m_pFontData )
    {
        throw NExcept::CCriticalException("Not a sprite font!",
            boost::str( boost::format("Can't ask for the font size from a sprite that is not a sprite font.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }
    
    return m_pFontData->m_fontStrSize;

}   // GetFontSize

