
/************************************************************************
*    FILE NAME:       shaderdata.cpp
*
*    DESCRIPTION:     shader data class
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <common/shaderdata.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CShaderData::CShaderData()
    : m_programID(0), m_vertexID(0), m_fragmentID(0)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CShaderData::~CShaderData()
{
    // DO NOT USE FOR FREEING DATA!

    // This class is for holding and copying data. Freeing OpenGL assets
    // is done from the out side with the Free() call.
}


/************************************************************************
*    desc:  Set/Get the program ID
************************************************************************/
void CShaderData::SetProgramID( uint32_t location )
{
    m_programID = location;

}   // SetProgramID

uint32_t CShaderData::GetProgramID() const
{
    return m_programID;

}   // GetProgramID


/************************************************************************
*    desc:  Set/Get the vertex ID
************************************************************************/
void CShaderData::SetVertexID( uint32_t location )
{
    m_vertexID = location;

}   // SetVertexID

uint32_t CShaderData::GetVertexID() const
{
    return m_vertexID;

}   // GetVertexID


/************************************************************************
*    desc:  Set/Get the fragment ID
************************************************************************/
void CShaderData::SetFragmentID( uint32_t location )
{
    m_fragmentID = location;

}   // SetFragmentID

uint32_t CShaderData::GetFragmentID() const
{
    return m_fragmentID;

}   // GetFragmentID


/************************************************************************
*    desc:  Set/Get the attribute location
************************************************************************/
void CShaderData::SetAttributeLocation( const std::string & name, int32_t location )
{
    auto iter = m_attributeMap.find( name );
    if( iter != m_attributeMap.end() )
    {
        throw NExcept::CCriticalException("Set Attribute Error!",
            boost::str( boost::format("Shader attribute allready exists (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    m_attributeMap.emplace( name, location );

}   // SetAttributeLocation

int32_t CShaderData::GetAttributeLocation( const std::string & name ) const
{
    auto iter = m_attributeMap.find( name );
    if( iter == m_attributeMap.end() )
    {
        throw NExcept::CCriticalException("Get Attribute Error!",
            boost::str( boost::format("Shader attribute does not exist (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;

}   // GetAttributeLocation


/************************************************************************
*    desc:  Set/Get the uniform location
************************************************************************/
void CShaderData::SetUniformLocation( const std::string & name, int32_t location )
{
    auto iter = m_uniformMap.find( name );
    if( iter != m_uniformMap.end() )
    {
        throw NExcept::CCriticalException("Set Attribute Error!",
            boost::str( boost::format("Shader attribute allready exists (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    m_uniformMap.emplace( name, location );

}   // SetUniformLocation

int32_t CShaderData::GetUniformLocation( const std::string & name ) const
{
    auto iter = m_uniformMap.find( name );
    if( iter == m_uniformMap.end() )
    {
        throw NExcept::CCriticalException("Get Uniform Error!",
            boost::str( boost::format("Shader uniform does not exists (%s).\n\n%s\nLine: %s") % name % __FUNCTION__ % __LINE__ ));
    }

    return iter->second;

}   // GetUniformLocation


/************************************************************************
*    desc:  Check for the uniform location
************************************************************************/
bool CShaderData::HasUniformLocation( const std::string & name ) const
{
    auto iter = m_uniformMap.find( name );
    if( iter != m_uniformMap.end() )
        return true;

    return false;

}   // GetUniformLocation


/************************************************************************
*    desc:  Get the vertex attribute count
************************************************************************/
size_t CShaderData::GetVertexAttribCount()
{
    return m_attributeMap.size();
}


/************************************************************************
*    desc:  Free the data
************************************************************************/
void CShaderData::Free()
{
    // Detach the shaders
    if( (m_vertexID > 0) && (m_fragmentID > 0) && (m_programID > 0) )
    {
        glDetachShader(m_programID, m_vertexID);
        glDetachShader(m_programID, m_fragmentID);
    }

    // Delete the shaders
    if( m_vertexID > 0 )
        glDeleteShader(m_vertexID);

    if( m_fragmentID > 0 )
        glDeleteShader(m_fragmentID);

    // Delete the shader program
    if( m_programID > 0 )
        glDeleteProgram(m_programID);

    Clear();

}   // Free


/************************************************************************
*    desc:  Clear the data
************************************************************************/
void CShaderData::Clear()
{
    m_programID = m_vertexID = m_fragmentID = 0;

    m_attributeMap.clear();
    m_uniformMap.clear();

}   // Clear

