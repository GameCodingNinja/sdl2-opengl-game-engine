
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
}


/************************************************************************
*    desc:  Get the paytable ID
************************************************************************/
const std::string & CPaytableSet::getId() const
{
    return m_id;
}


/************************************************************************
*    desc:  Get the paytable type
************************************************************************/
NSlotDefs::EPayType CPaytableSet::getType() const
{
    return m_type;
}
