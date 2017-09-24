
/************************************************************************
*    FILE NAME:       slotmath.h
*
*    DESCRIPTION:     Class to hold math data
************************************************************************/

#ifndef __slot_math_h__
#define __slot_math_h__

// Game lib dependencies
#include <slot/mathsymbol.h>
#include <slot/paycombo.h>
#include <slot/stripset.h>
#include <slot/stripstop.h>
#include <slot/paytableset.h>
#include <slot/weightedtable.h>
#include <slot/valuetable.h>

// Standard lib dependencies
#include <string>
#include <map>
#include <vector>

// Forward declaration(s)
struct XMLNode;

class CSlotMath
{
public:

    // Constructor
    CSlotMath( const std::string & group );
    
    // Get the payline set ID
    const std::string & GetPaylineSetID() const;
    
    // Get the symbol set
    const std::map<const std::string, CMathSymbol> &
        GetSymbolSet( const std::string & id ) const;
    
    // Get the reel strip
    const std::vector<CStripStop> &
        GetStrip( const std::string & id ) const;
    
    // Get the reel strip set
    const std::vector<CStripSet> &
        GetStripSet( const std::string & id ) const;
    
    // Get the pay combo set
    const std::vector<CPayCombo> &
        GetPayComboSet( const std::string & id ) const;
    
    // Get the paytable set
    const std::vector<CPaytableSet> &
        GetPaytableSet( const std::string & id ) const;
    
    // Get the weighted table
    const CWeightedTable &
        GetWeightedTable( const std::string & id ) const;
    
    // Get the value table
    const CValueTable &
        GetValueTable( const std::string & id ) const;
    
    // Load thes reel group data from node
    void LoadFromNode( const XMLNode & node );
    
private:
    
    // Load the symbol set data from node
    void LoadSymbolSetsFromNode( const XMLNode & node );
    
    // Load the math strip data from node
    void LoadStripFromNode( const XMLNode & node );
    
    // Load the strip set list data from node
    void LoadStripSetListFromNode( const XMLNode & node );
    
    // Load the pay combo data from node
    void LoadPayComboFromNode( const XMLNode & node );
    
    // Load the paytable set list data from node
    void LoadPaytableSetListFromNode( const XMLNode & node );
    
    // Load the weighted table data from node
    void LoadWeightedTableFromNode( const XMLNode & node );
    
    // Load the value table data from node
    void LoadValueTableFromNode( const XMLNode & node );
    
private:
    
    // The group the math data is in
    // Mainly used for error reporting for easier identification 
    // of which math file is causing a problem.
    std::string m_group;
    
    // The name of the math id
    std::string m_id;
    
    // The name of the payline set id
    std::string m_paylineSetId;
    
    // The math percentage
    float m_percenatge;
    
    // Map of math symbol set
    std::map<const std::string, std::map<const std::string, CMathSymbol> > m_symbolSetMapMap;
    
    // Map of math symbol strips
    std::map<const std::string, std::vector<CStripStop>> m_stripMapVec;
    
    // Map of reel strip sets
    std::map<const std::string, std::vector<CStripSet>> m_stripSetMapVec;
    
    // Map of payline combo data
    std::map<const std::string, std::vector<CPayCombo>> m_payComboMapVec;
    
    // Map of paytable set lists
    std::map<const std::string, std::vector<CPaytableSet>> m_paytableSetMapVec;
    
    // Map of weighted table data
    std::map<const std::string, CWeightedTable> m_weightedTableMap;
    
    // Map of value table data
    std::map<const std::string, CValueTable> m_valueTableMap;

};

#endif  // __slot_math_h__


