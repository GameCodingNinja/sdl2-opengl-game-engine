
/************************************************************************
*    FILE NAME:       main.cpp
*
*    DESCRIPTION:     main class
************************************************************************/

// Game dependencies
#include "game.h"

#if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#endif

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <system/device.h>

// Standard lib dependencies
#include <string>
#include <ctime>
#include <cstdlib>

int main( int argc, char* args[] )
{
// Moved here so as to allows building in Linux Mint to link GLEW
#if !(defined(__IOS__) || defined(__ANDROID__) || defined(__arm__))
glewExperimental = GL_TRUE;
#endif

    std::srand ( unsigned ( std::time(0) ) );

    CGame game;

    try
    {
        // Create the game
        game.Create();

        // Call the game loop
        while( game.GameLoop() )
        { }
    }
    catch( NExcept::CCriticalException & ex )
    {
        game.DisplayErrorMsg( ex.GetErrorTitle(), ex.GetErrorMsg() );
    }
    catch( std::exception const & ex )
    {
        game.DisplayErrorMsg( "Standard Exception", ex.what() );
    }
    catch(...)
    {
        game.DisplayErrorMsg( "Unknown Error", "Something bad happened and I'm not sure what it was." );
    }

    return 0;

}   // main

