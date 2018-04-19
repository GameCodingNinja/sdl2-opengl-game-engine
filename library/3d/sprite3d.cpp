
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
#include <common/iaibase.h>

// Bullet Physics lib dependencies
#include <btBulletCollisionCommon.h>

/************************************************************************
 *    desc:  Constructor
 ************************************************************************/
CSprite3D::CSprite3D( const CObjectData3D & objectData ) :
    m_objectData( objectData ),
    m_visualComponent( objectData.GetVisualData() ),
    m_physicsComponent( objectData.GetPhysicsData() )
{
    // If there's no visual data, set the hide flag
    SetVisible( objectData.GetVisualData().IsActive() );

}   // constructor


/************************************************************************
 *    desc:  destructor                                                             
 ************************************************************************/
CSprite3D::~CSprite3D()
{
}   // destructor


/************************************************************************
 *    desc:  Init the physics                                                           
 ************************************************************************/
void CSprite3D::InitPhysics()
{
    m_physicsComponent.Init( *this );
}


/************************************************************************
*    desc:  Set the translation/rotation from Bullet Physics
************************************************************************/
void CSprite3D::SetTransform( const btTransform & trans )
{
    m_parameters.Add( NDefs::ROTATE | NDefs::PHYSICS_TRANSFORM );
    
    // Set the position
    const btVector3 & btVec = trans.getOrigin();
    SetPos( btVec.x(), btVec.y(), btVec.z() );
    
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

}   // SetTransform


/************************************************************************
*    desc:  React to what the player is doing
************************************************************************/
void CSprite3D::HandleEvent( const SDL_Event & rEvent )
{
    if( m_upAI )
        m_upAI->HandleEvent( rEvent );

}   // HandleEvent


/************************************************************************
 *    desc:  Update the sprite                                                           
 ************************************************************************/
void CSprite3D::Update()
{
    if( IsVisible() )
        m_physicsComponent.Update( this );

    m_scriptComponent.Update();

    if( m_upAI )
        m_upAI->Update();

}   // Update


/************************************************************************
*    desc:  Update the physics
************************************************************************/
void CSprite3D::PhysicsUpdate()
{
    m_physicsComponent.Update( this );

}   // PhysicsUpdate


/************************************************************************
 *    desc:  do the render                                                            
 ************************************************************************/
void CSprite3D::Render( const CMatrix & projMatrix, const CMatrix & normalMatrix )
{
    if( IsVisible() )
        m_visualComponent.Render( m_matrix * projMatrix, m_rotMatrix * normalMatrix );

}   // Render


/************************************************************************
 *    desc:  Get the visual component                                                            
 ************************************************************************/
CVisualComponent3D & CSprite3D::GetVisualComponent()
{
    return m_visualComponent;

}   // GetVisualComponent


/************************************************************************
 *    desc:  Get the physics component                                                            
 ************************************************************************/
CPhysicsComponent3D & CSprite3D::GetPhysicsComponent()
{
    return m_physicsComponent;

}   // GetPhysicsComponent


/************************************************************************
 *    desc:  Get the scripting component                                                            
 ************************************************************************/
CScriptComponent & CSprite3D::GetScriptComponent()
{
    return m_scriptComponent;

}   // GetScriptingComponent


/************************************************************************
 *    desc:  Get the object data                                                            
 ************************************************************************/
const CObjectData3D & CSprite3D::GetObjectData() const
{
    return m_objectData;

}   // GetObjectData


/************************************************************************
*    desc:  Set/Get the AI pointer. This class owns the pointer
************************************************************************/
void CSprite3D::SetAI( iAIBase * pAIBase )
{
    m_upAI.reset( pAIBase );

    // Handle any initialization in a separate function
    m_upAI->Init();

}   // SetAI


/************************************************************************
*    desc:  Get the unique id number
************************************************************************/
int CSprite3D::GetId() const
{
    return m_id;

}   // GetId


/************************************************************************
 *    desc:  Set the color
 ************************************************************************/
void CSprite3D::SetColor( const CColor & color )
{
    m_visualComponent.SetColor( color );

}   // SetColor

void CSprite3D::SetColor( float r, float g, float b, float a )
{
    // This function assumes values between 0.0 to 1.0.
    m_visualComponent.SetColor( r, g, b, a );

}   // SetColor


/************************************************************************
 *    desc:  Set the default color
 ************************************************************************/
void CSprite3D::SetDefaultColor()
{
    m_visualComponent.SetColor( m_objectData.GetVisualData().GetColor() );

}   // SetColor


/************************************************************************
 *    desc:  Get the color
 ************************************************************************/
const CColor & CSprite3D::GetColor() const
{
    return m_visualComponent.GetColor();

}   // GetColor


/************************************************************************
 *    desc:  Get the default color
 ************************************************************************/
const CColor & CSprite3D::GetDefaultColor() const
{
    return m_objectData.GetVisualData().GetColor();

}   // GetDefaultColor


/************************************************************************
 *    desc:  Set the Alpha
 ************************************************************************/
void CSprite3D::SetAlpha( float alpha )
{
    if( alpha > 1.5 )
        alpha *= defs_RGB_TO_DEC;

    m_visualComponent.SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
 *    desc:  Get the Alpha
 ************************************************************************/
float CSprite3D::GetAlpha() const
{
    return m_visualComponent.GetAlpha();

}   // GetAlpha


/************************************************************************
 *    desc:  Get the default alpha
 ************************************************************************/
float CSprite3D::GetDefaultAlpha() const
{
    return m_objectData.GetVisualData().GetColor().GetA();

}   // GetDefaultAlpha
