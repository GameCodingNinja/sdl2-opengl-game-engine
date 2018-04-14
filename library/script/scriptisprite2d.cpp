
/************************************************************************
*    FILE NAME:       scriptisprite2d.cpp
*
*    DESCRIPTION:     Sprite2D interface script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptisprite2d.h>

// Game lib dependencies
#include <2d/sprite2d.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptiSprite2D
{
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "iSprite2D", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "int GetId()",                                   asMETHOD(iSprite2D,   GetId),            asCALL_THISCALL) );

        // Need to use CObject for the below to compile under MSVSC++
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetVisible(bool)",                              asMETHOD(CObject,   SetVisible),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "bool IsVisible()",                                   asMETHOD(CObject,   IsVisible),            asCALL_THISCALL) );
        
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetPos(CPoint &in)",                             asMETHODPR(CObject,   SetPos, (const CPoint<float> &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, SetPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void IncPos(CPoint & in)",                            asMETHODPR(CObject, IncPos, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void IncPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, IncPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "CPoint GetPos()",                                       asMETHOD(CObject, GetPosFloat),   asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   IncRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject, IncRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "const CPoint & GetRot()",                               asMETHOD(CObject, GetRot),               asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetScale(CPoint & in)",                            asMETHODPR(CObject, SetScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void SetScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, SetScale, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void IncScale(CPoint & in)",                            asMETHODPR(CObject, IncScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite2D", "void IncScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, IncScale, (float,float,float), void), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("iSprite2D", "const CPoint & GetScale()",     asMETHOD(CObject, GetScale),             asCALL_THISCALL) );
    }
}