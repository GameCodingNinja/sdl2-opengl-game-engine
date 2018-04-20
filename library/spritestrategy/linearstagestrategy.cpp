
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
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CLinearStageStrategy::~CLinearStageStrategy()
{
}   // destructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CLinearStageStrategy::LoadFromNode( const XMLNode & node )
{
    CBasicStageStrategy::LoadFromNode( node );

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
            if( (iter.GetPos().x.i == startSector.x.i) && (iter.GetPos().y.i == startSector.y.i) )
                break;

            ++m_startIndex;
        }
    }

    // Init the range of sectors to check
    InitRange();

}   // LoadFromNode


/************************************************************************
*    desc:  Init the range of sectors to check
************************************************************************/
void CLinearStageStrategy::InitRange()
{
    m_firstIndex = m_startIndex;

    m_lastIndex = m_startIndex +
        (CSettings::Instance().GetDefaultSize().w /
        CSettings::Instance().GetSectorSize()) + 1;

}   // InitRange


/***************************************************************************
*    desc:  Set the range based on the sector's visibility
****************************************************************************/
bool CLinearStageStrategy::SetRange( const size_t index )
{
    if( !m_sectorDeq.at(index).InView() )
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

}   // SetRange


/***************************************************************************
*    desc:  Update the actors
****************************************************************************/
void CLinearStageStrategy::Update()
{
    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).Update();

}   // Update


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CLinearStageStrategy::Transform()
{
    size_t tmpFirstIndex = m_firstIndex;

    for( size_t i = m_firstIndex; i < m_sectorDeq.size(); ++i )
    {
        m_sectorDeq.at(i).Transform();

        if( SetRange(i) )
            break;

        // Going backwards will decrement to an index that
        // hasn't been transformed so do that now
        if( m_firstIndex < tmpFirstIndex )
        {
            tmpFirstIndex = m_firstIndex;
            m_sectorDeq.at(m_firstIndex).Transform();
        }
    }

}   // Transform

void CLinearStageStrategy::Transform( const CObject2D & object )
{
    size_t tmpFirstIndex = m_firstIndex;

    for( size_t i = m_firstIndex; i < m_sectorDeq.size(); ++i )
    {
        m_sectorDeq.at(i).Transform( object );

        if( SetRange(i) )
            break;

        // Going backwards will decrement to an index that
        // hasn't been transformed so do that now
        if( m_firstIndex < tmpFirstIndex )
        {
            tmpFirstIndex = m_firstIndex;
            m_sectorDeq.at(m_firstIndex).Transform( object );
        }
    }

}   // Transform


/***************************************************************************
*    desc:  Render the actors
****************************************************************************/
void CLinearStageStrategy::Render( const CMatrix & matrix )
{
    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).Render( matrix );

}   // Render

void CLinearStageStrategy::Render()
{
    auto & camera = CCameraMgr::Instance().GetCamera( m_cameraId );

    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).Render( camera );

}   // Render
