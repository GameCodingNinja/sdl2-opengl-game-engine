
/************************************************************************
*    FILE NAME:       ojectvisualdata2d.h
*
*    DESCRIPTION:     Class containing the 2D object's visual data
************************************************************************/

#ifndef __object_visual_data_2d_h__
#define __object_visual_data_2d_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <common/size.h>
#include <common/color.h>
#include <common/rect.h>
#include <common/point.h>
#include <common/defs.h>
#include <common/face2d.h>
#include <common/scaledframe.h>
#include <common/texture.h>
#include <common/spritesheet.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward Declarations
struct XMLNode;

class CObjectVisualData2D
{
public:

    // Constructor/Destructor
    CObjectVisualData2D();
    ~CObjectVisualData2D();

    // Load thes object data from node
    void LoadFromNode( const XMLNode & objectNode );

    // Create the object from data
    void CreateFromData( const std::string & group, CSize<int> & rSize );

    // Get the gne type
    NDefs::EGenerationType GetGenerationType() const;

    // Get the texture ID
    GLuint GetTextureID( uint index = 0 ) const;

    // Get the name of the shader ID
    const std::string & GetShaderID() const;

    // Get the color
    const CColor & GetColor() const;

    // Get the vertex scale
    const CSize<float> & GetVertexScale() const;

    // Get the VBO
    GLuint GetVBO() const;

    // Get the IBO
    GLuint GetIBO() const;

    // Get the ibo count
    int GetIBOCount() const;

    // Get the frame count
    size_t GetFrameCount() const;

    // Whether or not the visual tag was specified
    bool IsActive() const;
    
    // Get the sprite sheet
    const CSpriteSheet & GetSpriteSheet() const;
    
    // Access functions for the default uniform scale
    const float GetDefaultUniformScale() const;

private:
    
    // Try to load the texture if one exists
    void LoadTexture( const std::string & group, CTexture & rTexture, CSize<int> & rSize );
    
    // Generate a quad
    void GenerateQuad( const std::string & group );
    
    // Generate a scaled frame
    void GenerateScaledFrame(
        const std::string & group,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & textureOffset );
    
    // Generate a scaled frame with a mesh file
    void GenerateScaledFrameMeshFile( 
        const std::string & group,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & textureOffset );
    
    // Generate a mesh file
    void GenerateFromMeshFile( 
        const std::string & group, const CSize<int> & textureSize, const CSize<int> & size );
    
    // Load a mesh from XML file
    void LoadMeshFromXML(
        const std::string & group,
        const CSize<int> & textureSize,
        const CSize<int> & size,
        const CRect<float> & textureOffset,
        int iboOffset,
        std::vector<CVertex2D> & rVertVec,
        std::vector<GLubyte> & rIboVec );
    
private:

    // Loaded texture data
    std::vector<GLuint> m_textureIDVec;

    // VBO
    GLuint m_vbo;

    // IBO
    GLuint m_ibo;

    // VBO/IBO generation type
    NDefs::EGenerationType m_genType;

    // Name of the shader
    std::string m_shaderID;

    // Initial color of the object
    CColor m_color;

    // texture file path
    std::string m_textureFilePath;
    
    // Texture Sequence count
    int m_textureSequenceCount;

    // Compressed flag
    bool m_compressed;

    // mesh file path
    std::string m_meshFilePath;

    // ibo count
    int m_iboCount;

    // The vertex scale of the object
    CSize<float> m_vertexScale;

    // Scaled frame
    CScaledFrame m_scaledFrame;
    
    // Sprite Sheet
    CSpriteSheet m_spriteSheet;
    
    // Sprite sheet file path
    std::string m_spriteSheetFilePath;
    
    // String of glyph ids
    std::vector<std::string> m_glyphIDs;
    
    // Default scale
    float m_defaultUniformScale;
    
    // Mirror enum
    NDefs::EMirror m_mirror;
    
    // File extension for resolution swap
    std::string m_resExt;
};

#endif  // __object_visual_data_2d_h__
