
/************************************************************************
*    FILE NAME:       scriptplaylist.cpp
*
*    DESCRIPTION:     CPlayList script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptplaylist.h>

// Game lib dependencies
#include <common/playlist.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptPlayLst
{
    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CPlayList();
    }

    /************************************************************************
    *    desc:  Copy Constructor
    ************************************************************************/
    void CopyConstructor(const CPlayList & other, void * pThisPointer)
    {
        new(pThisPointer) CPlayList(other);
    }

    /************************************************************************
    *    desc:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CPlayList*)pThisPointer)->~CPlayList();
    }
    
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals;
        
        asIScriptEngine * pEngine = CScriptManager::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("CPlayList", sizeof(CPlayList), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CPlayList", asBEHAVE_CONSTRUCT, "void f()",                  asFUNCTION(NScriptPlayLst::Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPlayList", asBEHAVE_CONSTRUCT, "void f(const CSound & in)", asFUNCTION(NScriptPlayLst::CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPlayList", asBEHAVE_DESTRUCT,  "void f()",                  asFUNCTION(NScriptPlayLst::Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CPlayList", "CPlayList & opAssign(const CPlayList & in)", asMETHODPR(CPlayList, operator =, (const CPlayList &), CPlayList &), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Play( int channel = -1, int loopCount = 0 )", asMETHOD(CPlayList, play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Stop()",                                      asMETHOD(CPlayList, stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Pause()",                                     asMETHOD(CPlayList, pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Resume()",                                    asMETHOD(CPlayList, resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void SetVolume( int volume )",                     asMETHOD(CPlayList, setVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "int GetVolume()",                                  asMETHOD(CPlayList, getVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool IsPlaying()",                                 asMETHOD(CPlayList, isPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool IsPaused()",                                  asMETHOD(CPlayList, isPaused),  asCALL_THISCALL) );

    }
}
