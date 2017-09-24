
/************************************************************************
*    FILE NAME:       betmanager.h
*
*    DESCRIPTION:     Singleton to manage the bet
************************************************************************/

#ifndef __bet_manager_h__
#define __bet_manager_h__

// Game lib dependencies
#include <common/defs.h>

class CBetMgr
{
public:

    // Get the instance of the singleton class
    static CBetMgr & Instance()
    {
        static CBetMgr betMgr;
        return betMgr;
    }
    
    // Set/Get the line bet
    void SetLineBet( uint lineBet );
    uint GetLineBet() const;
    
    // Set the total lines being bet
    void SetTotalLines( uint totalLinesBet );
    
    // Get the total bet
    uint GetTotalBet() const;
    
    // Set/Get the credits
    void SetCredits( uint credits );
    uint GetCredits() const;
    
    // Is there anough credits to play?
    bool AllowPlay() const;
    
    // Deduct the bet amount from the credits
    void DeductBet();
    
    // Add in the award
    void AddAward( uint award );
    
private:
    
    // Constructor
    CBetMgr();

    // Destructor
    ~CBetMgr();
    
private:
    
    // line bet
    uint m_lineBet;
    
    // Total bet
    uint m_totalBet;
    
    // Total number of lines being bet
    uint m_totalLines;
    
    // Total credits
    uint m_credits;

};

#endif  // __bet_manager_h__


