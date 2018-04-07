
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
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();

        Throw( pEngine->RegisterGlobalFunction("void PlaySound( string &in, string &in, int loopCount = 0 )", asMETHOD(CSoundMgr, Play), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void PauseSound( string &in, string &in )", asMETHOD(CSoundMgr, Pause), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void ResumeSound( string &in, string &in )", asMETHOD(CSoundMgr, Resume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void StopSound( string &in, string &in )", asMETHOD(CSoundMgr, Stop), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void SetVolume( string &in, string &in, int volume )", asMETHOD(CSoundMgr, SetVolume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("int GetVolume( string &in, string &in )", asMETHOD(CSoundMgr, GetVolume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool IsSoundPlaying( string &in, string &in )", asMETHOD(CSoundMgr, IsPlaying), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool IsSoundPaused( string &in, string &in )", asMETHOD(CSoundMgr, IsPaused), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("int GetNextSoundChannel()",               asMETHOD(CSoundMgr, GetNextChannel), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CSound & GetSound( string &in, string &in )", asMETHOD(CSoundMgr, GetSound), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CPlayList & GetPlayList( string &in, string &in )", asMETHOD(CSoundMgr, GetPlayList), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
    }

}   // NScriptColor