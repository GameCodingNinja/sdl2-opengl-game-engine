
/************************************************************************
*    FILE NAME:       main.cpp
*
*    DESCRIPTION:     main function
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
        game.Create();

        // Call the game loop
        while( game.GameLoop() )
        { }
    }
    catch( NExcept::CCriticalException & ex )
    {
        game.DisplayErrorMsg( ex.getErrorTitle(), ex.getErrorMsg() );
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

