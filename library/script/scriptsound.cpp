
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
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("CSound", sizeof(CSound), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_CONSTRUCT, "void f()",                  asFUNCTION(NScriptSound::Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_CONSTRUCT, "void f(const CSound & in)", asFUNCTION(NScriptSound::CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_DESTRUCT,  "void f()",                  asFUNCTION(NScriptSound::Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSound", "CSound & opAssign(const CSound & in)", asMETHODPR(CSound, operator =, (const CSound &), CSound &), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSound", "void Play( int channel = -1, int loopCount = 0 )", asMETHOD(CSound, Play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void Stop()",                                      asMETHOD(CSound, Stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void Pause()",                                     asMETHOD(CSound, Pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void Resume()",                                    asMETHOD(CSound, Resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void SetVolume( int volume )",                     asMETHOD(CSound, SetVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "int GetVolume()",                                  asMETHOD(CSound, GetVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool IsPlaying()",                                 asMETHOD(CSound, IsPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool IsPaused()",                                  asMETHOD(CSound, IsPaused),  asCALL_THISCALL) );

    }   // Register
    
}   // NScriptColor