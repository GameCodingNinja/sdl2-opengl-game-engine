
/************************************************************************
*    FILE NAME:       shaderdata.h
*
*    DESCRIPTION:     shader data class
************************************************************************/

#ifndef __shader_data_h__
#define __shader_data_h__

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

class CShaderData
{
public:

    CShaderData();
    ~CShaderData();

    // Set/Get the program location
    void SetProgramID( uint32_t location );
    uint32_t GetProgramID() const;

    // Set/Get the vertex ID
    void SetVertexID( uint32_t location );
    uint32_t GetVertexID() const;

    // Set/Get the fragment ID
    void SetFragmentID( uint32_t location );
    uint32_t GetFragmentID() const;

    // Get the attribute location
    void SetAttributeLocation( const std::string & name, int32_t location );
    int32_t GetAttributeLocation( const std::string & name ) const;

    // Get the uniform location
    void SetUniformLocation( const std::string & name, int32_t location );
    int32_t GetUniformLocation( const std::string & name ) const;
    
    // Check for the uniform location
    bool HasUniformLocation( const std::string & name ) const;
    
    // Get the vertex attribute count
    size_t GetVertexAttribCount();

    // Free the data
    void Free();

private:

    // Clear the data
    void Clear();

private:

    // OpenGL ID's
    uint32_t m_programID;
    uint32_t m_vertexID;
    uint32_t m_fragmentID;

    // attribute location shader map
    std::map<const std::string, int32_t > m_attributeMap;

    // uniform location shader map
    std::map<const std::string, int32_t > m_uniformMap;

};

#endif  // __shader_data_h__


