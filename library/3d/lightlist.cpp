
/************************************************************************
*    FILE NAME:       lightlist.cpp
*
*    DESCRIPTION:     Light List Class
************************************************************************/

// Physical component dependency
#include <3d/lightlist.h>

// Game lib dependencies
#include <3d/light.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies


/************************************************************************
*    desc:  Constructor
************************************************************************/
CLightLst::CLightLst()
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CLightLst::~CLightLst()
{
}   // destructor


/************************************************************************
*    desc:  Add a light
************************************************************************/
void CLightLst::Add(
    const std::string & name,
    NDefs::ELightType lightType,
    const CColor & color,
    bool enable,
    const CPoint<float> & posDir,
    float radius )
{
    auto iter = m_lightMap.emplace( name, lightType );
    
    // Check for duplicate names
    if( !iter.second )
    {
        throw NExcept::CCriticalException("Add light Error",
            boost::str( boost::format("Duplicate light name id (%s).\n\n%s\nLine: %s")
                % name % __FUNCTION__ % __LINE__ ));
    }

}   // Add


/************************************************************************
*    desc:  Turn the light on or off
************************************************************************/
void CLightLst::Enable( const std::string & name, bool enable )
{
    auto iter = m_lightMap.find(name);

    if( iter != m_lightMap.end() )
        iter->second.Enable( enable );

}   // Enable

