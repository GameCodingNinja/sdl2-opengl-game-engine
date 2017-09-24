
/************************************************************************
*    FILE NAME:       iaibase2d.cpp
*
*    DESCRIPTION:     iAIBase interface Class
************************************************************************/

// Physical component dependency
#include <2d/iaibase2d.h>

// Game lib dependencies
#include <2d/isprite2d.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
iAIBase2D::iAIBase2D()
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
iAIBase2D::~iAIBase2D()
{
}   // destructor


/************************************************************************
*    desc:  Do any initializing
************************************************************************/
void iAIBase2D::Init()
{
    // empty by design
}


/************************************************************************
*    desc:  Handle player related messages
************************************************************************/
void iAIBase2D::HandleEvent( const SDL_Event & rEvent )
{
    // empty by design
}


/************************************************************************
*    desc:  Update animations, move sprites, etc.
************************************************************************/
void iAIBase2D::Update()
{
    // empty by design
}

/************************************************************************
*    desc:  Update the physics
************************************************************************/
void iAIBase2D::PhysicsUpdate()
{
    // empty by design
}