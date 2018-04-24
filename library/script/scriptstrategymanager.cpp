
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
    void CreateBasicSpriteStrategy( const std::string & strategyId, const std::string & cameraId, int idOffset, int idDir )
    {
        try
        {
            CSpriteStrategyMgr::Instance().addStrategy( strategyId, new CBasicSpriteStrategy( cameraId, idOffset, idDir ) );
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
    
    void CreateBasicSpriteStrategy( const std::string & strategyId, int idOffset, int idDir )
    {
        try
        {
            CSpriteStrategyMgr::Instance().addStrategy( strategyId, new CBasicSpriteStrategy( idOffset, idDir ) );
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
    void CreateBasicStageStrategy( const std::string & strategyId, const std::string & cameraId )
    {
        try
        {
            CSpriteStrategyMgr::Instance().addStrategy( strategyId, new CBasicStageStrategy(cameraId) );
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
    void DeleteStrategy( const std::string & strategyId )
    {
        try
        {
            CSpriteStrategyMgr::Instance().deleteStrategy( strategyId );
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
    iSprite * CreateSprite( const std::string & strategyId, const std::string & group, const std::string & name )
    {
        try
        {
            return CSpriteStrategyMgr::Instance().create( strategyId, group, name );
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
    
    iSprite * CreateSprite( const std::string & strategyId, const std::string & name )
    {
        try
        {
            return CSpriteStrategyMgr::Instance().create( strategyId, name );
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
    *    desc:  Create a basic stage strategy                                                            
    ************************************************************************/
    /*CActorSprite2D * CreateActorSprite( const std::string & strategyId, const std::string & group, const std::string & name )
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
    }*/
    
    
    /************************************************************************
    *    desc:  Set the strategy camera id                                                            
    ************************************************************************/
    void SetCameraId( const std::string & strategyId, const std::string & cameraId )
    {
        try
        {
            CSpriteStrategyMgr::Instance().get<iSpriteStrategy>( strategyId ).setCameraId( cameraId );
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
        
        Throw( pEngine->RegisterGlobalFunction("void Strategy_CreateBasicSpriteStrategy(string &in, string &in, int idOffset = 0, int idDir = 1)", asFUNCTIONPR(CreateBasicSpriteStrategy, (const std::string &, const std::string &, int, int), void), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_CreateBasicSpriteStrategy(string &in, int idOffset = 0, int idDir = 1)", asFUNCTIONPR(CreateBasicSpriteStrategy, (const std::string &, int, int), void), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_CreateStageStrategy(string &in, string &in)", asFUNCTION(CreateBasicStageStrategy), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_DeleteStrategy(string &in)", asFUNCTION(DeleteStrategy), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_DeleteSprite(string &in, int)", asMETHOD(CSpriteStrategyMgr, deleteSprite), asCALL_THISCALL_ASGLOBAL, &CSpriteStrategyMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("iSprite & Strategy_CreateSprite(string &in, string &in, string &in)", asFUNCTIONPR(CreateSprite, (const std::string &, const std::string &, const std::string &), iSprite *), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("iSprite & Strategy_CreateSprite(string &in, string &in)", asFUNCTIONPR(CreateSprite, (const std::string &, const std::string &), iSprite *), asCALL_CDECL) );
        //Throw( pEngine->RegisterGlobalFunction("CActorSprite2D & Strategy_CreateActorSprite(string &in, string &in, string &in)", asFUNCTIONPR(CreateActorSprite, (const std::string &, const std::string &, const std::string &), CActorSprite2D *), asCALL_CDECL) );
        //Throw( pEngine->RegisterGlobalFunction("CActorSprite2D & Strategy_CreateActorSprite(string &in, string &in)", asFUNCTIONPR(CreateActorSprite, (const std::string &, const std::string &), CActorSprite2D *), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void Strategy_SetCameraId(string &in, string &in)", asFUNCTION(SetCameraId), asCALL_CDECL) );
    }
}
