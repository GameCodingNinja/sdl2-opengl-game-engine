
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
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CSoundMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void play(string &in, string &in, int loopCount=0)", asMETHOD(CSoundMgr, play),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void pause(string &in, string &in)",                 asMETHOD(CSoundMgr, pause),       asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void resume(string &in, string &in)",                asMETHOD(CSoundMgr, resume),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void stop(string &in, string &in)",                  asMETHOD(CSoundMgr, stop),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "void setVolume(string &in, string &in, int)",        asMETHOD(CSoundMgr, setVolume),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "int getVolume(string &in, string &in)",              asMETHOD(CSoundMgr, getVolume),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "bool isPlaying(string &in, string &in)",             asMETHOD(CSoundMgr, isPlaying),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "bool isPaused(string &in, string &in)",              asMETHOD(CSoundMgr, isPaused),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "CSound & getSound(string &in, string &in)",          asMETHOD(CSoundMgr, getSound),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSoundMgr", "CPlayList & getPlayList(string &in, string &in)",    asMETHOD(CSoundMgr, getPlayList), asCALL_THISCALL) );
        
        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CSoundMgr SoundMgr", &CSoundMgr::Instance()) );
    }
}