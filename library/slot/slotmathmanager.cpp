
/************************************************************************
*    FILE NAME:       slotmathmanager.cpp
*
*    DESCRIPTION:     Singlton for managing slot math data
************************************************************************/

// Physical component dependency
#include <slot/slotmathmanager.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/xmlParser.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSlotMathMgr::CSlotMathMgr()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSlotMathMgr::~CSlotMathMgr()
{
}   // destructor


/************************************************************************
 *    desc:  Get the slot math
 ************************************************************************/
const CSlotMath & CSlotMathMgr::GetSlotMath( const std::string & group, const std::string & id ) const
{
    auto groupMapIter = m_slotMathMapMap.find( group );
    if( groupMapIter == m_slotMathMapMap.end() )
        throw NExcept::CCriticalException("Get Slot Math Data Error!",
            boost::str( boost::format("Slot Math group can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    auto dataMapIter = groupMapIter->second.find( id );
    if( dataMapIter == groupMapIter->second.end() )
        throw NExcept::CCriticalException("Get Slot Math Data Error!",
            boost::str( boost::format("Slot Math name can't be found (%s).\n\n%s\nLine: (%s - %s)")
                % group % id % __FUNCTION__ % __LINE__ ));

    return dataMapIter->second;

}   // GetSlotMath


/************************************************************************
 *    desc:  Load all of the reel group data from a specific group
 ************************************************************************/
void CSlotMathMgr::LoadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Symbol config Load Group Data Error!",
            boost::str( boost::format("Symbol config group name can't be found (%s).\n\n%s\nLine: %s")
            % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_slotMathMapMap.find( group ) == m_slotMathMapMap.end() )
    {
        // Create a new group map inside of our map
        m_slotMathMapMap.emplace( group, std::map<const std::string, CSlotMath>() );
        
        LoadFromXML( group, listTableIter->second.back() );
    }
    else
    {
        throw NExcept::CCriticalException("Math config load Error!",
            boost::str( boost::format("Math config group has alread been loaded (%s).\n\n%s\nLine: %s")
            % group % __FUNCTION__ % __LINE__ ));
    }

}   // LoadGroup


/************************************************************************
 *    desc:  Load all math data from an xml
 ************************************************************************/
void CSlotMathMgr::LoadFromXML( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "math" );
    
    // Get the id
    const std::string id = node.getAttribute( "id" );
    
    // Get an iterator to the group
    auto groupMapIter = m_slotMathMapMap.find( group );
    
    // Allocate the math group data to the map
    auto iter = groupMapIter->second.emplace( id, group );

    // Check for duplicate names
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Math Data Load Group Error",
            boost::str( boost::format("Duplicate math group id (%s - %s).\n\n%s\nLine: %s")
                % id % group % __FUNCTION__ % __LINE__ ));
    }

    // Load the data from node
    iter.first->second.LoadFromNode( node );
    
}   // LoadFromXML


/************************************************************************
*    desc:  Load the payline configuration from XML file
************************************************************************/
void CSlotMathMgr::LoadPaylineSetFromFile( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode node = XMLNode::openFileHelper( filePath.c_str(), "paylineSetList" );
    
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        const XMLNode paylineNode = node.getChildNode(i);
        
        // Get the id
        const std::string id = paylineNode.getAttribute( "id" );
        
        auto iter = m_paylineSetMap.emplace( id, paylineNode );
        
        // Check for duplicate ids
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Load Payline Config Error!",
                boost::str( boost::format("Duplicate id (%s).\n\n%s\nLine: %s")
                    % id % __FUNCTION__ % __LINE__ ));
        }
    }
    
}   // LoadPaylineCfgFromFile


/************************************************************************
*    desc:  Get the payline set
************************************************************************/
const CPaylineSet & CSlotMathMgr::GetPaylineSet( const std::string & id ) const
{
    // Make sure the group we are looking for exists
    auto iter = m_paylineSetMap.find( id );
    if( iter == m_paylineSetMap.end() )
        throw NExcept::CCriticalException("Payline Set Data Error!",
            boost::str( boost::format("Payline Set id can't be found (%s).\n\n%s\nLine: %s")
                % id % __FUNCTION__ % __LINE__ ));
    
    return iter->second;
    
}   // GetPaylineSet


/************************************************************************
 *    desc:  Unload all of the reel group data from a specific group
 ************************************************************************/
void CSlotMathMgr::FreeGroup( const std::string & group )
{
    // Make sure the group we are looking for exists
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Obj Data List 2D Free Group Data Error!",
            boost::str( boost::format("Object data list group name can't be found (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));

    // See if this group is still loaded
    // Unload the group data
    auto groupMapIter = m_slotMathMapMap.find( group );
    if( groupMapIter != m_slotMathMapMap.end() )
        m_slotMathMapMap.erase( groupMapIter );

}   // FreeGroup


/************************************************************************
*    desc:  Free the payline set
************************************************************************/
void CSlotMathMgr::FreePaylineSet()
{
    m_paylineSetMap.clear();
}


/************************************************************************
*    desc:  Clear out all the data
************************************************************************/
void CSlotMathMgr::Clear()
{
    m_slotMathMapMap.clear();
    FreePaylineSet();
}
