
/************************************************************************
*    FILE NAME:       scriptmanager.h
*
*    DESCRIPTION:     Class to manage AngelScripts
************************************************************************/

#ifndef __script_manager_h__
#define __script_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <utilities/smartpointers.h>
#include <script/scriptdefs.h>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
class asIScriptEngine;
class asIScriptContext;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
struct asSMessageInfo;

class CScriptManager : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CScriptManager & Instance()
    {
        static CScriptManager scriptManager;
        return scriptManager;
    }

    // Get the pointer to the script engine
    asIScriptEngine * GetEnginePtr();

    // Load all of the scripts of a specific group
    void LoadGroup( const std::string & group );

    // Free all of the scripts of a specific data group
    void FreeGroup( const std::string & group );

    // Get the script engine contex from a managed pool
    // NOTE: The receiver of this pointer is the owner if it's still 
    //       holding on to it when the game terminates
    asIScriptContext * GetContext();
    
    // Update the active scripts
    void Update();
    void Update( std::vector<asIScriptContext *> & pContextVec );

    // Add the script context back to the managed pool
    void RecycleContext( asIScriptContext * pContext );

    // Get pointer to function
    asIScriptFunction * GetPtrToFunc( const std::string & group, const std::string & name );
    
    // Prepare the script function to run
    void Prepare(
        const std::string & group,
        const std::string & funcName,
        std::vector<asIScriptContext *> & pContextVec,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );
    
    void Prepare(
        const std::string & group,
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec );
    
    //////////////////////////////////////////////////////
    // Register Object with AngelScript
    //////////////////////////////////////////////////////
    void Register();
    
    // Prepare the spawn script function to run
    void PrepareSpawn( const std::string & funcName );
    void PrepareSpawnObj( const std::string & funcName, void * pVoid );

private:

    // Constructor
    CScriptManager();

    // Destructor
    virtual ~CScriptManager();

    // Add the script to the module
    void AddScript( asIScriptModule * pScriptModule, const std::string & filePath );

    // Build all the scripts added to the module
    void BuildScript( asIScriptModule * pScriptModule, const std::string & group );

    // Call back to display AngelScript messages
    void MessageCallback(const asSMessageInfo & msg);

private:

    // Smart com pointer to AngelScript script engine
    NSmart::scoped_com_ptr<asIScriptEngine> scpEngine;

    // Smart com pointer to AngelScript context
    NSmart::scoped_com_ptr<asIScriptContext> scpContext;

    // Map containing a group of function pointers
    std::map< const std::string, std::map< const std::string, asIScriptFunction * > > m_scriptFunctMapMap;

    // Holds the pool of script contexts
    std::vector<asIScriptContext *> m_pContextPoolVec;
    
    // Holds active contexts that are executing scripts
    std::vector<asIScriptContext *> m_pActiveContextVec;

};

#endif  // __script_manager_h__


