
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
    *    desc:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetVisible(bool value, CSprite2D & sprite)
    {
        sprite.SetVisible(value);
    }
    
    bool IsVisible(CSprite2D & sprite)
    {
        return sprite.IsVisible();
    }
    
    void SetPos1(const CPoint<float> & pos, CSprite2D & sprite)
    {
        sprite.SetPos(pos);
    }
    
    void SetPos2(float x, float y, float z, CSprite2D & sprite)
    {
        sprite.SetPos(x,y,z);
    }
    
    void IncPos1(const CPoint<float> & pos, CSprite2D & sprite)
    {
        sprite.IncPos(pos);
    }
    
    void IncPos2(float x, float y, float z, CSprite2D & sprite)
    {
        sprite.IncPos(x,y,z);
    }
    
    CPoint<float> GetPos(CSprite2D & sprite)
    {
        return sprite.GetPosFloat();
    }
    
    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CSprite2D & sprite)
    {
        sprite.SetRot(rot, convertToRadians);
    }
    
    void SetRot2(float x, float y, float z, bool convertToRadians, CSprite2D & sprite)
    {
        sprite.SetRot(x,y,z, convertToRadians);
    }
    
    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CSprite2D & sprite)
    {
        sprite.IncRot(rot, convertToRadians);
    }
    
    void IncRot2(float x, float y, float z, bool convertToRadians, CSprite2D & sprite)
    {
        sprite.IncRot(x,y,z, convertToRadians);
    }
    
    const CPoint<float> & GetRot(CSprite2D & sprite)
    {
        return sprite.GetRot();
    }
    
    void SetScale1(const CPoint<float> & rot, CSprite2D & sprite)
    {
        sprite.SetScale(rot);
    }
    
    void SetScale2(float x, float y, float z, CSprite2D & sprite)
    {
        sprite.SetScale(x,y,z);
    }
    
    void IncScale1(const CPoint<float> & rot, CSprite2D & sprite)
    {
        sprite.IncScale(rot);
    }
    
    void IncScale2(float x, float y, float z, CSprite2D & sprite)
    {
        sprite.IncScale(x,y,z);
    }

    const CPoint<float> & GetScale(CSprite2D & sprite)
    {
        return sprite.GetScale();
    }
    
    
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

        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetVisible(bool)",                              asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "bool IsVisible()",                                   asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );
        
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "CPoint GetPos()",                                     asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "const CPoint & GetRot()",                               asFUNCTION(GetRot),               asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void SetScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite2D", "void IncScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite2D", "const CPoint & GetScale()",     asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );

        
        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, CSprite2D @)", asMETHOD(CScriptManager, PrepareSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void LocalSpawn(string &in, CSprite2D @)", asMETHOD(CScriptManager, PrepareLocalSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    }
}