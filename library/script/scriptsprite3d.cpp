
/************************************************************************
*    FILE NAME:       scriptsprite3d.cpp
*
*    DESCRIPTION:     Sprite3D script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsprite3d.h>

// Game lib dependencies
#include <3d/sprite3d.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSprite3D
{
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CSprite3d", 0, asOBJ_REF | asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetVisible(bool visible)",                    asMETHOD(CObject,   SetVisible),         asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetColor(const CColor &in)",                 asMETHODPR(CSprite3D, SetColor, (const CColor &), void),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetColor(float r, float g, float b, float a)", asMETHODPR(CSprite3D, SetColor, (float,float,float,float), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetDefaultColor()",                           asMETHOD(CSprite3D, SetDefaultColor),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "const CColor & GetColor()",                        asMETHOD(CSprite3D, GetColor),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "const CColor & GetDefaultColor()",                 asMETHOD(CSprite3D, GetDefaultColor),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetAlpha(float alpha)",                       asMETHOD(CSprite3D, SetAlpha),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "float GetAlpha()",                                 asMETHOD(CSprite3D, GetAlpha),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "float GetDefaultAlpha()",                          asMETHOD(CSprite3D, GetDefaultAlpha),    asCALL_THISCALL) );
        
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetPos(CPoint &in)",                 asMETHODPR(CObject,   SetPos, (const CPoint<float> &), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, SetPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncPos(CPoint & in)",                asMETHODPR(CObject, IncPos, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncPos(float x = 0, float y = 0, float z = 0)",  asMETHODPR(CObject, IncPos, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "CPoint GetPos()",               asMETHOD(CObject, GetPosFloat),   asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject,   SetRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncRot(CPoint &in, bool convertToRadians = true)", asMETHODPR(CObject,   IncRot, (const CPoint<float> &, bool), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asMETHODPR(CObject, IncRot, (float, float, float, bool), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "const CPoint & GetRot()",       asMETHOD(CObject, GetRot),               asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetScale(CPoint & in)",                asMETHODPR(CObject, SetScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, SetScale, (float,float,float), void), asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncScale(CPoint & in)",                asMETHODPR(CObject, IncScale, (const CPoint<float> &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncScale(float x = 1, float y = 1, float z = 1)",  asMETHODPR(CObject, IncScale, (float,float,float), void), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSprite3d", "const CPoint & GetScale()",     asMETHOD(CObject, GetScale),             asCALL_THISCALL) );
    }
}
