
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
    iSprite(id),
    m_rObjectData(objectData),
    m_visualComponent(objectData.getVisualData()),
    m_physicsComponent(objectData.getPhysicsData())
{
    // If there's no visual data, set the hide flag
    setVisible( objectData.getVisualData().isActive() );
    
    // Set the sprite type
    m_parameters.Add( NDefs::SPRITE2D );
    
    if( objectData.getVisualData().getGenerationType() == NDefs::EGT_SPRITE_SHEET )
        setCropOffset( objectData.getVisualData().getSpriteSheet().getGlyph().getCropOffset() );
}


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSprite2D::~CSprite2D()
{
}


/************************************************************************
*    desc:  Load the sprite data
************************************************************************/
void CSprite2D::load( const XMLNode & node )
{
    // Load the transform data
    loadTransFromNode( node );

    // Init the script functions
    initScriptFunctions( node );
    
    // Load the font properties from XML node
    if( m_visualComponent.isFontSprite() )
        m_visualComponent.loadFontPropFromNode( node );
}

void CSprite2D::load( const CSpriteData & spriteData )
{
    // Copy over the transform
    copyTransform( &spriteData );
    
    // Copy over the script functions
    copyScriptFunctions( spriteData.getScriptFunctions() );
    
    // See if this sprite is used for rendering a font string
    if( m_visualComponent.isFontSprite() && (spriteData.getFontData() != nullptr) )
        m_visualComponent.setFontData( *spriteData.getFontData() );
}


/************************************************************************
*    desc:  Init the sprite
*           NOTE: Do not call from a constructor!
************************************************************************/
void CSprite2D::init()
{
    if( m_visualComponent.isFontSprite() )
        m_visualComponent.createFontString();
}


/************************************************************************
*    desc:  Clean up the sprite
*           NOTE: Do not call from a destructor!
************************************************************************/
void CSprite2D::cleanUp()
{
    // This is handled in the destructor but it doesn't hurt to handle it here as well.
    if( m_visualComponent.isFontSprite() )
        m_visualComponent.deleteFontVBO();
    
    // Deleting the physics always needs to be done externally and
    // under the right conditions
    m_physicsComponent.destroyBody();
}


/************************************************************************
*    desc:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CSprite2D::initScriptFunctions( const XMLNode & node )
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
}


/************************************************************************
*    desc:  Copy over the script functions
************************************************************************/
void CSprite2D::copyScriptFunctions( const std::map<std::string, std::string> & scriptFunctionMap )
{
    for( auto & iter : scriptFunctionMap )
        m_scriptFunctionMap.emplace( iter );
}


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
bool CSprite2D::prepareFuncId( const std::string & scriptFuncId, bool forceUpdate )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
    {
        m_scriptComponent.prepare( m_rObjectData.getGroup(), iter->second, {this});
        
        // Allow the script to execute and return it's context to the queue
        // for the scripts that don't animate
        if( forceUpdate )
            m_scriptComponent.update();
        
        return true;
    }

    return false;
}

void CSprite2D::prepare(
    const std::string & funcName,
    const std::vector<CScriptParam> & paramVec,
    bool forceUpdate )
{
    m_scriptComponent.prepare( m_rObjectData.getGroup(), funcName, paramVec);
    
    // Allow the script to execute and return it's context to the queue
    // for the scripts that don't animate
    if( forceUpdate )
        m_scriptComponent.update();
}


/************************************************************************
*    desc:  Init the physics                                                           
************************************************************************/
void CSprite2D::initPhysics()
{
    m_physicsComponent.init(*this);
}


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CSprite2D::handleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->handleEvent( rEvent );
}


/************************************************************************
*    desc:  Update the sprite                                                           
************************************************************************/
void CSprite2D::update()
{
    m_scriptComponent.update();
    
    if( m_upAI )
        m_upAI->update();
}


/************************************************************************
*    desc:  Update the physics                                                           
************************************************************************/
void CSprite2D::physicsUpdate()
{
    m_physicsComponent.update( this );
}


/************************************************************************
*    desc:  do the render                                                            
************************************************************************/
void CSprite2D::render( const CMatrix & matrix )
{
    if( isVisible() )
        m_visualComponent.render( m_matrix, matrix );
}

void CSprite2D::render( const CCamera & camera )
{
    if( isVisible() )
        m_visualComponent.render( m_matrix, camera.getFinalMatrix() );
}


/************************************************************************
*    desc:  Get the visual component                                                            
************************************************************************/
CVisualComponent2D & CSprite2D::getVisualComponent()
{
    return m_visualComponent;
}


/************************************************************************
*    desc:  Get the physics component                                                            
************************************************************************/
CPhysicsComponent2D & CSprite2D::getPhysicsComponent()
{
    return m_physicsComponent;
}


/************************************************************************
*    desc:  Get the scripting component                                                            
************************************************************************/
CScriptComponent & CSprite2D::getScriptComponent()
{
    return m_scriptComponent;
}


/************************************************************************
*    desc:  Get the object data                                                            
************************************************************************/
const CObjectData2D & CSprite2D::getObjectData() const
{
    return m_rObjectData;
}


/************************************************************************
*    desc:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite2D::setAI( iAIBase * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->init();
}


/************************************************************************
*    desc:  Set the color
************************************************************************/
void CSprite2D::setColor( const CColor & color )
{
    m_visualComponent.setColor( color );
}

void CSprite2D::setColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_visualComponent.setColor( r, g, b, a );
}


/************************************************************************
*    desc:  Set the default color
************************************************************************/
void CSprite2D::setDefaultColor()
{
    m_visualComponent.setColor( m_rObjectData.getVisualData().getColor() );
}


/************************************************************************
*    desc:  Get the color
************************************************************************/
const CColor & CSprite2D::getColor() const
{
    return m_visualComponent.getColor();
}


/************************************************************************
*    desc:  Get the default color
************************************************************************/
const CColor & CSprite2D::getDefaultColor() const
{
    return m_rObjectData.getVisualData().getColor();
}


/************************************************************************
*    desc:  Set the Alpha
************************************************************************/
void CSprite2D::setAlpha( float alpha )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;

    m_visualComponent.setAlpha( alpha );
}


/************************************************************************
*    desc:  Get the Alpha
************************************************************************/
float CSprite2D::getAlpha() const
{
    return m_visualComponent.getAlpha();
}


/************************************************************************
*    desc:  Get the default alpha
************************************************************************/
float CSprite2D::getDefaultAlpha() const
{
    return m_rObjectData.getVisualData().getColor().getA();
}


/************************************************************************
*    desc:  Set the default alpha
************************************************************************/
void CSprite2D::setDefaultAlpha()
{
    m_visualComponent.setAlpha( m_rObjectData.getVisualData().getColor().getA() );
}


/************************************************************************
*    desc:  Get the frame count
************************************************************************/
uint CSprite2D::getFrameCount() const 
{
    return m_rObjectData.getVisualData().getFrameCount();
}


/************************************************************************
*    desc:  Get the current frame
************************************************************************/
uint CSprite2D::getCurrentFrame() const 
{
    return m_visualComponent.getCurrentFrame();
}


/************************************************************************
*    desc:  Set the texture ID from index
************************************************************************/
void CSprite2D::setFrame( uint index )
{
    if( m_visualComponent.getCurrentFrame() != index )
    {
        m_visualComponent.setFrame( index );
        
        if( m_rObjectData.getVisualData().getGenerationType() == NDefs::EGT_SPRITE_SHEET )
            setCropOffset( m_rObjectData.getVisualData().getSpriteSheet().getGlyph(index).getCropOffset() );
    }
}


/************************************************************************
*    desc:  Create the font string
************************************************************************/
void CSprite2D::createFontString( const std::string & fontString )
{
    m_visualComponent.createFontString( fontString );
}


/************************************************************************
*    desc:  Get the font size
************************************************************************/
const CSize<float> & CSprite2D::getFontSize() const
{
    return m_visualComponent.getFontSize();
}


/************************************************************************
*    desc:  Set the physics position and rotation
************************************************************************/
void CSprite2D::setPhysicsTransform( float x, float y, float angle, bool resetVelocity )
{
    m_physicsComponent.setTransform( x, y, angle, resetVelocity );
}
