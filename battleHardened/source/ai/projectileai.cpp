
/************************************************************************
*    FILE NAME:       projectileai.cpp
*
*    DESCRIPTION:     Class projectile AI
************************************************************************/

// Physical component dependency
#include "projectileai.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <spritestrategy/basicspritestrategy.h>
#include <spritestrategy/spritestrategymanager.h>
#include <managers/soundmanager.h>
#include <utilities/settings.h>
#include <utilities/highresolutiontimer.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CProjectileAI::CProjectileAI( iSprite * pSprite ) :
    m_sprite( *dynamic_cast<CSprite2D *>(pSprite) ),
    m_firstUpdate(true),
    m_rStrategy(CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy>("(actor)"))
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CProjectileAI::~CProjectileAI()
{
}   // destructor


/************************************************************************
*    desc:  Do any initalizing                                                            
************************************************************************/
void CProjectileAI::Init()
{
    // Speed of the projectile
    const float PROJECTILE_SPEED = 2.6f;

    // Get the sprite group
    m_initialPos = m_sprite.GetPos();

    // Set the velocity of the projectile
    float rotation = m_sprite.GetRot().z;
    m_velocity.x = cos( rotation );
    m_velocity.y = sin( rotation );
    m_velocity *= PROJECTILE_SPEED;
    //m_velocity += shooterVel; // value not needed sonce ship is not moving

    // Play the laser01 sound effect
    //CSoundMgr::Instance().Play( "(effects)", "laser" );
    
}   // Init


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void CProjectileAI::Update()
{
    // If it's the projectile's first update, we don't want to increment 
    // it's position by the velocity 
    if( !m_firstUpdate )
        m_sprite.IncPos( m_velocity * CHighResTimer::Instance().GetElapsedTime() );
    else
        m_firstUpdate = false;
    
    // Did we collide with an enemy ship?
    //if( m_rStrategy.IsCollision( m_sprite ) != nullptr )
    //    m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, m_sprite.GetId() );

    // Delete if goes out of view
    if( m_sprite.GetTransPos().GetLengthSquared2D() > 250000.f )
        m_rStrategy.SetToDestroy( m_sprite.GetId() );

}   // Update
