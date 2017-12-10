
/************************************************************************
*    FILE NAME:       ballai.cpp
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

// Physical component dependency
#include "ballai.h"

// Game lib dependencies
#include <2d/sprite2d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBallAI::CBallAI( iSprite2D * pSprite ) :
    m_sprite(*dynamic_cast<CSprite2D *>(pSprite)),
    m_generator(std::random_device{}())
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
    InitPhysics();
    
}   // Init


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void CBallAI::Update()
{
    if( m_sprite.GetPos().y < -600.f )
        InitPhysics();
    
}   // Update


/************************************************************************
*    desc:  Init the ball placement
************************************************************************/
void CBallAI::InitPhysics()
{
    std::uniform_int_distribution<int> m_RandX(-700,700);
    std::uniform_int_distribution<int> m_RandY(600,1000);
    std::uniform_int_distribution<int> m_RandR(0,360);
    
    m_sprite.GetPhysicsComponent().SetTransform(
        m_RandX(m_generator),
        m_RandY(m_generator),
        m_RandR(m_generator),
        true );
    
    // Reposition the sprite based on the new physics position and rotation
    m_sprite.PhysicsUpdate();
    
}   // InitPhysics
