
/************************************************************************
*    FILE NAME:       scriptcomponent.cpp
*
*    DESCRIPTION:     Class for handling the scripting
************************************************************************/

// Physical component dependency
#include <script/scriptcomponent.h>

// Game lib dependencies
#include <script/scriptmanager.h>
#include <utilities/exceptionhandling.h>
#include <utilities/statcounter.h>

// AngelScript lib dependencies
#include <angelscript.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <iostream>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CScriptComponent::CScriptComponent()
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CScriptComponent::~CScriptComponent()
{
    // Release the contextes we are still holding on to
    for( auto iter : m_pContextVec )
        iter->Release();

}   // destructor


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
void CScriptComponent::Prepare(
    const std::string & group,
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec )
{
    CScriptManager::Instance().Prepare( group, funcName, m_pContextVec, paramVec );

}   // Prepare


/************************************************************************
*    desc:  Update the script
************************************************************************/
void CScriptComponent::Update()
{
    if( !m_pContextVec.empty() )
        CScriptManager::Instance().Update( m_pContextVec );

}   // Update


/************************************************************************
*    desc:  Is this component active?
************************************************************************/
bool CScriptComponent::IsActive()
{
    return !m_pContextVec.empty();

}   // IsActive


/************************************************************************
*    desc:  Reset the contexts and recycle
************************************************************************/
void CScriptComponent::ResetAndRecycle()
{
    if( !m_pContextVec.empty() )
    {
        for( auto iter : m_pContextVec )
        {
            if( iter->GetState() == asEXECUTION_SUSPENDED )
                iter->Abort();

            CScriptManager::Instance().RecycleContext( iter );
        }

        m_pContextVec.clear();
    }

}   // ResetAndRecycle


/************************************************************************
*    desc:  Stop a function if it is being called and recycle it
*           This function assume you're checking for the original calling function
************************************************************************/
void CScriptComponent::StopAndRecycle( const std::string & funcName )
{
    if( !m_pContextVec.empty() )
    {
        // See if the function in question is still running
        auto iter = std::find_if( m_pContextVec.begin(), m_pContextVec.end(), 
            [funcName](asIScriptContext * pContex)
            { return (funcName == pContex->GetFunction(pContex->GetCallstackSize()-1)->GetName()); } );

        if( iter != m_pContextVec.end() )
        {
            if( (*iter)->GetState() == asEXECUTION_SUSPENDED )
                (*iter)->Abort();

            CScriptManager::Instance().RecycleContext( (*iter) );

            m_pContextVec.erase( iter );
        }
    }
    
}   // StopAndRecycle
