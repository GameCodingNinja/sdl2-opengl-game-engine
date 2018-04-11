
/************************************************************************
*    FILE NAME:       scriptobject.cpp
*
*    DESCRIPTION:     CObject script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptobject.h>

// Game lib dependencies
#include <common/object.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptObject
{
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CObject", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "CPoint GetPos()",               asMETHOD(CObject, GetPos_AngelScript),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void SetPos(CPoint & in)",      asMETHOD(CObject, SetPos_AngelScript),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncPos(CPoint & in)",      asMETHOD(CObject, IncPos_AngelScript),   asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetRot(CPoint & in, bool convertToRadians = true)", asMETHOD(CObject, SetRot), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncRot(CPoint & in, bool convertToRadians = true)", asMETHOD(CObject, IncRot), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "const CPoint & GetRot()",       asMETHOD(CObject, GetRot),               asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetScale(CPoint & in)",    asMETHOD(CObject, SetScale),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncScale(CPoint & in)",    asMETHOD(CObject, IncScale),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "const CPoint & GetScale()",     asMETHOD(CObject, GetScale),             asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetPosXYZ(float x, float y, float z)",   asMETHOD(CObject, SetPosXYZ), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncPosXYZ(float x, float y, float z)",   asMETHOD(CObject, IncPosXYZ), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetRotXYZ(float x, float y, float z, bool convertToRadians = true)",   asMETHOD(CObject, SetRotXYZ), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncRotXYZ(float x, float y, float z, bool convertToRadians = true)",   asMETHOD(CObject, IncRotXYZ), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetScaleXYZ(float x, float y, float z)", asMETHOD(CObject, SetScaleXYZ), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncScaleXYZ(float x, float y, float z)", asMETHOD(CObject, IncScaleXYZ), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetVisible(bool visible)", asMETHOD(CObject, SetVisible),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "bool IsVisible()",              asMETHOD(CObject, IsVisible),            asCALL_THISCALL) );
    }
}
