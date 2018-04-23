
/************************************************************************
*    FILE NAME:       scriptmenu.cpp
*
*    DESCRIPTION:     CMenu script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptmenu.h>

// Game lib dependencies
#include <gui/menu.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptMenu
{
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CMenu", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CMenu", "void SetAlpha(float alpha)",  asMETHOD(CMenu, setAlpha), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CMenu", "float GetAlpha()",            asMETHOD(CMenu, getAlpha), asCALL_THISCALL) );

        // Need to use CObject for the below to compile under MSVSC++
        Throw( pEngine->RegisterObjectMethod("CMenu", "void SetVisible(bool visible)", asMETHOD(CObject, setVisible), asCALL_THISCALL) );
    }
}
