
/************************************************************************
*    FILE NAME:       scriptsoundmanager.cpp
*
*    DESCRIPTION:     CSoundMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptsoundmanager.h>

// Game lib dependencies
#include <managers/soundmanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSoundManager
{
    /************************************************************************
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().getEnginePtr();

        Throw( pEngine->RegisterGlobalFunction("void PlaySound( string &in, string &in, int loopCount = 0 )", asMETHOD(CSoundMgr, play), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void PauseSound( string &in, string &in )", asMETHOD(CSoundMgr, pause), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void ResumeSound( string &in, string &in )", asMETHOD(CSoundMgr, resume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void StopSound( string &in, string &in )", asMETHOD(CSoundMgr, stop), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void SetVolume( string &in, string &in, int volume )", asMETHOD(CSoundMgr, setVolume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("int GetVolume( string &in, string &in )", asMETHOD(CSoundMgr, getVolume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool IsSoundPlaying( string &in, string &in )", asMETHOD(CSoundMgr, isPlaying), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool IsSoundPaused( string &in, string &in )", asMETHOD(CSoundMgr, isPaused), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("int GetNextSoundChannel()",               asMETHOD(CSoundMgr, getNextChannel), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CSound & GetSound( string &in, string &in )", asMETHOD(CSoundMgr, getSound), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CPlayList & GetPlayList( string &in, string &in )", asMETHOD(CSoundMgr, getPlayList), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
    }

}   // NScriptColor