
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
    m_musicTimer.disable( true );
}


/***************************************************************************
*    desc:  Handle update checks
****************************************************************************/
void CBaseGameMusic::update()
{
    m_scriptComponent.update();

    // Fade down the music if the player is not spinning
    if( m_allowSpinMusic && m_musicTimer.expired() )
    {
        m_musicTimer.disable( true );
        m_scriptComponent.prepare( m_group, m_stopMusicFunc );
    }
}


/***************************************************************************
*    desc:  Start the music
****************************************************************************/
void CBaseGameMusic::startMusic()
{
    if( m_allowSpinMusic )
    {
        m_scriptComponent.resetAndRecycle();
        m_scriptComponent.prepare( m_group, m_startMusicFunc );
    }
}


/***************************************************************************
*    desc:  Start the music timeout
****************************************************************************/
void CBaseGameMusic::setTimeOut()
{
    // Set the timer that waits to see if the music should time out
    if( m_allowSpinMusic )
    {
        m_musicTimer.set( m_musicTimeOut );
        m_musicTimer.disable( false );
    }
}


/***************************************************************************
*    desc:  Start the music
****************************************************************************/
void CBaseGameMusic::fastFadeDown()
{
    if( m_allowSpinMusic )
    {
        m_scriptComponent.resetAndRecycle();
        m_scriptComponent.prepare( m_group, m_fastFadeFunc );
    }
}


/************************************************************************
*    desc:  Do we play the music
************************************************************************/
void CBaseGameMusic::allowMusic( bool allow )
{
    m_allowSpinMusic = allow;
}
