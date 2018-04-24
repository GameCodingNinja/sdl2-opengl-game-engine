
/************************************************************************
*    FILE NAME:       symbol2d.cpp
*
*    DESCRIPTION:     Class for rendering a symbol and it's animations
************************************************************************/

// Physical component dependency
#include <slot/symbol2d.h>

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <common/spritedata.h>
#include <common/fontdata.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSymbol2d::CSymbol2d( const std::vector<CSpriteData> & rSpriteDataVec, const std::string & id ) :
    m_id(id),
    m_deferredRender(false)
{
    for( auto & iter : rSpriteDataVec )
    {
        m_spriteDeq.emplace_back( CObjectDataMgr::Instance().getData2D( iter.getGroup(), iter.getObjectName() ) );
        
        m_spriteDeq.back().setVisible( iter.isVisible() );
        
        if( m_spriteDeq.back().getVisualComponent().isFontSprite() )
        {
            m_spriteDeq.back().getVisualComponent().setFontProperties( iter.getFontData()->m_fontProp );
            m_spriteDeq.back().getVisualComponent().createFontString( iter.getFontData()->m_fontString );
        }
        
        m_spriteDeq.back().copyTransform( &iter );
        
        m_spriteDeq.back().copyScriptFunctions( iter.getScriptFunctions() );
    }
        
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CSymbol2d::~CSymbol2d()
{
}   // destructor


/************************************************************************
*    desc:  Get the sprite
************************************************************************/
CSprite2D & CSymbol2d::GetSprite( int index )
{
    return m_spriteDeq.at( index );
}


/************************************************************************
*    desc:  Get the sprite
************************************************************************/
size_t CSymbol2d::GetSpriteCount()
{
    return m_spriteDeq.size();
}


/************************************************************************
*    desc:  Get the string id
************************************************************************/
const std::string & CSymbol2d::GetId()
{
    return m_id;
}


/************************************************************************
*    desc:  Update the symbol
************************************************************************/
void CSymbol2d::Update()
{
    for( auto & iter : m_spriteDeq )
        iter.update();

}   // Update


/************************************************************************
*    desc:  Transform
************************************************************************/
void CSymbol2d::transform()
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
    {
        transformLocal( m_finalMatrix );
    
        m_transPos = m_pos;
    }
    
    for( auto & iter : m_spriteDeq )
        iter.transform( m_finalMatrix, wasWorldPosTranformed() );

}   // Transform

void CSymbol2d::transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
        transformLocal( m_matrix );
        
    if( m_parameters.IsSet( NDefs::WAS_TRANSFORMED ) || tranformWorldPos )
    {
        m_parameters.Add( NDefs::WAS_TRANSFORMED );

        m_finalMatrix.Copy( m_matrix );

        m_finalMatrix.MergeMatrix( matrix );
    }

    for( auto & iter : m_spriteDeq )
        iter.transform( m_finalMatrix, wasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  do the render
************************************************************************/
void CSymbol2d::Render( const CMatrix & matrix )
{
    for( auto & iter : m_spriteDeq )
        iter.render( matrix );

}   // Render


/************************************************************************
*    desc:  Get/Set if deferred render
************************************************************************/
bool CSymbol2d::IsDeferredRender()
{
    return m_deferredRender;
}

void CSymbol2d::SetDeferredRender( bool value )
{
    m_deferredRender = value;
}
