
/************************************************************************
*    FILE NAME:       playlist.h
*
*    DESCRIPTION:     Play list class
************************************************************************/

#ifndef __play_list_h__
#define __play_list_h__

// Game lib dependencies
#include <common/sound.h>
#include <common/defs.h>

// Standard lib dependencies
#include <vector>
#include <map>
#include <string>
#include <cstdint>

// Forward declaration(s)
struct XMLNode;

class CPlayList
{
public:

    enum EPlayListType
    {
        EST_NULL=0,
        EST_RANDOM,
        EST_SEQUENTIAL
    };

    // Constructor
    CPlayList( const std::string strType );
    CPlayList( const CPlayList & playLst );
    CPlayList();

    // Destructor
    virtual ~CPlayList();
    
    // Load  the playlist from node
    void LoadFromNode(
        const XMLNode & node,
        const std::string & group,
        std::map< const std::string, CSound > & soundMap );

    // Set the sound for the playlist
    CSound & GetSound();

    // Play the play list
    void Play( int channel = -1, int loopCount = 0 );
    
    // Stop the sound
    void Stop();

    // Pause the sound
    void Pause();

    // Resume the sound
    void Resume();
    
    // Set/Get the volume for music or channel
    void SetVolume( int volume );
    int GetVolume();
    
    // Is music or channel playing?
    bool IsPlaying();
    
    // Is music or channel paused?
    bool IsPaused();

private:

    // Play the play list
    void Shuffle();
    
private:
    
    // Counter
    int16_t m_counter;
    
    // current index
    int16_t m_current;

    // playlist type - random or sequential
    EPlayListType m_type;

    // vector of sounds
    std::vector<CSound> m_soundVec;
    
    // Dummy sound
    CSound m_DummySound;

};

#endif  // __play_list_h__


