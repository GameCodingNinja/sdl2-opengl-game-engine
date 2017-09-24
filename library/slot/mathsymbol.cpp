
/************************************************************************
*    FILE NAME:       mathsymbol.cpp
*
*    DESCRIPTION:     Class for holding a math symbol
************************************************************************/

// Physical component dependency
#include <slot/mathsymbol.h>

// Standard lib dependencies
#include <algorithm>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CMathSymbol::CMathSymbol( 
    const std::string & symbolID, const std::vector<std::string> & wildMatches ) :
        m_id( symbolID ),
        m_wildMatches( wildMatches )
{
}   // constructor


/************************************************************************
*    desc:  Get the string ID
************************************************************************/
const std::string & CMathSymbol::GetID() const
{
    return m_id;
    
}   // GetID


/************************************************************************
*    desc:  Is this a wild symbol?
************************************************************************/
bool CMathSymbol::IsWild() const
{
    return !m_wildMatches.empty();
    
}   // IsWildSymbol


/************************************************************************
*    desc:  Is wild for this symbol?
************************************************************************/
bool CMathSymbol::IsWildFor( const std::string & symbolID ) const
{
    return std::find( m_wildMatches.begin(), m_wildMatches.end(), symbolID ) != m_wildMatches.end();
    
}   // IsWildFor


/************************************************************************
*    desc:  Does symbol match?
************************************************************************/
bool CMathSymbol::IsMatch( const std::string & symbolID ) const
{
    return ((m_id == symbolID) || IsWildFor(symbolID));
    
}   // Matches


/************************************************************************
*    desc:  Equality operator. The symbols are equal if their id's are
************************************************************************/
bool CMathSymbol::operator == ( const CMathSymbol & mathSymbol ) const
{
    if( m_id == mathSymbol.GetID() )
        return true;

    return false;

}   // operator ==

bool CMathSymbol::operator == ( const std::string & symbolID ) const
{
    if( m_id == symbolID )
        return true;

    return false;

}   // operator ==