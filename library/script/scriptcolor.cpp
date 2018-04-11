
/************************************************************************
*    FILE NAME:       scriptcolor.cpp
*
*    DESCRIPTION:     CColor script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptcolor.h>

// Game lib dependencies
#include <common/color.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptColor
{
    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CColor();
    }

    /************************************************************************
    *    desc:  Copy Constructor
    ************************************************************************/
    void CopyConstructor(const CColor & other, void * pThisPointer)
    {
        new(pThisPointer) CColor(other);
    }

    void ConstructorFromFloats(float r, float g, float b, float a, void * pThisPointer)
    {
        new(pThisPointer) CColor(r, g, b, a);
    }

    /************************************************************************
    *    desc:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CColor*)pThisPointer)->~CColor();
    }

    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CColor", sizeof(CColor), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f()",                           asFUNCTION(Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(const CColor & in)",          asFUNCTION(CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(ConstructorFromFloats), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CColor", asBEHAVE_DESTRUCT,  "void f()",                           asFUNCTION(Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor & opAssign(const CColor & in)", asMETHODPR(CColor, operator =, (const CColor &), CColor &), asCALL_THISCALL) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAdd ( const CColor & in )", asMETHODPR(CColor, operator +, (const CColor &) const, CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSub ( const CColor & in )", asMETHODPR(CColor, operator -, (const CColor &) const, CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMul ( const CColor & in )", asMETHODPR(CColor, operator *, (const CColor &) const, CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDiv ( const CColor & in )", asMETHODPR(CColor, operator /, (const CColor &) const, CColor), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAdd ( float )", asMETHODPR(CColor, operator +, (float) const, CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSub ( float )", asMETHODPR(CColor, operator -, (float) const, CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMul ( float )", asMETHODPR(CColor, operator *, (float) const, CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDiv ( float )", asMETHODPR(CColor, operator /, (float) const, CColor), asCALL_THISCALL) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAddAssign ( const CColor & in )", asMETHODPR(CColor, operator +=, (const CColor &), CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSubAssign ( const CColor & in )", asMETHODPR(CColor, operator -=, (const CColor &), CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMulAssign ( const CColor & in )", asMETHODPR(CColor, operator *=, (const CColor &), CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDivAssign ( const CColor & in )", asMETHODPR(CColor, operator /=, (const CColor &), CColor), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opAddAssign ( float )", asMETHODPR(CColor, operator +=, (float), CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opSubAssign ( float )", asMETHODPR(CColor, operator -=, (float), CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opMulAssign ( float )", asMETHODPR(CColor, operator *=, (float), CColor), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "CColor opDivAssign ( float )", asMETHODPR(CColor, operator /=, (float), CColor), asCALL_THISCALL) );

	// Register property
        Throw( pEngine->RegisterObjectProperty("CColor", "float r", asOFFSET(CColor, r)) );
        Throw( pEngine->RegisterObjectProperty("CColor", "float g", asOFFSET(CColor, g)) );
        Throw( pEngine->RegisterObjectProperty("CColor", "float b", asOFFSET(CColor, b)) );
        Throw( pEngine->RegisterObjectProperty("CColor", "float a", asOFFSET(CColor, a)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CColor", "void Set( float r, float g, float b, float a )",       asMETHOD(CColor, Set), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CColor", "void TransformHSV( float hue, float sat, float val )", asMETHOD(CColor, TransformHSV), asCALL_THISCALL) );
	Throw( pEngine->RegisterObjectMethod("CColor", "void Convert()", asMETHOD( CColor, Convert ), asCALL_THISCALL ) );
    }
}
