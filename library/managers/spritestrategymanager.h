
/************************************************************************
*    FILE NAME:       spritestrategymanager.h
*
*    DESCRIPTION:     Sprite strategy manager singleton
************************************************************************/

#ifndef __sprite_strategy_manager_h__
#define __sprite_strategy_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>

// Standard lib dependencies
#include <string>
#include <unordered_map>
#include <vector>

class CSpriteStrategyMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CSpriteStrategyMgr & Instance()
    {
        static CSpriteStrategyMgr spriteStrategyMgr;
        return spriteStrategyMgr;
    }
    
    // Load strategy data from id
    void Load( const std::string & strategyId, class iSpriteStrategy * pSpriteStrategy );
    
    // create the sprite and provide a unique id number for each one
    const std::vector<int> & Create(
        const std::string & strategyId,
        const std::string & name,
        const int count,
        const CPoint<CWorldValue> & pos = CPoint<CWorldValue>(),
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );
    
    int Create(
        const std::string & strategyId,
        const std::string & name,
        const CPoint<CWorldValue> & pos = CPoint<CWorldValue>(),
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );

    // Delete all the sprites
    void Clear();
    
    // Do any pre-game loop init's
    void Init();
    
    // Do some cleanup
    void CleanUp();
    
    // Handle any misc processing before the real work is started.
    void MiscProcess();
    
    // Update the sprites
    void Update();

    // Transform the sprite
    void Transform();
    void Transform( const class CObject2D & object );

    // Render the sprites
    void Render( const class CMatrix & matrix );
    
    // Get a pointer to the strategy
    template <typename target>
    target & Get( const std::string & strategyId )
    {
        return *dynamic_cast<target *>(get( strategyId ));
    }

private:

    // Constructor
    CSpriteStrategyMgr();

    // Destructor
    virtual ~CSpriteStrategyMgr();
    
    // Get the pointer to the strategy
    iSpriteStrategy * get( const std::string & strategyId );

private:

    // Map of unique strategy pointers
    std::unordered_map<std::string, iSpriteStrategy *> m_pStrategyMap;
    
    // Sprite Id incrementor
    int m_SpriteInc;
    
    // Temporary vector to hold the return ids
    std::vector<int> m_incReturn;

};

#endif  // __sprite_strategy_manager_h__


