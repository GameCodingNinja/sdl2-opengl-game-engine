
/************************************************************************
*    FILE NAME:       scriptstrategymanager.cpp
*
*    DESCRIPTION:     CSpriteStrategyMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptspritestrategymanager.h>

// Game lib dependencies
#include <spritestrategy/spritestrategymanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <spritestrategy/basicstagestrategy.h>
#include <spritestrategy/basicspritestrategy.h>
#include <spritestrategy/menustrategy.h>
#include <common/isprite.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptSpriteStrategyManager
{
    /************************************************************************
    *    DESC:  Create a basic sprite strategy                                                            
    ************************************************************************/
    iSpriteStrategy * CreateBasicSpriteStrategy( const std::string & strategyId, CSpriteStrategyMgr & rStrategyMgr )
    {
        iSpriteStrategy * pSpriteStrategy = nullptr;
        
        try
        {
            pSpriteStrategy = rStrategyMgr.addStrategy( strategyId, new CBasicSpriteStrategy );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pSpriteStrategy;
    }
    
    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    iSpriteStrategy * CreateBasicStageStrategy( const std::string & strategyId, CSpriteStrategyMgr & rStrategyMgr )
    {
        iSpriteStrategy * pSpriteStrategy = nullptr;
        
        try
        {
            pSpriteStrategy = rStrategyMgr.addStrategy( strategyId, new CBasicStageStrategy );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pSpriteStrategy;
    }
    
    /************************************************************************
    *    DESC:  Create a menu strategy                                                            
    ************************************************************************/
    iSpriteStrategy * CreateMenuStrategy( const std::string & strategyId, CSpriteStrategyMgr & rStrategyMgr )
    {
        iSpriteStrategy * pSpriteStrategy = nullptr;
        
        try
        {
            pSpriteStrategy = rStrategyMgr.addStrategy( strategyId, new CMenuStrategy );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pSpriteStrategy;
    }

    /************************************************************************
    *    DESC:  Get the sprite strategy                                                            
    ************************************************************************/
    iSpriteStrategy * GetSpriteStrategy( const std::string & strategyId, CSpriteStrategyMgr & rStrategyMgr )
    {
        iSpriteStrategy * pSpriteStrategy = nullptr;
        
        try
        {
            pSpriteStrategy = rStrategyMgr.getStrategy( strategyId );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return pSpriteStrategy;
    }

    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    void DeleteStrategy( const std::string & strategyId, CSpriteStrategyMgr & rStrategyMgr )
    {
        try
        {
            rStrategyMgr.deleteStrategy( strategyId );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
    }

    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    iSprite * CreateSprite1( const std::string & strategyId, const std::string & group, const std::string & name, CSpriteStrategyMgr & rStrategyMgr )
    {
        try
        {
            return rStrategyMgr.create( strategyId, group, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
    }
    
    iSprite * CreateSprite2( const std::string & strategyId, const std::string & name, CSpriteStrategyMgr & rStrategyMgr )
    {
        try
        {
            return rStrategyMgr.create( strategyId, name );
        }
        catch( NExcept::CCriticalException & ex )
        {
            asGetActiveContext()->SetException(ex.getErrorMsg().c_str());
        }
        catch( std::exception const & ex )
        {
            asGetActiveContext()->SetException(ex.what());
        }
        
        return nullptr;
    }

    /************************************************************************
    *    DESC:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSpriteStrategy & createBasicSpriteStrategy(string &in)",    asFUNCTION(CreateBasicSpriteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSpriteStrategy & createBasicStageStrategy(string &in)",     asFUNCTION(CreateBasicStageStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSpriteStrategy & createMenuStrategy(string &in)",           asFUNCTION(CreateMenuStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSpriteStrategy & getStrategy(string &in)",                  asFUNCTION(GetSpriteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",                            asFUNCTION(DeleteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteSprite(string &in, int)",                         asMETHOD(CSpriteStrategyMgr, deleteSprite),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSprite & createSprite(string &in, string &in, string &in)", asFUNCTION(CreateSprite1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSprite & createSprite(string &in, string &in)",             asFUNCTION(CreateSprite2), asCALL_CDECL_OBJLAST) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CSpriteStrategyMgr::Instance()) );
    }
}
