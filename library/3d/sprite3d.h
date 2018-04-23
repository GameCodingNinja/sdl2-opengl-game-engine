
/************************************************************************
*    FILE NAME:       sprite3d.h
*
*    DESCRIPTION:     2D sprite class
************************************************************************/

#ifndef __sprite_3d_h__
#define __sprite_3d_h__

// Physical component dependency
#include <common/isprite.h>
#include <3d/object3d.h>

// Game lib dependencies
#include <3d/visualcomponent3d.h>
#include <physics/physicscomponent3d.h>
#include <script/scriptcomponent.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Forward declaration(s)
class CObjectData3D;
class CColor;

class CSprite3D : public iSprite, public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CSprite3D( const CObjectData3D & objectData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSprite3D();

    // Get the object data
    const CObjectData3D & getObjectData() const;

    // Get the visual component
    CVisualComponent3D & getVisualComponent();

    // Get the physics component
    CPhysicsComponent3D & getPhysicsComponent();

    // Get the scripting component
    CScriptComponent & getScriptComponent();
    
    // Init the physics
    void initPhysics() override;

    // React to what the player is doing
    void handleEvent( const SDL_Event & rEvent ) override;
    
    // Set the translation/rotation from Bullet Physics
    void setTransform( const btTransform & trans );

    // Update the sprite 
    void update() override;
    
    // Update the physics
    void physicsUpdate() override;

    // do the render
    void render( const CCamera & camera ) override;
    void render( const CMatrix & matrix, const CMatrix & rotMatrix ) override;

    // Set/Get the AI pointer
    void setAI( iAIBase * pAIBase ) override;
    
    // Set the physics position and rotation
    void setPhysicsTransform( float x, float y, float angle, bool resetVelocity = true ) override {}
    
    // Set/Get the color
    void setColor( const CColor & color ) override;
    void setColor( float r, float g, float b, float a ) override;
    void setDefaultColor() override;
    const CColor & getColor() const override;
    const CColor & getDefaultColor() const override;

    // Set/Get the alpha
    void setAlpha( float alpha ) override;
    float getAlpha() const override;
    void setDefaultAlpha() override;
    float getDefaultAlpha() const override;

private:

    // The object data
    const CObjectData3D & m_objectData;
    
    // The script part of the 2d sprite
    CScriptComponent m_scriptComponent;

    // The visual part of the 2d sprite
    CVisualComponent3D m_visualComponent;

    // The physics part of the 2d sprite
    CPhysicsComponent3D m_physicsComponent;

    // Base AI scoped pointer
    std::unique_ptr<iAIBase> m_upAI;

};

#endif  // __sprite_3d_h__


