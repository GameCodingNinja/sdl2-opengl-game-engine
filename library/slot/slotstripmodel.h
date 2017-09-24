
/************************************************************************
*    FILE NAME:       slotstripmodel.h
*
*    DESCRIPTION:     Slot strip model
************************************************************************/

#ifndef __slot_strip_model_h__
#define __slot_strip_model_h__

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <vector>
#include <random>
#include <cstdint>

// Forward declaration(s)
class CMathSymbol;
class CSlotMath;
class CStripStop;

class CSlotStripModel : public boost::noncopyable
{
public:

    // Constructor
    CSlotStripModel(
        const std::vector<CStripStop> & rMathStripVec,
        std::mt19937 & rRng,
        const std::vector<int8_t> & evalSymbIndexVec );

    // Destructor
    virtual ~CSlotStripModel();
    
    // Generate the strip stop
    void GenerateStop();
    
    // Get the last stop
    int GetLastStop() const;
    
    // Get the strip stop
    int GetStop() const;
    
    // Get the math symbol
    const CMathSymbol & GetSymbol( const int stop ) const;
    
    // Get the math symbol index
    int GetSymbolIndex( const int stop ) const;
    
    // Get the evaluation symbol count
    const std::vector<int8_t> & GetEvalIndexVec() const;
    
    // Get the math symbol strip vector
    const std::vector<CStripStop> & GetStripVec() const;
    
private:
    
    // Math strip math symbols
    const std::vector<CStripStop> & m_rMathStripVec;
    
    // Random number generator reference based on the Mersenne Twister algorithm
    std::mt19937 & m_rRng;
    
    // Evaluation symbol index vector
    std::vector<int8_t> m_evalSymbIndexVec;
    
    // total weight
    int m_totalWeight;
    
    // last strip stop
    int m_lastStop;
    
    // strip stop
    int m_stop;

};

#endif  // __slot_strip_model_h__


