
/************************************************************************
*    FILE NAME:       spritestrategymanager.cpp
*
*    DESCRIPTION:     Sprite strategy manager singleton
************************************************************************/

// Physical component dependency
#include <spritestrategy/spritestrategymanager.h>

// Game lib dependencies
#include <2d/object2d.h>
#include <utilities/deletefuncs.h>
#include <spritestrategy/ispritestrategy.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CSpriteStrategyMgr::CSpriteStrategyMgr()
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CSpriteStrategyMgr::~CSpriteStrategyMgr()
{
    NDelFunc::DeleteMapPointers(m_pStrategyMap);
}


/************************************************************************
 *    DESC:  Add strategy
 ************************************************************************/
iSpriteStrategy * CSpriteStrategyMgr::addStrategy( const std::string & strategyId, iSpriteStrategy * pSpriteStrategy )
{
    auto mapIter = m_pStrategyMap.emplace( strategyId, pSpriteStrategy );

    // Check for duplicate groups being used
    if( !mapIter.second )
    {
        throw NExcept::CCriticalException("Sprite Manager Strategy Load Error!",
            boost::str( boost::format("Duplicate id name (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }

    // See if there is any files associated with the strategy id in the list table
    auto listTableIter = m_listTableMap.find( strategyId );
    if( listTableIter != m_listTableMap.end() )
        for( auto & filePathIter : listTableIter->second )
            mapIter.first->second->loadFromFile( filePathIter );

    // Add the strategy pointer to the vector for rendering
    m_pStrategyVec.push_back( pSpriteStrategy );
    
    return pSpriteStrategy;
}


/************************************************************************
 *    DESC:  Delete strategy
 ************************************************************************/
void CSpriteStrategyMgr::deleteStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter != m_pStrategyMap.end() )
    {
        mapIter->second->cleanUp();

        auto strategyIter = std::find( m_pStrategyVec.begin(), m_pStrategyVec.end(), mapIter->second );
        if( strategyIter != m_pStrategyVec.end() )
            m_pStrategyVec.erase( strategyIter );

        NDelFunc::Delete( mapIter->second );
        m_pStrategyMap.erase( mapIter );
    }
}


/************************************************************************
 *    DESC:  Delete sprite
 ************************************************************************/
void CSpriteStrategyMgr::deleteSprite( const std::string & strategyId, int spriteId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter != m_pStrategyMap.end() )
        mapIter->second->setToDestroy( spriteId );
}


/************************************************************************
*    DESC:  create the sprite and provide a unique id number for each one
************************************************************************/
iSprite * CSpriteStrategyMgr::create(
    const std::string & strategyId,
    const std::string & dataName,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Manager Strategy Group Find Error!",
            boost::str( boost::format("Sprite Manager strategy id can't be found (%s, %s).\n\n%s\nLine: %s")
                % strategyId % dataName % __FUNCTION__ % __LINE__ ));

    return mapIter->second->create( dataName, pos, rot, scale );
}

iSprite * CSpriteStrategyMgr::create(
    const std::string & strategyId,
    const std::string & group,
    const std::string & name,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Manager Strategy Group Find Error!",
            boost::str( boost::format("Sprite Manager strategy id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second->create( group, name, pos, rot, scale );
}

iSprite * CSpriteStrategyMgr::create(
    const std::string & strategyId,
    const std::string & dataName )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Manager Strategy Group Find Error!",
            boost::str( boost::format("Sprite Manager strategy id can't be found (%s, %s).\n\n%s\nLine: %s")
                % strategyId % dataName % __FUNCTION__ % __LINE__ ));

    return mapIter->second->create( dataName );
}

iSprite * CSpriteStrategyMgr::create(
    const std::string & strategyId,
    const std::string & group,
    const std::string & name )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Manager Strategy Group Find Error!",
            boost::str( boost::format("Sprite Manager strategy id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second->create( group, name );
}


/************************************************************************
*    DESC:  Delete all the strategies
************************************************************************/
void CSpriteStrategyMgr::clear()
{
    // Do the pre-delete cleanup
    cleanUp();

    NDelFunc::DeleteMapPointers(m_pStrategyMap);
    m_pStrategyMap.clear();
    m_pStrategyVec.clear();
    iSpriteStrategy::clearSpriteCounter();
}


/************************************************************************
*    DESC:  Do any pre-game loop init's
************************************************************************/
void CSpriteStrategyMgr::init()
{
    for( auto iter : m_pStrategyVec )
        iter->init();
}


/************************************************************************
*    DESC:  Do some cleanup
************************************************************************/
void CSpriteStrategyMgr::cleanUp()
{
    for( auto iter : m_pStrategyVec )
        iter->cleanUp();
}


/************************************************************************
*    DESC:  Handle any misc processing before the real work is started
************************************************************************/
void CSpriteStrategyMgr::miscProcess()
{
    for( auto iter : m_pStrategyVec )
        iter->miscProcess();
}


/***************************************************************************
*    DESC:  Update the sprites
****************************************************************************/
void CSpriteStrategyMgr::update()
{
    for( auto iter : m_pStrategyVec )
        iter->update();
}


/************************************************************************
*    DESC:  Transform the sprite
************************************************************************/
void CSpriteStrategyMgr::transform()
{
    for( auto iter : m_pStrategyVec )
        iter->transform();
}

void CSpriteStrategyMgr::transform( const CObject2D & object )
{
    for( auto iter : m_pStrategyVec )
        iter->transform( object );
}


/***************************************************************************
*    DESC:  Render the sprites
****************************************************************************/
void CSpriteStrategyMgr::render()
{
    for( auto iter : m_pStrategyVec )
        iter->render();
}

void CSpriteStrategyMgr::render( const CMatrix & matrix )
{
    for( auto iter : m_pStrategyVec )
        iter->render( matrix );
}

void CSpriteStrategyMgr::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    for( auto iter : m_pStrategyVec )
        iter->render( matrix, rotMatrix );
}


/************************************************************************
*    DESC:  Get the pointer to the strategy
************************************************************************/
iSpriteStrategy * CSpriteStrategyMgr::getStrategy( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Strategy Manager Id Find Error!",
            boost::str( boost::format("Sprite Manager strategy Id can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second;
}
