
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
        m_spriteDeq.emplace_back( CObjectDataMgr::Instance().GetData2D( iter.GetGroup(), iter.GetObjectName() ) );
        
        m_spriteDeq.back().SetVisible( iter.IsVisible() );
        
        if( m_spriteDeq.back().GetVisualComponent().IsFontSprite() )
        {
            m_spriteDeq.back().GetVisualComponent().SetFontProperties( iter.GetFontData()->m_fontProp );
            m_spriteDeq.back().GetVisualComponent().CreateFontString( iter.GetFontData()->m_fontString );
        }
        
        m_spriteDeq.back().CopyTransform( &iter );
        
        m_spriteDeq.back().CopyScriptFunctions( iter.GetScriptFunctions() );
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
        iter.Update();

}   // Update


/************************************************************************
*    desc:  Transform
************************************************************************/
void CSymbol2d::Transform()
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
    {
        TransformLocal( m_finalMatrix );
    
        m_transPos = m_pos;
    }
    
    for( auto & iter : m_spriteDeq )
        iter.Transform( m_finalMatrix, WasWorldPosTranformed() );

}   // Transform

void CSymbol2d::Transform( const CMatrix & matrix, bool tranformWorldPos )
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
        TransformLocal( m_matrix );
        
    if( m_parameters.IsSet( NDefs::WAS_TRANSFORMED ) || tranformWorldPos )
    {
        m_parameters.Add( NDefs::WAS_TRANSFORMED );

        m_finalMatrix.Copy( m_matrix );

        m_finalMatrix.MergeMatrix( matrix );
    }

    for( auto & iter : m_spriteDeq )
        iter.Transform( m_finalMatrix, WasWorldPosTranformed() );
    
}   // Transform


/************************************************************************
*    desc:  do the render
************************************************************************/
void CSymbol2d::Render( const CMatrix & matrix )
{
    for( auto & iter : m_spriteDeq )
        iter.Render( matrix );

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
