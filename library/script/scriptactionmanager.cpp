
/************************************************************************
*    FILE NAME:       scriptsoundmanager.cpp
*
*    DESCRIPTION:     CActionMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsoundmanager.h>

// Game lib dependencies
#include <managers/actionmanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptActionManager
{
    /************************************************************************
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();
        
        Throw( pEngine->RegisterGlobalFunction("void Action_Load( string &in )", asMETHOD(CActionMgr, loadActionFromXML), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );

        Throw( pEngine->RegisterGlobalFunction("bool Action_WasEvent( string &in, int actionPress = 1)", asMETHOD(CActionMgr, wasEvent), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        
        Throw( pEngine->RegisterGlobalFunction("bool Action_WasKeyboard( string &in, int actionPress = 1)", asMETHOD(CActionMgr, wasKeyboard), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool Action_WasMouse( string &in, int actionPress = 1)", asMETHOD(CActionMgr, wasMouse), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool Action_WasGamepad( string &in, int actionPress = 1)", asMETHOD(CActionMgr, wasGamepad), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        
        Throw( pEngine->RegisterGlobalFunction("const CPoint & Action_GetMouseAbsolutePos()", asMETHOD(CActionMgr, getMouseAbsolutePos), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("const CPoint & Action_GetMouseRelativePos()", asMETHOD(CActionMgr, getMouseRelativePos), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("const CPoint & Action_GetControllerPosLeft()", asMETHOD(CActionMgr, getControllerPosLeft), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("const CPoint & Action_GetControllerPosRight()", asMETHOD(CActionMgr, getControllerPosRight), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        
        Throw( pEngine->RegisterGlobalFunction("bool Action_WasLastDeviceGamepad()", asMETHOD(CActionMgr, wasLastDeviceGamepad), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool Action_WasLastDeviceKeyboard()", asMETHOD(CActionMgr, wasLastDeviceKeyboard), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool Action_WasLastDeviceMouse()", asMETHOD(CActionMgr, wasLastDeviceMouse), asCALL_THISCALL_ASGLOBAL, &CActionMgr::Instance()) );
    }

}   // NScriptColor