
/************************************************************************
*    FILE NAME:       shadermanager.h
*
*    DESCRIPTION:     shader class singleton
************************************************************************/

#ifndef __shader_manager_h__
#define __shader_manager_h__

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <common/shaderdata.h>
#include <common/color.h>

// Boost lib dependencies
#include <boost/signals2.hpp>

// Standard lib dependencies
#include <string>
#include <map>

class CShaderMgr
{
public:
    
    // Boost signal definition
    typedef boost::signals2::signal<void (const std::string)> InitShaderSignal;

    // Get the instance of the singleton class
    static CShaderMgr & Instance()
    {
        static CShaderMgr shader;
        return shader;
    }

    // Load the shader from xml file path
    void LoadFromXML( const std::string & filePath );

    // Get the shader data
    CShaderData & GetShaderData( const std::string & shaderId );

    // Function call used to manage what shader is currently bound.
    void Bind( CShaderData * pShaderData );

    // Unbind the shader and reset the flag
    void Unbind();
    
    // Free the shader
    void FreeShader( const std::string & shaderId );
    
    // Set the shader member variable
    void SetShaderColor( const std::string & shaderId, const std::string & locationId, CColor color );
    
    // Set all the shaders using this color location
    void SetAllShaderColor( const std::string & locationId, CColor color );
    
    // Connect to the Init Shader signal
    void Connect_InitShader( const InitShaderSignal::slot_type & slot );

private:

    // Constructor
    CShaderMgr();

    // Destructor
    ~CShaderMgr();

    // Create the shader
    void CreateShader( const XMLNode & node );

    // Create the shader
    void CreateShader( GLenum shaderType, const std::string & filePath );

    // Create the shader program
    void CreateProgram();

    // Bind the attribute location
    void BindAttributeLocation( const XMLNode & vertexNode );

    // Link the shader program
    void LinkProgram();

    // Find the location of the shader variables
    void LocateShaderVariables(
        const XMLNode & vertexNode,
        const XMLNode & fragmentNode );
    
    // Get the uniform location
    void GetUniformLocation( const XMLNode & node );
    
    // Set the shader member variable
    void SetShaderColor( CShaderData & shaderData, const std::string & locationId, CColor color );

private:

    // Map containing shader programs
    std::map< const std::string, CShaderData > m_shaderMap;
    std::map< const std::string, CShaderData >::iterator m_Iter;

    // Current shader data pointer - We DON'T own this pointer, don't free
    CShaderData * m_pCurrentShaderData;
    
    // Current vertex attribute count
    size_t m_currentVertexAttribCount;
    
    // Init Shader signal
    InitShaderSignal m_initShaderSignal;

};

#endif  // __shader_manager_h__
