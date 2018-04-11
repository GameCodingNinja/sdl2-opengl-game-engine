
/************************************************************************
*    FILE NAME:       ispritestrategy.h
*
*    DESCRIPTION:     Sprite Strategy Interface Class - Represents a group of sprites
************************************************************************/

#ifndef __i_sprite_strategy_h__
#define __i_sprite_strategy_h__

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
class iSprite2D;

class iSpriteStrategy
{
public:

    // Constructor
    iSpriteStrategy(){};

    // Destructor
    virtual ~iSpriteStrategy(){};

    // Load the data from file
    virtual void LoadFromFile( const std::string & file ) = 0;
    
    // Set to Destroy the sprite
    virtual void SetToDestroy( int spriteIndex ){}
    
    // Set to create the sprite
    virtual void SetToCreate( const std::string & name ){}
    
    // Create the sprite
    virtual iSprite2D * Create(
        const std::string & name,
        const int id,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ){ return nullptr; }
    
    virtual iSprite2D * Create(
        const std::string & group,
        const std::string & name,
        const int id,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ){ return nullptr; }

    virtual iSprite2D * Create(
        const std::string & name,
        const int id ){ return nullptr; }
    
    virtual iSprite2D * Create(
        const std::string & group,
        const std::string & name,
        const int id ){ return nullptr; }
    
    // Do any pre-loop init
    virtual void Init(){}
    
    // Do some cleanup
    virtual void CleanUp(){};
    
    // Load the data from file
    virtual void MiscProcess(){}

    // Update the sprite
    virtual void Update() = 0;

    // Transform the sprite
    virtual void Transform() = 0;
    virtual void Transform( const class CObject2D & object ){}

    // Render the sprite
    virtual void Render( const class CMatrix & matrix ) {}
    virtual void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix ) {}
    
protected:
    
    // Delete any sprites scheduled to die
    virtual void HandleDelete(){}

};

#endif  // __i_sprite_strategy_h__
