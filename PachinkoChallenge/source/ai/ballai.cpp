
/************************************************************************
*    FILE NAME:       ballai.cpp
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

// Physical component dependency
#include "ballai.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <managers/soundmanager.h>
#include <strategy/strategymanager.h>
#include <strategy/basicspritestrategy.h>
#include <utilities/settings.h>
#include <utilities/highresolutiontimer.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CBallAI::CBallAI( iSprite * pSprite ) :
    m_sprite(*dynamic_cast<CSprite2D *>(pSprite)),
    m_rStrategy(CStrategyMgr::Instance().find<CBasicSpriteStrategy>("_spriteStrategy")), // Find the strategy that has this ball
    m_generator(std::random_device{}()),
    m_angularImpulse(-1, 1),
    m_rotation(-M_PI, M_PI)
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CBallAI::~CBallAI()
{
}


/************************************************************************
*    DESC:  Do any initializing
************************************************************************/
void CBallAI::init()
{
    m_sprite.prepareFuncId( "fadeIn" );

    // Put ball into a random rotation
    auto pBody = m_sprite.getPhysicsComponent().getBody();
    pBody->SetTransform( pBody->GetPosition(), m_rotation(m_generator) );

    // Add a slight rotation to the ball so that it doesn't fall flat on a peg and stay there
    pBody->ApplyAngularImpulse( m_angularImpulse(m_generator), false );
}


/************************************************************************
*    DESC:  Update animations, move sprites, etc.
************************************************************************/
void CBallAI::update()
{
    if( m_sprite.getPos().y < -1650.0f )
        m_rStrategy.setToDestroy( m_sprite.getId() );

}
