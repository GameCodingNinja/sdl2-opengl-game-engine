
/************************************************************************
*    FILE NAME:       spritestrategymanager.cpp
*
*    DESCRIPTION:     Sprite strategy manager singleton
************************************************************************/

// Physical component dependency
#include <managers/spritestrategymanager.h>

// Game lib dependencies
#include <2d/object2d.h>
#include <utilities/deletefuncs.h>
#include <common/ispritestrategy.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSpriteStrategyMgr::CSpriteStrategyMgr() :
    m_spriteInc(0)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSpriteStrategyMgr::~CSpriteStrategyMgr()
{
    NDelFunc::DeleteMapPointers(m_pStrategyMap);
    
}   // destructor


/************************************************************************
 *    desc:  Load strategy data from id
 ************************************************************************/
void CSpriteStrategyMgr::Load( const std::string & strategyId, iSpriteStrategy * pSpriteStrategy )
{
    auto mapIter = m_pStrategyMap.emplace( strategyId, pSpriteStrategy );
    
    // Check for duplicate groups being used
    if( !mapIter.second )
    {
        throw NExcept::CCriticalException("Sprite Manager Strategy Load Error!",
            boost::str( boost::format("Duplicate id name (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));
    }
    
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( strategyId );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Sprite Manager List Load Group Data Error!",
            boost::str( boost::format("Sprite Manager list id name can't be found (%s).\n\n%s\nLine: %s")
                % strategyId % __FUNCTION__ % __LINE__ ));

    for( auto & filePathIter : listTableIter->second ) 
        mapIter.first->second->LoadFromFile( filePathIter );
    
    // Add the strategy pointer to the vector for rendering
    m_pStrategyVec.push_back( pSpriteStrategy );

}   // Load


/************************************************************************
*    desc:  create the sprite and provide a unique id number for each one
************************************************************************/
const std::vector<int> & CSpriteStrategyMgr::Create(
    const std::string & strategyId,
    const std::string & name,
    const int count,
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
    
    m_incReturn.clear();
    m_incReturn.reserve(count);
    
    // Create the requested number of sprites
    for( int i = 0; i < count; ++i )
        m_incReturn.push_back( mapIter->second->Create( name, ++m_spriteInc, pos, rot, scale ) );
    
    return m_incReturn;

}   // Create

int CSpriteStrategyMgr::Create(
    const std::string & strategyId,
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
    
    return mapIter->second->Create( name, ++m_spriteInc, pos, rot, scale );

}   // Create


/************************************************************************
*    desc:  Delete all the sprites
************************************************************************/
void CSpriteStrategyMgr::Clear()
{
    NDelFunc::DeleteMapPointers(m_pStrategyMap);
    m_pStrategyMap.clear();
    m_pStrategyVec.clear();
    m_spriteInc = 0;

}   // Clear


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CSpriteStrategyMgr::Init()
{
    for( auto iter : m_pStrategyVec )
        iter->Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CSpriteStrategyMgr::CleanUp()
{
    for( auto iter : m_pStrategyVec )
        iter->CleanUp();
    
}   // CleanUp


/************************************************************************
*    desc:  Handle any misc processing before the real work is started
************************************************************************/
void CSpriteStrategyMgr::MiscProcess()
{
    for( auto iter : m_pStrategyVec )
        iter->MiscProcess();
    
}   // MiscProcess


/***************************************************************************
*    desc:  Update the sprites
****************************************************************************/
void CSpriteStrategyMgr::Update()
{
    for( auto iter : m_pStrategyVec )
        iter->Update();

}   // Update


/************************************************************************
*    desc:  Transform the sprite
************************************************************************/
void CSpriteStrategyMgr::Transform()
{
    for( auto iter : m_pStrategyVec )
        iter->Transform();

}   // Transform

void CSpriteStrategyMgr::Transform( const CObject2D & object )
{
    for( auto iter : m_pStrategyVec )
        iter->Transform( object );

}   // Transform


/***************************************************************************
*    desc:  Render the sprites
****************************************************************************/
void CSpriteStrategyMgr::Render( const CMatrix & matrix )
{
    for( auto iter : m_pStrategyVec )
        iter->Render( matrix );

}   // Render

void CSpriteStrategyMgr::Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix )
{
    for( auto iter : m_pStrategyVec )
        iter->Render( projMatrix, cameraMatrix );

}   // Render


/************************************************************************
*    desc:  Get the pointer to the strategy
************************************************************************/
iSpriteStrategy * CSpriteStrategyMgr::get( const std::string & strategyId )
{
    // Make sure the strategy we are looking for is available
    auto mapIter = m_pStrategyMap.find( strategyId );
    if( mapIter == m_pStrategyMap.end() )
        throw NExcept::CCriticalException("Sprite Strategy Manager Id Find Error!",
            boost::str( boost::format("Sprite Manager strategy Id can't be found (%s).\n\n%s\nLine: %s") 
                % strategyId % __FUNCTION__ % __LINE__ ));

    return mapIter->second;

}   // get
