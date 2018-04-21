
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
    virtual void Init(){};
    
    // Clean up the sprite
    virtual void CleanUp(){};
    
    // Init the physics
    virtual void InitPhysics() = 0;
    
    // React to what the player is doing
    virtual void HandleEvent( const SDL_Event & rEvent ) = 0;

    // Update the sprite 
    virtual void Update() = 0;
    
    // Update the physics 
    virtual void PhysicsUpdate() = 0;

    // do the render
    virtual void Render( const CCamera & camera ){};
    virtual void Render( const CMatrix & matrix ){};
    virtual void Render( const CMatrix & matrix, const CMatrix & rotMatrix ){};
    
    // Get the unique id number
    int GetId() const;
    
    // Set/Get the AI pointer
    virtual void SetAI( iAIBase * pAIBase ) = 0;
    
    // Set/Get the color
    virtual void SetColor( const CColor & color ) = 0;
    virtual void SetColor( float r, float g, float b, float a ) = 0;
    virtual void SetDefaultColor() = 0;
    virtual const CColor & GetColor() const = 0;
    virtual const CColor & GetDefaultColor() const = 0;

    // Set/Get the alpha
    virtual void SetAlpha( float alpha ) = 0;
    virtual float GetAlpha() const = 0;
    virtual void SetDefaultAlpha() = 0;
    virtual float GetDefaultAlpha() const = 0;
    
    // Set the physics position and rotation
    virtual void SetPhysicsTransform( float x, float y, float angle, bool resetVelocity = true ) = 0;
    
    // Get the frame count
    virtual uint GetFrameCount() const { return 0; }

    // Set the texture ID from index
    virtual void SetFrame( uint index ) {}
    
    // Get the current frame
    virtual uint GetCurrentFrame() const { return 0; }
    
    // Create the font string
    virtual void CreateFontString( const std::string & fontString ) {}
    
    // Is this a 2D sprite?
    bool IsSprite2D() const;
    
    // Is this a 2D actor?
    bool IsActor2D() const;
    
    // Is this a 3D sprite?
    bool IsSprite3D() const;
    
    // Is this a 3D actor?
    bool IsActor3D() const;
    
protected:
    
    // Unique Id number
    int m_id;
};

#endif  // __i_sprite_2d_h__
