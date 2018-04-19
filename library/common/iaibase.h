
/************************************************************************
*    FILE NAME:       iaibase.h
*
*    DESCRIPTION:     iAIBase interface Class
************************************************************************/

#ifndef __i_ai_base_h__
#define __i_ai_base_h__

// Forward declaration(s)
union SDL_Event;

class iAIBase
{
public:

    // Constructor
    iAIBase();

    // Destructor
    virtual ~iAIBase();
    
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
