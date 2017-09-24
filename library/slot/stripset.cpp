
/************************************************************************
*    FILE NAME:       stripset.cpp
*
*    DESCRIPTION:     Strip set math class
************************************************************************/

// Physical component dependency
#include <slot/stripset.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CStripSet::CStripSet( const std::string & id, const std::vector<int8_t> & evalSymbIndexVec ) :
    m_id(id),
    m_evalSymbIndexVec(evalSymbIndexVec)
{
}   // constructor


/************************************************************************
*    desc:  Get the paytable ID
************************************************************************/
const std::string & CStripSet::GetId() const
{
    return m_id;
    
}   // GetId


/************************************************************************
*    desc:  Get the evaluation symbol index count
************************************************************************/
const std::vector<int8_t> & CStripSet::GetEvalIndexVec() const
{
    return m_evalSymbIndexVec;
}
