
/************************************************************************
*    FILE NAME:       ballai.cpp
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

// Physical component dependency
#include "ballai.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <2d/basicspritestrategy2d.h>
#include <managers/soundmanager.h>
#include <managers/spritestrategymanager.h>
#include <utilities/settings.h>
#include <utilities/highresolutiontimer.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBallAI::CBallAI( iSprite2D * pSprite ) :
    m_sprite(*dynamic_cast<CSprite2D *>(pSprite)),
    m_rStrategy(CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy2D>("(sprite)")),
    m_generator(std::random_device{}()),
    m_angularImpulse(-1, 1),
    m_rotation(-M_PI, M_PI)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CBallAI::~CBallAI()
{
}   // destructor


/************************************************************************
*    desc:  Do any initializing                                                            
************************************************************************/
void CBallAI::Init()
{
    // Put ball into a random rotation
    auto pBody = m_sprite.GetPhysicsComponent().GetBody();
    pBody->SetTransform( pBody->GetPosition(), m_rotation(m_generator) );
    
    // Add a slight rotation to the ball so that it doesn't fall flat on a peg and stay there
    m_sprite.GetPhysicsComponent().GetBody()->ApplyAngularImpulse( m_angularImpulse(m_generator), false );
    
}   // Init


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void CBallAI::Update()
{
    if( m_sprite.GetPos().y < -900.0f )
        m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, m_sprite.GetId() );
        
}   // Update
