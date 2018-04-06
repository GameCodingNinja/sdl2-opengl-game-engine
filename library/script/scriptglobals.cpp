
/************************************************************************
*    FILE NAME:       scriptglobals.cpp
*
*    DESCRIPTION:     AngelScript global function registration
************************************************************************/

// Physical component dependency
#include <script/scriptglobals.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <managers/soundmanager.h>
#include <managers/signalmanager.h>
#include <script/scriptmanager.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptGlobals
{
    /************************************************************************
    *    desc:  Throw an exception for values less then 0
    ************************************************************************/
    void Throw( int value )
    {
        if( value < 0 )
            throw NExcept::CCriticalException("Error Registering AngelScript Function!",
                boost::str( boost::format("Function could not be created.\n\n%s\nLine: %s")
                    % __FUNCTION__ % __LINE__ ));

    }   // Throw

    /************************************************************************
    *    desc:  Suspend the script to the game loop                                                             
    ************************************************************************/
    void Suspend()
    {
        asIScriptContext *ctx = asGetActiveContext();

        // Suspend the context so the game loop can resumed
        if( ctx )
            ctx->Suspend();

    }   // Suspend
    
    
    /************************************************************************
    *    desc:  Register the global functions
    ************************************************************************/
    void Register( asIScriptEngine * pEngine )
    {
        Throw( pEngine->RegisterGlobalFunction("double GetElapsedTime()", asMETHOD(CHighResTimer, GetElapsedTime), asCALL_THISCALL_ASGLOBAL, &CHighResTimer::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void TimerStart()", asMETHOD(CHighResTimer, TimerStart), asCALL_THISCALL_ASGLOBAL, &CHighResTimer::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("float TimerStop()", asMETHOD(CHighResTimer, TimerStop), asCALL_THISCALL_ASGLOBAL, &CHighResTimer::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void Print(string &in)", asFUNCTION(NGenFunc::PostDebugMsg), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Suspend()", asFUNCTION(Suspend), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("int UniformRandomInt(int startRange, int endRange, int seed = 0)", asFUNCTION( NGenFunc::UniformRandomInt), asCALL_CDECL ) );
        Throw( pEngine->RegisterGlobalFunction("float UniformRandomFloat(float startRange, float endRange, int seed = 0)", asFUNCTION( NGenFunc::UniformRandomFloat), asCALL_CDECL ) );
        // The DispatchEvent function has 4 parameters and because they are not defined here, they only return garbage
        // AngelScript is not allowing the other two voided pointers
        Throw( pEngine->RegisterGlobalFunction("void DispatchEvent(int type, int code = 0)", asFUNCTION(NGenFunc::DispatchEvent), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Spawn(string &in)", asMETHOD(CScriptManager, PrepareSpawn), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    }

}   // NScriptGlobals
