
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
    m_rStrategy(CSpriteStrategyMgr::Instance().Get<CBasicSpriteStrategy2D>("(sprite)"))
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
}   // Init


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void CBallAI::Update()
{
    if( m_sprite.GetPos().y < -900.0f )
        m_rStrategy.HandleMessage( NDefs::ESM_KILL_SPRITE, m_sprite.GetId() );
        
}   // Update
