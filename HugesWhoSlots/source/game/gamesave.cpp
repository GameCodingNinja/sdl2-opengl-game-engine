
/************************************************************************
*    FILE NAME:       gamesave.cpp
*
*    DESCRIPTION:     Singleton class for saving game data
************************************************************************/

// Physical component dependency
#include "gamesave.h"

// Standard lib dependencies

// Boost lib dependencies

// Game lib dependencies
#include <utilities/exceptionhandling.h>

// SDL/OpenGL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructer
************************************************************************/
CGameSave::CGameSave() :
    m_file(nullptr)
{
}   // constructor


/************************************************************************
*    desc:  destructer                                                             
************************************************************************/
CGameSave::~CGameSave()
{
    Close();

}   // destructer


/************************************************************************
*    desc:  Load the game data
************************************************************************/
void CGameSave::Load()
{
    if( Open() )
    {
        Save();
    }
    else
    {
        // File was opened so read the data
        if( SDL_RWread( m_file, &m_saveData, 1, sizeof(m_saveData)) == 0 )
            throw NExcept::CCriticalException("Error reading game save data!", "No data was read in!" );
    }
    
    Close();
    
}   // Load


/************************************************************************
*    desc:  Save/Save/Close the game data
************************************************************************/
void CGameSave::OpenSaveClose()
{
    Open();
    Save();
    Close();
    
}   // OpenSaveClose


/************************************************************************
*    desc:  Open the game data
************************************************************************/
bool CGameSave::Open()
{
    bool result(false);
    
    if( m_file == nullptr )
    {
        std::string filePath;
    
        #if defined(__ANDROID__)
            char * pChar = SDL_GetPrefPath("HugesWhoProduction", "HugesWhoSlots");
            filePath = pChar;
            filePath.append( "gamesave.dat" );
            SDL_free( pChar );
        #else
            filePath = "data/settings/gamesave.dat";
        #endif

        // Does the save file exist?
        if( (m_file = SDL_RWFromFile( filePath.c_str(), "rb+" )) == nullptr )
        {
            // Create a new one from scratch
            if( (m_file = SDL_RWFromFile( filePath.c_str(), "wb+" )) == nullptr )
                throw NExcept::CCriticalException("Error creating save file!", SDL_GetError() );
            
            result = true;
        }
    }
    else
    {
        throw NExcept::CCriticalException("Game Save File Error!", "Game save file has already been opened." );
    }
    
    return result;
    
}   // Open


/************************************************************************
*    desc:  Save the game data
************************************************************************/
void CGameSave::Save()
{
    if( m_file == nullptr )
        throw NExcept::CCriticalException("Game Save File Error!", "Game save file has NOT been loaded!" );
    
    SDL_RWseek( m_file, 0, RW_SEEK_SET );
    
    if( SDL_RWwrite( m_file, &m_saveData, 1, sizeof(m_saveData)) != sizeof(m_saveData) )
        throw NExcept::CCriticalException("Game Save File Error!", "Not all the data was written to file!" );
    
}   // Save


/************************************************************************
*    desc:  Close the game data file
************************************************************************/
void CGameSave::Close()
{
    if( m_file != nullptr )
    {
        SDL_RWclose(m_file);
        m_file = nullptr;
    }
    
}   // Close


/************************************************************************
*    desc:  Set/Get play lobby music
************************************************************************/
void CGameSave::SetPlayLobbyMusic( bool value )
{
    m_saveData.m_playLobbyMusic = value;
}

bool CGameSave::GetPlayLobbyMusic() const
{
    return m_saveData.m_playLobbyMusic;
}


/************************************************************************
*    desc:  Set/Get play stop sounds
************************************************************************/
void CGameSave::SetPlayStopSounds( bool value )
{
    m_saveData.m_playStopSounds = value;
}

bool CGameSave::GetPlayStopSounds() const
{
    return m_saveData.m_playStopSounds;
}


/************************************************************************
*    desc:  Set/Get play spin music
************************************************************************/
void CGameSave::SetPlaySpinMusic( bool value )
{
    m_saveData.m_playSpinMusic = value;
}

bool CGameSave::GetPlaySpinMusic() const
{
    return m_saveData.m_playSpinMusic;
}


/************************************************************************
*    desc:  Set/Get total credits
************************************************************************/
void CGameSave::SetTotalCredits( uint value )
{
    m_saveData.m_totalCredits = value;
}

uint CGameSave::GetTotalCredits() const
{
    return m_saveData.m_totalCredits;
}