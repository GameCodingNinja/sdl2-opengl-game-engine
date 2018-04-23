
/************************************************************************
*    FILE NAME:       uiprogressbar.h
*
*    DESCRIPTION:     Class for user interface progress bar
************************************************************************/

#ifndef __ui_progress_bar_h__
#define __ui_progress_bar_h__

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <common/defs.h>
#include <common/size.h>

// Standard lib dependencies
#include <memory>

class CUIProgressBar : public CUIControl
{
public:

    // Constructor
    CUIProgressBar( const std::string & group );

    // Destructor
    virtual ~CUIProgressBar();
    
    // Load the initial info from XML node
    void loadFromNode( const XMLNode & node ) override;
    
    // Load the control specific info from XML node
    void loadControlFromNode( const XMLNode & controlNode ) override;
    
    // Transform the control
    void transform( const CObject2D & object ) override;
    
    // do the render
    void render( const CMatrix & matrix ) override;
    
    // Calculate the progress bar size and position
    void setSizePos();
    
    // Set/Get current value
    void setCurValue( float value );
    float getCurValue();
    void incCurValue( float value );
    
    // Set/Get min value
    void setMinValue( float value );
    float getMinValue();
    
    // Set/Get max value
    void setMaxValue( float value );
    float getMaxValue();
    
private:
    
    // stencil mask sprite
    std::unique_ptr<CSprite2D> m_upStencilMaskSprite;
    
    // current value of progress bar
    float m_curValue;
    
    // Minimum value
    float m_minValue;

    // Max value of progress bar
    float m_maxValue;
    
    // Sprite index to apply stencil mask to
    int m_spriteApplyIndex;
    
    // Orentation
    NDefs::EOrentation m_orentation;
    
    // progress bar size
    CSize<float> m_size;
    
    // progress bar pos
    CPoint<float> m_pos;
    
    // progress bar scale
    CPoint<float> m_scale;
    
    // alignment
    union UAlignment
    {
        UAlignment() : horz(NDefs::EHA_HORZ_LEFT){}
        NDefs::EHorzAlignment horz;
        NDefs::EVertAlignment vert;
    } m_alignment;

};

#endif  // __ui_progress_bar_h__


