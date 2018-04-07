
/************************************************************************
*    FILE NAME:       playlist.cpp
*
*    DESCRIPTION:     Play list class
************************************************************************/

// Physical component dependency
#include <common/playlist.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <algorithm>
#include <cstring>

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
    if( !m_soundVec.empty() )
    {
        // Is it time to shuffle?
        if( (m_type == EST_RANDOM) && (m_counter == 0) )
            Shuffle();

        m_current = m_counter;
        m_counter = (m_counter + 1) % m_soundVec.size();

        return m_soundVec[m_current];
    }
    
    return m_DummySound;

}   // GetSound


/************************************************************************
*    desc:  Play the play list
************************************************************************/
void CPlayList::Play( int channel, int loopCount )
{
    if( !m_soundVec.empty() )
    {
        if( (m_type == EST_RANDOM) && (m_counter == 0) )
            Shuffle();

        m_current = m_counter;
        m_soundVec[m_current].Play( channel, loopCount );
        m_counter = (m_counter + 1) % m_soundVec.size();
    }

}   // Play


/************************************************************************
*    desc:  Stop the sound
************************************************************************/
void CPlayList::Stop()
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].Stop();

}   // Stop


/************************************************************************
*    desc:  Pause the sound
************************************************************************/
void CPlayList::Pause()
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].Pause();

}   // Pause


/************************************************************************
*    desc:  Resume the sound
************************************************************************/
void CPlayList::Resume()
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].Resume();

}   // Resume


/************************************************************************
*    desc: Set/Get the volume for music or channel
************************************************************************/
void CPlayList::SetVolume( int volume )
{
    if( !m_soundVec.empty() )
        m_soundVec[m_current].SetVolume( volume );
    
}   // SetVolume

int CPlayList::GetVolume()
{
    if( !m_soundVec.empty() )
        return m_soundVec[m_current].GetVolume();
    
    return m_DummySound.GetVolume();
    
}   // GetVolume


/************************************************************************
*    desc:  Is music or channel playing?
************************************************************************/
bool CPlayList::IsPlaying()
{
    if( !m_soundVec.empty() )
        return m_soundVec[m_current].IsPlaying();
    
    return m_DummySound.IsPlaying();

}   // IsPlaying


/************************************************************************
*    desc:  Is music or channel paused?
************************************************************************/
bool CPlayList::IsPaused()
{
    if( !m_soundVec.empty() )
        return m_soundVec[m_current].IsPaused();
    
    return m_DummySound.IsPaused();

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
