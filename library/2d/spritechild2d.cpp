
/************************************************************************
*    FILE NAME:       spritechild2d.cpp
*
*    DESCRIPTION:     Class specific for child sprites where the parent 
*                     does a lot of movement to optimize matrix translations 
*                     for the child and parent.
************************************************************************/

// Physical component dependency
#include <2d/spritechild2d.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSpriteChild2D::CSpriteChild2D( const CObjectData2D & objectData, int id ) :
    CSprite2D( objectData, id )
{
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CSpriteChild2D::~CSpriteChild2D()
{
}   // destructor


/************************************************************************
*    desc:  Transform
************************************************************************/
void CSpriteChild2D::Transform()
{
    m_parameters.Remove( NDefs::WAS_TRANSFORMED );
    
    if( m_parameters.IsSet( NDefs::TRANSFORM ) )
    {
        TransformLocal( m_finalMatrix );
    
        m_transPos = m_pos;
    }

}   // Transform

void CSpriteChild2D::Transform( const CMatrix & matrix, bool tranformWorldPos )
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

}   // Transform


/************************************************************************
*    desc:  do the render                                                            
************************************************************************/
void CSpriteChild2D::Render( const CMatrix & matrix )
{
    if( IsVisible() )
        m_visualComponent.Render( m_finalMatrix, matrix );

}   // Render
