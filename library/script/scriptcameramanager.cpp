
/************************************************************************
*    FILE NAME:       scriptcameramanager.cpp
*
*    DESCRIPTION:     CCameraMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsoundmanager.h>

// Game lib dependencies
#include <managers/cameramanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptCameraManager
{
    /************************************************************************
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        Throw( pEngine->RegisterGlobalFunction("CCamera & Camera_CreateOrthographic( string &in, float minZDist = 5, float maxZDist = 1000, float scale = 1.f )", asMETHOD(CCameraMgr, createOrthographic), asCALL_THISCALL_ASGLOBAL, &CCameraMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CCamera & Camera_CreatePerspective( string &in, float angle = 45, float minZDist = 5, float maxZDist = 1000, float scale = 1.f )", asMETHOD(CCameraMgr, createPerspective), asCALL_THISCALL_ASGLOBAL, &CCameraMgr::Instance()) );
    }

}   // NScriptColor