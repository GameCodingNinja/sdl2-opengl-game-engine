
/************************************************************************
*    FILE NAME:       soundmanager.h
*
*    DESCRIPTION:     Sound Manager class singleton
************************************************************************/

#ifndef __sound_manager_h__
#define __sound_manager_h__

// Physical component dependency
#include <managers/managerbase.h>

// Game lib dependencies
#include <common/sound.h>
#include <common/playlist.h>


class CSoundMgr : public CManagerBase
{
public:

    // Get the instance of the singleton class
    static CSoundMgr & Instance()
    {
        static CSoundMgr soundMgr;
        return soundMgr;
    }

    // Load all of the sounds of a specific group
    void LoadGroup( const std::string & group );
    
    // Free a sound group
    void FreeGroup( const std::string & group );
    
    // Get the next channel
    int GetNextChannel();

    // Play a sound
    void Play( const std::string & group, const std::string & soundID, int loopCount = 0 );

    // Pause a sound
    void Pause( const std::string & group, const std::string & soundID );

    // Resume a sound
    void Resume( const std::string & group, const std::string & soundID );

    // Resume a sound
    void Stop( const std::string & group, const std::string & soundID );
    
    // Set volume for music or channel
    void SetVolume( const std::string & group, const std::string & soundID, int volume );
    int GetVolume( const std::string & group, const std::string & soundID );
    
    // Is music or channel playing?
    bool IsPlaying( const std::string & group, const std::string & soundID );
    
    // Is music or channel paused?
    bool IsPaused( const std::string & group, const std::string & soundID );

    // Get the sound
    CSound & GetSound( const std::string & group, const std::string & soundID );
    
    // Get the playlist
    CPlayList & GetPlayList( const std::string & group, const std::string & playLstID );
    
    // Is music playing?
    bool IsMusicPlaying();
    
    // Is music paused?
    bool IsMusicPaused();
    
    // Stop the music
    void StopMusic();
    
    // Pause the music
    void PauseMusic();

private:

    // Constructor
    CSoundMgr();

    // Destructor
    virtual ~CSoundMgr();

    // Load all object information from an xml
    void LoadFromXML( const std::string & group, const std::string & filePath );

private:

    // Map containing a group of sound ID's
    std::map< const std::string, std::map< const std::string, CSound > > m_soundMapMap;

    // Map containing a group of play list ID's
    // Do not free the sounds copied to the play list
    std::map< const std::string, std::map< const std::string, CPlayList > > m_playListMapMap;

    // mix channel for the next sound
    int m_mixChannel;

    // max mix channels
    int m_maxMixChannels;
    
    // dummys
    CPlayList m_dummyPlayLst;

};


namespace NScriptSoundMgr
{
    // Register Script Object
    void Register(asIScriptEngine * pEngine);
}


#endif  // __sound_manager_h__