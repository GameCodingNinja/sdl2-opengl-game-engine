
/************************************************************************
*    FILE NAME:       scriptstrategymanager.cpp
*
*    DESCRIPTION:     CSpriteStrategyMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptstrategymanager.h>

// Game lib dependencies
#include <managers/spritestrategymanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <2d/basicstagestrategy2d.h>
#include <2d/basicspritestrategy2d.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptStrategyManager
{
    /************************************************************************
    *    desc:  Create a basic sprite strategy                                                            
    ************************************************************************/
    void CreateBasicSpriteStrategy( const std::string & name )
    {
        try
        {
            CSpriteStrategyMgr::Instance().AddStrategy( name, new CBasicSpriteStrategy2D );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    void CreateBasicStageStrategy( const std::string & name )
    {
        try
        {
            CSpriteStrategyMgr::Instance().AddStrategy( name, new CBasicStageStrategy2D );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    void CreateSprite( const std::string & strategyId, const std::string & group, const std::string & name )
    {
        try
        {
            CSpriteStrategyMgr::Instance().Create( strategyId, group, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    void CreateSprite( const std::string & strategyId, const std::string & name )
    {
        try
        {
            CSpriteStrategyMgr::Instance().Create( strategyId, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }
    
    /************************************************************************
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().GetEnginePtr();
        
        Throw( pEngine->RegisterGlobalFunction("void Strategy_CreateBasicSpriteStrategy(string &in)", asFUNCTION(CreateBasicSpriteStrategy), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_CreateStageStrategy(string &in)", asFUNCTION(CreateBasicStageStrategy), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_Create(string &in, string &in, string &in)", asFUNCTIONPR(CreateSprite, (const std::string &, const std::string &, const std::string &), void), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_Create(string &in, string &in)", asFUNCTIONPR(CreateSprite, (const std::string &, const std::string &), void), asCALL_CDECL) );
    }
}
