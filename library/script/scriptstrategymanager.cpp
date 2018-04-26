
/************************************************************************
*    FILE NAME:       scriptstrategymanager.cpp
*
*    DESCRIPTION:     CSpriteStrategyMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptstrategymanager.h>

// Game lib dependencies
#include <spritestrategy/spritestrategymanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <spritestrategy/basicstagestrategy.h>
#include <spritestrategy/basicspritestrategy.h>
#include <common/isprite.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptStrategyManager
{
    /************************************************************************
    *    desc:  Create a basic sprite strategy                                                            
    ************************************************************************/
    void CreateBasicSpriteStrategy1( const std::string & strategyId, const std::string & cameraId, int idOffset, int idDir, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            rStrategy.addStrategy( strategyId, new CBasicSpriteStrategy( cameraId, idOffset, idDir ) );
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
    
    void CreateBasicSpriteStrategy2( const std::string & strategyId, int idOffset, int idDir, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            rStrategy.addStrategy( strategyId, new CBasicSpriteStrategy( idOffset, idDir ) );
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
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    void CreateBasicStageStrategy( const std::string & strategyId, const std::string & cameraId, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            rStrategy.addStrategy( strategyId, new CBasicStageStrategy(cameraId) );
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
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    void DeleteStrategy( const std::string & strategyId, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            rStrategy.deleteStrategy( strategyId );
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
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    iSprite * CreateSprite1( const std::string & strategyId, const std::string & group, const std::string & name, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            return rStrategy.create( strategyId, group, name );
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
    
    iSprite * CreateSprite2( const std::string & strategyId, const std::string & name, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            return rStrategy.create( strategyId, name );
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
    *    desc:  Set the strategy camera id                                                            
    ************************************************************************/
    void SetCameraId( const std::string & strategyId, const std::string & cameraId, CSpriteStrategyMgr & rStrategy )
    {
        try
        {
            rStrategy.get<iSpriteStrategy>( strategyId ).setCameraId( cameraId );
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
    *    desc:  Register global functions
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptManager::Instance().getEnginePtr();
        
        // Register type
        Throw( pEngine->RegisterObjectType( "CStrategyMgr", 0, asOBJ_REF|asOBJ_NOCOUNT) );
        
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void createBasicSpriteStrategy(string &in, string &in, int idOffset = 0, int idDir = 1)", asFUNCTION(CreateBasicSpriteStrategy1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void createBasicSpriteStrategy(string &in, int idOffset = 0, int idDir = 1)",             asFUNCTION(CreateBasicSpriteStrategy2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void createStageStrategy(string &in, string &in)",                                        asFUNCTION(CreateBasicStageStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteStrategy(string &in)",                                                         asFUNCTION(DeleteStrategy), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void deleteSprite(string &in, int)",                                                      asMETHOD(CSpriteStrategyMgr, deleteSprite),   asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSprite & createSprite(string &in, string &in, string &in)",                              asFUNCTION(CreateSprite1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "iSprite & createSprite(string &in, string &in)",                                          asFUNCTION(CreateSprite2), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("CStrategyMgr", "void setCameraId(string &in, string &in)",                                                asFUNCTION(SetCameraId), asCALL_CDECL_OBJLAST) );

        // Set this object registration as a global property to simulate a singleton
        Throw( pEngine->RegisterGlobalProperty("CStrategyMgr StrategyMgr", &CSpriteStrategyMgr::Instance()) );
    }
}
