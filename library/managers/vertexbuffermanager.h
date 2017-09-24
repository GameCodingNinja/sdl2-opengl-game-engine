
/************************************************************************
*    FILE NAME:       vertexbuffermanager.h
*
*    DESCRIPTION:     vertex buffer manager class singleton
************************************************************************/  

#ifndef __vertex_buffer_manager_h__
#define __vertex_buffer_manager_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <common/rect.h>
#include <common/size.h>
#include <common/point.h>
#include <common/vertex2d.h>

// Standard lib dependencies
#include <string>
#include <vector>
#include <map>

// Forward declaration(s)
class CShaderData;
class CScaledFrame;
class CQuad2D;
class CUV;

class CVertBufMgr
{
public:

    // Get the instance of the singleton class
    static CVertBufMgr & Instance()
    {
        static CVertBufMgr VertBufMgr;
        return VertBufMgr;
    }
    
    // Create VBO from vertex data
    GLuint CreateVBO( 
        const std::string & group,
        const std::string & name,
        const std::vector<CVertex2D> & vertVec );

    // Create a IBO buffer
    GLuint CreateIBO(
        const std::string & group,
        const std::string & name,
        GLubyte indexData[],
        int sizeInBytes );

    // Creates and manages the dynamic font indicies IBO
    GLuint CreateDynamicFontIBO(
        const std::string & group,
        const std::string & name,
        GLushort * pIndexData,
        int maxIndicies );

    // Create a scaled frame
    GLuint CreateScaledFrame( 
        const std::string & group,
        const std::string & name,
        const CScaledFrame & scaledFrame,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & spriteSheetOffset,
        const std::vector<CVertex2D> & vertVec );

    void CreateQuad(
        const CPoint<float> & vert,
        const CSize<float> & vSize,
        const CUV & uv,
        const CSize<float> & uvSize,
        const CSize<float> & textureSize,
        const CSize<float> & frameSize,
        const CRect<float> & spriteSheetOffset,
        CQuad2D & quadBuf );
    
    // Generate a scaled frame
    void GenerateScaledFrame(
        std::vector<CVertex2D> & vertVec,
        const CScaledFrame & scaledFrame,
        const CSize<int> & textureSize,
        const CSize<int> & glyphSize,
        const CSize<int> & frameSize,
        const CRect<float> & spriteSheetOffset );

    // Function call used to manage what buffer is currently bound.
    void Bind( GLuint vboID, GLuint iboID );

    // Unbind the buffers and reset the flag
    void Unbind();

    // Delete buffer group
    void DeleteBufferGroupFor2D( const std::string & group );
    
    // See if a VBO already exists
    GLuint IsVBO( const std::string & group, const std::string & name );
    
    // Get the current max font indices
    int GetCurrentMaxFontIndices();

private:

    // Constructor
    CVertBufMgr();

    // Destructor
    ~CVertBufMgr();

private:

    // Map containing a group of VBO handles
    std::map< const std::string, std::map< const std::string, GLuint > > m_vertexBuf2DMapMap;

    // Map containing a group of IBO handles
    std::map< const std::string, std::map< const std::string, GLuint > > m_indexBuf2DMapMap;

    // Current vbo ID
    GLuint m_currentVBOID;

    // Current IBO ID
    GLuint m_currentIBOID;

    // Current dynamic font IBO indices size
    int m_currentMaxFontIndices;

};

#endif  // __vertex_buffer_manager_h__


