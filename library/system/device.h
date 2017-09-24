
/************************************************************************
*    FILE NAME:       device.h
*
*    DESCRIPTION:     Class used for window and openGL management
************************************************************************/

#ifndef __device_h__
#define __device_h__

// Game lib dependencies
#include <utilities/matrix.h>
#include <common/defs.h>

// SDL lib dependencies
#include <SDL.h>

// Standard lib dependencies
#include <string>
#include <map>

class CDevice
{
public:

    // Get the instance
    static CDevice & Instance()
    {
        static CDevice device;
        return device;
    }

    // Create the window and OpenGL context
    void Create();

    // Show/Hide the Window
    void ShowWindow( bool visible );

    // Get the projection matrix
    const CMatrix & GetProjectionMatrix( NDefs::EProjectionType type ) const;

    // Get the OpenGl Context
    SDL_GLContext GetContext();

    // Get the SDL window
    SDL_Window * GetWindow();
    
    // Get the SDL window surface
    SDL_Surface * GetSurface();

    // Close out the game pads
    void CloseGamepads();

    // Set full screen or windowed mode
    void SetFullScreen( bool fullscreen );

    // Create the projection matrixes
    void CreateProjMatrix();

    // Enable/disable v-sync
    void EnableVSync( bool enable );

    // Add/Remove the game pad
    void AddGamepad( int id );
    void RemoveGamepad( int id );
    
    // Get the gamepad count
    size_t GetGamepadCount();

private:

    // Constructor
    CDevice();

    // Destructor
    virtual ~CDevice();
    
    // Init current gamepads plugged in at startup
    void InitStartupGamepads();

private:

    // The window we'll be rendering to
    SDL_Window * m_pWindow;

    // OpenGL context
    SDL_GLContext m_context;
    
    // The windows surface for 2d software rendering (Do Not FREE))
    SDL_Surface * m_pScreenSurface;

    // Projection martixes
    CMatrix m_perspectiveMatrix;
    CMatrix m_orthographicMatrix;

    // Vector of gamepage pointers
    std::map<int, SDL_GameController *> m_pGamepadMap;

};

#endif  // __device_h__

