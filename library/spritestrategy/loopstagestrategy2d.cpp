
/************************************************************************
*    FILE NAME:       loopstagestrategy2d.h
*
*    DESCRIPTION:     Loop 2D stage strategy
*                     Strategy is optimized to move in a looping fassion
************************************************************************/

// Physical component dependency
#include <spritestrategy/loopstagestrategy2d.h>

// Game lib dependencies
#include <utilities/highresolutiontimer.h>
#include <utilities/xmlparsehelper.h>
#include <spritestrategy/sector2d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLoopStageStrategy2D::CLoopStageStrategy2D()
{
    //m_loopEnd.x.i = -1;
    //m_loopInc.x = -0.05f;

    //m_loopOffset.SetPos( m_loopStart );

}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CLoopStageStrategy2D::~CLoopStageStrategy2D()
{
}   // destructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CLoopStageStrategy2D::LoadFromNode( const XMLNode & node )
{
    CLinearStageStrategy2D::LoadFromNode( node );

    const XMLNode loopStageNode = node.getChildNode( "loopingStage" );

    // Load the start position
    const XMLNode startNode = loopStageNode.getChildNode( "start" );
    if( !startNode.isEmpty() )
        m_loopStart = -NParseHelper::LoadPosition( startNode );

    // Load the end position
    const XMLNode endNode = loopStageNode.getChildNode( "end" );
    if( !endNode.isEmpty() )
        m_loopEnd = -NParseHelper::LoadPosition( endNode );

    // Load the inc position
    const XMLNode incNode = loopStageNode.getChildNode( "inc" );
    if( !incNode.isEmpty() )
        m_loopInc = -NParseHelper::LoadPosition( incNode );
    
    // Init the offset
    m_loopOffset.SetPos( m_loopStart );

}   // LoadFromNode


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CLoopStageStrategy2D::Transform()
{
    if( m_dirType > ESD_NULL )
    {
        m_loopOffset.IncPos( m_loopInc * (float)CHighResTimer::Instance().GetElapsedTime() );

        if( m_dirType == ESD_FORWARD )
        {
            if( m_loopOffset.GetPos() < m_loopEnd )
            {
                m_loopOffset.SetPos( m_loopStart + (m_loopOffset.GetPos() - m_loopEnd) );
                InitRange();
            }
        }
        else if( m_dirType == ESD_BACKWARD )
        {
            if( m_loopOffset.GetPos() > m_loopEnd )
            {
                m_loopOffset.SetPos( m_loopStart + (m_loopOffset.GetPos() - m_loopEnd) );
                InitRange();
            }
        }

        m_loopOffset.Transform();

        CLinearStageStrategy2D::Transform( m_loopOffset );
    }
    else
        CLinearStageStrategy2D::Transform();

}   // Transform
