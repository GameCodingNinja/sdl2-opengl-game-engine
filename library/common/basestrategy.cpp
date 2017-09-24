
/************************************************************************
*    FILE NAME:       basestrategy.cpp
*
*    DESCRIPTION:     Base strategy class
************************************************************************/

// Physical component dependency
#include <common/basestrategy.h>

// Game lib dependencies
#include <managers/signalmanager.h>
#include <common/defs.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBaseStrategy::CBaseStrategy( int idOffset, int idDir ) :
    m_idOffset(idOffset),
    m_idDir(idDir)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CBaseStrategy::~CBaseStrategy()
{
}   // destructor


/************************************************************************
 *    desc:  Handle messages
 ************************************************************************/
void CBaseStrategy::HandleMessage( int msg, int code )
{
    // Add to the delete index
    if( msg == NDefs::ESM_KILL_SPRITE )
        m_deleteSet.insert( code );

}   // HandleMessage

void CBaseStrategy::HandleMessage( int msg, const std::string & code )
{
    // Add the create name
    if( msg == NDefs::ESM_CREATE_SPRITE )
        m_createSet.insert( code );

}   // HandleMessage


/************************************************************************
*    desc:  Handle any misc processing before the real work is started
************************************************************************/
void CBaseStrategy::MiscProcess()
{
    HandleDelete();
    
    HandleCreate();
    
}   // MiscProcess


/***************************************************************************
*    desc:  Handle the deleting of any object by Id
****************************************************************************/
void CBaseStrategy::HandleDelete()
{
    if( !m_deleteSet.empty() )
    {
        for( auto iter : m_deleteSet )
            DeleteObj( iter );

        m_deleteSet.clear();
    }

}   // HandleDelete


/***************************************************************************
*    desc:  Handle the deleting of any object by Id
****************************************************************************/
void CBaseStrategy::DeleteObj( int index )
{
    // Virtual function meant to be over written by inherited class
}


/***************************************************************************
*    desc:  Handle the creating of new sprite objects by name
****************************************************************************/
void CBaseStrategy::HandleCreate()
{
    if( !m_createSet.empty() )
    {
        for( auto iter : m_createSet )
            CreateObj( iter );

        m_createSet.clear();
    }

}   // HandleDelete


/***************************************************************************
*    desc:  Handle the creating of any object by name
****************************************************************************/
void CBaseStrategy::CreateObj( const std::string & name )
{
    // Virtual function meant to be over written by inherited class
}