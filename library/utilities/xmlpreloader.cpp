
/************************************************************************
*    FILE NAME:       xmlpreloader.cpp
*
*    DESCRIPTION:     Preload XML data from file.
*                     Utility for preloading XML data for objects that can
*                     only be allocated in there specific state 
************************************************************************/

// Physical component dependency
#include <utilities/xmlpreloader.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CXMLPreloader::CXMLPreloader()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CXMLPreloader::~CXMLPreloader()
{
}   // destructor


/************************************************************************
 *    desc:  Load the XML file
 ************************************************************************/
const XMLNode & CXMLPreloader::Load( const std::string & filePath, const std::string & firstNode )
{
    // Just return it if it's already been loaded
    auto findIter = m_xmlNodeMap.find( filePath );
    if( findIter != m_xmlNodeMap.end() )
        return findIter->second;
    
    // Add a new XML node to the map
    auto nodeIter = m_xmlNodeMap.emplace( filePath, XMLNode() );
    
    // Open and parse the XML file:
    nodeIter.first->second = XMLNode::openFileHelper( filePath.c_str(), firstNode.c_str() );
    
    return nodeIter.first->second;
    
}   // LoadFromXML


/************************************************************************
 *    desc:  Get the loaded XML node
 ************************************************************************/
const XMLNode & CXMLPreloader::GetNode( const std::string & filePath ) const
{
    // Make sure we have loaded this xml file
    auto iter = m_xmlNodeMap.find( filePath );
    if( iter == m_xmlNodeMap.end() )
        throw NExcept::CCriticalException("XML Preloader load error",
            boost::str( boost::format("XML file has NOT been loaded (%s).\n\n%s\nLine: %s")
                % filePath % __FUNCTION__ % __LINE__ ));
    
    return iter->second;
    
}   // GetNode


/************************************************************************
 *    desc:  Clear all the XML data
 ************************************************************************/
void CXMLPreloader::Clear()
{
    m_xmlNodeMap.clear();
    
}   // Clear
