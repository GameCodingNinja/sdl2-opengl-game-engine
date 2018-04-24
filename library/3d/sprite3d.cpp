
/************************************************************************
 *    FILE NAME:       sprite3d.cpp
 *
 *    DESCRIPTION:     3D sprite class
 ************************************************************************/

// Physical component dependency
#include <3d/sprite3d.h>

// Game lib dependencies
#include <objectdata/objectdata3d.h>
#include <objectdata/objectphysicsdata3d.h>
#include <utilities/exceptionhandling.h>
#include <common/color.h>
#include <common/camera.h>
#include <common/iaibase.h>

// Bullet Physics lib dependencies
#include <btBulletCollisionCommon.h>

/************************************************************************
 *    desc:  Constructor
 ************************************************************************/
CSprite3D::CSprite3D( const CObjectData3D & objectData, int id ) :
    iSprite(id),
    m_objectData(objectData),
    m_visualComponent(objectData.getVisualData()),
    m_physicsComponent(objectData.getPhysicsData())
{
    // If there's no visual data, set the hide flag
    setVisible( objectData.getVisualData().isActive() );
    
    // Set the sprite type
    m_parameters.Add( NDefs::SPRITE3D );
}


/************************************************************************
 *    desc:  destructor                                                             
 ************************************************************************/
CSprite3D::~CSprite3D()
{
}


/************************************************************************
 *    desc:  Init the physics                                                           
 ************************************************************************/
void CSprite3D::initPhysics()
{
    m_physicsComponent.init( *this );
}


/************************************************************************
*    desc:  Set the translation/rotation from Bullet Physics
************************************************************************/
void CSprite3D::setTransform( const btTransform & trans )
{
    m_parameters.Add( NDefs::ROTATE | NDefs::PHYSICS_TRANSFORM );
    
    // Set the position
    const btVector3 & btVec = trans.getOrigin();
    setPos( btVec.x(), btVec.y(), btVec.z() );
    
    // Set the rotation
    const btMatrix3x3 & btMat = trans.getBasis();
    for( int i = 0; i < 3; ++i )
    {
        const btVector3 & vec = btMat.getRow(i);
        m_rotMatrix.SetColumn( i, vec.x(), vec.y(), vec.z() );
    }
    
    // This is an example of how to get the rotation out of bullet physics
    // but it's a lot of extra work to only do the rotation calculation all over again.
    
    // Get the rotation
    //const btMatrix3x3 & btMat = trans.getBasis();
    //btScalar z, y, x;
    //btMat.getEulerYPR( z, y, x );
    
    // Set the rotation
    //SetRotXYZ( x, y, z, false );
}


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CSprite3D::handleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->handleEvent( rEvent );
}


/************************************************************************
 *    desc:  Update the sprite                                                           
 ************************************************************************/
void CSprite3D::update()
{
    if( isVisible() )
        m_physicsComponent.update( this );

    m_scriptComponent.Update();

    if( m_upAI )
        m_upAI->update();
}


/************************************************************************
*    desc:  Update the physics
************************************************************************/
void CSprite3D::physicsUpdate()
{
    m_physicsComponent.update( this );
}


/************************************************************************
 *    desc:  do the render                                                            
 ************************************************************************/
void CSprite3D::render( const CMatrix & matrix, const CMatrix & rotMatrix )
{
    if( isVisible() )
        m_visualComponent.render( m_matrix * matrix, m_rotMatrix * rotMatrix );
}

void CSprite3D::render( const CCamera & camera )
{
    if( isVisible() )
        m_visualComponent.render( m_matrix * camera.getFinalMatrix(), m_rotMatrix * camera.getRotMatrix() );
}


/************************************************************************
 *    desc:  Get the visual component                                                            
 ************************************************************************/
CVisualComponent3D & CSprite3D::getVisualComponent()
{
    return m_visualComponent;
}


/************************************************************************
 *    desc:  Get the physics component                                                            
 ************************************************************************/
CPhysicsComponent3D & CSprite3D::getPhysicsComponent()
{
    return m_physicsComponent;
}


/************************************************************************
 *    desc:  Get the scripting component                                                            
 ************************************************************************/
CScriptComponent & CSprite3D::getScriptComponent()
{
    return m_scriptComponent;
}


/************************************************************************
 *    desc:  Get the object data                                                            
 ************************************************************************/
const CObjectData3D & CSprite3D::getObjectData() const
{
    return m_objectData;
}


/************************************************************************
*    desc:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite3D::setAI( iAIBase * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->init();
}


/************************************************************************
 *    desc:  Set the color
 ************************************************************************/
void CSprite3D::setColor( const CColor & color )
{
    m_visualComponent.setColor( color );
}

void CSprite3D::setColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_visualComponent.setColor( r, g, b, a );
}


/************************************************************************
 *    desc:  Set the default color
 ************************************************************************/
void CSprite3D::setDefaultColor()
{
    m_visualComponent.setColor( m_objectData.getVisualData().getColor() );
}


/************************************************************************
 *    desc:  Get the color
 ************************************************************************/
const CColor & CSprite3D::getColor() const
{
    return m_visualComponent.getColor();
}


/************************************************************************
 *    desc:  Get the default color
 ************************************************************************/
const CColor & CSprite3D::getDefaultColor() const
{
    return m_objectData.getVisualData().getColor();
}


/************************************************************************
 *    desc:  Set the Alpha
 ************************************************************************/
void CSprite3D::setAlpha( float alpha )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;

    m_visualComponent.setAlpha( alpha );
}


/************************************************************************
 *    desc:  Get the Alpha
 ************************************************************************/
float CSprite3D::getAlpha() const
{
    return m_visualComponent.getAlpha();
}


/************************************************************************
 *    desc:  Get the default alpha
 ************************************************************************/
float CSprite3D::getDefaultAlpha() const
{
    return m_objectData.getVisualData().getColor().getA();
}


/************************************************************************
*    desc:  Set the default alpha
************************************************************************/
void CSprite3D::setDefaultAlpha()
{
    m_visualComponent.setAlpha( m_objectData.getVisualData().getColor().getA() );
}
