
/************************************************************************
*    FILE NAME:       scriptispritestrategy.cpp
*
*    DESCRIPTION:     iSpriteStrategy script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptispritestrategy.h>

// Game lib dependencies
#include <spritestrategy/ispritestrategy.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptiSpriteStrategy
{
    /************************************************************************
    *    DESC:  Create a basic stage strategy                                                            
    ************************************************************************/
    iSprite * CreateSprite1( const std::string & group, const std::string & name, iSpriteStrategy & rStrategy )
    {
        try
        {
            return rStrategy.create( group, name );
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
    
    iSprite * CreateSprite2( const std::string & strategyId, const std::string & name, iSpriteStrategy & rStrategy )
    {
        try
        {
            return rStrategy.create( name );
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
    *    DESC:  Register the class with AngelScript
    ************************************************************************/
    void Register()
    {
        using namespace NScriptGlobals; // Used for Throw
        
        asIScriptEngine * pEngine = CScriptMgr::Instance().getEnginePtr();

        // Register type
        Throw( pEngine->RegisterObjectType("iSpriteStrategy", 0, asOBJ_REF|asOBJ_NOCOUNT) );

        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "void setCameraId(string &in)",   asMETHOD(iSpriteStrategy, setCameraId),         asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "void setIdOffset(int)",          asMETHOD(iSpriteStrategy, setIdOffset),         asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "void setIdDir(int)",             asMETHOD(iSpriteStrategy, setIdDir),            asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "void setToDestroy(int)",         asMETHOD(iSpriteStrategy, setToDestroy),        asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "void setToCreate(string &in)",   asMETHOD(iSpriteStrategy, setToCreate),         asCALL_THISCALL) );
        
        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "iSprite & createSprite(string &in, string &in)", asFUNCTION(CreateSprite1), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectMethod("iSpriteStrategy", "iSprite & createSprite(string &in)",             asFUNCTION(CreateSprite1), asCALL_CDECL_OBJLAST) );
    }
}
