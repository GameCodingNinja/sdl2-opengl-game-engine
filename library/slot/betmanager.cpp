
/************************************************************************
*    FILE NAME:       betmanager.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include <slot/betmanager.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBetMgr::CBetMgr() :
    m_lineBet(0),
    m_totalBet(0),
    m_totalLines(0),
    m_credits(0)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CBetMgr::~CBetMgr()
{
}   // destructor


/************************************************************************
*    desc:  Set/Get the line bet
************************************************************************/
void CBetMgr::SetLineBet( uint lineBet )
{
    m_lineBet = lineBet;
    
}   // SetLineBet

uint CBetMgr::GetLineBet() const
{
    return m_lineBet;
    
}   // GetLineBet


/************************************************************************
*    desc:  Set the total lines being bet
************************************************************************/
void CBetMgr::SetTotalLines( uint totalLines )
{
    m_totalLines = totalLines;
    
}   // SetTotalLinesBet


/************************************************************************
*    desc:  Get the total bet
************************************************************************/
uint CBetMgr::GetTotalBet() const
{
    return m_lineBet * m_totalLines;
    
}   // GetTotalBet


/************************************************************************
*    desc:  Set/Get the credits
************************************************************************/
void CBetMgr::SetCredits( uint credits )
{
    m_credits = credits;
    
}   // SetCredits

uint CBetMgr::GetCredits() const
{
    return m_credits;
    
}   // GetCredits


/************************************************************************
*    desc:  Is there anough credits to play?
************************************************************************/
bool CBetMgr::AllowPlay() const
{
    return ((m_credits > 0) && (m_credits >= GetTotalBet()));
    
}   // AllowPlay


/************************************************************************
*    desc:  Deduct the bet amount from the credits
************************************************************************/
void CBetMgr::DeductBet()
{
    if( AllowPlay() )
        m_credits -= GetTotalBet();
    
}   // DeductBet


/************************************************************************
*    desc:  Add in the award
************************************************************************/
void CBetMgr::AddAward( uint award )
{
    m_credits += award;
    
}   // AddAward
