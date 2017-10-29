
/************************************************************************
*    FILE NAME:       igamemusic.h
*
*    DESCRIPTION:     Interface for handling game music
************************************************************************/

#ifndef __i_game_music_h__
#define __i_game_music_h__

class iGameMusic
{
public:
    
    // Handle update checks
    virtual void Update() = 0;
    
    // Start the music
    virtual void StartMusic() = 0;
    
    // Start the music timeout
    virtual void SetTimeOut() = 0;
};

#endif  // __i_game_music_h__


