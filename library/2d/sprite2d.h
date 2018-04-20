
/************************************************************************
*    FILE NAME:       sprite2d.h
*
*    DESCRIPTION:     2D sprite class
************************************************************************/

#ifndef __sprite_2d_h__
#define __sprite_2d_h__

// Physical component dependency
#include <common/isprite.h>

// Game lib dependencies
#include <2d/visualcomponent2d.h>
#include <physics/physicscomponent2d.h>
#include <script/scriptcomponent.h>

// Boost lib dependencies
#include <boost/noncopyable.hpp>

// Standard lib dependencies
#include <string>
#include <map>
#include <memory>

// Forward declaration(s)
class CObjectData2D;
class CColor;
class CSpriteData;
class iAIBase;
struct XMLNode;

class CSprite2D : public iSprite, boost::noncopyable
{
public:

    // Constructor
    CSprite2D( const CObjectData2D & objectData, int id = defs_SPRITE_DEFAULT_ID );

    // Destructor
    virtual ~CSprite2D();
    
    // Init the script functions
    void InitScriptFunctions( const XMLNode & node );
    
    // Prepare the script function to run
    bool PrepareFuncId( const std::string & scriptFuncId, bool forceUpdate = false );
    void Prepare(
        const std::string & funcName,
        const std::vector<CScriptParam> & paramVec = std::vector<CScriptParam>(),
        bool forceUpdate = false );
    
    // Load the sprite data
    void Load( const XMLNode & node );
    void Load( const CSpriteData & spriteData );
    
    // Init the sprite
    void Init() override;
    
    // Clean up the sprite
    void CleanUp() override;

    // Get the object data
    const CObjectData2D & GetObjectData() const;

    // Get the visual component
    CVisualComponent2D & GetVisualComponent();

    // Get the physics component
    CPhysicsComponent2D & GetPhysicsComponent();

    // Get the scripting component
    CScriptComponent & GetScriptComponent();
    
    // Init the physics
    void InitPhysics() override;
    
    // React to what the player is doing
    void HandleEvent( const SDL_Event & rEvent ) override;

    // Update the sprite 
    void Update() override;
    
    // Update the physics 
    void PhysicsUpdate() override;

    // do the render
    void Render( const CCamera & camera ) override;
    void Render( const CMatrix & matrix ) override;
    
    // Get the unique id number
    int GetId() const override;
    
    // Set/Get the AI pointer
    void SetAI( iAIBase * pAIBase ) override;
    
    // Get the font size
    const CSize<float> & GetFontSize() const;
    
    // Copy over the script functions
    void CopyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap );
    
    // Set the physics position and rotation
    void SetPhysicsTransform( float x, float y, float angle, bool resetVelocity = true );
    
    // Set/Get the color
    void SetColor( const CColor & color );
    void SetColor( float r, float g, float b, float a );
    void SetDefaultColor();
    const CColor & GetColor() const;
    const CColor & GetDefaultColor() const;

    // Set/Get the alpha
    void SetAlpha( float alpha );
    float GetAlpha() const;
    void SetDefaultAlpha();
    float GetDefaultAlpha() const;

    // Get the frame count
    uint GetFrameCount() const;

    // Set the texture ID from index
    void SetFrame( uint index );
    
    // Get the current frame
    uint GetCurrentFrame() const;
    
    // Create the font string
    void CreateFontString( const std::string & fontString );

protected:

    // The object data
    const CObjectData2D & m_rObjectData;
    
    // The visual part of the 2d sprite
    CVisualComponent2D m_visualComponent;
    
    // The script part of the 2d sprite
    CScriptComponent m_scriptComponent;

    // The physics part of the 2d sprite
    CPhysicsComponent2D m_physicsComponent;
    
    // Unique Id number
    int m_id;
    
    // Base AI scoped pointer
    std::unique_ptr<iAIBase> m_upAI;
    
    // Script function map. Tie events to script functions
    std::map<const std::string, std::string> m_scriptFunctionMap;

};

#endif  // __sprite_2d_h__


