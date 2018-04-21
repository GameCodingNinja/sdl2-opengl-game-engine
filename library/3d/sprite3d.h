
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
    const CObjectData3D & GetObjectData() const;

    // Get the visual component
    CVisualComponent3D & GetVisualComponent();

    // Get the physics component
    CPhysicsComponent3D & GetPhysicsComponent();

    // Get the scripting component
    CScriptComponent & GetScriptComponent();
    
    // Init the physics
    void InitPhysics() override;

    // React to what the player is doing
    void HandleEvent( const SDL_Event & rEvent ) override;
    
    // Set the translation/rotation from Bullet Physics
    void SetTransform( const btTransform & trans );

    // Update the sprite 
    void Update() override;
    
    // Update the physics
    void PhysicsUpdate() override;

    // do the render
    void Render( const CCamera & camera ) override;
    void Render( const CMatrix & matrix, const CMatrix & rotMatrix ) override;

    // Set/Get the AI pointer
    void SetAI( iAIBase * pAIBase ) override;
    
    // Set the physics position and rotation
    void SetPhysicsTransform( float x, float y, float angle, bool resetVelocity = true ) override {}
    
    // Set/Get the color
    void SetColor( const CColor & color ) override;
    void SetColor( float r, float g, float b, float a ) override;
    void SetDefaultColor() override;
    const CColor & GetColor() const override;
    const CColor & GetDefaultColor() const override;

    // Set/Get the alpha
    void SetAlpha( float alpha ) override;
    float GetAlpha() const override;
    void SetDefaultAlpha() override;
    float GetDefaultAlpha() const override;

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


