
/************************************************************************
*    FILE NAME:       gamesave.h
*
*    DESCRIPTION:     Singleton class for saving game data
************************************************************************/

#ifndef __game_save_h__
#define __game_save_h__

// Game lib dependencies
#include <common/defs.h>

// Forward declaration(s)
struct SDL_RWops;

class CGameSave
{
public:
    
    struct SSaveData
    {
        bool m_playLobbyMusic = true;
        bool m_playStopSounds = true;
        bool m_playSpinMusic = true;
        uint m_totalCredits = 5000;
    };
    
    // Get the instance of the singleton class
    static CGameSave & Instance()
    {
        static CGameSave gameSave;
        return gameSave;
    }
    
    // Load the game data
    void Load();
    
    // Save/Save/Close the game data
    void OpenSaveClose();
    
    
    // Set/Get play lobby music
    void SetPlayLobbyMusic( bool value );
    bool GetPlayLobbyMusic() const;
    
    // Set/Get play stop sounds
    void SetPlayStopSounds( bool value );
    bool GetPlayStopSounds() const;
    
    // Set/Get play spin music
    void SetPlaySpinMusic( bool value );
    bool GetPlaySpinMusic() const;
    
    // Set/Get total credits
    void SetTotalCredits( uint value );
    uint GetTotalCredits() const;
    
private:

    // Constructor
    CGameSave();

    // Destructor
    ~CGameSave();
    
    // Open the game data
    bool Open();
    
    // Save the game data
    void Save();
    
    // Close the game data file
    void Close();
    
private:

    // Save file
    SDL_RWops * m_file;
    
    // Save Data
    SSaveData m_saveData;

};

#endif  // __game_save_h__


