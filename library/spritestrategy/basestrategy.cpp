
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
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CBaseStrategy::~CBaseStrategy()
{
}


/************************************************************************
 *    desc:  Set to Destroy the sprite
 ************************************************************************/
void CBaseStrategy::setToDestroy( int spriteIndex )
{
    m_deleteSet.insert( spriteIndex );
}


/************************************************************************
 *    desc:  Set to create the sprite
 ************************************************************************/
void CBaseStrategy::setToCreate( const std::string & name )
{
    m_createSet.insert( name );
}


/************************************************************************
*    desc:  Handle any misc processing before the real work is started
************************************************************************/
void CBaseStrategy::miscProcess()
{
    handleDelete();

    handleCreate();
}


/***************************************************************************
*    desc:  Handle the deleting of any object by Id
****************************************************************************/
void CBaseStrategy::handleDelete()
{
    if( !m_deleteSet.empty() )
    {
        for( auto iter : m_deleteSet )
            deleteObj( iter );

        m_deleteSet.clear();
    }
}


/***************************************************************************
*    desc:  Handle the deleting of any object by Id
****************************************************************************/
void CBaseStrategy::deleteObj( int index )
{
    // Virtual function meant to be over written by inherited class
}


/***************************************************************************
*    desc:  Handle the creating of new sprite objects by name
****************************************************************************/
void CBaseStrategy::handleCreate()
{
    if( !m_createSet.empty() )
    {
        for( auto iter : m_createSet )
            createObj( iter );

        m_createSet.clear();
    }
}


/***************************************************************************
*    desc:  Handle the creating of any object by name
****************************************************************************/
void CBaseStrategy::createObj( const std::string & name )
{
    // Virtual function meant to be over written by inherited class
}
