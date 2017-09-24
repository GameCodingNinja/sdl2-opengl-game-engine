
/************************************************************************
*    FILE NAME:       scriptmanager.cpp
*
*    DESCRIPTION:     Class to manage AngelScripts
************************************************************************/

// Physical component dependency
#include <script/scriptmanager.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/statcounter.h>
#include <script/scriptglobals.h>

// AngelScript lib dependencies
#include <angelscript.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

// AngelScript lib dependencies
#include <angelscript.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CScriptManager::CScriptManager()
{
    // Create the script engine
    scpEngine.reset( asCreateScriptEngine(ANGELSCRIPT_VERSION) );
    if( scpEngine.isNull() )
    {
        throw NExcept::CCriticalException("Error Creating AngelScript Engine!",
            boost::str( boost::format("AngelScript engine could not be created.\n\n%s\nLine: %s")
                % __FUNCTION__ % __LINE__ ));
    }

    // We don't use call backs for every line execution so set this property to not build with line cues
    scpEngine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);

    // Set the message callback to print the messages that the engine gives in case of errors
    if( scpEngine->SetMessageCallback(asMETHOD(CScriptManager, MessageCallback), this, asCALL_THISCALL) < 0 )
    throw NExcept::CCriticalException("Error Creating AngelScript Engine!",
        boost::str( boost::format("AngelScript message callback could not be created.\n\n%s\nLine: %s")
            % __FUNCTION__ % __LINE__ ));

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CScriptManager::~CScriptManager()
{
    // Release the context pool
    for( auto iter : m_pContextPoolVec )
        iter->Release();
    
    for( auto iter : m_pActiveContextVec )
        iter->Release();

}   // destructor


/************************************************************************
*    desc:  Load all of the scripts of a specific group
*
*    param: string & group - specified group of scripts to load
************************************************************************/
void CScriptManager::LoadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Script List Load Group Data Error!",
            boost::str( boost::format("Script list group name can't be found (%s).\n\n%s\nLine: %s") 
                % group % __FUNCTION__ % __LINE__ ));

    // Create the module if it doesn't already exist
    asIScriptModule * pScriptModule = scpEngine->GetModule(group.c_str(), asGM_CREATE_IF_NOT_EXISTS);
    if( pScriptModule == nullptr )
    {
        throw NExcept::CCriticalException("Script List load Error!",
            boost::str( boost::format("Error creating script group module (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

    // Add the scripts to the module
    for( auto & iter : listTableIter->second )
        AddScript( pScriptModule, iter );

    // Build all the scripts added to the module
    BuildScript( pScriptModule, group );

}   // LoadGroup


/************************************************************************
*    desc:  Add the script to the module
************************************************************************/
void CScriptManager::AddScript( asIScriptModule * pScriptModule, const std::string & filePath )
{
    // Load the script file into a charater array
    std::shared_ptr<char> spChar = NGenFunc::FileToBuf( filePath );

    // Load script into module section - the file path is it's ID
    if( pScriptModule->AddScriptSection(filePath.c_str(), spChar.get() ) < 0 ) // std::strlen( spChar.get() )
    {
        throw NExcept::CCriticalException("Script List load Error!",
            boost::str( boost::format("Error loading script (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    }

}   // AddScript


/************************************************************************
*    desc:  Build all the scripts added to the module
************************************************************************/
void CScriptManager::BuildScript( asIScriptModule * pScriptModule, const std::string & group )
{
    // Build the script
    int error = pScriptModule->Build();
    if( error < 0 )
    {
        throw NExcept::CCriticalException("Script List build Error!",
            boost::str( boost::format("Error building script group with error code %d. (%s).\n\n%s\nLine: %s")
                % error % group % __FUNCTION__ % __LINE__ ));
    }

}   // BuildScript


/************************************************************************
*    desc:  Get the script context from a managed pool
*
*    NOTE: The receiver of this pointer is the owner if it's still 
*          holding on to it when the game terminates
************************************************************************/
asIScriptContext * CScriptManager::GetContext()
{
    if( !m_pContextPoolVec.empty() )
    {
        asIScriptContext * pContex = m_pContextPoolVec.back();
        m_pContextPoolVec.pop_back();

        return pContex;
    }
    
    // Maintain a total count of contexts
    CStatCounter::Instance().IncScriptContexCounter();

    return scpEngine->CreateContext();

}   // GetContext


/************************************************************************
*    desc:  Add the script context back to the managed pool
************************************************************************/
void CScriptManager::RecycleContext( asIScriptContext * pContext )
{
    m_pContextPoolVec.push_back( pContext );

}   // RecycleContext


/************************************************************************
*    desc:  Get pointer to function name
************************************************************************/
asIScriptFunction * CScriptManager::GetPtrToFunc( const std::string & group, const std::string & name )
{
    // Create the map group if it doesn't already exist
    auto mapMapIter = m_scriptFunctMapMap.find( group );
    if( mapMapIter == m_scriptFunctMapMap.end() )
        mapMapIter = m_scriptFunctMapMap.emplace( group, std::map<const std::string, asIScriptFunction *>() ).first;

    // See if this function pointer has already been saved
    auto mapIter = mapMapIter->second.find( name );

    // If it's not found, find the function and add it to the map
    // It's faster to keep the function pointers in a map then to use AngelScript's GetFunction call.
    if( mapIter == mapMapIter->second.end() )
    {
        asIScriptModule * pScriptModule = scpEngine->GetModule(group.c_str(), asGM_ONLY_IF_EXISTS);
        if( pScriptModule == nullptr )
        {
            throw NExcept::CCriticalException("Error Getting Pointer to Function!",
                boost::str( boost::format("AngelScript module does not exist (%s, %s).\n\n%s\nLine: %s")
                    % group % name % __FUNCTION__ % __LINE__ ));
        }

        // Try to get the pointer to the function
        asIScriptFunction *pScriptFunc = pScriptModule->GetFunctionByName(name.c_str());
        if( pScriptFunc == nullptr )
        {
            throw NExcept::CCriticalException("Error Getting Pointer to Function!",
                boost::str( boost::format("AngelScript function does not exist (%s, %s).\n\n%s\nLine: %s")
                    % group % name % __FUNCTION__ % __LINE__ ));
        }

        // Insert the function pointer into the map
        mapIter = mapMapIter->second.emplace( name, pScriptFunc ).first;
    }

    return mapIter->second;

}   // GetPtrToFunc


/************************************************************************
*    desc:  Call back to display AngelScript messages                                                             
************************************************************************/
void CScriptManager::MessageCallback(const asSMessageInfo & msg)
{
    std::string type = "ERROR";
    if( msg.type == asMSGTYPE_WARNING ) 
        type = "WARN";

    else if( msg.type == asMSGTYPE_INFORMATION ) 
        type = "INFO";

    std::string msgStr = boost::str( boost::format( "%s (%d, %d) : %s : %s" ) % msg.section % msg.row % msg.col % type % msg.message );

    NGenFunc::PostDebugMsg( msgStr );

}   // MessageCallback


/************************************************************************
*    desc:  Get the pointer to the script engine
************************************************************************/
asIScriptEngine * CScriptManager::GetEnginePtr()
{
    return scpEngine.get();

}   // GetEnginePtr


/************************************************************************
*    desc:  Free all of the scripts of a specific data group
************************************************************************/
void CScriptManager::FreeGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Script List Group Error!",
            boost::str( boost::format("Script group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // Discard the module and free its memory.
    scpEngine->DiscardModule( group.c_str() );

    // Erase the group from the map
    auto mapMapIter = m_scriptFunctMapMap.find( group );
    if( mapMapIter != m_scriptFunctMapMap.end() )
            m_scriptFunctMapMap.erase( mapMapIter );

}   // FreeGroup


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
void CScriptManager::Prepare(
    const std::string & group,
    const std::string & funcName,
    std::vector<asIScriptContext *> & pContextVec,
    const std::vector<CScriptParam> & paramVec )
{
    // Get a context from the script manager pool
    pContextVec.push_back( GetContext() );
    
    // Get the function pointer
    asIScriptFunction * pScriptFunc = GetPtrToFunc(group, funcName);

    // Prepare the function to run
    if( pContextVec.back()->Prepare(pScriptFunc) < 0 )
    {
        throw NExcept::CCriticalException("Error Preparing Script!",
            boost::str( boost::format("There was an error preparing the script (%s).\n\n%s\nLine: %s")
                % funcName % __FUNCTION__ % __LINE__ ));
    }
    
    // Pass the parameters to the script function
    for( size_t i = 0; i < paramVec.size(); ++i )
    {
        int returnVal(0);
        
        if( paramVec[i].GetType() == CScriptParam::EPT_BOOL )
        {
            returnVal = pContextVec.back()->SetArgByte(i, paramVec[i].Get<bool>());
        }
        else if( paramVec[i].GetType() == CScriptParam::EPT_INT )
        {
            returnVal = pContextVec.back()->SetArgDWord(i, paramVec[i].Get<int>());
        }
        else if( paramVec[i].GetType() == CScriptParam::EPT_UINT )
        {
            returnVal = pContextVec.back()->SetArgDWord(i, paramVec[i].Get<uint>());
        }
        else if( paramVec[i].GetType() == CScriptParam::EPT_FLOAT )
        {
            returnVal = pContextVec.back()->SetArgFloat(i, paramVec[i].Get<float>());
        }
        else if( paramVec[i].GetType() == CScriptParam::EPT_REG_OBJ )
        {
            returnVal = pContextVec.back()->SetArgObject(i, paramVec[i].Get<void *>());
        }
        
        if( returnVal < 0 )
        {
            throw NExcept::CCriticalException("Error Setting Script Param!",
                boost::str( boost::format("There was an error setting the script parameter (%s).\n\n%s\nLine: %s")
                    % funcName % __FUNCTION__ % __LINE__ ));
        }
    }

}   // Prepare

void CScriptManager::Prepare(
    const std::string & group,
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec )
{
    Prepare( group, funcName, m_pActiveContextVec, paramVec );
    
}   // Prepare


/************************************************************************
*    desc:  Prepare the spawn script function to run
************************************************************************/
void CScriptManager::PrepareSpawn( const std::string & funcName )
{
    auto pContex = asGetActiveContext();
    if( pContex )
    {
        // Get the module name
        std::string group = pContex->GetFunction()->GetModuleName();
        
        // Prepare the script function to run
        Prepare( group, funcName, m_pActiveContextVec );
    }

}   // PrepareSpawn

void CScriptManager::PrepareSpawnObj( const std::string & funcName, void * pVoid )
{
    auto pContex = asGetActiveContext();
    if( pContex )
    {
        // Get the module name
        std::string group = pContex->GetFunction()->GetModuleName();
        
        // Prepare the script function to run
        Prepare( group, funcName, m_pActiveContextVec, {pVoid} );
    }

}   // PrepareSpawnObj


/************************************************************************
*    desc:  Update the script
************************************************************************/
void CScriptManager::Update()
{
    if( !m_pActiveContextVec.empty() )
        Update( m_pActiveContextVec );
}

void CScriptManager::Update( std::vector<asIScriptContext *> & pContextVec )
{
    auto iter = pContextVec.begin();
    while( iter != pContextVec.end() )
    {
        // See if this context is still being used
        if( ((*iter)->GetState() == asEXECUTION_SUSPENDED) || 
            ((*iter)->GetState() == asEXECUTION_PREPARED) )
        {
            // Increment the active script contex counter
            CStatCounter::Instance().IncActiveScriptContexCounter();

            // Execute the script and check for errors
            // Since the script can be suspended, this also is used to continue execution
            if( (*iter)->Execute() < 0 )
            {
                throw NExcept::CCriticalException("Error Calling Spawn Script!",
                    boost::str( boost::format("There was an error executing the script.\n\n%s\nLine: %s")
                        % __FUNCTION__ % __LINE__ ));
            }

            // Return the context to the pool if it has not been suspended
            if( (*iter)->GetState() != asEXECUTION_SUSPENDED )
            {
                RecycleContext( (*iter) );
                iter = pContextVec.erase( iter );
            }
            else
            {
                ++iter;
            }
        }
    }

}   // UpdateSpawnScripts


/************************************************************************
*    desc:  Register the class with AngelScript
************************************************************************/
void CScriptManager::Register()
{
    using namespace NScriptGlobals;
    
    Throw( scpEngine->RegisterGlobalFunction("void Spawn(string &in)", asMETHOD(CScriptManager, PrepareSpawn), asCALL_THISCALL_ASGLOBAL, &CScriptManager::Instance()) );
    
}   // Register
