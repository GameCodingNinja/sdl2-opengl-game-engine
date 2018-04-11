
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
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "CPoint GetPos()",                                  asMETHOD(CObject,   GetPos_AngelScript), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void SetPos(CPoint & in)",                         asMETHOD(CObject,   SetPos_AngelScript), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3d", "void IncRot(CPoint & in)",                         asMETHOD(CObject,   IncRot),             asCALL_THISCALL) );
    }
}
