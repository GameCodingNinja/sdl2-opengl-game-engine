
/************************************************************************
*    FILE NAME:       main.cpp
*
*    DESCRIPTION:     main class
************************************************************************/

// Game dependencies
#include "game.h"

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// Standard lib dependencies
#include <string>
#include <ctime>

int main( int argc, char* args[] )
{
    std::srand ( unsigned ( std::time(0) ) );

    CGame game;

    try
    {
        // Create the game
        game.create();

        // Call the game loop
        while( game.gameLoop() )
        { }
    }
    catch( NExcept::CCriticalException & ex )
    {
        game.displayErrorMsg( ex.getErrorTitle(), ex.getErrorMsg() );
    }
    catch( std::exception const & ex )
    {
        game.displayErrorMsg( "Standard Exception", ex.what() );
    }
    catch(...)
    {
        game.displayErrorMsg( "Unknown Error", "Something bad happened and I'm not sure what it was." );
    }

    return 0;

}   // main
