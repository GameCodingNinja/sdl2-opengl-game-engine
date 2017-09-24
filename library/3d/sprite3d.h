
/************************************************************************
*    FILE NAME:       sprite3d.h
*
*    DESCRIPTION:     2D sprite class
************************************************************************/

#ifndef __sprite_3d_h__
#define __sprite_3d_h__

// Physical component dependency
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
class asIScriptEngine;

class CSprite3D : public CObject3D, boost::noncopyable
{
public:

    // Constructor
    CSprite3D( const CObjectData3D & objectData );

    // Destructor
    virtual ~CSprite3D();
    
    //////////////////////////////////////////////////////
    // Register CScriptComponent2d Object with AngelScript
    //////////////////////////////////////////////////////
    static void Register( asIScriptEngine * pEngine );

    // Get the object data
    const CObjectData3D & GetObjectData() const;

    // Get the visual component
    CVisualComponent3D & GetVisualComponent();

    // Get the physics component
    CPhysicsComponent3D & GetPhysicsComponent();

    // Get the scripting component
    CScriptComponent & GetScriptComponent();
    
    // Init the physics
    void InitPhysics();

    // Update the sprite 
    void Update();
    
    // Update the physics
    void PhysicsUpdate();

    // do the render
    void Render( const CMatrix & projMatrix, const CMatrix & cameraMatrix );
    
    //////////////////////////////////
    // Functions accessed via script
    //////////////////////////////////
    
    // Set/Get the color
    void SetColor( const CColor & color );
    void SetRGBA( float r, float g, float b, float a );
    void SetDefaultColor();
    const CColor & GetColor() const;
    const CColor & GetDefaultColor() const;

    // Set/Get the alpha
    void SetAlpha( float alpha );
    float GetAlpha() const;
    float GetDefaultAlpha() const;

private:

    // The object data
    const CObjectData3D & m_objectData;
    
    // The script part of the 2d sprite
    CScriptComponent m_scriptComponent;

    // The visual part of the 2d sprite
    CVisualComponent3D m_visualComponent;

    // The physics part of the 2d sprite
    CPhysicsComponent3D m_physicsComponent;

};

#endif  // __sprite_3d_h__

