
/************************************************************************
*    FILE NAME:       main.cpp
*
*    DESCRIPTION:     main class
************************************************************************/

// Game dependencies
#include "game.h"

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <system/device.h>

// Standard lib dependencies
#include <string>
#include <ctime>
#include <cstdlib>

int main( int argc, char* args[] )
{
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

