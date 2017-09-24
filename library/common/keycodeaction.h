
/************************************************************************
*    FILE NAME:       keycodeaction.h
*
*    DESCRIPTION:     Class for holding key codes to test for action events
************************************************************************/

#ifndef __key_code_action_h__
#define __key_code_action_h__

// Standard lib dependencies
#include <vector>
#include <algorithm>

class CKeyCodeAction
{
public:
    
    CKeyCodeAction(int id)
    {
        SetId( id );
    }

    // Set additional id's
    void SetId( int id )
    {
        // Only set id's that are positive numbers
        if( id > -1 )
            m_id.push_back(id);
    }
    
    // Remove an id
    void RemoveId( int id )
    {
        auto iter = std::find(m_id.begin(), m_id.end(), id);
        
        if( iter != m_id.end() )
            m_id.erase( iter );
    }

    // Check for action
    bool WasAction( const int id ) const
    {
        for( int iter : m_id )
            if( iter == id )
                return true;

        return false;
    }

private:

    std::vector<int> m_id;
};

#endif  // __key_code_action_h__


