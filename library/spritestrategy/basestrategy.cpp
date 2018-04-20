
/************************************************************************
*    FILE NAME:       basestrategy.cpp
*
*    DESCRIPTION:     Base strategy class
************************************************************************/

// Physical component dependency
#include <spritestrategy/basestrategy.h>

// Game lib dependencies
#include <managers/signalmanager.h>
#include <common/defs.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBaseStrategy::CBaseStrategy( const std::string & cameraId, int idOffset, int idDir ) :
    iSpriteStrategy( cameraId ),
    m_idOffset(idOffset),
    m_idDir(idDir)
{
}   // constructor

CBaseStrategy::CBaseStrategy( int idOffset, int idDir ) :
    iSpriteStrategy(),
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
 *    desc:  Set to Destroy the sprite
 ************************************************************************/
void CBaseStrategy::SetToDestroy( int spriteIndex )
{
    m_deleteSet.insert( spriteIndex );

}   // SetToDestroy


/************************************************************************
 *    desc:  Set to create the sprite
 ************************************************************************/
void CBaseStrategy::SetToCreate( const std::string & name )
{
    m_createSet.insert( name );

}   // SetToCreate


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