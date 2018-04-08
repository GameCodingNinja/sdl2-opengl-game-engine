
/************************************************************************
*    FILE NAME:       scriptobjectdatamanager.cpp
*
*    DESCRIPTION:     CShaderMgr script object registration
************************************************************************/

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