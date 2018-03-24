
/************************************************************************
*    FILE NAME:       texture.h
*
*    DESCRIPTION:     Class for holding texture data
************************************************************************/

#ifndef __texture_h__
#define __texture_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <common/size.h>

// Texture type
enum ETextureType
{
    ETT_NULL = -1,
    ETT_DIFFUSE,
    ETT_NORMAL,
    ETT_SPECULAR,
    ETT_DISPLACEMENT,
    ETT_MAX_TEXT_TYPES,
};

class CTexture
{
public:

    // OpenGL texture ID
    GLuint m_id = 0;
    
    // Texture type
    ETextureType m_type = ETT_DIFFUSE;
    
    // Texture size - mostly needed for 2D
    CSize<int> m_size;
    
    // Texture channels
    int channels = 0;
    
    // Texture byte data
    unsigned char * pData = nullptr;

    GLuint GetID() const {return m_id;}
    const CSize<int> & GetSize() const {return m_size;}
};

#endif  // __texture_h__


