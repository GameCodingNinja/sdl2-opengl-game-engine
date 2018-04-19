
/************************************************************************
*    FILE NAME:       gametemplate.h
*
*    DESCRIPTION:     CGame class
************************************************************************/

#ifndef __game_h__
#define __game_h__

// Standard lib dependencies
#include <string>

// SDL lib dependencies
#include <SDL.h>

// Forward declaration(s)
class CSprite2D;
class CUIControl;
class iGameState;
class CMenu;
class iSprite;

class CGame
{
public:

    // Constructor
    CGame();

    // Destructor
    virtual ~CGame();
    
    // Create the game Window
    void Create();
    
    // Main game loop for script driven games
    bool GameLoop();
    
    // Display error massage
    void DisplayErrorMsg( const std::string & title, const std::string & msg );
    
    // Is the game running?
    bool IsGameRunning() const;

private:

    // OpenGL Init
    void OpenGLInit();
    
    // Game start init
    void Init();
    
    // Poll for game events
    void PollEvents();

    // Handle events
    bool HandleEvent( const SDL_Event & rEvent );
    
    // Start the game
    void StartGame();

    // Stop the game
    void StopGame();

    // Callback for when a smart gui control is created
    void SmartGuiControlCreateCallBack( CUIControl * pUIControl );
    
    // Callback for when a smart menu is created
    void SmartMenuCreateCallBack( CMenu * pMenu );
    
    // Call back function to create sprite ai
    void AICreateCallBack( const std::string & aiName, iSprite * pSprite );
    
    // Callback for the state string
    void StatStringCallBack( const std::string & statStr );
    
private:
    
    // The window we'll be rendering to
    SDL_Window * m_pWindow;

    // OpenGL context
    SDL_GLContext m_context;

    // flag to indicate the game is running
    bool m_gameRunning;

    // Clear buffer mask
    uint m_clearBufferMask;

};

int FilterEvents( void * userdata, SDL_Event * pEvent );

#endif  // __game_h__


