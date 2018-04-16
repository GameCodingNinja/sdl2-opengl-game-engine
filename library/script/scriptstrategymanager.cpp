
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
#include <2d/sprite2d.h>
#include <2d/actorsprite2d.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptStrategyManager
{
    /************************************************************************
    *    desc:  Create a basic sprite strategy                                                            
    ************************************************************************/
    void CreateBasicSpriteStrategy( const std::string & strategyId )
    {
        try
        {
            CSpriteStrategyMgr::Instance().AddStrategy( strategyId, new CBasicSpriteStrategy2D );
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
    void CreateBasicStageStrategy( const std::string & strategyId )
    {
        try
        {
            CSpriteStrategyMgr::Instance().AddStrategy( strategyId, new CBasicStageStrategy2D );
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
    void DeleteStrategy( const std::string & strategyId )
    {
        try
        {
            CSpriteStrategyMgr::Instance().DeleteStrategy( strategyId );
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
    CSprite2D * CreateSprite( const std::string & strategyId, const std::string & group, const std::string & name )
    {
        try
        {
            return CSpriteStrategyMgr::Instance().CreateSprite<CSprite2D>( strategyId, group, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
    }
    
    CSprite2D * CreateSprite( const std::string & strategyId, const std::string & name )
    {
        try
        {
            return CSpriteStrategyMgr::Instance().CreateSprite<CSprite2D>( strategyId, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
    }
    
    /************************************************************************
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    CActorSprite2D * CreateActorSprite( const std::string & strategyId, const std::string & group, const std::string & name )
    {
        try
        {
            return CSpriteStrategyMgr::Instance().CreateSprite<CActorSprite2D>( strategyId, group, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
    }
    
    CActorSprite2D * CreateActorSprite( const std::string & strategyId, const std::string & name )
    {
        try
        {
            return CSpriteStrategyMgr::Instance().CreateSprite<CActorSprite2D>( strategyId, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.GetErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
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
        Throw( pEngine->RegisterGlobalFunction("void Strategy_DeleteStrategy(string &in)", asFUNCTION(DeleteStrategy), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_DeleteSprite(string &in, int)", asMETHOD(CSpriteStrategyMgr, DeleteSprite), asCALL_THISCALL_ASGLOBAL, &CSpriteStrategyMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CSprite2D & Strategy_CreateSprite(string &in, string &in, string &in)", asFUNCTIONPR(CreateSprite, (const std::string &, const std::string &, const std::string &), CSprite2D *), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("CSprite2D & Strategy_CreateSprite(string &in, string &in)", asFUNCTIONPR(CreateSprite, (const std::string &, const std::string &), CSprite2D *), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("CActorSprite2D & Strategy_CreateActorSprite(string &in, string &in, string &in)", asFUNCTIONPR(CreateActorSprite, (const std::string &, const std::string &, const std::string &), CActorSprite2D *), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("CActorSprite2D & Strategy_CreateActorSprite(string &in, string &in)", asFUNCTIONPR(CreateActorSprite, (const std::string &, const std::string &), CActorSprite2D *), asCALL_CDECL) );
    }
}
