
/************************************************************************
*    FILE NAME:       scriptactionmanager.cpp
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
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CActionMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void load(string &in)",                             asMETHOD(CActionMgr, loadActionFromXML),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void wasEvent(string &in, int actionPress = 1)",    asMETHOD(CActionMgr, wasEvent),              asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void wasKeyboard(string &in, int actionPress = 1)", asMETHOD(CActionMgr, wasKeyboard),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void wasMouse(string &in, int actionPress = 1)",    asMETHOD(CActionMgr, wasMouse),              asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "void wasGamepad(string &in, int actionPress = 1)",  asMETHOD(CActionMgr, wasGamepad),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getMouseAbsolutePos()",              asMETHOD(CActionMgr, getMouseAbsolutePos),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getMouseRelativePos()",              asMETHOD(CActionMgr, getMouseRelativePos),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getControllerPosLeft()",             asMETHOD(CActionMgr, getControllerPosLeft),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "const CPoint & getControllerPosRight()",            asMETHOD(CActionMgr, getControllerPosRight), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceGamepad()",                       asMETHOD(CActionMgr, wasLastDeviceGamepad),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceKeyboard()",                      asMETHOD(CActionMgr, wasLastDeviceKeyboard), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CActionMgr", "bool wasLastDeviceMouse()",                         asMETHOD(CActionMgr, wasLastDeviceMouse),    asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CActionMgr ActionMgr", &CActionMgr::Instance()) );
    }
}