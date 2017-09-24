
/************************************************************************
*    FILE NAME:       soundmanager.cpp
*
*    DESCRIPTION:     Sound Manager class singleton
************************************************************************/

// Physical component dependency
#include <managers/soundmanager.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>
#include <common/defs.h>
#include <script/scriptglobals.h>

// SDL lib dependencies
#include <SDL_mixer.h>

// Boost lib dependencies
#include <boost/format.hpp>

// AngelScript lib dependencies
#include <angelscript.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSoundMgr::CSoundMgr() :
    m_mixChannel(0),
    m_maxMixChannels(MIX_CHANNELS)
{
    // Init for the OGG compressed file format
    Mix_Init(MIX_INIT_OGG);

    // Setup the audio format
    // High frenquency plus low chunk size = low latency audio playback
    Mix_OpenAudio(
        CSettings::Instance().GetFrequency(),     // Usually 22050 or 44100
        MIX_DEFAULT_FORMAT,
        CSettings::Instance().GetSoundChannels(), // mono, stero, quad, suround, etc
        CSettings::Instance().GetChunkSize() );
            
    if( CSettings::Instance().GetMixChannels() != m_maxMixChannels )
        m_maxMixChannels = Mix_AllocateChannels( CSettings::Instance().GetMixChannels() );
    
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSoundMgr::~CSoundMgr()
{
    // Free all sounds in all groups
    for( auto & mapMapIter : m_soundMapMap )
    {
        for( auto & mapIter : mapMapIter.second )
        {
            mapIter.second.Free();
        }
    }

}	// destructor


/************************************************************************
*    desc:  Load all of the sounds of a specific group
*
*    param: string & group - specified group of scripts to load
************************************************************************/
void CSoundMgr::LoadGroup( const std::string & group )
{
    // Make sure the group we are looking has been defined in the list table file
    auto listTableIter = m_listTableMap.find( group );
    if( listTableIter == m_listTableMap.end() )
        throw NExcept::CCriticalException("Sound List Load Group Data Error!",
            boost::str( boost::format("Sound list group name can't be found (%s).\n\n%s\nLine: %s") 
                % group % __FUNCTION__ % __LINE__ ));

    // Load the group data if it doesn't already exist
    if( m_soundMapMap.find( group ) == m_soundMapMap.end() )
    {
        for( auto & iter : listTableIter->second )
            LoadFromXML( group, iter );
    }
    else
    {
        throw NExcept::CCriticalException("Sound Data List 2D load Error!",
            boost::str( boost::format("Sound data list group has alread been loaded (%s).\n\n%s\nLine: %s")
                % group % __FUNCTION__ % __LINE__ ));
    }

}   // LoadGroup


/************************************************************************
*    desc:  Load all object information from an xml
*
*	 param:	string & filePath - file path of the object data list XML
************************************************************************/
void CSoundMgr::LoadFromXML( const std::string & group, const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "soundList" );

    // Create a new map inside of our map and get an iterator into it
    auto soundMapIter = m_soundMapMap.emplace( group, std::map<const std::string, CSound>() ).first;

    // Get the node to the sound files
    const XMLNode soundFilesNode = mainNode.getChildNode( "soundFiles" );

    const CSound::ESoundType typeEnumArry[] = { CSound::EST_LOADED, CSound::EST_STREAM };
    const char typeCharArry[CSound::EST_MAX][10] = { "load", "stream" };

    for( uint i = 0; i < CSound::EST_MAX; ++i )
    {
        // Get the number of load children
        int count = soundFilesNode.nChildNode( typeCharArry[i] );

        for( int j = 0; j < count; ++j )
        {
            // Get the object data node
            const XMLNode loadNode = soundFilesNode.getChildNode(typeCharArry[i], j);

            // Get the id
            const std::string id = loadNode.getAttribute( "id" );

            // Add the sound data to the map
            auto iter = soundMapIter->second.emplace( id, typeEnumArry[i] );

            // Check for duplicate id names
            if( !iter.second )
            {
                throw NExcept::CCriticalException("Sound Data Load Group Error!",
                    boost::str( boost::format("Duplicate sound ID (%s - %s).\n\n%s\nLine: %s")
                        % id % group % __FUNCTION__ % __LINE__ ));
            }

            // Now try to load the sound
            iter.first->second.LoadFromNode( loadNode );
        }
    }

    // Get the node to the play lists
    const XMLNode playListSetNode = mainNode.getChildNode( "playlistSet" );
    if( !playListSetNode.isEmpty() )
    {
        // Create a new map inside of our map and get an iterator into it
        auto playListMapIter = m_playListMapMap.emplace( group, std::map<const std::string, CPlayList>() ).first;

        for( int i = 0; i < playListSetNode.nChildNode(); ++i )
        {
            // Get the play list node
            const XMLNode playListNode = playListSetNode.getChildNode(i);

            // Get the id
            const std::string id = playListNode.getAttribute( "id" );

            // Add the playlist data to the map
            auto iter = playListMapIter->second.emplace( id, std::string(playListNode.getAttribute( "playtype" )) );

            // Check for duplicate names
            if( !iter.second )
            {
                throw NExcept::CCriticalException("Playlist Data Group Load Error!",
                    boost::str( boost::format("Duplicate playlist name! (%s - %s).\n\n%s\nLine: %s")
                        % id % group % __FUNCTION__ % __LINE__ ));
            }

            iter.first->second.LoadFromNode( playListNode, group, soundMapIter->second );
        }
    }

}   // LoadFromXML


/************************************************************************
*    desc:  Free a sound group
************************************************************************/
void CSoundMgr::FreeGroup( const std::string & group )
{
    // Free the sound group if it exists
    auto soundMapIter = m_soundMapMap.find( group );
    if( soundMapIter != m_soundMapMap.end() )
    {
        // Free all the sounds in this group
        for( auto & mapIter : soundMapIter->second )
            mapIter.second.Free();

        // Erase this group
        m_soundMapMap.erase( soundMapIter );
    }
    
    // Free the playlist group if it exists
    auto playLstMapIter = m_playListMapMap.find( group );
    if( playLstMapIter != m_playListMapMap.end() )
        m_playListMapMap.erase( playLstMapIter );

}   // FreeGroup


/************************************************************************
*    desc:  Get the sound
************************************************************************/
CSound & CSoundMgr::GetSound( const std::string & group, const std::string & soundID )
{
    // Check if this is a playlist sound ID
    auto playListMapIter = m_playListMapMap.find( group );
    if( playListMapIter != m_playListMapMap.end() )
    {
        auto iter = playListMapIter->second.find( soundID );
        if( iter != playListMapIter->second.end() )
            return iter->second.GetSound();
    }

    auto soundMapIter = m_soundMapMap.find( group );
    if( soundMapIter == m_soundMapMap.end() )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sound group can't be found (%s).") % group ) );

    auto iter = soundMapIter->second.find( soundID );
    if( iter == soundMapIter->second.end() )
        NGenFunc::PostDebugMsg( boost::str( boost::format("Sound ID can't be found (%s - %s).") % group % soundID ) );

    return iter->second;

}   // GetSound


/************************************************************************
*    desc:  Get the playlist
************************************************************************/
CPlayList & CSoundMgr::GetPlayList( const std::string & group, const std::string & playLstID )
{
    auto playListMapIter = m_playListMapMap.find( group );
    if( playListMapIter != m_playListMapMap.end() )
    {
        auto iter = playListMapIter->second.find( playLstID );
        if( iter != playListMapIter->second.end() )
            return iter->second;
    }
    
    return m_dummyPlayLst;
    
}   // GetPlayList


/************************************************************************
*    desc:  Get the next channel
************************************************************************/
int CSoundMgr::GetNextChannel()
{
    m_mixChannel = (m_mixChannel + 1) % m_maxMixChannels;
    return m_mixChannel;

}   // GetNextChannel


/************************************************************************
*    desc:  Play a sound
************************************************************************/
void CSoundMgr::Play( const std::string & group, const std::string & soundID, int loopCount )
{
    GetSound( group, soundID ).Play( GetNextChannel(), loopCount );

}   // Play


/************************************************************************
*    desc:  Pause a sound
************************************************************************/
void CSoundMgr::Pause( const std::string & group, const std::string & soundID )
{
    GetSound( group, soundID ).Pause();

}   // Pause


/************************************************************************
*    desc:  Resume a sound
************************************************************************/
void CSoundMgr::Resume( const std::string & group, const std::string & soundID )
{
    GetSound( group, soundID ).Resume();

}   // Resume


/************************************************************************
*    desc:  Resume a sound
************************************************************************/
void CSoundMgr::Stop( const std::string & group, const std::string & soundID )
{
    GetSound( group, soundID ).Stop();

}   // Resume


/************************************************************************
*    desc: Set/Get the volume for music or channel
************************************************************************/
void CSoundMgr::SetVolume( const std::string & group, const std::string & soundID, int volume )
{
    GetSound( group, soundID ).SetVolume( volume );
    
}   // SetVolume

int CSoundMgr::GetVolume( const std::string & group, const std::string & soundID )
{
    return GetSound( group, soundID ).GetVolume();
    
}   // GetVolume


/************************************************************************
*    desc:  Is music or channel playing?
************************************************************************/
bool CSoundMgr::IsPlaying( const std::string & group, const std::string & soundID )
{
    return GetSound( group, soundID ).IsPlaying();

}   // IsPlaying


/************************************************************************
*    desc:  Is music or channel paused?
************************************************************************/
bool CSoundMgr::IsPaused( const std::string & group, const std::string & soundID )
{
    return GetSound( group, soundID ).IsPaused();
    
}   // IsPaused


/************************************************************************
*    desc:  Is music playing?
************************************************************************/
bool CSoundMgr::IsMusicPlaying()
{
    return Mix_PlayingMusic();

}   // IsMusicPlaying


/************************************************************************
*    desc:  Is music paused?
************************************************************************/
bool CSoundMgr::IsMusicPaused()
{
    return Mix_PausedMusic();

}   // IsMusicPaused


/************************************************************************
*    desc:  Stop the music
************************************************************************/
void CSoundMgr::StopMusic()
{
    Mix_HaltMusic();

}   // StopMusic


/************************************************************************
*    desc:  Pause the music
************************************************************************/
void CSoundMgr::PauseMusic()
{
    if( Mix_PlayingMusic() )
        Mix_PauseMusic();

}   // PauseMusic


namespace NScriptSoundMgr
{
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register( asIScriptEngine * pEngine )
    {
        using namespace NScriptGlobals;

        // Global sound calls
        Throw( pEngine->RegisterGlobalFunction("void PlaySound( string &in, string &in, int loopCount = 0 )", asMETHOD(CSoundMgr, Play), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void PauseSound( string &in, string &in )", asMETHOD(CSoundMgr, Pause), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void ResumeSound( string &in, string &in )", asMETHOD(CSoundMgr, Resume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void StopSound( string &in, string &in )", asMETHOD(CSoundMgr, Stop), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("void SetVolume( string &in, string &in, int volume )", asMETHOD(CSoundMgr, SetVolume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("int GetVolume( string &in, string &in )", asMETHOD(CSoundMgr, GetVolume), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool IsSoundPlaying( string &in, string &in )", asMETHOD(CSoundMgr, IsPlaying), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("bool IsSoundPaused( string &in, string &in )", asMETHOD(CSoundMgr, IsPaused), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("int GetNextSoundChannel()",               asMETHOD(CSoundMgr, GetNextChannel), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CSound & GetSound( string &in, string &in )", asMETHOD(CSoundMgr, GetSound), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );
        Throw( pEngine->RegisterGlobalFunction("CPlayList & GetPlayList( string &in, string &in )", asMETHOD(CSoundMgr, GetPlayList), asCALL_THISCALL_ASGLOBAL, &CSoundMgr::Instance()) );

    }   // Register
}
