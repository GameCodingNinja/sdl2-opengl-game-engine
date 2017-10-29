
/************************************************************************
*    FILE NAME:       basegamemusic.cpp
*
*    DESCRIPTION:     Class for handling base game music
************************************************************************/

// Physical component dependency
#include <slot/basegamemusic.h>

// Game lib dependencies
#include <managers/soundmanager.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CBaseGameMusic::CBaseGameMusic(
    const std::string & group,
    const std::string & startMusicFunc,
    const std::string & stopMusicFunc,
    const std::string & fastFadeFunc,
    int musicTimeOut ) :
        m_group( group ),
        m_startMusicFunc( startMusicFunc ),
        m_stopMusicFunc( stopMusicFunc ),
        m_fastFadeFunc( fastFadeFunc ),
        m_musicTimeOut( musicTimeOut ),
        m_allowSpinMusic( true )
{
    m_musicTimer.Disable( true );
    
}   // constructor


/***************************************************************************
*    desc:  Handle update checks
****************************************************************************/
void CBaseGameMusic::Update()
{
    m_scriptComponent.Update();
    
    // Fade down the music if the player is not spinning
    if( m_allowSpinMusic && m_musicTimer.Expired() )
    {
        m_musicTimer.Disable( true );
        m_scriptComponent.Prepare( m_group, m_stopMusicFunc );
    }
    
}   // Update


/***************************************************************************
*    desc:  Start the music
****************************************************************************/
void CBaseGameMusic::StartMusic()
{
    if( m_allowSpinMusic )
    {
        m_scriptComponent.ResetAndRecycle();
        m_scriptComponent.Prepare( m_group, m_startMusicFunc );
    }
    
}   // StartMusic


/***************************************************************************
*    desc:  Start the music timeout
****************************************************************************/
void CBaseGameMusic::SetTimeOut()
{
    // Set the timer that waits to see if the music should time out
    if( m_allowSpinMusic )
    {
        m_musicTimer.Set( m_musicTimeOut );
        m_musicTimer.Disable( false );
    }
    
}   // SetTimeOut


/***************************************************************************
*    desc:  Start the music
****************************************************************************/
void CBaseGameMusic::FastFadeDown()
{
    if( m_allowSpinMusic )
    {
        m_scriptComponent.ResetAndRecycle();
        m_scriptComponent.Prepare( m_group, m_fastFadeFunc );
    }
    
}   // ForceFadeDown


/************************************************************************
*    desc:  Do we play the music
************************************************************************/
void CBaseGameMusic::AllowMusic( bool allow )
{
    m_allowSpinMusic = allow;
}