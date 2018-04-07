
/************************************************************************
*    FILE NAME:       scriptshadermanager.cpp
*
*    DESCRIPTION:     CShaderMgr script object registration
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <script/scriptshadermanager.h>

// Game lib dependencies
#include <managers/shadermanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptShaderManager
{
    /************************************************************************
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        Throw( pEngine->RegisterGlobalFunction("void SetAllShaderColor( string &in, CColor color )", asMETHOD(CShaderMgr, SetAllShaderColor), asCALL_THISCALL_ASGLOBAL, &CShaderMgr::Instance()) );
    }

}   // NScriptColor