
/************************************************************************
*    FILE NAME:       iaibase2d.h
*
*    DESCRIPTION:     iAIBase interface Class
************************************************************************/

#ifndef __i_ai_base_2d_h__
#define __i_ai_base_2d_h__

// Forward declaration(s)
union SDL_Event;

class iAIBase2D
{
public:

    // Constructor
    iAIBase2D();

    // Destructor
    virtual ~iAIBase2D();
    
    // Do any initializing
    virtual void Init();

    // Handle player related messages
    virtual void HandleEvent( const SDL_Event & rEvent );

    // Update animations, move sprites, etc.
    virtual void Update();
    
    // Update the physics
    virtual void PhysicsUpdate();
};

#endif  // __i_ai_base_h__
