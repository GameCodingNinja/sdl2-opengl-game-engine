
/************************************************************************
*    FILE NAME:       linearstagestrategy2d.h
*
*    DESCRIPTION:     Linear 2D stage strategy
*                     Strategy is optimized to move in a linear fassion
************************************************************************/

// Physical component dependency
#include <2d/linearstagestrategy2d.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/settings.h>
#include <2d/sector2d.h>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CLinearStageStrategy2D::CLinearStageStrategy2D() :
    m_startIndex(0),
    m_dirType(ESD_NULL)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CLinearStageStrategy2D::~CLinearStageStrategy2D()
{
}   // destructor


/************************************************************************
*    desc:  Load thes object data from node
************************************************************************/
void CLinearStageStrategy2D::LoadFromNode( const XMLNode & node )
{
    CBasicStageStrategy2D::LoadFromNode( node );

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
void CLinearStageStrategy2D::InitRange()
{
    m_firstIndex = m_startIndex;

    m_lastIndex = m_startIndex +
        (CSettings::Instance().GetDefaultSize().w /
        CSettings::Instance().GetSectorSize()) + 1;

}   // InitRange


/***************************************************************************
*    desc:  Set the range based on the sector's visibility
****************************************************************************/
bool CLinearStageStrategy2D::SetRange( const size_t index )
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
void CLinearStageStrategy2D::Update()
{
    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).Update();

}   // Update


/************************************************************************
*    desc:  Transform the actor
************************************************************************/
void CLinearStageStrategy2D::Transform()
{
    size_t tmpFirstIndex = m_firstIndex;

    for( size_t i = m_firstIndex; i < m_sectorDeq.size(); ++i )
    {
        m_sectorDeq.at(i).DoTransform();

        if( SetRange(i) )
            break;

        // Going backwards will decrement to an index that
        // hasn't been transformed so do that now
        if( m_firstIndex < tmpFirstIndex )
        {
            tmpFirstIndex = m_firstIndex;
            m_sectorDeq.at(m_firstIndex).DoTransform();
        }
    }

}   // Transform

void CLinearStageStrategy2D::Transform( const CObject2D & object )
{
    size_t tmpFirstIndex = m_firstIndex;

    for( size_t i = m_firstIndex; i < m_sectorDeq.size(); ++i )
    {
        m_sectorDeq.at(i).DoTransform( object );

        if( SetRange(i) )
            break;

        // Going backwards will decrement to an index that
        // hasn't been transformed so do that now
        if( m_firstIndex < tmpFirstIndex )
        {
            tmpFirstIndex = m_firstIndex;
            m_sectorDeq.at(m_firstIndex).DoTransform( object );
        }
    }

}   // Transform


/***************************************************************************
*    desc:  Render the actors
****************************************************************************/
void CLinearStageStrategy2D::Render( const CMatrix & matrix )
{
    for( size_t i = m_firstIndex; i < m_lastIndex; ++i )
        m_sectorDeq.at(i).Render( matrix );

}   // Render
