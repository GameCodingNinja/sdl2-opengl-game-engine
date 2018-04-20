
/************************************************************************
*    FILE NAME:       sprite2d.cpp
*
*    DESCRIPTION:     2D sprite class
************************************************************************/

// Physical component dependency
#include <2d/sprite2d.h>

// Game lib dependencies
#include <objectdata/objectdata2d.h>
#include <objectdata/objectphysicsdata2d.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>
#include <common/spritedata.h>
#include <common/iaibase.h>
#include <common/camera.h>
#include <utilities/xmlParser.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSprite2D::CSprite2D( const CObjectData2D & objectData, int id ) :
    m_rObjectData( objectData ),
    m_visualComponent( objectData.GetVisualData() ),
    m_physicsComponent( objectData.GetPhysicsData() ),
    m_id(id)
{
    // If there's no visual data, set the hide flag
    SetVisible( objectData.GetVisualData().IsActive() );
    
    // Set the sprite type
    m_parameters.Add( NDefs::SPRITE2D );
    
    if( objectData.GetVisualData().GetGenerationType() == NDefs::EGT_SPRITE_SHEET )
        SetCropOffset( objectData.GetVisualData().GetSpriteSheet().GetGlyph().GetCropOffset() );

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSprite2D::~CSprite2D()
{
}   // destructor


/************************************************************************
*    desc:  Load the sprite data
************************************************************************/
void CSprite2D::Load( const XMLNode & node )
{
    // Load the transform data
    LoadTransFromNode( node );

    // Init the script functions
    InitScriptFunctions( node );
    
    // Load the font properties from XML node
    if( m_visualComponent.IsFontSprite() )
        m_visualComponent.LoadFontPropFromNode( node );
    
}   // Load

void CSprite2D::Load( const CSpriteData & spriteData )
{
    // Copy over the transform
    CopyTransform( &spriteData );
    
    // Copy over the script functions
    CopyScriptFunctions( spriteData.GetScriptFunctions() );
    
    // See if this sprite is used for rendering a font string
    if( m_visualComponent.IsFontSprite() && (spriteData.GetFontData() != nullptr) )
        m_visualComponent.SetFontData( *spriteData.GetFontData() );
    
}   // Load


/************************************************************************
*    desc:  Init the sprite
*           NOTE: Do not call from a constructor!
************************************************************************/
void CSprite2D::Init()
{
    if( m_visualComponent.IsFontSprite() )
        m_visualComponent.CreateFontString();
    
} // Init


/************************************************************************
*    desc:  Clean up the sprite
*           NOTE: Do not call from a destructor!
************************************************************************/
void CSprite2D::CleanUp()
{
    // This is handled in the destructor but it doesn't hurt to handle it here as well.
    if( m_visualComponent.IsFontSprite() )
        m_visualComponent.DeleteFontVBO();
    
    // Deleting the physics always needs to be done externally and
    // under the right conditions
    m_physicsComponent.DestroyBody();
    
} // CleanUp


/************************************************************************
*    desc:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CSprite2D::InitScriptFunctions( const XMLNode & node )
{
    // Check for scripting - Add an empty string for scripts not defined
    XMLNode scriptLstNode = node.getChildNode( "scriptLst" );
    if( !scriptLstNode.isEmpty() )
    {
        for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
        {
            const XMLNode scriptNode = scriptLstNode.getChildNode(i);
            
            // Only the first attribute is used
            const XMLAttribute attribute = scriptNode.getAttribute(0);
            const std::string attrName = attribute.lpszName;
            const std::string attrValue = attribute.lpszValue;

            // Add the attribute name and value to the map
            if( !attrValue.empty() )
                m_scriptFunctionMap.emplace( attrName, attrValue );
        }
    }
    
}   // InitScriptFunctions

/************************************************************************
*    desc:  Copy over the script functions
************************************************************************/
void CSprite2D::CopyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap )
{
    for( auto & iter : scriptFunctionMap )
        m_scriptFunctionMap.emplace( iter );
    
}   // CopyScriptFunctions


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
bool CSprite2D::PrepareFuncId( const std::string & scriptFuncId, bool forceUpdate )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.Prepare( m_rObjectData.GetGroup(), iter->second, {this});
        
        // Allow the script to execute and return it's context to the queue
        // for the scripts that don't animate
        if( forceUpdate )
            m_scriptComponent.Update();
        
        return true;
    }

    return false;
    
}   // Prepare

void CSprite2D::Prepare(
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec,
    bool forceUpdate )
{
    m_scriptComponent.Prepare( m_rObjectData.GetGroup(), funcName, paramVec);
    
    // Allow the script to execute and return it's context to the queue
    // for the scripts that don't animate
    if( forceUpdate )
        m_scriptComponent.Update();
    
}   // Prepare


/************************************************************************
*    desc:  Init the physics                                                           
************************************************************************/
void CSprite2D::InitPhysics()
{
    m_physicsComponent.Init(*this);
}


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CSprite2D::HandleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->HandleEvent( rEvent );

}   // HandleEvent


/************************************************************************
*    desc:  Update the sprite                                                           
************************************************************************/
void CSprite2D::Update()
{
    m_scriptComponent.Update();
    
    if( m_upAI )
        m_upAI->Update();

}   // Update


/************************************************************************
*    desc:  Update the physics                                                           
************************************************************************/
void CSprite2D::PhysicsUpdate()
{
    m_physicsComponent.Update( this );

}   // PhysicsUpdate


/************************************************************************
*    desc:  do the render                                                            
************************************************************************/
void CSprite2D::Render( const CMatrix & matrix )
{
    if( IsVisible() )
        m_visualComponent.Render( m_matrix, matrix );

}   // Render

void CSprite2D::Render( const CCamera & camera )
{
    if( IsVisible() )
        m_visualComponent.Render( m_matrix, camera.GetFinalMatrix() );

}   // Render


/************************************************************************
*    desc:  Get the visual component                                                            
************************************************************************/
CVisualComponent2D & CSprite2D::GetVisualComponent()
{
    return m_visualComponent;

}   // GetVisualComponent


/************************************************************************
*    desc:  Get the physics component                                                            
************************************************************************/
CPhysicsComponent2D & CSprite2D::GetPhysicsComponent()
{
    return m_physicsComponent;

}   // GetPhysicsComponent


/************************************************************************
*    desc:  Get the scripting component                                                            
************************************************************************/
CScriptComponent & CSprite2D::GetScriptComponent()
{
    return m_scriptComponent;

}   // GetScriptingComponent


/************************************************************************
*    desc:  Get the object data                                                            
************************************************************************/
const CObjectData2D & CSprite2D::GetObjectData() const
{
    return m_rObjectData;

}   // GetObjectData


/************************************************************************
*    desc:  Get the unique id number
************************************************************************/
int CSprite2D::GetId() const
{
    return m_id;

}   // GetId


/************************************************************************
*    desc:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite2D::SetAI( iAIBase * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->Init();

}   // SetAI


/************************************************************************
*    desc:  Set the color
************************************************************************/
void CSprite2D::SetColor( const CColor & color )
{
    m_visualComponent.SetColor( color );

}   // SetColor

void CSprite2D::SetColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_visualComponent.SetColor( r, g, b, a );

}   // SetColor


/************************************************************************
*    desc:  Set the default color
************************************************************************/
void CSprite2D::SetDefaultColor()
{
    m_visualComponent.SetColor( m_rObjectData.GetVisualData().GetColor() );

}   // SetColor


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CSprite2D::GetColor() const
{
    return m_visualComponent.GetColor();

}   // GetColor


/************************************************************************
*    desc:  Get the default color
************************************************************************/
const CColor & CSprite2D::GetDefaultColor() const
{
    return m_rObjectData.GetVisualData().GetColor();

}   // GetDefaultColor


/************************************************************************
*    desc:  Set the Alpha
************************************************************************/
void CSprite2D::SetAlpha( float alpha )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;

    m_visualComponent.SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
*    desc:  Get the Alpha
************************************************************************/
float CSprite2D::GetAlpha() const
{
    return m_visualComponent.GetAlpha();

}   // GetAlpha


/************************************************************************
*    desc:  Get the default alpha
************************************************************************/
float CSprite2D::GetDefaultAlpha() const
{
    return m_rObjectData.GetVisualData().GetColor().GetA();

}   // GetDefaultAlpha


/************************************************************************
*    desc:  Set the default alpha
************************************************************************/
void CSprite2D::SetDefaultAlpha()
{
    m_visualComponent.SetAlpha( m_rObjectData.GetVisualData().GetColor().GetA() );

}   // SetDefaultAlpha


/************************************************************************
*    desc:  Get the frame count
************************************************************************/
uint CSprite2D::GetFrameCount() const 
{
    return m_rObjectData.GetVisualData().GetFrameCount();

}   // GetFrameCount


/************************************************************************
*    desc:  Get the current frame
************************************************************************/
uint CSprite2D::GetCurrentFrame() const 
{
    return m_visualComponent.GetCurrentFrame();

}   // GetFrameCount


/************************************************************************
*    desc:  Set the texture ID from index
************************************************************************/
void CSprite2D::SetFrame( uint index )
{
    if( m_visualComponent.GetCurrentFrame() != index )
    {
        m_visualComponent.SetFrame( index );
        
        if( m_rObjectData.GetVisualData().GetGenerationType() == NDefs::EGT_SPRITE_SHEET )
            SetCropOffset( m_rObjectData.GetVisualData().GetSpriteSheet().GetGlyph(index).GetCropOffset() );
    }

}   // SetFrame


/************************************************************************
*    desc:  Create the font string
************************************************************************/
void CSprite2D::CreateFontString( const std::string & fontString )
{
    m_visualComponent.CreateFontString( fontString );

}   // CreateFontString


/************************************************************************
*    desc:  Get the font size
************************************************************************/
const CSize<float> & CSprite2D::GetFontSize() const
{
    return m_visualComponent.GetFontSize();

}   // CreateFontString


/************************************************************************
*    desc:  Set the physics position and rotation
************************************************************************/
void CSprite2D::SetPhysicsTransform( float x, float y, float angle, bool resetVelocity )
{
    m_physicsComponent.SetTransform( x, y, angle, resetVelocity );
    
}   // SetPhysicsTransform