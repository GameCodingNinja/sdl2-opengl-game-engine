
/************************************************************************
*    FILE NAME:       scriptsprite.cpp
*
*    DESCRIPTION:     Sprite script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsprite.h>

// Game lib dependencies
#include <common/isprite.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSprite
{
    /************************************************************************
    *    desc:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetVisible(bool value, iSprite & sprite)
    {
        sprite.setVisible(value);
    }
    
    bool IsVisible(iSprite & sprite)
    {
        return sprite.isVisible();
    }
    
    void SetPos1(const CPoint<float> & pos, iSprite & sprite)
    {
        sprite.setPos(pos);
    }
    
    void SetPos2(float x, float y, float z, iSprite & sprite)
    {
        sprite.setPos(x,y,z);
    }
    
    void IncPos1(const CPoint<float> & pos, iSprite & sprite)
    {
        sprite.incPos(pos);
    }
    
    void IncPos2(float x, float y, float z, iSprite & sprite)
    {
        sprite.incPos(x,y,z);
    }
    
    CPoint<float> GetPos(iSprite & sprite)
    {
        return sprite.getPosFloat();
    }
    
    void SetRot1(const CPoint<float> & rot, bool convertToRadians, iSprite & sprite)
    {
        sprite.setRot(rot, convertToRadians);
    }
    
    void SetRot2(float x, float y, float z, bool convertToRadians, iSprite & sprite)
    {
        sprite.setRot(x,y,z, convertToRadians);
    }
    
    void IncRot1(const CPoint<float> & rot, bool convertToRadians, iSprite & sprite)
    {
        sprite.incRot(rot, convertToRadians);
    }
    
    void IncRot2(float x, float y, float z, bool convertToRadians, iSprite & sprite)
    {
        sprite.incRot(x,y,z, convertToRadians);
    }
    
    const CPoint<float> & GetRot(iSprite & sprite)
    {
        return sprite.getRot();
    }
    
    void SetScale1(const CPoint<float> & rot, iSprite & sprite)
    {
        sprite.setScale(rot);
    }
    
    void SetScale2(float x, float y, float z, iSprite & sprite)
    {
        sprite.setScale(x,y,z);
    }
    
    void IncScale1(const CPoint<float> & rot, iSprite & sprite)
    {
        sprite.incScale(rot);
    }
    
    void IncScale2(float x, float y, float z, iSprite & sprite)
    {
        sprite.incScale(x,y,z);
    }

    const CPoint<float> & GetScale(iSprite & sprite)
    {
        return sprite.getScale();
    }
    
    
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "iSprite", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void createFontString(string &in)",                  asMETHOD(iSprite, CreateFontString),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "uint getFrameCount()",                               asMETHOD(iSprite, GetFrameCount),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setFrame(uint)",                                asMETHOD(iSprite, SetFrame),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "uint getCurrentFrame()",                             asMETHOD(iSprite, GetCurrentFrame),      asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "int getId()",                                        asMETHOD(iSprite,   GetId),            asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setColor(const CColor &in)",                    asMETHODPR(iSprite, SetColor, (const CColor &), void),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setColor(float, float, float, float)",          asMETHODPR(iSprite, SetColor, (float,float,float,float), void), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setDefaultColor()",                             asMETHOD(iSprite, SetDefaultColor),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "const CColor & getColor()",                          asMETHOD(iSprite, GetColor),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "const CColor & getDefaultColor()",                   asMETHOD(iSprite, GetDefaultColor),      asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setAlpha(float alpha)",                         asMETHOD(iSprite, SetAlpha),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "float getAlpha()",                                   asMETHOD(iSprite, GetAlpha),             asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "float getDefaultAlpha()",                            asMETHOD(iSprite, GetDefaultAlpha),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setDefaultAlpha()",                             asMETHOD(iSprite, SetDefaultAlpha),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setPhysicsTransform(float, float, float, bool resetVelocity = true)", asMETHOD(iSprite, SetPhysicsTransform),             asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setVisible(bool)",                              asFUNCTION(SetVisible), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "bool isVisible()",                                   asFUNCTION(IsVisible), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "CPoint getPos()",                                     asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "const CPoint & getRot()",                               asFUNCTION(GetRot),               asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSprite", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("iSprite", "const CPoint & getScale()",     asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );

        
        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in, iSprite @)", asMETHOD(CScriptManager, PrepareSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void LocalSpawn(string &in, iSprite @)", asMETHOD(CScriptManager, PrepareLocalSpawnVoid), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    }
}