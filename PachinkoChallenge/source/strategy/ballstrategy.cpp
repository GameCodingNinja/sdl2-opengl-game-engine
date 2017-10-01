
/************************************************************************
*    FILE NAME:       ballstrategy.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "ballstrategy.h"

// Game lib dependencies
#include <2d/sprite2d.h>
#include <physics/physicscomponent2d.h>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CBallStrategy::CBallStrategy( int idOffset ) :
    CBasicSpriteStrategy2D( idOffset ),
        m_generator(std::random_device{}()),
        m_angularImpulse(-1, 1),
        m_rotation(-M_PI, M_PI)
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CBallStrategy::~CBallStrategy()
{
}   // destructor


/************************************************************************
*    desc:  create the sprite sprite
************************************************************************/
int CBallStrategy::Create(
    const std::string & name,
    const int id,
    const CPoint<CWorldValue> & pos,
    const CPoint<float> & rot,
    const CPoint<float> & scale )
{
    const int spriteId = CBasicSpriteStrategy2D::Create( name, id, pos, rot, scale );
    
    // Put ball into a random rotation
    auto pBody = m_iter.first->second->GetPhysicsComponent().GetBody();
    pBody->SetTransform( pBody->GetPosition(), m_rotation(m_generator) );
    
    // Add a slight rotation to the ball so that it doesn't fall flat on a peg and stay there
    m_iter.first->second->GetPhysicsComponent().GetBody()->ApplyAngularImpulse( m_angularImpulse(m_generator), false );
    
    return spriteId;
    
}   // Create
