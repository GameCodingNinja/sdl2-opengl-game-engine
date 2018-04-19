
/************************************************************************
*    FILE NAME:       playershipai.cpp
*
*    DESCRIPTION:     Class player ship AI
************************************************************************/

// Physical component dependency
#include "playershipai.h"

// Game lib dependencies
#include <2d/actorsprite2d.h>
#include <2d/sprite2d.h>
#include <utilities/settings.h>
#include <managers/actionmanager.h>
#include <managers/spritestrategymanager.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPlayerShipAI::CPlayerShipAI( iSprite * pSprite ) :
    m_sprite( *dynamic_cast<CActorSprite2D *>(pSprite) ),
    m_fireTailSprite( m_sprite.GetSprite("player_fire_tail") ),
    m_gunSprite( m_sprite.GetSprite("ship_gun") ),
    m_gunRotation(0),
    m_playerShootTimer(200, true)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CPlayerShipAI::~CPlayerShipAI()
{
}   // destructor


/************************************************************************
*    desc:  Do any initalizing
************************************************************************/
void CPlayerShipAI::Init()
{
    m_fireTailSprite.Prepare( "Play", {24, &m_fireTailSprite, true} );

}   // Init


/************************************************************************
*    desc:  Handle player related messages
************************************************************************/
void CPlayerShipAI::HandleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == SDL_MOUSEMOTION )
    {
        // Get the position of the gun
        const CPoint<float> pos = m_sprite.GetSprite(-1).GetTransPos();
        const float ratio = 1 / CSettings::Instance().GetOrthoAspectRatio().GetH();
        m_gunXY.x = ((ratio * (float)rEvent.motion.x) - pos.x) - CSettings::Instance().GetDefaultSizeHalf().w;
        m_gunXY.y = CSettings::Instance().GetDefaultSizeHalf().h - ((ratio * (float)rEvent.motion.y) + pos.y);
    }
    else if( rEvent.type == SDL_CONTROLLERAXISMOTION )
    {
        if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX )
        {
            m_gunXY.x = rEvent.caxis.value;
        }
        else if( rEvent.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY )
        {
            m_gunXY.y = -rEvent.caxis.value;
        }
    }
    else if( CActionMgr::Instance().WasAction( rEvent, "Shoot", NDefs::EAP_DOWN ) )
    {
        HandleShooting();
    }

}   // HandleEvent


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void CPlayerShipAI::Update()
{
    m_gunRotation = atan2( m_gunXY.y, m_gunXY.x );
    m_gunSprite.SetRot( 0, 0, m_gunRotation - m_sprite.GetRot().z, false );

}   // Update


/************************************************************************
*    desc:  Handle the ship's shooting
************************************************************************/
void CPlayerShipAI::HandleShooting()
{
    // If the player hits the shoot button, and our time has expired, create a projectile actor
    if( m_playerShootTimer.Expired() )
    {
        const CPoint<float> & centerPos = m_gunSprite.GetCenterPos();
        const CPoint<float> & pos = m_gunSprite.GetPos();
        
        CPoint<float> projectileOffset;
        const float OFFSET(60.f);
        projectileOffset.x = (OFFSET * cos( m_gunRotation )) - centerPos.x + pos.x;
        projectileOffset.y = (OFFSET * sin( m_gunRotation )) + centerPos.y + pos.y;

        // Create the projectile actor
        CSpriteStrategyMgr::Instance().Create(
            "(actor)", "player_projectile", m_sprite.GetPos() + projectileOffset, CPoint<float>(0,0,m_gunRotation) );

        m_playerShootTimer.Reset();
    }

}   // HandleShooting
