
/************************************************************************
*    FILE NAME:       sound.cpp
*
*    DESCRIPTION:     Class to hold the sound pointer and type
************************************************************************/

// Physical component dependency
#include <common/sound.h>

// Game lib dependencies
#include <utilities/exceptionhandling.h>
#include <utilities/genfunc.h>
#include <utilities/xmlParser.h>
#include <script/scriptglobals.h>

// SDL lib dependencies
#include <SDL_mixer.h>

// AngelScript lib dependencies
#include <angelscript.h>

// Boost lib dependencies
#include <boost/format.hpp>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSound::CSound( ESoundType type ) :
    m_type(type),
    m_pVoid(nullptr),
    m_channel(-1),
    m_volume(MIX_MAX_VOLUME)
{
}   // constructor

CSound::CSound()
{
}   // constructor


/************************************************************************
*    desc:  Copy Constructor
************************************************************************/
CSound::CSound( const CSound & sound ) :
    m_type(sound.m_type),
    m_pVoid(sound.m_pVoid),
    m_channel(sound.m_channel),
    m_volume(sound.m_volume)
{
}   // copy constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSound::~CSound()
{
    // Don't free the sound here because this destructor get's
    // called when the reference is passed around.
	
}   // destructor


/************************************************************************
*    desc:  Free the sound                                                             
************************************************************************/
void CSound::Free()
{
    Stop();
    
    if( m_type == EST_LOADED )
        Mix_FreeChunk( (Mix_Chunk *)m_pVoid );

    else if( m_type == EST_STREAM )
        Mix_FreeMusic( (Mix_Music *)m_pVoid );

}   // Free


/************************************************************************
*    desc:  Load the sound                                                             
************************************************************************/
void CSound::LoadFromNode( const XMLNode & node )
{
    auto file = node.getAttribute( "file" );
            
    if( m_type == EST_LOADED )
        m_pVoid = Mix_LoadWAV( file );

    else if( m_type == EST_STREAM )
        m_pVoid = Mix_LoadMUS( file );
    
    // Set the volume if defined
    if( node.isAttributeSet("volume") )
        SetVolume( std::atoi(node.getAttribute( "volume" )) );

    if( m_pVoid == nullptr )
        throw NExcept::CCriticalException("Sound load Error!",
            boost::str( boost::format("Error loading sound (%s)(%s).\n\n%s\nLine: %s")
                % SDL_GetError() % file % __FUNCTION__ % __LINE__ ));

}   // LoadFromNode


/************************************************************************
*    desc:  Play the sound
*    NOTE: Loop and channel default to -1
************************************************************************/
void CSound::Play( int channel, int loopCount )
{
    if( m_type == EST_LOADED )
    {
        m_channel = Mix_PlayChannel( channel, (Mix_Chunk *)m_pVoid, loopCount );
        Mix_Volume( m_channel, m_volume );
    }
    else if( m_type == EST_STREAM )
        Mix_PlayMusic( (Mix_Music *)m_pVoid, loopCount );

}   // Play


/************************************************************************
*    desc:  Stop the sound
************************************************************************/
void CSound::Stop()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Playing( m_channel ) )
            Mix_HaltChannel( m_channel );
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PlayingMusic() )
            Mix_HaltMusic();
    }

}   // Stop


/************************************************************************
*    desc:  Pause the sound
************************************************************************/
void CSound::Pause()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Playing( m_channel ) )
            Mix_Pause( m_channel );
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PlayingMusic() )
            Mix_PauseMusic();
    }

}   // Pause


/************************************************************************
*    desc:  Resume the sound
************************************************************************/
void CSound::Resume()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Paused( m_channel ) )
            Mix_Resume( m_channel );
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PausedMusic() )
            Mix_ResumeMusic();
    }

}   // Pause


/************************************************************************
*    desc: Set/Get the volume for music or channel
************************************************************************/
void CSound::SetVolume( int volume )
{
    if( m_type == EST_STREAM )
        Mix_VolumeMusic( volume );

    m_volume = volume;
    
}   // SetVolume

int CSound::GetVolume()
{
    if( m_type == EST_LOADED )
        m_volume = Mix_Volume( m_channel, -1 );

    else if( m_type == EST_STREAM )
        m_volume = Mix_VolumeMusic( -1 );
    
    return m_volume;
    
}   // GetVolume


/************************************************************************
*    desc:  Is music or channel playing?
************************************************************************/
bool CSound::IsPlaying()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Playing( m_channel ) )
            return true;
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PlayingMusic() )
            return true;
    }
    
    return false;

}   // IsPlaying


/************************************************************************
*    desc:  Is music or channel paused?
************************************************************************/
bool CSound::IsPaused()
{
    if( m_type == EST_LOADED )
    {
        if( (m_channel > -1) && Mix_Paused( m_channel ) )
            return true;
    }
    else if( m_type == EST_STREAM )
    {
        if( Mix_PausedMusic() )
            return true;
    }
    
    return false;

}   // IsPaused


/************************************************************************
*    desc:  Find an open channel and set the class member
************************************************************************/
void CSound::SetOpenChannel()
{
    int i;
    for( i = 0; i < MIX_CHANNELS; ++i )
    {
        if( !Mix_Playing(i) )
            break;
    }

    if( i == MIX_CHANNELS )
    {
        NGenFunc::PostDebugMsg( "No free channels available" );
    }
    else
    {
        m_channel = i;
    }

}   // SetOpenChannel

/************************************************************************
*    desc:  The equality operator
************************************************************************/
bool CSound::operator == ( const CSound & sound ) const
{
    return (m_pVoid == sound.m_pVoid);

}   // operator ==

/************************************************************************
*    desc:  The inequality operator
************************************************************************/
bool CSound::operator != ( const CSound & sound ) const
{
    return (m_pVoid != sound.m_pVoid);

}   // operator !=


namespace NScriptSound
{
    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CSound();
    }

    /************************************************************************
    *    desc:  Copy Constructor
    ************************************************************************/
    void CopyConstructor(const CSound & other, void * pThisPointer)
    {
        new(pThisPointer) CSound(other);
    }

    /************************************************************************
    *    desc:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CSound*)pThisPointer)->~CSound();
    }

    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register( asIScriptEngine * pEngine )
    {
        using namespace NScriptGlobals;

        // Register CScriptComponent2d reference and methods
        Throw( pEngine->RegisterObjectType("CSound", sizeof(CSound), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_CONSTRUCT, "void f()",                  asFUNCTION(NScriptSound::Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_CONSTRUCT, "void f(const CSound & in)", asFUNCTION(NScriptSound::CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CSound", asBEHAVE_DESTRUCT,  "void f()",                  asFUNCTION(NScriptSound::Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CSound", "CSound & opAssign(const CSound & in)", asMETHODPR(CSound, operator =, (const CSound &), CSound &), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CSound", "void Play( int channel = -1, int loopCount = 0 )", asMETHOD(CSound, Play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void Stop()",                                      asMETHOD(CSound, Stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void Pause()",                                     asMETHOD(CSound, Pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void Resume()",                                    asMETHOD(CSound, Resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "void SetVolume( int volume )",                     asMETHOD(CSound, SetVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "int GetVolume()",                                  asMETHOD(CSound, GetVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool IsPlaying()",                                 asMETHOD(CSound, IsPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CSound", "bool IsPaused()",                                  asMETHOD(CSound, IsPaused),  asCALL_THISCALL) );

    }   // Register
}
