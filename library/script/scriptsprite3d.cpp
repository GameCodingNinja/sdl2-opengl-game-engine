
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
    *    desc:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetVisible(bool value, CSprite3D & sprite)
    {
        sprite.SetVisible(value);
    }
    
    bool IsVisible(CSprite3D & sprite)
    {
        return sprite.IsVisible();
    }
    
    void SetPos1(const CPoint<float> & pos, CSprite3D & sprite)
    {
        sprite.SetPos(pos);
    }
    
    void SetPos2(float x, float y, float z, CSprite3D & sprite)
    {
        sprite.SetPos(x,y,z);
    }
    
    void IncPos1(const CPoint<float> & pos, CSprite3D & sprite)
    {
        sprite.IncPos(pos);
    }
    
    void IncPos2(float x, float y, float z, CSprite3D & sprite)
    {
        sprite.IncPos(x,y,z);
    }
    
    CPoint<float> GetPos(CSprite3D & sprite)
    {
        return sprite.GetPosFloat();
    }
    
    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CSprite3D & sprite)
    {
        sprite.SetRot(rot, convertToRadians);
    }
    
    void SetRot2(float x, float y, float z, bool convertToRadians, CSprite3D & sprite)
    {
        sprite.SetRot(x,y,z, convertToRadians);
    }
    
    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CSprite3D & sprite)
    {
        sprite.IncRot(rot, convertToRadians);
    }
    
    void IncRot2(float x, float y, float z, bool convertToRadians, CSprite3D & sprite)
    {
        sprite.IncRot(x,y,z, convertToRadians);
    }
    
    const CPoint<float> & GetRot(CSprite3D & sprite)
    {
        return sprite.GetRot();
    }
    
    void SetScale1(const CPoint<float> & rot, CSprite3D & sprite)
    {
        sprite.SetScale(rot);
    }
    
    void SetScale2(float x, float y, float z, CSprite3D & sprite)
    {
        sprite.SetScale(x,y,z);
    }
    
    void IncScale1(const CPoint<float> & rot, CSprite3D & sprite)
    {
        sprite.IncScale(rot);
    }
    
    void IncScale2(float x, float y, float z, CSprite3D & sprite)
    {
        sprite.IncScale(x,y,z);
    }

    const CPoint<float> & GetScale(CSprite3D & sprite)
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
        Throw( pEngine->RegisterObjectType(  "CSprite3D", 0, asOBJ_REF | asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetColor(const CColor &in)",                 asMETHODPR(CSprite3D, SetColor, (const CColor &), void),  asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetColor(float r, float g, float b, float a)", asMETHODPR(CSprite3D, SetColor, (float,float,float,float), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetDefaultColor()",                           asMETHOD(CSprite3D, SetDefaultColor),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "const CColor & GetColor()",                        asMETHOD(CSprite3D, GetColor),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "const CColor & GetDefaultColor()",                 asMETHOD(CSprite3D, GetDefaultColor),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetAlpha(float alpha)",                       asMETHOD(CSprite3D, SetAlpha),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "float GetAlpha()",                                 asMETHOD(CSprite3D, GetAlpha),           asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "float GetDefaultAlpha()",                          asMETHOD(CSprite3D, GetDefaultAlpha),    asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetVisible(bool)",                              asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "bool IsVisible()",                                   asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void IncPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void IncPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "CPoint GetPos()",                                     asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void IncRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void IncRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "const CPoint & GetRot()",                               asFUNCTION(GetRot),               asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void SetScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void IncScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CSprite3D", "void IncScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CSprite3D", "const CPoint & GetScale()",                             asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );
    }
}
