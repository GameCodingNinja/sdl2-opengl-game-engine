
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

// Forward Declarations
class iSprite;

class CSpriteStrategyMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CSpriteStrategyMgr & Instance()
    {
        static CSpriteStrategyMgr spriteStrategyMgr;
        return spriteStrategyMgr;
    }
    
    // Add strategy
    void AddStrategy( const std::string & strategyId, class iSpriteStrategy * pSpriteStrategy );
    
    // Delete strategy
    void DeleteStrategy( const std::string & strategyId );
    
    // Delete sprite
    void DeleteSprite( const std::string & strategyId, int spriteId );
    
    // create the sprite and provide a unique id number for each one
    iSprite * Create(
        const std::string & strategyId,
        const std::string & dataName,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );
    
    iSprite * Create(
        const std::string & strategyId,
        const std::string & group,
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) );
    
    iSprite * Create(
        const std::string & strategyId,
        const std::string & dataName );
    
    iSprite * Create(
        const std::string & strategyId,
        const std::string & group,
        const std::string & name );

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
    void Render();
    void Render( const class CMatrix & matrix );
    void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix );
    
    // Process all states
    void ProcessAllStates();
    
    // Get a reference to the strategy
    template <typename target>
    target & Get( const std::string & strategyId )
    {
        return *dynamic_cast<target *>(get( strategyId ));
    }
    
    // Get a pointer to the strategy based on if the sprite can be found
    template <typename target>
    target & Find( iSprite * piSprite )
    {
        target * pStrategy = nullptr;
        
        for( auto iter : m_pStrategyVec )
        {
            pStrategy = dynamic_cast<target *>(iter);
            
            if( (pStrategy != nullptr) && pStrategy->Find(piSprite) )
                return *pStrategy;
            
            pStrategy = nullptr;
        }
        
        return *pStrategy;
    }
    
    // Get a pointer to the strategy based on if the sprite can be found
    template <typename target>
    target & Find( const std::string & strategyId )
    {
        target * pStrategy = nullptr;
        
        for( auto & iter : m_pStrategyMap )
        {
            pStrategy = dynamic_cast<target *>(iter.second);
            
            if( (pStrategy != nullptr) && (iter.first.find(strategyId) != std::string::npos) )
                return *pStrategy;
            
            pStrategy = nullptr;
        }
        
        return *pStrategy;
    }
    
    // Create templates
    template <typename target>
    target * CreateSprite(
        const std::string & strategyId,
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) )
    {
        return dynamic_cast<target *>(Create( strategyId, name, pos, rot, scale ));
    }
    
    template <typename target>
    target * CreateSprite(
        const std::string & strategyId,
        const std::string & group,
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot = CPoint<float>(),
        const CPoint<float> & scale = CPoint<float>(1,1,1) )
    {
        return dynamic_cast<target *>(Create( strategyId, group, name, pos, rot, scale ));
    }
    
    template <typename target>
    target * CreateSprite(
        const std::string & strategyId,
        const std::string & name )
    {
        return dynamic_cast<target *>(Create( strategyId, name ));
    }
    
    template <typename target>
    target * CreateSprite(
        const std::string & strategyId,
        const std::string & group,
        const std::string & name )
    {
        return dynamic_cast<target *>(Create( strategyId, group, name ));
    }

private:

    // Constructor
    CSpriteStrategyMgr();

    // Destructor
    virtual ~CSpriteStrategyMgr();
    
    // Get the pointer to the strategy
    iSpriteStrategy * get( const std::string & strategyId );

private:

    // Map of strategy pointers
    std::map<std::string, iSpriteStrategy *> m_pStrategyMap;
    
    // Vector of strategy pointers
    std::vector<iSpriteStrategy *> m_pStrategyVec;
    
    // Sprite Id incrementor
    int m_spriteInc;
    
    // Temporary vector to hold the return ids
    std::vector<iSprite *> m_incReturn;

};

#endif  // __sprite_strategy_manager_h__


