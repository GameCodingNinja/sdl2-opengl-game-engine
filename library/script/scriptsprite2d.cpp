
/************************************************************************
*    FILE NAME:       scriptsprite2d.cpp
*
*    DESCRIPTION:     Sprite2D script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsprite2d.h>

// Game lib dependencies
#include <2d/sprite2d.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSprite2D
{
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CSprite2D", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "int GetId()",                                        asMETHOD(CSprite2D,   GetId),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetColor(const CColor &in)",                    asMETHODPR(CSprite2D, SetColor, (const CColor &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetColor(float, float, float, float)",          asMETHODPR(CSprite2D, SetColor, (float,float,float,float), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetDefaultColor()",                             asMETHOD(CSprite2D, SetDefaultColor),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "const CColor & GetColor()",                          asMETHOD(CSprite2D, GetColor),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "const CColor & GetDefaultColor()",                   asMETHOD(CSprite2D, GetDefaultColor),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "uint GetFrameCount()",                               asMETHOD(CSprite2D, GetFrameCount),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetFrame(uint)",                                asMETHOD(CSprite2D, SetFrame),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "uint GetCurrentFrame()",                             asMETHOD(CSprite2D, GetCurrentFrame),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetAlpha(float alpha)",                         asMETHOD(CSprite2D, SetAlpha),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "float GetAlpha()",                                   asMETHOD(CSprite2D, GetAlpha),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "float GetDefaultAlpha()",                            asMETHOD(CSprite2D, GetDefaultAlpha),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetDefaultAlpha()",                             asMETHOD(CSprite2D, SetDefaultAlpha),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void CreateFontString(string &in)",                  asMETHOD(CSprite2D, CreateFontString),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetPhysicsTransform(float, float, float, bool resetVelocity = true)", asMETHOD(CSprite2D, SetPhysicsTransform),             asCALL_THISCALL) );

        // Need to use CObject for the below to compile under MSVSC++
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetVisible(bool)",                              asMETHOD(CObject,   SetVisible),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "bool IsVisible()",                                   asMETHOD(CObject,   IsVisible),            asCALL_THISCALL) );
        
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetPos(CPoint &in)",                             asMETHODPR(CObject,   SetPos, (const CPoint<float> &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, SetPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncPos(CPoint & in)",                            asMETHODPR(CObject, IncPos, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, IncPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "CPoint GetPos()",                                       asMETHOD(CObject, GetPosFloat),   asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   IncRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject, IncRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "const CPoint & GetRot()",                               asMETHOD(CObject, GetRot),               asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetScale(CPoint & in)",                            asMETHODPR(CObject, SetScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, SetScale, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncScale(CPoint & in)",                            asMETHODPR(CObject, IncScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, IncScale, (float,float,float), void), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite2D", "const CPoint & GetScale()",     asMETHOD(CObject, GetScale),             asCALL_THISCALL) );

        
        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, CSprite2D @)", asMETHOD(CScriptManager, PrepareSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void LocalSpawn(string &in, CSprite2D @)", asMETHOD(CScriptManager, PrepareLocalSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    }
}