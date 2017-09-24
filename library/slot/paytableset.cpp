
/************************************************************************
*    FILE NAME:       paytableset.cpp
*
*    DESCRIPTION:     Paytable set math class
************************************************************************/

// Physical component dependency
#include <slot/paytableset.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPaytableSet::CPaytableSet( const NSlotDefs::EPayType type, const std::string & id ) :
    m_type(type),
    m_id(id)
{
}   // constructor


/************************************************************************
*    desc:  Get the paytable ID
************************************************************************/
const std::string & CPaytableSet::GetId() const
{
    return m_id;
    
}   // GetId


/************************************************************************
*    desc:  Get the paytable type
************************************************************************/
NSlotDefs::EPayType CPaytableSet::GetType() const
{
    return m_type;
}
