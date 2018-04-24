
/************************************************************************
*    FILE NAME:       scriptobjectdatamanager.cpp
*
*    DESCRIPTION:     CObjectDataMgr script object registration
************************************************************************/

// Physical component dependency
#include <script/scriptobjectdatamanager.h>

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <script/scriptmanager.h>
#include <script/scriptglobals.h>
#include <utilities/exceptionhandling.h>

// AngelScript lib dependencies
#include <angelscript.h>

namespace NScriptObjectDataManager
{
    /************************************************************************
    *    desc:  Load the data list table                                                            
    ************************************************************************/
    void LoadListTable( const std::string & filePath )
    {
        try
        {
            CObjectDataMgr::Instance().loadListTable( filePath );
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
    *    desc:  Load all of the meshes and materials of a specific data group                                                           
    ************************************************************************/
    void LoadGroup2D( const std::string & group, bool createFromData )
    {
        try
        {
            CObjectDataMgr::Instance().LoadGroup2D( group, createFromData );
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

    void LoadGroup3D( const std::string & group, bool createFromData )
    {
        try
        {
            CObjectDataMgr::Instance().LoadGroup3D( group, createFromData );
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
    *    desc:  Create the group's VBO, IBO, textures, etc                                                           
    ************************************************************************/
    void CreateFromData2D( const std::string & group )
    {
        try
        {
            CObjectDataMgr::Instance().CreateFromData2D( group );
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
    
    void CreateFromData3D( const std::string & group )
    {
        try
        {
            CObjectDataMgr::Instance().CreateFromData2D( group );
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
    *    desc:  Free all of the meshes and materials of a specific data group                                                           
    ************************************************************************/
    void FreeGroup2D( const std::string & group, bool freeOpenGLObjects )
    {
        try
        {
            CObjectDataMgr::Instance().FreeGroup2D( group, freeOpenGLObjects );
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
    
    void FreeGroup3D( const std::string & group, bool freeOpenGLObjects )
    {
        try
        {
            CObjectDataMgr::Instance().FreeGroup3D( group, freeOpenGLObjects );
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
    *    desc:  Free all of the meshes and materials of a specific data group                                                           
    ************************************************************************/
    void FreeOpenGL2D( const std::string & group )
    {
        try
        {
            CObjectDataMgr::Instance().FreeOpenGL2D( group );
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
    
    void FreeOpenGL3D( const std::string & group )
    {
        try
        {
            CObjectDataMgr::Instance().FreeOpenGL3D( group );
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
        
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_LoadListTable(string &in)", asFUNCTION(LoadListTable), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_LoadGroup2D(string &in, bool createFromData = true)", asFUNCTION(LoadGroup2D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_LoadGroup3D(string &in, bool createFromData = true)", asFUNCTION(LoadGroup3D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_CreateFromData2D(string &in)", asFUNCTION(CreateFromData2D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_CreateFromData3D(string &in)", asFUNCTION(CreateFromData3D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_FreeGroup2D(string &in, bool freeOpenGLObjects = true)", asFUNCTION(FreeGroup2D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_FreeGroup3D(string &in, bool freeOpenGLObjects = true)", asFUNCTION(FreeGroup3D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_FreeOpenGL2D(string &in)", asFUNCTION(FreeOpenGL2D), asCALL_CDECL) );
        Throw( pEngine->RegisterGlobalFunction("void ObjectData_FreeOpenGL3D(string &in)", asFUNCTION(FreeOpenGL3D), asCALL_CDECL) );
    }
}