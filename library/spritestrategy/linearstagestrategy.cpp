
/************************************************************************
*    FILE NAME:       linearstagestrategy.h
*
*    DESCRIPTION:     Linear stage strategy
*                     Strategy is optimized to move in a linear fashion
************************************************************************/

// Physical component dependency
#include <spritestrategy/linearstagestrategy.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/settings.h>
#include <managers/cameramanager.h>
#include <spritestrategy/sector.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLinearStageStrategy::CLinearStageStrategy() :
    m_startIndex(0),
    m_dirType(ESD_NULL)
{
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CLinearStageStrategy::~CLinearStageStrategy()
{
}


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CLinearStageStrategy::loadFromNode( const XMLNode & node )
{
    CBasicStageStrategy::loadFromNode( node );

    const XMLNode linearStageNode = node.getChildNode( "linearStage" );
    if( !linearStageNode.isEmpty() )
    {
        if( std::strcmp(linearStageNode.getAttribute( "direction" ), "forward") == 0 )
            m_dirType = ESD_FORWARD;
        else
            m_dirType = ESD_BACKWARD;

        // Get the sector to use to find the index
        const CPoint<CWorldValue> startSector = NParseHelper::LoadPosition( linearStageNode );

        // Find the index of the desired sector
        for( auto & iter : m_sectorDeq )
        {
            if( (iter.getPos().x.i == startSector.x.i) && (iter.getPos().y.i == startSector.y.i) )
                break;

            ++m_startIndex;
        }
    }

    // Init the range of sectors to check
    initRange();
}


/************************************************************************
*    desc:  Init the range of sectors to check
************************************************************************/
void CLinearStageStrategy::initRange()
{
    m_firstIndex = m_startIndex;

    m_lastIndex = m_startIndex +
        (CSettings::Instance().getDefaultSize().w /
        CSettings::Instance().getSectorSize()) + 1;
}


/***************************************************************************
*    desc:  Set the range based on the sector's visibility
****************************************************************************/
bool CLinearStageStrategy::setRange( const size_t index )
{
    if( !m_sectorDeq.at(index).inView() )
    {
        if( m_dirType == ESD_FORWARD )
        {
            if( (index == m_firstIndex) && (m_firstIndex < m_sectorDeq.size() - 1) )
            {
                ++m_firstIndex;
            }
        }

        // The last index that can't be seen needs to be saved
        if( index > m_firstIndex )
        {
            m_lastIndex = index;
            return true;
        }
    }
    else
    {
        if( m_dirType == ESD_BACKWARD )
        {
            if( (index == m_firstIndex) && (m_firstIndex > 0) )
            {
                --m_firstIndex;
            }
        }
    }

    return false;
}


/***************************************************************************
*    desc:  Update the actors
****************************************************************************/
void CLinearStageStrategy::update()
{
    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).update();
}


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CLinearStageStrategy::transform()
{
    size_t tmpFirstIndex = m_firstIndex;

    for( size_t i = m_firstIndex; i < m_sectorDeq.size(); ++i )
    {
        m_sectorDeq.at(i).transform();

        if( setRange(i) )
            break;

        // Going backwards will decrement to an index that
        // hasn't been transformed so do that now
        if( m_firstIndex < tmpFirstIndex )
        {
            tmpFirstIndex = m_firstIndex;
            m_sectorDeq.at(m_firstIndex).transform();
        }
    }
}

void CLinearStageStrategy::transform( const CObject2D & object )
{
    size_t tmpFirstIndex = m_firstIndex;

    for( size_t i = m_firstIndex; i < m_sectorDeq.size(); ++i )
    {
        m_sectorDeq.at(i).transform( object );

        if( setRange(i) )
            break;

        // Going backwards will decrement to an index that
        // hasn't been transformed so do that now
        if( m_firstIndex < tmpFirstIndex )
        {
            tmpFirstIndex = m_firstIndex;
            m_sectorDeq.at(m_firstIndex).transform( object );
        }
    }
}


/***************************************************************************
*    desc:  Render the actors
****************************************************************************/
void CLinearStageStrategy::render( const CMatrix & matrix )
{
    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).render( matrix );
}

void CLinearStageStrategy::render()
{
    auto & camera = CCameraMgr::Instance().getCamera( m_cameraId );

    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).render( camera );
}
