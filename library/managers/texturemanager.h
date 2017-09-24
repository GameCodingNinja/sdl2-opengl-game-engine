
/************************************************************************
*    FILE NAME:       texturemanager.h
*
*    DESCRIPTION:     texture class singleton
************************************************************************/ 

#ifndef __texture_manager_h__
#define __texture_manager_h__

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Game lib dependencies
#include <common/texture.h>

// Standard lib dependencies
#include <string>
#include <map>


class CTextureMgr
{
public:

    // Get the instance of the singleton class
    static CTextureMgr & Instance()
    {
        static CTextureMgr textMgr;
        return textMgr;
    }

    // Load the texture from file path
    const CTexture & LoadFor2D( const std::string & group, const std::string & filePath, bool compressed = false );
    const CTexture & LoadFor3D( const std::string & group, const std::string & filePath, bool compressed = false );

    // Texture deleting
    void DeleteTextureGroupFor2D( const std::string & group );
    void DeleteTextureGroupFor3D( const std::string & group );

    // Function call used to manage what texture is currently bound
    void Bind( GLuint textureID );

    // Unbind the texture and reset the flag
    void Unbind();
    
    // Init the Anisotropic filtering
    void InitAnisotropic();

private:

    // Constructor
    CTextureMgr();

    // Destructor
    ~CTextureMgr();
    
    // Load the texture from file path
    void LoadTexture( CTexture & texture, const std::string & filePath, bool compressed );

private:

    // Map containing a group of texture handles
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureFor2DMapMap;
    std::map< const std::string, std::map< const std::string, CTexture > > m_textureFor3DMapMap;

    // Current texture ID
    GLuint m_currentTextureID;
    
    // Largest possible anisotropic value
    GLint m_anisotropicLevel;
};

#endif  // __texture_manager_h__

