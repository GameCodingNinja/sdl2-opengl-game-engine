
/************************************************************************
*    FILE NAME:       scriptshadermanager.cpp
*
*    DESCRIPTION:     CShaderMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptshadermanager.h>

// Game lib dependencies
#include <managers/shadermanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptShaderManager
{
    /************************************************************************
    *    desc:  Load the shader from xml file path                                                        
    ************************************************************************/
    void LoadFromXML( const std::string & filePath )
    {
        try
        {
            CShaderMgr::Instance().loadFromXML( filePath );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().getEnginePtr();
        
        Throw( pEngine->RegisterGlobalFunction("void Shader_Load(string &in)", asFUNCTION(LoadFromXML), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Shader_SetAllShaderColor( string &in, CColor color )", asMETHODPR(CShaderMgr, setAllShaderColor, (const std::string &, CColor), void), asCALL_THISCALL_ASGLOBAL, &CShaderMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void Shader_SetAllShaderColor( string &in, float r, float g, float b, float a )", asMETHODPR(CShaderMgr, setAllShaderColor, (const std::string &, float, float, float, float), void), asCALL_THISCALL_ASGLOBAL, &CShaderMgr::Instance()) );
        
        Throw( pEngine->RegisterGlobalFunction("void Shader_SetShaderColor( string &in, string &in, CColor color )", asMETHODPR(CShaderMgr, setShaderColor, (const std::string &, const std::string &, CColor), void), asCALL_THISCALL_ASGLOBAL, &CShaderMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void Shader_SetShaderColor( string &in, string &in, float r, float g, float b, float a )", asMETHODPR(CShaderMgr, setShaderColor, (const std::string &, const std::string &, float, float, float, float), void), asCALL_THISCALL_ASGLOBAL, &CShaderMgr::Instance()) );
    }
}
