
/************************************************************************
*    FILE NAME:       playlist.cpp
*
*    DESCRIPTION:     Play list class
************************************************************************/

// Physical component dependency
#include <common/playlist.h>

// Game lib dependencies
#include <script/scriptglobals.h>
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <algorithm>
#include <cstring>

// AngelScript lib dependencies
#include <angelscript.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CPlayList::CPlayList( const std::string strType ) :
    m_counter(0),
    m_current(0),
    m_type( (strType == "random") ? EST_RANDOM : EST_SEQUENTIAL )
      
{
}   // constructor

CPlayList::CPlayList() :
    m_counter(0),
    m_current(0),
    m_type(EST_NULL)
      
{
}   // constructor


/************************************************************************
*    desc:  Copy Constructor
************************************************************************/
CPlayList::CPlayList( const CPlayList & playLst ) :
    m_counter(playLst.m_counter),
    m_current(playLst.m_current),
    m_type(playLst.m_type),
    m_soundVec(playLst.m_soundVec)
{
}   // copy constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CPlayList::~CPlayList()
{
}   // destructor


/************************************************************************
*    desc:  Load  the playlist from node                                                            
************************************************************************/
void CPlayList::LoadFromNode(
    const XMLNode & node,
    const std::string & group,
    std::map< const std::string, CSound > & soundMap )
{
    for( int i = 0; i < node.nChildNode(); ++i )
    {
        // Get the sound list node
        const XMLNode soundNode = node.getChildNode(i);

        // Get the id
        const std::string id = soundNode.getAttribute( "id" );

        // Copy the sound to the playlist
        auto soundIter = soundMap.find( id );
        if( soundIter != soundMap.end() )
        {
            m_soundVec.push_back( soundIter->second );

            // Set the volume if defined
            if( soundNode.isAttributeSet("volume") )
                m_soundVec.back().SetVolume( std::atoi(soundNode.getAttribute( "volume" )) );
        }
        else
        {
            throw NExcept::CCriticalException("Playlist Data Group Load Error!",
                boost::str( boost::format("Playlist sound Id does not exist! (%s - %s).\n\n%s\nLine: %s")
                    % id % group % __FUNCTION__ % __LINE__ ));
        }
    }
                
}   // LoadFromNode


/************************************************************************
*    desc:  Get the sound for the playlist                                                             
************************************************************************/
CSound & CPlayList::GetSound()
{
    // Is it time to shuffle?
    if( (m_type == EST_RANDOM) && (m_counter == 0) )
        Shuffle();

    m_current = m_counter;
    m_counter = (m_counter + 1) % m_soundVec.size();

    return m_soundVec[m_current];

}   // GetSound


/************************************************************************
*    desc:  Play the play list
************************************************************************/
void CPlayList::Play( int channel, int loopCount )
{
    if( (m_type == EST_RANDOM) && (m_counter == 0) )
        Shuffle();

    m_current = m_counter;
    m_soundVec[m_current].Play( channel, loopCount );
    m_counter = (m_counter + 1) % m_soundVec.size();

}   // Play


/************************************************************************
*    desc:  Stop the sound
************************************************************************/
void CPlayList::Stop()
{
    m_soundVec[m_current].Stop();

}   // Stop


/************************************************************************
*    desc:  Pause the sound
************************************************************************/
void CPlayList::Pause()
{
    m_soundVec[m_current].Pause();

}   // Pause


/************************************************************************
*    desc:  Resume the sound
************************************************************************/
void CPlayList::Resume()
{
    m_soundVec[m_current].Resume();

}   // Resume


/************************************************************************
*    desc: Set/Get the volume for music or channel
************************************************************************/
void CPlayList::SetVolume( int volume )
{
    m_soundVec[m_current].SetVolume( volume );
    
}   // SetVolume

int CPlayList::GetVolume()
{
    return m_soundVec[m_current].GetVolume();
    
}   // GetVolume


/************************************************************************
*    desc:  Is music or channel playing?
************************************************************************/
bool CPlayList::IsPlaying()
{
    return m_soundVec[m_current].IsPlaying();

}   // IsPlaying


/************************************************************************
*    desc:  Is music or channel paused?
************************************************************************/
bool CPlayList::IsPaused()
{
    return m_soundVec[m_current].IsPaused();

}   // IsPaused


/************************************************************************
*    desc:  Play the play list
************************************************************************/
void CPlayList::Shuffle()
{
    if( m_soundVec.size() > 2 )
    {
        // Get the last sound that was just played
        CSound oldLastSound(m_soundVec.back());
        
        // Shuffle
        std::random_shuffle( m_soundVec.begin(), m_soundVec.end() );
        
        // Make sure the new first sound is not the old last sound.
        // Don't want the same two sounds playing back to back
        // If it is just stick it in the middle
        if( oldLastSound == m_soundVec.front() )
        {
            int midPos = m_soundVec.size() / 2;
            m_soundVec[0] = m_soundVec[midPos];
            m_soundVec[midPos] = oldLastSound;
        }
    }
    
}   // Shuffle


namespace NScriptPlayLst
{
    /************************************************************************
    *    desc:  Constructor
    ************************************************************************/
    void Constructor(void * thisPointer)
    {
        new(thisPointer) CPlayList();
    }

    /************************************************************************
    *    desc:  Copy Constructor
    ************************************************************************/
    void CopyConstructor(const CPlayList & other, void * pThisPointer)
    {
        new(pThisPointer) CPlayList(other);
    }

    /************************************************************************
    *    desc:  Destructor
    ************************************************************************/
    void Destructor(void * pThisPointer)
    {
        ((CPlayList*)pThisPointer)->~CPlayList();
    }
    
    /************************************************************************
    *    desc:  Register the class with AngelScript
    ************************************************************************/
    void Register( asIScriptEngine * pEngine )
    {
        using namespace NScriptGlobals;

        // Register CScriptComponent2d reference and methods
        Throw( pEngine->RegisterObjectType("CPlayList", sizeof(CPlayList), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS | asOBJ_APP_CLASS_CONSTRUCTOR | asOBJ_APP_CLASS_COPY_CONSTRUCTOR | asOBJ_APP_CLASS_DESTRUCTOR ) );

        // Register the object constructor
        Throw( pEngine->RegisterObjectBehaviour("CPlayList", asBEHAVE_CONSTRUCT, "void f()",                  asFUNCTION(NScriptPlayLst::Constructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPlayList", asBEHAVE_CONSTRUCT, "void f(const CSound & in)", asFUNCTION(NScriptPlayLst::CopyConstructor), asCALL_CDECL_OBJLAST) );
        Throw( pEngine->RegisterObjectBehaviour("CPlayList", asBEHAVE_DESTRUCT,  "void f()",                  asFUNCTION(NScriptPlayLst::Destructor), asCALL_CDECL_OBJLAST) );

        // assignment operator
        Throw( pEngine->RegisterObjectMethod("CPlayList", "CPlayList & opAssign(const CPlayList & in)", asMETHODPR(CPlayList, operator =, (const CPlayList &), CPlayList &), asCALL_THISCALL) );

        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Play( int channel = -1, int loopCount = 0 )", asMETHOD(CPlayList, Play),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Stop()",                                      asMETHOD(CPlayList, Stop),      asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Pause()",                                     asMETHOD(CPlayList, Pause),     asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void Resume()",                                    asMETHOD(CPlayList, Resume),    asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "void SetVolume( int volume )",                     asMETHOD(CPlayList, SetVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "int GetVolume()",                                  asMETHOD(CPlayList, GetVolume), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool IsPlaying()",                                 asMETHOD(CPlayList, IsPlaying), asCALL_THISCALL) );
        Throw( pEngine->RegisterObjectMethod("CPlayList", "bool IsPaused()",                                  asMETHOD(CPlayList, IsPaused),  asCALL_THISCALL) );

    }   // Register
}
