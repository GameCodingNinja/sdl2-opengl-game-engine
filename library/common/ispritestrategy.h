
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

class iSpriteStrategy
{
public:

    // Constructor
    iSpriteStrategy(){};

    // Destructor
    virtual ~iSpriteStrategy(){};

    // Load the data from file
    virtual void LoadFromFile( const std::string & file ) = 0;

    // Allow player sprite to respond to input
    virtual void HandleEvent( const union SDL_Event & rEvent ){}
    
    // Handle messages
    virtual void HandleMessage( int msg, int code ){}
    virtual void HandleMessage( int msg, const std::string & code ){}
    
    // Create the sprite
    virtual void Create(
        const std::string & name,
        const int id,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ){}
    
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
    virtual void Render( const class CMatrix & matrix ) = 0;
    
protected:
    
    // Delete any sprites scheduled to die
    virtual void HandleDelete(){}

};

#endif  // __i_sprite_strategy_h__