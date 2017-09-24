
/************************************************************************
*    FILE NAME:       scriptcomponent.h
*
*    DESCRIPTION:     Class for handling the scripting
************************************************************************/

#ifndef __script_component_h__
#define __script_component_h__

// Game lib dependencies
#include <common/defs.h>
#include <script/scriptdefs.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <vector>

// Forward declaration(s)
class asIScriptContext;

class CScriptComponent : boost::noncopyable
{
public:
    
    // Constructor
    CScriptComponent();

    // Destructor
    ~CScriptComponent();
    
    // Prepare the script function to run
    void Prepare(
        const std::string & group,
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>() );

    // Update the script
    void Update();

    // Reset the contexts and recycle
    void ResetAndRecycle();
    
    // Stop a function if it is being called and recycle it
    void StopAndRecycle( const std::string & funcName );

    // Is this component active?
    bool IsActive();

private:

    // dynamic context vector
    // NOTE: This class does not own the pointer
    std::vector<asIScriptContext *> m_pContextVec;

};

#endif  // __script_component_2d_h__


