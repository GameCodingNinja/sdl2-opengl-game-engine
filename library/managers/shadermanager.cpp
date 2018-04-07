
/************************************************************************
*    FILE NAME:       shadermanager.cpp
*
*    DESCRIPTION:     shader class singleton
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <managers/shadermanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <memory>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CShaderMgr::CShaderMgr() :
    m_pCurrentShaderData(nullptr),
    m_currentVertexAttribCount(0)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CShaderMgr::~CShaderMgr()
{
    // Free all textures in all groups
    for( m_Iter = m_shaderMap.begin();
         m_Iter != m_shaderMap.end();
         ++m_Iter )
    {
        m_Iter->second.Free();
    }

}   // destructor


/************************************************************************
*    desc:  Load the shader from xml file path
************************************************************************/
void CShaderMgr::LoadFromXML( const std::string & filePath )
{
    // Open and parse the XML file:
    XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "shaderLst" );
    if( mainNode.isEmpty() )
    {
        throw NExcept::CCriticalException("Shader Load Error!",
            boost::str( boost::format("Shader XML empty (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    }

    for( int i = 0; i < mainNode.nChildNode(); ++i )
        CreateShader( mainNode.getChildNode(i) );

}   // LoadFromXML


/************************************************************************
*    desc:  Create the shader
************************************************************************/
void CShaderMgr::CreateShader( const XMLNode & node )
{
    const std::string shaderStrId = node.getAttribute("Id");

    const XMLNode vertexNode = node.getChildNode("vertDataLst");
    const XMLNode fragmentNode = node.getChildNode("fragDataLst");

    // Check that the name doesn't exist
    auto iter = m_shaderMap.find( shaderStrId );
    if( iter != m_shaderMap.end() )
    {
        throw NExcept::CCriticalException("Shader Load Error!",
            boost::str( boost::format("Shader of this name already exists (%s).\n\n%s\nLine: %s")
                % shaderStrId % __FUNCTION__ % __LINE__ ));
    }

    // Insert the new shader
    // Get an iterator to the newly added shader data.
    // This is to aid in cleanup in the event of an error
    m_Iter = m_shaderMap.emplace( shaderStrId, CShaderData() ).first;

    // Create the vertex shader
    CreateShader( GL_VERTEX_SHADER, vertexNode.getAttribute("file") );

    // Create the vertex shader
    CreateShader( GL_FRAGMENT_SHADER, fragmentNode.getAttribute("file") );

    // Link the shader
    CreateProgram();

    // Bind the attribute location by index defined in shader config file
    BindAttributeLocation( vertexNode );

    // Link the shader program
    LinkProgram();

    // Set all the shader attributes
    LocateShaderVariables( vertexNode, fragmentNode );
    
    // Send out a signal to init this shader
    m_initShaderSignal( shaderStrId );

}   // CreateShader


/************************************************************************
*    desc:  Create the shader
************************************************************************/
void CShaderMgr::CreateShader( GLenum shaderType, const std::string & filePath )
{
    // Create the shader
    GLuint shaderID = glCreateShader( shaderType );
    if( shaderID == 0 )
    {
        throw NExcept::CCriticalException("Create Shader Error!", 
            boost::str( boost::format("Error creating shader (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    }

    if( shaderType == GL_VERTEX_SHADER )
        m_Iter->second.SetVertexID( shaderID );
    else
        m_Iter->second.SetFragmentID( shaderID );

    // Load the shader from file
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath );
    const char * pChar = spChar.get();
    glShaderSource( shaderID, 1, &pChar, nullptr );

    // Compile shader source
    glCompileShader( shaderID );

    // Check shader for errors
    GLint success( GL_FALSE );
    glGetShaderiv( shaderID, GL_COMPILE_STATUS, &success );
    if( success != GL_TRUE )
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        std::unique_ptr<char[]> upError( new char[maxLength] );
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, upError.get());

        throw NExcept::CCriticalException("Create Vertex Shader Error!", 
            boost::str( boost::format("Error compiling vertex shader (%s).\n\n%s.\n\n%s\nLine: %s")
                % filePath % upError.get() % __FUNCTION__ % __LINE__ ));
    }

}   // CreateShader


/************************************************************************
*    desc:  Create the shader program
************************************************************************/
void CShaderMgr::CreateProgram()
{
    // Create the program - OpenGL shaders are called programs
    m_Iter->second.SetProgramID( glCreateProgram() );
    if( m_Iter->second.GetProgramID() == 0 )
    {
        throw NExcept::CCriticalException("Create Shader Error!", 
            boost::str( boost::format("Error creating shader (%s).\n\n%s\nLine: %s")
                % m_Iter->first % __FUNCTION__ % __LINE__ ));
    }

    // Attach shaders to the program
    glAttachShader( m_Iter->second.GetProgramID(), m_Iter->second.GetVertexID() );
    glAttachShader( m_Iter->second.GetProgramID(), m_Iter->second.GetFragmentID() );

}   // CreateProgram


/************************************************************************
*    desc:  Bind the attribute location
*           NOTE: This function is called if 0 or greater is specified in
*           the shader config to allow the defining of the attribute index
************************************************************************/
void CShaderMgr::BindAttributeLocation( const XMLNode & vertexNode )
{
    // Get the location ID for the vertex variables
    for( int i = 0; i < vertexNode.nChildNode(); ++i )
    {
        const XMLNode node = vertexNode.getChildNode(i);

        if( node.isAttributeSet( "location" ) )
        {
            const int location = std::atoi( node.getAttribute("location") );
            const std::string attributeName = node.getAttribute("name");

            // Save the attribute for use later
            m_Iter->second.SetAttributeLocation( attributeName, location );

            // Bind a constant attribute location for positions of vertices
            glBindAttribLocation( m_Iter->second.GetProgramID(), (GLuint)location, attributeName.c_str() );

            GLenum error(glGetError());
            if( error != GL_NO_ERROR)
                throw NExcept::CCriticalException("Create Shader Error!", 
                    boost::str( boost::format("Error binding attribute (%s).\n\n%s\nLine: %s")
                        % attributeName % __FUNCTION__ % __LINE__ ));
        }
    }

}   // BindAttributeLocation


/************************************************************************
*    desc:  Link the shader program
************************************************************************/
void CShaderMgr::LinkProgram()
{
    // Link shader program
    glLinkProgram( m_Iter->second.GetProgramID() );

    // Check for errors
    GLint success( GL_TRUE );
    glGetProgramiv( m_Iter->second.GetProgramID(), GL_LINK_STATUS, &success );
    if( success != GL_TRUE )
    {
        throw NExcept::CCriticalException("Link Shader Error!", 
            boost::str( boost::format("Error linking shader (%s).\n\n%s\nLine: %s")
                % m_Iter->first % __FUNCTION__ % __LINE__ ));
    }

}   // LinkProgram


/************************************************************************
*    desc:  Find the location of the custom shader variables
************************************************************************/
void CShaderMgr::LocateShaderVariables(
    const XMLNode & vertexNode,
    const XMLNode & fragmentNode )
{
    // Get the location ID for the vertex attributes and uniforms
    for( int i = 0; i < vertexNode.nChildNode(); ++i )
    {
        const XMLNode node = vertexNode.getChildNode(i);

        if( !node.isAttributeSet( "location" ) )
            GetUniformLocation( node );
    }

    // Get the location ID for the fragment variables
    for( int i = 0; i < fragmentNode.nChildNode(); ++i )
        GetUniformLocation( fragmentNode.getChildNode(i) );

}   // LocateShaderVariables


/************************************************************************
*    desc:  Get the uniform location
************************************************************************/
void CShaderMgr::GetUniformLocation( const XMLNode & node )
{
    std::string name = node.getAttribute("name");

    GLint location = glGetUniformLocation( m_Iter->second.GetProgramID(), name.c_str() );

    m_Iter->second.SetUniformLocation( name, location );

    if( location < 0 )
    {
        throw NExcept::CCriticalException("Shader Uniform Location Error!", 
            boost::str( boost::format("Error Uniform Location (%s) not found (%s).\n\n%s\nLine: %s")
                % name % m_Iter->first % __FUNCTION__ % __LINE__ ));
    }

}   // GetUniformLocation


/************************************************************************
*    desc:  Get the shader data
************************************************************************/
CShaderData & CShaderMgr::GetShaderData( const std::string & shaderId )
{
    // See if we can find the shader
    m_Iter = m_shaderMap.find( shaderId );
    if( m_Iter == m_shaderMap.end() )
    {
        throw NExcept::CCriticalException("Shader Data Error!",
            boost::str( boost::format("Shader of this name doesn't exists (%s).\n\n%s\nLine: %s")
                % shaderId % __FUNCTION__ % __LINE__ ));
    }

    return m_Iter->second;

}   // GetShaderData


/************************************************************************
*    desc:  Function call used to manage what shader is currently bound.
*           This insures that we don't keep rebinding the same shader
*           Note: Vertex Attribute Arrays should always be defined in
*           the shader.cfg starting from 0 to max count. Makes live easy
************************************************************************/
void CShaderMgr::Bind( CShaderData * pShaderData )
{
    if( m_pCurrentShaderData != pShaderData )
    {
        if( m_pCurrentShaderData == nullptr )
        {
            m_currentVertexAttribCount = pShaderData->GetVertexAttribCount();
            
            for( size_t i = 0; i < m_currentVertexAttribCount; ++i )
                glEnableVertexAttribArray(i);
        }
        else if( m_currentVertexAttribCount != pShaderData->GetVertexAttribCount() )
        {
            const size_t attribCount( pShaderData->GetVertexAttribCount() );
            
            if( m_currentVertexAttribCount < attribCount )
            {
                for( size_t i = m_currentVertexAttribCount; i < attribCount; ++i )
                    glEnableVertexAttribArray(i);
            }
            else
            {
                for( size_t i = attribCount; i < m_currentVertexAttribCount; ++i )
                    glDisableVertexAttribArray(i);
            }
            
            m_currentVertexAttribCount = attribCount;
        }

        // save the current binding
        m_pCurrentShaderData = pShaderData;

        // Have OpenGL bind this shader now
        glUseProgram( pShaderData->GetProgramID() );
    }

}   // Bind


/************************************************************************
*    desc:  Unbind the shader and reset the flag
************************************************************************/
void CShaderMgr::Unbind()
{
    for( size_t i = 0; i < m_currentVertexAttribCount; ++i )
        glDisableVertexAttribArray(i);
    
    m_pCurrentShaderData = nullptr;
    m_currentVertexAttribCount = 0;
    glUseProgram( 0 );

}   // Unbind


/************************************************************************
*    desc:  Free the shader
************************************************************************/
void CShaderMgr::FreeShader( const std::string & shaderId )
{
    // See if we can find the shader
    m_Iter = m_shaderMap.find( shaderId );
    if( m_Iter != m_shaderMap.end() )
    {
        m_Iter->second.Free();
        
        // Erase this shader
        m_shaderMap.erase( m_Iter );
    }

}   // FreeShader


/************************************************************************
*    desc:  Connect to the Init Shader signal
************************************************************************/
void CShaderMgr::Connect_InitShader( const InitShaderSignal::slot_type & slot )
{
    m_initShaderSignal.connect(slot);

}   // Connect_InitShader


/************************************************************************
*    desc:  Set the shader member variable
************************************************************************/
void CShaderMgr::SetShaderColor( const std::string & shaderId, const std::string & locationId, CColor color )
{
    SetShaderColor( GetShaderData( shaderId ), locationId, color );
    
}   // SetShaderColor

void CShaderMgr::SetShaderColor( CShaderData & shaderData, const std::string & locationId, CColor color )
{
    // Check for the shader location
    if( shaderData.HasUniformLocation( locationId ) )
    {
        // Get the location of the additive
        GLint location = shaderData.GetUniformLocation( locationId );

        // Bind the shader so that we can change the value of the member
        Bind( &shaderData );

        // Set the color
        glUniform4fv( location, 1, (float *)&color );

        // Unbind now that we are done
        Unbind();
    }
    
}   // SetShaderColor


/************************************************************************
*    desc:  Set all the shaders using this color location
************************************************************************/
void CShaderMgr::SetAllShaderColor( const std::string & locationId, CColor color )
{
    for( auto & iter : m_shaderMap )
        SetShaderColor( iter.second, locationId, color );
    
}   // SetAllShaderColor
