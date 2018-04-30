
/************************************************************************
*    FILE NAME:       isprite.h
*
*    DESCRIPTION:     iSprite Class
************************************************************************/

#ifndef __i_sprite_h__
#define __i_sprite_h__

// Physical component dependency
#include <2d/object2d.h>
#include <common/color.h>

// Forward declaration(s)
union SDL_Event;
class iAIBase;
class CCamera;

class iSprite : public virtual CObject2D
{
public:
    
    // Constructor
    iSprite( int id );
    
    // Init the sprite
    virtual void init(){};
    
    // Clean up the sprite
    virtual void cleanUp(){};
    
    // Init the physics
    virtual void initPhysics() = 0;
    
    // React to what the player is doing
    virtual void handleEvent( const SDL_Event & rEvent ) = 0;

    // Update the sprite 
    virtual void update() = 0;
    
    // Update the physics 
    virtual void physicsUpdate() = 0;

    // do the render
    virtual void render( const CCamera & camera ){};
    virtual void render( const CMatrix & matrix ){};
    virtual void render( const CMatrix & matrix, const CMatrix & rotMatrix ){};
    
    // Get the unique id number
    int getId() const;
    
    // Set/Get the AI pointer
    virtual void setAI( iAIBase * pAIBase ) = 0;
    
    // Set/Get the color
    virtual void setColor( const CColor & color ) = 0;
    virtual void setColor( float r, float g, float b, float a ) = 0;
    virtual void setDefaultColor() = 0;
    virtual const CColor & getColor() const = 0;
    virtual const CColor & getDefaultColor() const = 0;

    // Set/Get the alpha
    virtual void setAlpha( float alpha ) = 0;
    virtual float getAlpha() const = 0;
    virtual void setDefaultAlpha() = 0;
    virtual float getDefaultAlpha() const = 0;
    
    // Set the physics position and rotation
    virtual void setPhysicsTransform( float x, float y, float angle, bool resetVelocity = true ) = 0;
    
    // Get the frame count
    virtual uint getFrameCount() const { return 0; }

    // Set the texture ID from index
    virtual void setFrame( uint index ) {}
    
    // Get the current frame
    virtual uint getCurrentFrame() const { return 0; }
    
    // Create the font string
    virtual void createFontString( const std::string & fontString ) {}

protected:
    
    // Unique Id number
    int m_id;
};

#endif  // __i_sprite_2d_h__
