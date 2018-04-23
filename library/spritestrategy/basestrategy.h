
/************************************************************************
*    FILE NAME:       basespritestrategy.h
*
*    DESCRIPTION:     Base sprite strategy class
************************************************************************/

#ifndef __base_strategy_h__
#define __base_strategy_h__

// Physical component dependency
#include <spritestrategy/ispritestrategy.h>

// Standard lib dependencies
#include <set>
#include <string>

class CBaseStrategy : public iSpriteStrategy
{
public:

    // Constructor
    CBaseStrategy( const std::string & cameraId, int idOffset, int idDir );
    CBaseStrategy( int idOffset, int idDir );

    // Destructor
    virtual ~CBaseStrategy();
    
    // Set to Destroy the sprite
    void SetToDestroy( int spriteIndex ) override;
    
    // Set to create the sprite
    void SetToCreate( const std::string & name ) override;
    
    // Load the data from file
    virtual void MiscProcess() override;
    
protected:
    
    // Handle the deleting of any object by Id
    virtual void HandleDelete() override;
    
    // Handle the deleting of any object by Id
    virtual void DeleteObj( int index );
    
    // Handle the creating of new sprite objects by name
    virtual void HandleCreate();
    
    // Handle the creating of any object by name
    virtual void CreateObj( const std::string & name );
    
protected:
    
    // ID Offset for this strategy 
    int m_idOffset;
    
    // ID Direction
    int m_idDir;
    
private:
    
    // Set of indexes to delete
    // Using a set insures only unique entries are inserted
    std::set<int> m_deleteSet;
    
    // Set of sprites to create
    // Using a set insures only unique entries are inserted
    std::set<std::string> m_createSet;
    
};

#endif  // __base_strategy_h__