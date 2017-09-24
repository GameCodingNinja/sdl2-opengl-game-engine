
/************************************************************************
*    FILE NAME:       symbolsetviewmanager.h
*
*    DESCRIPTION:     Singleton for managing different symbol view sets
************************************************************************/

#ifndef __symbol_set_view_manager_h__
#define __symbol_set_view_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <slot/symbolsetview.h>

// Standard lib dependencies
#include <string>
#include <map>

class CSymbolSetViewMgr : public CManagerBase
{
public:
    
    // Get the instance of the singleton class
    static CSymbolSetViewMgr & Instance()
    {
        static CSymbolSetViewMgr symbolViewMgr;
        return symbolViewMgr;
    }
    
    // Load all of the symbols of a specific group
    void LoadGroup( const std::string & group );
    
    // Get the symbol set view data
    CSymbolSetView & Get( const std::string & group, const std::string & name );
    
    // Free a symbol group
    void FreeGroup( const std::string & group );
    
    // Clear all the data
    void Clear();
    
private:
        
    // Constructor
    CSymbolSetViewMgr();

    // Destructor
    virtual ~CSymbolSetViewMgr();
    
    // Load all symbol information from an xml
    void LoadFromXML( const std::string & group, const std::string & filePath );
    
private:
    
    // Map in a map of all the symbol sets
    std::map<const std::string, std::map<const std::string, CSymbolSetView> > m_symbolSetViewMap;

};

#endif  // __symbol_set_view_manager_h__


