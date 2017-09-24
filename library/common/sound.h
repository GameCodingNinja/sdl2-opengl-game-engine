/************************************************************************
*    FILE NAME:       sound.h
*
*    DESCRIPTION:     Class to hold the sound pointer and type
************************************************************************/  

#ifndef __sound_h__
#define __sound_h__

// Standard lib dependencies
#include <string>
#include <cstdint>

// Forward declaration(s)
class asIScriptEngine;
struct XMLNode;

class CSound
{
public:

    enum ESoundType
    {
        EST_NULL=0,
        EST_LOADED,
        EST_STREAM,
        EST_MAX,
    };

    CSound( ESoundType type );
    CSound( const CSound & sound );
    CSound();
    ~CSound();

    // Load the sound
    void LoadFromNode( const XMLNode & node );

    // Play the sound
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

    // Find an open channel and set the class member
    void SetOpenChannel();

    // Free the sound
    void Free();
    
    // equality/inequality operator
    bool operator == ( const CSound & sound ) const;
    bool operator != ( const CSound & sound ) const;

private:
    
    // Sound type - loaded or stream
    ESoundType m_type;
    
    // Voided pointer to hold the different sound type
    void * m_pVoid;

    // Channel the sound is currently running on
    int16_t m_channel;
    
    // Sounds current volume
    int16_t m_volume;
};


namespace NScriptSound
{
    // Register Script Object
    void Register(asIScriptEngine * pEngine);
}

#endif  // __sound_h__

