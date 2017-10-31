
/************************************************************************
*    FILE NAME:       uiprogressbar.cpp
*
*    DESCRIPTION:     Class for user interface progress bar
************************************************************************/

// Physical component dependency
#include <gui/uiprogressbar.h>

// Game lib dependencies
#include <2d/sprite2d.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>

// Boost lib dependencies

// Standard lib dependencies
#include <cstring>


/************************************************************************
*    desc:  Constructor
************************************************************************/
CUIProgressBar::CUIProgressBar( const std::string & group ) :
    CUIControl( group ),
    m_curValue(0),
    m_minValue(0),
    m_maxValue(100),
    m_spriteApplyIndex(0),
    m_orentation(NDefs::EO_HORZ)
{
    m_type = NUIControl::ECT_PROGRESS_BAR;
    
}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CUIProgressBar::~CUIProgressBar()
{
}   // destructor


/************************************************************************
*    desc:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIProgressBar::LoadFromNode( const XMLNode & node )
{
    CUIControl::LoadFromNode( node );
            
    // See if a range of values was specified
    const XMLNode rangeNode = node.getChildNode( "range" );
    if( !rangeNode.isEmpty() )
    {
        if( rangeNode.isAttributeSet( "cur" ) )
            m_curValue = std::atof( rangeNode.getAttribute( "cur" ) );
        
        if( rangeNode.isAttributeSet( "min" ) )
            m_minValue = std::atof( rangeNode.getAttribute( "min" ) );
        
        if( rangeNode.isAttributeSet( "max" ) )
            m_maxValue = std::atof( rangeNode.getAttribute( "max" ) );
    }
    
    const XMLNode orentNode = node.getChildNode( "orentation" );
    if( !orentNode.isEmpty() )
    {
        if( orentNode.isAttributeSet("type") )
            if( std::strcmp( orentNode.getAttribute("type"), "vert") == 0 )
                m_orentation = NDefs::EO_VERT;
        
        if( orentNode.isAttributeSet("alignment") )
        {
            const char * pAlign = orentNode.getAttribute("alignment");
            
            if( m_orentation == NDefs::EO_HORZ )
            {
                if( std::strcmp( pAlign, "right") == 0 )
                    m_alignment.horz = NDefs::EHA_HORZ_RIGHT;
                
                else if( std::strcmp( pAlign, "center") == 0 )
                    m_alignment.horz = NDefs::EHA_HORZ_CENTER;
            }
            else
            {
                if( std::strcmp( pAlign, "bottom") == 0 )
                    m_alignment.vert = NDefs::EVA_VERT_BOTTOM;
                
                else if( std::strcmp( pAlign, "center") == 0 )
                    m_alignment.vert = NDefs::EVA_VERT_CENTER;
            }
        }
    }
    
    // Calculate the progress bar size and position
    SetSizePos();
    
}   // LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
*  
*    param: controlNode - XML node
************************************************************************/
void CUIProgressBar::LoadControlFromNode( const XMLNode & controlNode )
{
    CUIControl::LoadControlFromNode( controlNode );
            
    // Get the stencil mask node
    const XMLNode stencilMaskNode = controlNode.getChildNode( "stencilMask" );
    if( !stencilMaskNode.isEmpty() )
    {
        const std::string objectName = stencilMaskNode.getAttribute( "objectName" );
        
        m_spriteApplyIndex = std::atoi(stencilMaskNode.getAttribute( "spriteIndex" ));

        if( !objectName.empty() )
        {
            m_upStencilMaskSprite.reset( new CSprite2D( CObjectDataMgr::Instance().GetData2D( GetGroup(), objectName ) ) );

            // Load the transform data
            m_upStencilMaskSprite->LoadTransFromNode( stencilMaskNode );
            
            // Get the size
            m_size = m_upStencilMaskSprite->GetObjectData().GetSize();
            
            // Get the initial position
            m_pos = m_upStencilMaskSprite->GetPos();
            
            // Get the initial scale
            m_scale = m_upStencilMaskSprite->GetScale();
        }
        else
        {
            // Get the size
            m_size = m_spriteDeq.at(m_spriteApplyIndex).GetObjectData().GetSize();
            
            // Get the initial position
            m_pos = m_spriteDeq.at(m_spriteApplyIndex).GetPos();
            
            // Get the initial scale
            m_scale = m_spriteDeq.at(m_spriteApplyIndex).GetScale();
        }
    }
    
}   // LoadControlFromNode


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIProgressBar::Transform( const CObject2D & object )
{
    CUIControl::Transform( object );
    
    if( m_upStencilMaskSprite )
        m_upStencilMaskSprite->Transform( GetMatrix(), WasWorldPosTranformed() );
    
}   // DoTransform


/************************************************************************
*    desc:  do the render
************************************************************************/
void CUIProgressBar::Render( const CMatrix & matrix )
{ 
    if( m_upStencilMaskSprite )
    {
        for( size_t i  = 0; i < m_spriteDeq.size(); ++i )
        {
            if( static_cast<int>(i) == m_spriteApplyIndex )
            {
                // Disable rendering to the color buffer
                glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
                glDepthMask( GL_FALSE );

                // Start using the stencil
                glEnable( GL_STENCIL_TEST );

                glStencilFunc( GL_ALWAYS, 0x1, 0x1 );
                glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );


                m_upStencilMaskSprite->Render( matrix );


                // Re-enable color
                glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

                // Where a 1 was not rendered
                glStencilFunc( GL_EQUAL, 0x1, 0x1 );

                // Keep the pixel
                glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

                // Disable any writing to the stencil buffer
                glDepthMask(GL_TRUE);
                
                m_spriteDeq[i].Render( matrix );
                
                // Finished using stencil
                glDisable( GL_STENCIL_TEST );
            }
            else
                m_spriteDeq[i].Render( matrix );
        }
    }
    else
    {
        CUIControl::Render( matrix );
    }

}   // Render


/************************************************************************
*    desc:  Calculate the progress bar size and position
************************************************************************/
void CUIProgressBar::SetSizePos()
{
    CPoint<float> scale(m_scale);
    CPoint<float> pos(m_pos);
    
    // Calculate the new scale for the progress bar
    float scaler = (m_curValue - m_minValue) / (m_maxValue - m_minValue);
    
    if( m_orentation == NDefs::EO_HORZ )
    {
        scale.x *= scaler;
        float offset = m_size.w * scaler;
        
        if( m_alignment.horz == NDefs::EHA_HORZ_LEFT )
            pos.x -= (m_size.w - offset) / 2;

        else if( m_alignment.horz == NDefs::EHA_HORZ_RIGHT )
            pos.x += (m_size.w - offset) / 2;
    }
    else
    {
        scale.y *= scaler;
        float offset = m_size.h * scaler;
        
        if( m_alignment.vert == NDefs::EVA_VERT_TOP )
            pos.y += (m_size.h - offset) / 2;

        else if( m_alignment.vert == NDefs::EVA_VERT_BOTTOM )
            pos.y -= (m_size.h - offset) / 2;
    }
    
    if( m_upStencilMaskSprite )
    {
        m_upStencilMaskSprite->SetScale( scale );
        m_upStencilMaskSprite->SetPos( pos );
    }
    else
    {
        m_spriteDeq.at(m_spriteApplyIndex).SetScale( scale );
        m_spriteDeq.at(m_spriteApplyIndex).SetPos( pos );
    }
    
}   // SetSizePos


/************************************************************************
*    desc:  Set/Get/Inc current value
************************************************************************/
void CUIProgressBar::SetCurValue( float value )
{
    m_curValue = value;

}   // SetCurValue

float CUIProgressBar::GetCurValue()
{
    return m_curValue;

}   // GetCurValue

void CUIProgressBar::IncCurValue( float value )
{
    m_curValue += value;
    
}   // IncCurValue


/************************************************************************
*    desc:  Set/Get min value
************************************************************************/
void CUIProgressBar::SetMinValue( float value )
{
    m_minValue = value;

}   // SetMinValue

float CUIProgressBar::GetMinValue()
{
    return m_minValue;

}   // GetMinValue


/************************************************************************
*    desc:  Set/Get max value
************************************************************************/
void CUIProgressBar::SetMaxValue( float value )
{
    m_maxValue = value;

}   // SetMaxValue

float CUIProgressBar::GetMaxValue()
{
    return m_maxValue;

}   // GetMaxValue
