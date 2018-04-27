
/************************************************************************
*    FILE NAME:       uiscrollbox.cpp
*
*    DESCRIPTION:     Class for user interface scroll boxes
************************************************************************/

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <GL/glew.h>     // Glew dependencies (have to be defined first)
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include <gui/uiscrollbox.h>

// Game lib dependencies
#include <2d/sprite2d.h>
#include <common/size.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/deletefuncs.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <gui/uicontrolfactory.h>
#include <gui/uislider.h>
#include <gui/messagecracker.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CUIScrollBox::CUIScrollBox( const std::string & group ) :
    CUISubControl( group ),
    IN_VIEWABLE_AREA(1),
    NEW_ACTIVE_CTRL(2),
    m_cullHeight(0),
    m_controlHeight(0),
    m_scrollCurPos(0),
    m_visibleCount(0),
    m_visStartPos(0),
    m_visEndPos(0),
    m_maxMoveAmount(0),
    m_activeScrollCtrl(NUIControl::NO_ACTIVE_CONTROL),
    m_firstScrollCtrlIndex(0),
    m_scrollSpeed(0.05),
    m_pageSpeed(0.05),
    m_scrollVector(0),
    m_paging(false),
    m_scrollCounter(0),
    m_scrollDistance(0),
    m_endScroll(false),
    m_scrollMsg(false),
    m_endScrollSelection(false)
{
    m_type = NUIControl::ECT_SCROLL_BOX;
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CUIScrollBox::~CUIScrollBox()
{
    NDelFunc::DeleteVectorPointers(m_pScrollControlVec);
}


/************************************************************************
*    DESC:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIScrollBox::loadFromNode( const XMLNode & node )
{
    CUISubControl::loadFromNode( node );

    // Init the slider
    NGenFunc::DynCast<CUISlider>(getSubControl())->setMaxValue(m_maxMoveAmount);
    NGenFunc::DynCast<CUISlider>(getSubControl())->setSlider();

    // Get the scrolling info
    const XMLNode scrollNode = node.getChildNode( "scroll" );
    if( !scrollNode.isEmpty() )
    {
        if( scrollNode.isAttributeSet( "scrollSpeed" ) )
            m_scrollSpeed = std::atof( scrollNode.getAttribute( "scrollSpeed" ) );

        if( scrollNode.isAttributeSet( "pageSpeed" ) )
            m_pageSpeed = std::atof( scrollNode.getAttribute( "pageSpeed" ) );
    }

    // Calc the start and end positions of what should
    // be viewable in the scroll box
    setStartEndPos();
}


/************************************************************************
*    DESC:  Load the control specific info from XML node
************************************************************************/
void CUIScrollBox::loadControlFromNode( const XMLNode & node )
{
    // Call the parent
    CUISubControl::loadControlFromNode( node );

    // Get the menu controls node
    const XMLNode menuControlsNode = node.getChildNode( "scrollBoxControlList" );
    if( !menuControlsNode.isEmpty() )
    {
        // Get the initial offset of the first control in the scroll box
        m_initialOffset = NParseHelper::LoadPosition( menuControlsNode );

        // Get the scroll boc info node
        const XMLNode controlInfoNode = menuControlsNode.getChildNode( "controlInfo" );
        m_controlHeight = std::atoi( controlInfoNode.getAttribute( "height" ) );
        m_visibleCount = std::atoi( controlInfoNode.getAttribute( "visibleInScrollBox" ) );

        // Get the number of controls in this scroll box
        const int scrollControlCount = menuControlsNode.nChildNode( "control" );

        m_pScrollControlVec.reserve( scrollControlCount );

        // Add the scroll control from node
        for( int i = 0; i < scrollControlCount; ++i )
            addScrollControlFromNode( menuControlsNode.getChildNode( "control", i ) );
    }

    // Get the stencil mask node
    const XMLNode stencilMaskNode = node.getChildNode( "stencilMask" );
    if( !stencilMaskNode.isEmpty() )
    {
        const std::string objectName = stencilMaskNode.getAttribute( "objectName" );

        m_upStencilMaskSprite.reset( new CSprite2D( CObjectDataMgr::Instance().getData2D( getGroup(), objectName ) ) );

        // Get the cull height
        m_cullHeight = (m_upStencilMaskSprite->getObjectData().getSize().getW() + m_controlHeight) / 2;

        // Load the transform data
        m_upStencilMaskSprite->loadTransFromNode( stencilMaskNode );
    }
}


/************************************************************************
*    DESC:  Add the scroll control from node
*           NOTE: This function recalulates the scroll box members because
*                 it is also used for run-time dynamic scroll boxes
************************************************************************/
CUIControl * CUIScrollBox::addScrollControlFromNode( const XMLNode & node )
{
    // The pointer is placed within a vector for all controls
    CUIControl * pCtrl = NUIControlFactory::Create( node, getGroup() );
    m_pScrollControlVec.push_back( pCtrl );

    // Get the position for this control
    CPoint<float> pos( m_initialOffset - CPoint<float>(0, m_controlHeight * (m_pScrollControlVec.size()-1), 0) );

    // Record the default y offset
    m_defaultOffsetVec.push_back( pos.y );

    // Set the position
    pCtrl->setPos( pos );

    // Init the control visual state
    pCtrl->deactivateControl();

    // Calculate the maximum scroll amount in pixels
    if( m_pScrollControlVec.size() > static_cast<uint>(m_visibleCount) )
        m_maxMoveAmount = (static_cast<int>(m_pScrollControlVec.size()) - m_visibleCount) * m_controlHeight;

    return pCtrl;
}


/************************************************************************
*    DESC:  Init the control
************************************************************************/
void CUIScrollBox::init()
{
    CUISubControl::init();

    // Init all controls
    for( auto iter : m_pScrollControlVec )
        iter->init();
}


/************************************************************************
*    DESC:  Do some cleanup
************************************************************************/
void CUIScrollBox::cleanUp()
{
    CUISubControl::cleanUp();

    // Init all controls
    for( auto iter : m_pScrollControlVec )
        iter->cleanUp();
}


/************************************************************************
*    DESC:  Handle events
************************************************************************/
void CUIScrollBox::handleEvent( const SDL_Event & rEvent )
{
    // Call the parent
    CUISubControl::handleEvent( rEvent );

    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->handleEvent( rEvent );
}


/************************************************************************
*    DESC:  Handle OnUpAction message
************************************************************************/
void CUIScrollBox::onUpAction( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        handleKeyboardGamepadScroll( -1 );

    else if( rEvent.user.code == NDefs::EAP_UP )
        m_endScroll = true;
}

/************************************************************************
*    DESC:  Handle OnDownAction message
************************************************************************/
void CUIScrollBox::onDownAction( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        handleKeyboardGamepadScroll( 1 );

    else if( rEvent.user.code == NDefs::EAP_UP )
        m_endScroll = true;
}

/************************************************************************
*    DESC:  Handle OnUpScroll message
************************************************************************/
void CUIScrollBox::onUpScroll( const SDL_Event & rEvent )
{
    handleKeyboardGamepadScroll( -1 );
    m_scrollMsg = true;
}

/************************************************************************
*    DESC:  Handle OnDownScroll message
************************************************************************/
void CUIScrollBox::onDownScroll( const SDL_Event & rEvent )
{
    handleKeyboardGamepadScroll( 1 );
    m_scrollMsg = true;
}

/************************************************************************
*    DESC:  Handle OnTabLeft message
************************************************************************/
void CUIScrollBox::onTabLeft( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        handlePageScroll( -1 );
}

/************************************************************************
*    DESC:  Handle OnTabRight message
************************************************************************/
void CUIScrollBox::onTabRight( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        handlePageScroll( 1 );
}


/************************************************************************
*    DESC:  Handle the mouse move
************************************************************************/
bool CUIScrollBox::onMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::onMouseMove( rEvent );

    // Invalidate the active control
    m_activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

    if( NGenFunc::DynCast<CUISlider>(getSubControl())->isMouseDown() )
    {
        // Get the current scroll position
        m_scrollCurPos = NGenFunc::DynCast<CUISlider>(getSubControl())->getValue();

        // Set the bounds
        setStartEndPos();

        // Reposition the scroll controls
        repositionScrollControls();
    }

    return result;
}


/************************************************************************
*    DESC:  Update the control
************************************************************************/
void CUIScrollBox::update()
{
    // Call the parent
    CUISubControl::update();

    // Update all controls
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->update();

    // Handle any scrolling
    handleScrollUpdate();
}


/************************************************************************
*    DESC:  Transform the control
************************************************************************/
void CUIScrollBox::transform( const CObject2D & object )
{
    // Call the parent
    CUISubControl::transform( object );

    // Transform all controls
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->transform( *this );

    // Transform the mask
    m_upStencilMaskSprite->transform( getMatrix(), wasWorldPosTranformed() );
}


/************************************************************************
*    DESC:  Render the sub control
************************************************************************/
void CUIScrollBox::render( const CMatrix & matrix )
{
    // Call the parent
    CUISubControl::render( matrix );

    // Disable rendering to the color buffer
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

    // Disable rendering to the depth mask
    glDepthMask( GL_FALSE );

    // Start using the stencil
    glEnable( GL_STENCIL_TEST );

    glStencilFunc( GL_ALWAYS, 0x1, 0x1 );
    glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );


    m_upStencilMaskSprite->render( matrix );


    // Re-enable color
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

    // Where a 1 was not rendered
    glStencilFunc( GL_EQUAL, 0x1, 0x1 );

    // Keep the pixel
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

    // Enable rendering to the depth mask
    glDepthMask( GL_TRUE );


    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->render( matrix );


    // Finished using stencil
    glDisable( GL_STENCIL_TEST );
}


/************************************************************************
*    DESC: Set the first inactive control to be active
*    NOTE: Don't want this functuality for the scroll box buttons and slider
************************************************************************/
bool CUIScrollBox::activateFirstInactiveControl()
{
    if( CUIControl::activateFirstInactiveControl() )
    {
        for( size_t i = 0; i < m_pScrollControlVec.size(); ++i )
        {
            if( m_pScrollControlVec[i]->activateFirstInactiveControl() )
            {
                m_activeScrollCtrl = i;
                break;
            }
        }
    }

    return m_activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL;
}


/************************************************************************
*    DESC:  Handle the select action
************************************************************************/
bool CUIScrollBox::handleSelectAction( const CSelectMsgCracker & msgCracker )
{
    bool result = CUISubControl::handleSelectAction( msgCracker );

    // Let the scroll controls handle any selection
    for( size_t i = 0; i < m_pScrollControlVec.size(); ++i )
        m_pScrollControlVec[i]->handleSelectAction( msgCracker );

    if( msgCracker.isDeviceMouse() && msgCracker.isPressDown() )
    {
        // Get the current scroll position
        m_scrollCurPos = NGenFunc::DynCast<CUISlider>(getSubControl())->getValue();

        // Set the bounds
        setStartEndPos();

        // Reposition the scroll controls
        repositionScrollControls();
    }

    return result;
}


/************************************************************************
*    DESC:  Handle the page scrolling
*
*   param: int scrollVector - direction to scroll
************************************************************************/
void CUIScrollBox::handlePageScroll( int scrollVector )
{
    if( m_scrollVector == 0 )
    {
        // If there's no controls to select or reposition, do the scroll
        if( !selectAndRepositionCtrl( scrollVector ) )
        {
            const bool SCROLL_DOWN((scrollVector > 0));
            const bool SCROLL_UP((scrollVector < 0));

            // Make sure we have some place to page to
            if( (SCROLL_UP && (m_firstScrollCtrlIndex > 0)) ||
                (SCROLL_DOWN && (m_firstScrollCtrlIndex + m_visibleCount < (int)m_pScrollControlVec.size())) )
            {
                int visibleCount = m_visibleCount;

                // Cap the scroll amount to what is capable
                if( SCROLL_UP && (visibleCount > m_firstScrollCtrlIndex) )
                {
                    visibleCount = m_firstScrollCtrlIndex;
                }
                else if( SCROLL_DOWN &&
                       ((visibleCount + m_firstScrollCtrlIndex + m_visibleCount - 1) >= (int)m_pScrollControlVec.size()) )
                {
                    visibleCount = m_pScrollControlVec.size() - m_firstScrollCtrlIndex - m_visibleCount;
                }

                // Init the scroll
                initScrolling( scrollVector, m_controlHeight * visibleCount, true, true );

                // Deactivate the last control if the scrolling has been activated
                if( m_scrollVector )
                {
                    if( m_activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
                        m_pScrollControlVec[m_activeScrollCtrl]->deactivateControl();
                }
            }
            else
            {
                m_activeScrollCtrl = m_firstScrollCtrlIndex;

                if( SCROLL_DOWN )
                    m_activeScrollCtrl += m_visibleCount - 1;

                // If the first control can't be selected, then find one that can
                if( !activateScrollCtrl( m_activeScrollCtrl ) )
                    selectNextControl( -scrollVector );
            }
        }
    }
}


/************************************************************************
*    DESC:  Handle the keyboard/Gamepad scrolling
*
*   param: int scrollVector - direction to scroll
************************************************************************/
void CUIScrollBox::handleKeyboardGamepadScroll( int scrollVector )
{
    // If there's no controls to select or reposition, do the scroll
    if( !selectAndRepositionCtrl( scrollVector ) )
    {
        // Try to select the next control
        auto scrollResult = selectNextControl( scrollVector );

        // Scroll the contents of the scroll box if we need to activate a control
        // that's outside of the viewable area of the scroll box.
        if( !scrollResult.isSet(IN_VIEWABLE_AREA) )
        {
            initScrolling( scrollVector, m_controlHeight );
        }
    }
}


/************************************************************************
*    DESC:  Select the next control
*
*   param: int scrollVector - direction to scroll
************************************************************************/
CBitmask<uint> CUIScrollBox::selectNextControl( int scrollVector )
{
    // Set the active control to the viewable area
    setActiveCtrlToViewableArea( scrollVector );

    // Scroll to the next control in the viewable area
    CBitmask<uint> scrollResult = scrollToTheNextCtrlInViewableArea( scrollVector );

    // If we are still in the viewable area but have no active control,
    // try to activate the current control
    if( scrollResult.isSet(IN_VIEWABLE_AREA) && !scrollResult.isSet(NEW_ACTIVE_CTRL) )
    {
        NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_ACTIVE,
            (void *)m_pScrollControlVec[m_activeScrollCtrl] );
    }

    return scrollResult;
}


/************************************************************************
*    DESC:  Do we need to select and reposition the control
************************************************************************/
bool CUIScrollBox::selectAndRepositionCtrl( int scrollVector )
{
    // If there's no selected control, don't scroll
    // just select the first selectable control
    if( setActiveCtrlToViewableArea( scrollVector ) )
    {
        // If the first control can't be selected, then find one that can
        if( !activateScrollCtrl( m_activeScrollCtrl ) )
            selectNextControl( 1 );

        // Get the alignment to see if it needs to be adjusted
        float diff = getControlAlignment();
        if( diff > 0.1f )
        {
            int pos( m_scrollCurPos / (float)m_controlHeight );

            bool nextCtrl( ((m_activeScrollCtrl - m_firstScrollCtrlIndex) * m_controlHeight) != 0 );

            if( nextCtrl || (m_firstScrollCtrlIndex > pos) )
                initScrolling( 1, m_controlHeight - diff, false );
            else
                initScrolling( -1, diff, false );
        }

        return true;
    }

    return false;
}


/************************************************************************
*    DESC:  Select the paged control
************************************************************************/
void CUIScrollBox::selectPagedControl( int scrollVector )
{
    m_activeScrollCtrl += scrollVector * m_visibleCount;

    // Cap the control index
    if( m_activeScrollCtrl <= 0 )
    {
        m_activeScrollCtrl = 0;
        scrollVector = 1;
    }
    else if( m_activeScrollCtrl >= (int)m_pScrollControlVec.size() - 1 )
    {
        m_activeScrollCtrl = (int)m_pScrollControlVec.size() -1;
        scrollVector = -1;
    }

    // If the first control can't be selected, then find one that can
    if( !activateScrollCtrl( m_activeScrollCtrl ) )
        selectNextControl( scrollVector );
}


/************************************************************************
*    DESC:  Set the active control to the viewable area
*           This also deactivates the last known active control
************************************************************************/
bool CUIScrollBox::setActiveCtrlToViewableArea( int scrollVector )
{
    // If the active control is not within the active area, make it so that it will be the first one selected
    if( (m_activeScrollCtrl < m_firstScrollCtrlIndex) || (m_activeScrollCtrl >= (m_firstScrollCtrlIndex + m_visibleCount)) )
    {
        if( m_activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
            m_pScrollControlVec[m_activeScrollCtrl]->deactivateControl();

        m_activeScrollCtrl = m_firstScrollCtrlIndex;

        return true;
    }

    return false;
}


/************************************************************************
*    DESC:  Scroll to the next control in the viewable area
*
*   param: int scrollVector - direction to scroll
************************************************************************/
CBitmask<uint> CUIScrollBox::scrollToTheNextCtrlInViewableArea( int scrollVector )
{
    int newActiveCtrl(0);
    int isInView = inView( m_activeScrollCtrl, scrollVector );

    // Only scroll within the viewable area
    if( isInView )
    {
        // Set a temp variable to the active scroll control
        int tmpScrollCtrl = m_activeScrollCtrl;

        // Loop until we hit a selectable control
        do
        {
            tmpScrollCtrl += scrollVector;

            if( activateScrollCtrl(tmpScrollCtrl) )
            {
                newActiveCtrl = NEW_ACTIVE_CTRL;

                m_activeScrollCtrl = tmpScrollCtrl;
                break;
            }

            isInView = inView( tmpScrollCtrl, scrollVector );
        }
        while( isInView );
    }

    CBitmask<uint> result( isInView | newActiveCtrl );

    return result;
}


/************************************************************************
*    DESC:  See if we can activate this scroll control
************************************************************************/
bool CUIScrollBox::activateScrollCtrl( int scrollControlIndex )
{
    if( (scrollControlIndex != NUIControl::NO_ACTIVE_CONTROL) &&
        (scrollControlIndex < (int)m_pScrollControlVec.size()) &&
        !m_pScrollControlVec.at(scrollControlIndex)->isDisabled() )
    {
        NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_ACTIVE,
            (void *)m_pScrollControlVec[scrollControlIndex] );

        return true;
    }

    return false;
}


/************************************************************************
*    DESC:  Init the variables that scroll the contents of the scroll box
************************************************************************/
void CUIScrollBox::initScrolling( int scrollVector, float distance, bool endScrollSelection, bool paging )
{
    if( m_scrollVector == 0 )
    {
        const bool SCROLL_DOWN((scrollVector > 0));
        const bool SCROLL_UP((scrollVector < 0));

        if( ((SCROLL_UP && (m_scrollCurPos > 0)) ||
            (SCROLL_DOWN && (m_scrollCurPos < m_maxMoveAmount))) )
        {
            m_scrollVector = scrollVector;
            m_scrollCounter = 0;
            m_endScroll = false;
            m_scrollMsg = false;
            m_paging = paging;
            m_endScrollSelection = endScrollSelection;
            m_scrollDistance = distance;
        }
    }
}


/************************************************************************
*    DESC:  Handle the time based Scrolling of the contents of the scroll box
************************************************************************/
void CUIScrollBox::handleScrollUpdate()
{
    if( m_scrollVector )
    {
        float dist = CHighResTimer::Instance().getElapsedTime() * m_scrollSpeed;

        if( m_paging )
            dist = CHighResTimer::Instance().getElapsedTime() * m_pageSpeed;

        if( m_scrollVector > 0 )
            m_scrollCurPos += dist;
        else
            m_scrollCurPos -= dist;

        NGenFunc::DynCast<CUISlider>(getSubControl())->setSlider(m_scrollCurPos);

        m_scrollCounter += dist;

        // Set the bounds
        setStartEndPos();

        if( m_scrollCounter >= m_scrollDistance )
        {
            if( m_endScroll || !m_scrollMsg || m_paging )
            {
                alignScrollPostion();

                if( m_endScrollSelection )
                {
                    if( m_paging )
                        selectPagedControl( m_scrollVector );
                    else
                        selectNextControl( m_scrollVector );
                }

                // This has to be last
                m_scrollVector = 0;
            }
            else
            {
                m_scrollDistance += m_controlHeight;
            }
        }
        // Sanity check
        else if( (m_scrollCurPos < 0) || (m_scrollCurPos > m_maxMoveAmount) )
        {
            alignScrollPostion();
            m_scrollVector = 0;
        }

        // Reposition the scroll controls
        repositionScrollControls();
    }
}


/************************************************************************
*    DESC:  Get the fractional amount the controls are off within the scroll box
************************************************************************/
float CUIScrollBox::getControlAlignment()
{
    float pos = m_scrollCurPos / (float)m_controlHeight;
    return (float)m_controlHeight * (pos - (int)pos);
}


/************************************************************************
*    DESC:  Is the scroll index in view
************************************************************************/
bool CUIScrollBox::inView( int scrollIndex, int scrollVector )
{
    return ((scrollVector < 0) && (scrollIndex > m_firstScrollCtrlIndex)) ||
           (((scrollVector > 0)) && (scrollIndex < (m_firstScrollCtrlIndex + m_visibleCount - 1)));
}


/************************************************************************
*    DESC:  Get the pointer to the subcontrol if found
************************************************************************/
CUIControl * CUIScrollBox::findSubControl( const std::string & name )
{
    CUIControl * pCtrl = CUISubControl::findSubControl( name );

    for( int i = m_visStartPos; i < m_visEndPos && (pCtrl == nullptr); ++i )
        pCtrl = m_pScrollControlVec[i]->findControl( name );

    return pCtrl;
}


/************************************************************************
*    DESC:  Find the sub control via is pointer
************************************************************************/
CUIControl * CUIScrollBox::findSubControl( void * pVoid )
{
    CUIControl * pCtrl = CUISubControl::findSubControl( pVoid );

    for( int i = m_visStartPos; i < m_visEndPos && (pCtrl == nullptr); ++i )
        if( (void *)m_pScrollControlVec[i] == pVoid )
            pCtrl = m_pScrollControlVec[i];

    return pCtrl;
}


/************************************************************************
*    DESC:  Handle the sub control mouse move
************************************************************************/
bool CUIScrollBox::onSubControlMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::onSubControlMouseMove( rEvent );

    // We only care about the scroll controls if the point is within the scroll box
    if( !result && isPointInControl( rEvent.motion.x, rEvent.motion.y ) )
    {
        for( int i = m_visStartPos; i < m_visEndPos && !result; ++i )
        {
            result = m_pScrollControlVec[i]->onMouseMove( rEvent );

            if( result )
                m_activeScrollCtrl = i;
        }
    }

    return result;
}


/************************************************************************
*    DESC:  Deactivate the sub control
************************************************************************/
void CUIScrollBox::deactivateSubControl()
{
    CUISubControl::deactivateSubControl();

    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->deactivateControl();
}


/************************************************************************
*    DESC:  Set the start and end positions
************************************************************************/
void CUIScrollBox::setStartEndPos()
{
    float pos( m_scrollCurPos / (float)m_controlHeight );

    // Push the ceiling so that the starting index is viewable
    m_firstScrollCtrlIndex = pos + 0.7f;

    m_visStartPos = pos;
    m_visEndPos = m_visStartPos + m_visibleCount + 1;

    // Sanity checks
    if( m_visStartPos < 0 )
        m_visStartPos = 0;

    if( m_visEndPos > static_cast<int>(m_pScrollControlVec.size()) )
        m_visEndPos = static_cast<int>(m_pScrollControlVec.size());
}


/************************************************************************
*    DESC:  Reposition the scroll controls
************************************************************************/
void CUIScrollBox::repositionScrollControls()
{
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
    {
        CPoint<float> pos( m_pScrollControlVec[i]->getPos() );
        pos.y = m_defaultOffsetVec[i] + m_scrollCurPos;
        m_pScrollControlVec[i]->setPos( pos );
    }
}


/************************************************************************
*    DESC:  Align the scroll box to it's proper stopping point
*           to account for floating point movement
************************************************************************/
void CUIScrollBox::alignScrollPostion()
{
    // Do bounds checking just for sanity reasons
    if( m_firstScrollCtrlIndex < 0 )
        m_firstScrollCtrlIndex = 0;

    else if( (m_firstScrollCtrlIndex + m_visibleCount) > static_cast<int>(m_pScrollControlVec.size()) )
        m_firstScrollCtrlIndex = static_cast<int>(m_pScrollControlVec.size() - m_visibleCount);

    // Recalucate the scroll position which will wipe the fractional component
    m_scrollCurPos = m_firstScrollCtrlIndex * m_controlHeight;
}


/************************************************************************
*    DESC:  Only deactivate sub controls
************************************************************************/
void CUIScrollBox::deactivateControl()
{
    deactivateSubControl();
}


/************************************************************************
*    DESC:  Get the scroll control vector
************************************************************************/
const std::vector<CUIControl *> & CUIScrollBox::getScrollCtrlVec()
{
    return m_pScrollControlVec;
} 


/************************************************************************
*    DESC:  Set the alpha value of this control
************************************************************************/
void CUIScrollBox::setAlpha( float alpha )
{
    CUISubControl::setAlpha( alpha );

    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->setAlpha( alpha );
}


/************************************************************************
*    DESC:  Get the pointer to the active control
************************************************************************/
CUIControl * CUIScrollBox::getPtrToActiveControl()
{
    CUIControl * pResult( CUISubControl::getPtrToActiveControl() );

    if( pResult == nullptr )
    {
        for( auto iter : m_pScrollControlVec )
        {
            if( iter->getState() > NUIControl::ECS_INACTIVE )
            {
                pResult = iter->getPtrToActiveControl();
                break;
            }
        }
    }

    return pResult;
}
