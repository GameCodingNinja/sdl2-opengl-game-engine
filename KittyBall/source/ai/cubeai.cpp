
/************************************************************************
*    FILE NAME:       cubeai.cpp
*
*    DESCRIPTION:     Class ball AI
************************************************************************/

// Physical component dependency
#include "cubeai.h"

// Game lib dependencies
#include <3d/sprite3d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CCubeAI::CCubeAI( iSprite3D * pSprite ) :
    m_sprite(*dynamic_cast<CSprite3D *>(pSprite)),
    m_generator(std::random_device{}())
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CCubeAI::~CCubeAI()
{
}   // destructor


/************************************************************************
*    desc:  Do any initializing                                                            
************************************************************************/
void CCubeAI::Init()
{
    InitPhysics();
    
}   // Init


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void CCubeAI::Update()
{
    if( m_sprite.GetPos().y < -600.f )
        InitPhysics();
    
}   // Update


/************************************************************************
*    desc:  Init the ball placement
************************************************************************/
void CCubeAI::InitPhysics()
{
    /*std::uniform_int_distribution<int> m_RandX(-700,700);
    std::uniform_int_distribution<int> m_RandY(600,1000);
    std::uniform_int_distribution<int> m_RandR(0,360);
    
    m_sprite.GetPhysicsComponent().SetTransform(
        m_RandX(m_generator),
        m_RandY(m_generator),
        m_RandR(m_generator),
        true );*/
    
    // Reposition the sprite based on the new physics position and rotation
    //m_sprite.PhysicsUpdate();
    
}   // InitPhysics
