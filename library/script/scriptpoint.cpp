
/************************************************************************
 *    FILE NAME:       scriptpoint.cpp
 *
 *    DESCRIPTION:     CPoint script object registration
 ************************************************************************/

// Physical component dependency
#include <script/scriptpoint.h>

// Game lib dependencies
#include <common/point.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptPoint
{

    /************************************************************************
     *    desc:  Constructor
     ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CPoint<float>();
    }

    /************************************************************************
     *    desc:  Copy Constructor
     ************************************************************************/
    void CopyConstructor(const CPoint<float> & other, void * pThisPointer)
    {
        new(pThisPointer) CPoint<float>(other);
    }

    void ConstructorFromThreeFloats(float x, float y, float z, void * pThisPointer)
    {
        new(pThisPointer) CPoint<float>(x, y, z);
    }

    void ConstructorFromTwoFloats(float x, float y, void * pThisPointer)
    {
        new(pThisPointer) CPoint<float>(x, y);
    }

    /************************************************************************
     *    desc:  Destructor
     ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CPoint<float>*)pThisPointer)->~CPoint();
    }

    /************************************************************************
     *    desc:  Register the class with AngelScript
     ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType("CPoint", sizeof(CPoint<float>), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object operator overloads
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f()",                    asFUNCTION(Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(const CPoint & in)",   asFUNCTION(CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(ConstructorFromThreeFloats), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_CONSTRUCT, "void f(float, float)",        asFUNCTION(ConstructorFromTwoFloats), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPoint", asBEHAVE_DESTRUCT, "void f()",                     asFUNCTION(Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint & opAssign(const CPoint & in)", asMETHODPR(CPoint<float>, operator =, (const CPoint<float> &), CPoint<float> &), asCALL_THISCALL) );

        // binary operators
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAdd ( const CPoint & in )", asMETHODPR(CPoint<float>, operator +, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSub ( const CPoint & in )", asMETHODPR(CPoint<float>, operator -, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMul ( const CPoint & in )", asMETHODPR(CPoint<float>, operator *, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDiv ( const CPoint & in )", asMETHODPR(CPoint<float>, operator /, (const CPoint<float> &) const, CPoint<float>), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAdd ( float )", asMETHODPR(CPoint<float>, operator +, (float) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSub ( float )", asMETHODPR(CPoint<float>, operator -, (float) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMul ( float )", asMETHODPR(CPoint<float>, operator *, (float) const, CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDiv ( float )", asMETHODPR(CPoint<float>, operator /, (float) const, CPoint<float>), asCALL_THISCALL) );

        // compound assignment operators
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAddAssign ( const CPoint & in )", asMETHODPR(CPoint<float>, operator +=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSubAssign ( const CPoint & in )", asMETHODPR(CPoint<float>, operator -=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMulAssign ( const CPoint & in )", asMETHODPR(CPoint<float>, operator *=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDivAssign ( const CPoint & in )", asMETHODPR(CPoint<float>, operator /=, (const CPoint<float> &), CPoint<float>), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opAddAssign ( float )", asMETHODPR(CPoint<float>, operator +=, (float) , CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opSubAssign ( float )", asMETHODPR(CPoint<float>, operator -=, (float) , CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opMulAssign ( float )", asMETHODPR(CPoint<float>, operator *=, (float) , CPoint<float>), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint opDivAssign ( float )", asMETHODPR(CPoint<float>, operator /=, (float) , CPoint<float>), asCALL_THISCALL) );

        // Register property
        Throw( pEngine->RegisterObjectProperty("CPoint", "float x", asOFFSET(CPoint<float>, x)) );
        Throw( pEngine->RegisterObjectProperty("CPoint", "float y", asOFFSET(CPoint<float>, y)) );
        Throw( pEngine->RegisterObjectProperty("CPoint", "float z", asOFFSET(CPoint<float>, z)) );

        // Class members
        Throw( pEngine->RegisterObjectMethod("CPoint", "void ClearX()",       asMETHOD(CPoint<float>, ClearX), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void ClearY()",       asMETHOD(CPoint<float>, ClearY), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void ClearZ()",       asMETHOD(CPoint<float>, ClearZ), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void Clear()",        asMETHOD(CPoint<float>, Clear), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void Cap( float x )", asMETHOD(CPoint<float>, CapFloat), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool IsEmpty()",      asMETHOD(CPoint<float>, IsEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool IsXEmpty()",     asMETHOD(CPoint<float>, IsXEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool IsYEmpty()",     asMETHOD(CPoint<float>, IsYEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "bool IsZEmpty()",     asMETHOD(CPoint<float>, IsZEmpty), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void Invert()",       asMETHOD(CPoint<float>, Invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void InvertX()",      asMETHOD(CPoint<float>, Invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void InvertY()",      asMETHOD(CPoint<float>, Invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void InvertZ()",      asMETHOD(CPoint<float>, Invert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint GetInvert()",  asMETHOD(CPoint<float>, GetInvert), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void Normalize()",    asMETHOD(CPoint<float>, Normalize), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "void Normalize2D()",  asMETHOD(CPoint<float>, Normalize2D), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "bool IsEquilEnough( const CPoint & in, float val )", asMETHOD(CPoint<float>, IsEquilEnoughFloat), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPoint", "float GetDotProduct( const CPoint & in )",    asMETHOD(CPoint<float>, GetDotProductFloat), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "float GetDotProduct2D( const CPoint & in )",  asMETHOD(CPoint<float>, GetDotProduct2DFloat), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPoint", "CPoint GetCrossProduct( const CPoint & in )", asMETHOD(CPoint<float>, GetCrossProduct), asCALL_THISCALL) );
    }
}
