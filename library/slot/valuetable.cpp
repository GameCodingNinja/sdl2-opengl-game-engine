
/************************************************************************
*    FILE NAME:       valuetable.cpp
*
*    DESCRIPTION:     Class for holding the value table
************************************************************************/

// Physical component dependency
#include <slot/valuetable.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CValueTable::CValueTable( const std::vector<int> & valueVec ) :
    m_valueVec(valueVec)
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CValueTable::~CValueTable()
{
}


/************************************************************************
 *    desc:  Get the value from the value table
 ************************************************************************/
int CValueTable::getValue( const int index ) const
{
    return m_valueVec.at(index);
}
