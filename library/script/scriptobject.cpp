
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
        
        Throw( pEngine->RegisterObjectMethod("CObject", "void SetPos(CPoint &in)",                 asMETHODPR(CObject,   SetPos, (const CPoint<float> &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void SetPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, SetPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncPos(CPoint & in)",                asMETHODPR(CObject, IncPos, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, IncPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "CPoint GetPos()",               asMETHOD(CObject, GetPosFloat),   asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   IncRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject, IncRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "const CPoint & GetRot()",       asMETHOD(CObject, GetRot),               asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "void SetScale(CPoint & in)",                asMETHODPR(CObject, SetScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void SetScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, SetScale, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncScale(CPoint & in)",                asMETHODPR(CObject, IncScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "void IncScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, IncScale, (float,float,float), void), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "const CPoint & GetScale()",     asMETHOD(CObject, GetScale),             asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CObject", "void SetVisible(bool visible)", asMETHOD(CObject, SetVisible),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CObject", "bool IsVisible()",              asMETHOD(CObject, IsVisible),            asCALL_THISCALL) );
    }
}
