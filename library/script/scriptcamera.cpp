
/************************************************************************
*    FILE NAME:       scriptcamera.cpp
*
*    DESCRIPTION:     Sprite2D script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsprite.h>

// Game lib dependencies
#include <common/camera.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptCamera
{
    /************************************************************************
    *    desc:  Wrapper function due to virtual inheritance
    ************************************************************************/
    void SetPos1(const CPoint<float> & pos, CCamera & camera)
    {
        camera.SetPos(pos);
    }
    
    void SetPos2(float x, float y, float z, CCamera & camera)
    {
        camera.SetPos(x,y,z);
    }
    
    void IncPos1(const CPoint<float> & pos, CCamera & camera)
    {
        camera.IncPos(pos);
    }
    
    void IncPos2(float x, float y, float z, CCamera & camera)
    {
        camera.IncPos(x,y,z);
    }
    
    CPoint<float> GetPos(CCamera & camera)
    {
        return camera.GetPosFloat();
    }
    
    void SetRot1(const CPoint<float> & rot, bool convertToRadians, CCamera & camera)
    {
        camera.SetRot(rot, convertToRadians);
    }
    
    void SetRot2(float x, float y, float z, bool convertToRadians, CCamera & camera)
    {
        camera.SetRot(x,y,z, convertToRadians);
    }
    
    void IncRot1(const CPoint<float> & rot, bool convertToRadians, CCamera & camera)
    {
        camera.IncRot(rot, convertToRadians);
    }
    
    void IncRot2(float x, float y, float z, bool convertToRadians, CCamera & camera)
    {
        camera.IncRot(x,y,z, convertToRadians);
    }
    
    const CPoint<float> & GetRot(CCamera & camera)
    {
        return camera.GetRot();
    }
    
    void SetScale1(const CPoint<float> & rot, CCamera & camera)
    {
        camera.SetScale(rot);
    }
    
    void SetScale2(float x, float y, float z, CCamera & camera)
    {
        camera.SetScale(x,y,z);
    }
    
    void IncScale1(const CPoint<float> & rot, CCamera & camera)
    {
        camera.IncScale(rot);
    }
    
    void IncScale2(float x, float y, float z, CCamera & camera)
    {
        camera.IncScale(x,y,z);
    }

    const CPoint<float> & GetScale(CCamera & camera)
    {
        return camera.GetScale();
    }
    
    void Transform(CCamera & camera)
    {
        camera.Transform();
    }
    
    
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType(  "CCamera", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setPos(CPoint &in)",                             asFUNCTION(SetPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(SetPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incPos(CPoint & in)",                            asFUNCTION(IncPos1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incPos(float x = 0, float y = 0, float z = 0)",  asFUNCTION(IncPos2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "CPoint getPos()",                                     asFUNCTION(GetPos),  asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "void setRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(SetRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(SetRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incRot(CPoint &in, bool convertToRadians = true)", asFUNCTION(IncRot1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incRot(float x = 0, float y = 0, float z = 0, bool convertToRadians = true)", asFUNCTION(IncRot2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "const CPoint & getRot()",                               asFUNCTION(GetRot),               asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setScale(CPoint & in)",                            asFUNCTION(SetScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void setScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(SetScale2), asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incScale(CPoint & in)",                            asFUNCTION(IncScale1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CCamera", "void incScale(float x = 1, float y = 1, float z = 1)",  asFUNCTION(IncScale2), asCALL_CDECL_OBJLAST) );

        Throw( pEngine->RegisterObjectMethod("CCamera", "const CPoint & getScale()",     asFUNCTION(GetScale),   asCALL_CDECL_OBJLAST) );
        
        Throw( pEngine->RegisterObjectMethod("CCamera", "void transform()",  asFUNCTION(Transform), asCALL_CDECL_OBJLAST) );
    }
}