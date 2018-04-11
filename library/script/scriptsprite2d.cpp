
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
        Throw( pEngine->RegisterObjectType(  "CSprite2d", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetColor(const CColor &in)",                    asMETHODPR(CSprite2D, SetColor, (const CColor &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetColor(float r, float g, float b, float a)",  asMETHODPR(CSprite2D, SetColor, (float,float,float,float), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetDefaultColor()",                             asMETHOD(CSprite2D, SetDefaultColor),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "const CColor & GetColor()",                          asMETHOD(CSprite2D, GetColor),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "const CColor & GetDefaultColor()",                   asMETHOD(CSprite2D, GetDefaultColor),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "uint GetFrameCount()",                               asMETHOD(CSprite2D, GetFrameCount),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetFrame(uint index)",                          asMETHOD(CSprite2D, SetFrame),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "uint GetCurrentFrame()",                             asMETHOD(CSprite2D, GetCurrentFrame),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetAlpha(float alpha)",                         asMETHOD(CSprite2D, SetAlpha),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "float GetAlpha()",                                   asMETHOD(CSprite2D, GetAlpha),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "float GetDefaultAlpha()",                            asMETHOD(CSprite2D, GetDefaultAlpha),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetDefaultAlpha()",                             asMETHOD(CSprite2D, SetDefaultAlpha),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void CreateFontString(string &in)",                  asMETHOD(CSprite2D, CreateFontString),     asCALL_THISCALL) );

        // Need to use CObject for the below to compile under MSVSC++
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetVisible(bool visible)",                      asMETHOD(CObject,   SetVisible),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "bool IsVisible()",                                   asMETHOD(CObject,   IsVisible),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "CPoint GetPos()",                                    asMETHOD(CObject,   GetPos_AngelScript),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetPos(CPoint &in)",                            asMETHOD(CObject,   SetPos_AngelScript),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHOD(CObject,   SetRot),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHOD(CObject,   IncRot),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetScale(CPoint &in)",                          asMETHOD(CObject,   SetScale),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "const CPoint & GetScale()",                          asMETHOD(CObject,   GetScale),             asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetPosXYZ(float x, float y, float z)",          asMETHOD(CObject,   SetPosXYZ),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncPosXYZ(float x, float y, float z)",          asMETHOD(CObject,   IncPosXYZ),            asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetRotXYZ(float x, float y, float z, bool convertToRadians = true)", asMETHOD(CObject, SetRotXYZ), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncRotXYZ(float x, float y, float z, bool convertToRadians = true)", asMETHOD(CObject, IncRotXYZ), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void SetScaleXYZ(float x, float y, float z)",        asMETHOD(CObject,   SetScaleXYZ),          asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite2d", "void IncScaleXYZ(float x, float y, float z)",        asMETHOD(CObject,   IncScaleXYZ),          asCALL_THISCALL) );

        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, CSprite2d @)", asMETHOD(CScriptManager, PrepareSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void LocalSpawn(string &in, CSprite2d @)", asMETHOD(CScriptManager, PrepareLocalSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    }
}