
/************************************************************************
*    FILE NAME:       ispritestrategy.h
*
*    DESCRIPTION:     Strategy Interface Class
************************************************************************/

#ifndef __i_strategy_h__
#define __i_strategy_h__

// Game lib dependencies
#include <common/point.h>
#include <common/worldvalue.h>

// Standard lib dependencies
#include <string>

// Forward Declarations
class iSprite;
union SDL_Event;

class iStrategy
{
public:

    // Constructor
    iStrategy();

    // Destructor
    virtual ~iStrategy(){}

    // Load the data from file
    virtual void loadFromFile( const std::string & file ){};
    
    // Set to Destroy the sprite
    virtual void setToDestroy( int spriteIndex ){}
    
    // Set to create the sprite
    virtual void setToCreate( const std::string & name ){}
    
    // Set to create the sprite
    void setCameraId( const std::string & cameraId );
    
    // Create the sprite
    virtual iSprite * create(
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ){ return nullptr; }
    
    virtual iSprite * create(
        const std::string & group,
        const std::string & name,
        const CPoint<CWorldValue> & pos,
        const CPoint<float> & rot,
        const CPoint<float> & scale ){ return nullptr; }

    virtual iSprite * create(
        const std::string & name ){ return nullptr; }
    
    virtual iSprite * create(
        const std::string & group,
        const std::string & name ){ return nullptr; }
    
    // Do any pre-loop init
    virtual void init(){}
    
    // Do some cleanup
    virtual void cleanUp(){};
    
    // Load the data from file
    virtual void miscProcess(){}

    // Update the sprite
    virtual void update() = 0;

    // Transform the sprite
    virtual void transform() = 0;
    virtual void transform( const class CObject2D & object ){}

    // Render the sprites
    virtual void render() {}
    virtual void render( const class CMatrix & matrix ) {}
    virtual void render( const CMatrix & matrix, const CMatrix & rotMatrix ) {}
    
    // Clear the sprite Id counter
    static void clearSpriteCounter();
    
    // Customize sprite id generation by defining an offset
    void setIdOffset( int offset );
    
    // Customize sprite id generation by defining a direction
    void setIdDir( int dir );
    
protected:
    
    // Delete any sprites scheduled to die
    virtual void handleDelete(){}
    
protected:
    
    // ID Offset for this strategy 
    int m_idOffset;
    
    // ID Direction
    int m_idDir;
    
    // camera id
    std::string m_cameraId;
    
    // Sprite Id incrementor
    static int m_spriteInc;
};

#endif  // __i_sprite_strategy_h__
