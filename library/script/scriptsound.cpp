
/************************************************************************
*    FILE NAME:       scriptsound.cpp
*
*    DESCRIPTION:     CSound script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsound.h>

// Game lib dependencies
#include <common/sound.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSound
{
    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CSound();
    }

    /************************************************************************
    *    desc:  Copy Constructor
    ************************************************************************/
    void CopyConstructor(const CSound & other, void * pThisPointer)
    {
        new(pThisPointer) CSound(other);
    }

    /************************************************************************
    *    desc:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CSound*)pThisPointer)->~CSound();
    }

    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals;
        
        asIScriptEngine * pEngine = CScriptManager::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("CSound", sizeof(CSound), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_CONSTRUCT, "void f()",                  asFUNCTION(NScriptSound::Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_CONSTRUCT, "void f(const CSound & in)", asFUNCTION(NScriptSound::CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_DESTRUCT,  "void f()",                  asFUNCTION(NScriptSound::Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSound", "CSound & opAssign(const CSound & in)", asMETHODPR(CSound, operator =, (const CSound &), CSound &), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSound", "void play( int channel = -1, int loopCount = 0 )", asMETHOD(CSound, play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void stop()",                                      asMETHOD(CSound, stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void pause()",                                     asMETHOD(CSound, pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void resume()",                                    asMETHOD(CSound, resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void setVolume( int volume )",                     asMETHOD(CSound, setVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "int getVolume()",                                  asMETHOD(CSound, getVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool isPlaying()",                                 asMETHOD(CSound, isPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool isPaused()",                                  asMETHOD(CSound, isPaused),  asCALL_THISCALL) );

    }
}
