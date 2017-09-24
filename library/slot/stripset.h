
/************************************************************************
*    FILE NAME:       stripset.h
*
*    DESCRIPTION:     strip set math class
************************************************************************/

#ifndef __strip_set_h__
#define __strip_set_h__

// Standard lib dependencies
#include <string>
#include <vector>
#include <cstdint>

class CStripSet
{
public:

    // Constructor
    CStripSet( const std::string & id, const std::vector<int8_t> & evalSymbIndexVec );
    
    // Get the paytable ID
    const std::string & GetId() const;
    
    // Get the evaluation symbol count
    const std::vector<int8_t> & GetEvalIndexVec() const;
    
private:
    
    // Paytable string id
    const std::string m_id;
    
    // Evaluation symbol index vector
    std::vector<int8_t> m_evalSymbIndexVec;

};

#endif  // __reel_strip_set_h__


