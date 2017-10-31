
/************************************************************************
*    FILE NAME:       uiscrollbox.cpp
*
*    DESCRIPTION:     Class for user interface scroll boxes
************************************************************************/

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
*    desc:  Constructor
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

}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CUIScrollBox::~CUIScrollBox()
{
    NDelFunc::DeleteVectorPointers(m_pScrollControlVec);

}   // destructor


/************************************************************************
*    desc:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIScrollBox::LoadFromNode( const XMLNode & node )
{
    CUISubControl::LoadFromNode( node );

    // Init the slider
    NGenFunc::DynCast<CUISlider>(GetSubControl())->SetMaxValue(m_maxMoveAmount);
    NGenFunc::DynCast<CUISlider>(GetSubControl())->SetSlider();

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
    SetStartEndPos();

}   // LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
************************************************************************/
void CUIScrollBox::LoadControlFromNode( const XMLNode & node )
{
    // Call the parent
    CUISubControl::LoadControlFromNode( node );

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
            AddScrollControlFromNode( menuControlsNode.getChildNode( "control", i ) );
    }

    // Get the stencil mask node
    const XMLNode stencilMaskNode = node.getChildNode( "stencilMask" );
    if( !stencilMaskNode.isEmpty() )
    {
        const std::string objectName = stencilMaskNode.getAttribute( "objectName" );

        m_upStencilMaskSprite.reset( new CSprite2D( CObjectDataMgr::Instance().GetData2D( GetGroup(), objectName ) ) );

        // Get the cull height
        m_cullHeight = (m_upStencilMaskSprite->GetObjectData().GetSize().GetW() + m_controlHeight) / 2;

        // Load the transform data
        m_upStencilMaskSprite->LoadTransFromNode( stencilMaskNode );
    }

}   // LoadControlFromNode


/************************************************************************
*    desc:  Add the scroll control from node
*           NOTE: This function recalulates the scroll box members because
*                 it is also used for run-time dynamic scroll boxes
************************************************************************/
CUIControl * CUIScrollBox::AddScrollControlFromNode( const XMLNode & node )
{
    // The pointer is placed within a vector for all controls
    CUIControl * pCtrl = NUIControlFactory::Create( node, GetGroup() );
    m_pScrollControlVec.push_back( pCtrl );

    // Get the position for this control
    CPoint<float> pos( m_initialOffset - CPoint<float>(0, m_controlHeight * (m_pScrollControlVec.size()-1), 0) );

    // Record the default y offset
    m_defaultOffsetVec.push_back( pos.y );

    // Set the position
    pCtrl->SetPos( pos );

    // Init the control visual state
    pCtrl->DeactivateControl();

    // Calculate the maximum scroll amount in pixels
    if( m_pScrollControlVec.size() > static_cast<uint>(m_visibleCount) )
        m_maxMoveAmount = (static_cast<int>(m_pScrollControlVec.size()) - m_visibleCount) * m_controlHeight;

    return pCtrl;

}   // AddControlFromNode


/************************************************************************
*    desc:  Init the control
************************************************************************/
void CUIScrollBox::Init()
{
    CUISubControl::Init();
    
    // Init all controls
    for( auto iter : m_pScrollControlVec )
        iter->Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CUIScrollBox::CleanUp()
{
    CUISubControl::CleanUp();
    
    // Init all controls
    for( auto iter : m_pScrollControlVec )
        iter->CleanUp();
    
}   // CleanUp


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CUIScrollBox::HandleEvent( const SDL_Event & rEvent )
{
    // Call the parent
    CUISubControl::HandleEvent( rEvent );

    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->HandleEvent( rEvent );

}   // HandleEvent


/************************************************************************
*    desc:  Handle OnUpAction message
************************************************************************/
void CUIScrollBox::OnUpAction( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        HandleKeyboardGamepadScroll( -1 );

    else if( rEvent.user.code == NDefs::EAP_UP )
        m_endScroll = true;

}   // OnUpAction

/************************************************************************
*    desc:  Handle OnDownAction message
************************************************************************/
void CUIScrollBox::OnDownAction( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        HandleKeyboardGamepadScroll( 1 );

    else if( rEvent.user.code == NDefs::EAP_UP )
        m_endScroll = true;

}   // OnDownAction

/************************************************************************
*    desc:  Handle OnUpScroll message
************************************************************************/
void CUIScrollBox::OnUpScroll( const SDL_Event & rEvent )
{
    HandleKeyboardGamepadScroll( -1 );
    m_scrollMsg = true;

}   // OnUpScroll

/************************************************************************
*    desc:  Handle OnDownScroll message
************************************************************************/
void CUIScrollBox::OnDownScroll( const SDL_Event & rEvent )
{
    HandleKeyboardGamepadScroll( 1 );
    m_scrollMsg = true;

}   // OnDownScroll

/************************************************************************
*    desc:  Handle OnTabLeft message
************************************************************************/
void CUIScrollBox::OnTabLeft( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        HandlePageScroll( -1 );

}   // OnTabLeft

/************************************************************************
*    desc:  Handle OnTabRight message
************************************************************************/
void CUIScrollBox::OnTabRight( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NDefs::EAP_DOWN )
        HandlePageScroll( 1 );

}   // OnTabRight


/************************************************************************
*    desc:  Handle the mouse move
************************************************************************/
bool CUIScrollBox::OnMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::OnMouseMove( rEvent );
    
    // Invalidate the active control
    m_activeScrollCtrl = NUIControl::NO_ACTIVE_CONTROL;

    if( NGenFunc::DynCast<CUISlider>(GetSubControl())->IsMouseDown() )
    {
        // Get the current scroll position
        m_scrollCurPos = NGenFunc::DynCast<CUISlider>(GetSubControl())->GetValue();

        // Set the bounds
        SetStartEndPos();

        // Reposition the scroll controls
        RepositionScrollControls();
    }

    return result;

}   // OnMouseMove


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUIScrollBox::Update()
{
    // Call the parent
    CUISubControl::Update();

    // Update all controls
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->Update();

    // Handle any scrolling
    HandleScrollUpdate();

}   // Update


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIScrollBox::Transform( const CObject2D & object )
{
    // Call the parent
    CUISubControl::Transform( object );

    // Transform all controls
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->Transform( *this );

    // Transform the mask
    m_upStencilMaskSprite->Transform( GetMatrix(), WasWorldPosTranformed() );

}   // Transform


/************************************************************************
*    desc:  Render the sub control
************************************************************************/
void CUIScrollBox::Render( const CMatrix & matrix )
{
    // Call the parent
    CUISubControl::Render( matrix );

    // Disable rendering to the color buffer
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    
    // Disable rendering to the depth mask
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

    // Enable rendering to the depth mask
    glDepthMask( GL_TRUE );


    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->Render( matrix );


    // Finished using stencil
    glDisable( GL_STENCIL_TEST );

}   // Render


/************************************************************************
*    desc: Set the first inactive control to be active
*    NOTE: Don't want this functuality for the scroll box buttons and slider
************************************************************************/
bool CUIScrollBox::ActivateFirstInactiveControl()
{
    if( CUIControl::ActivateFirstInactiveControl() )
    {
        for( size_t i = 0; i < m_pScrollControlVec.size(); ++i )
        {
            if( m_pScrollControlVec[i]->ActivateFirstInactiveControl() )
            {
                m_activeScrollCtrl = i;
                break;
            }
        }
    }

    return m_activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL;

}   // ActivateFirstInactiveControl


/************************************************************************
*    desc:  Handle the select action
************************************************************************/
bool CUIScrollBox::HandleSelectAction( const CSelectMsgCracker & msgCracker )
{
    bool result = CUISubControl::HandleSelectAction( msgCracker );
    
    // Let the scroll controls handle any selection
    for( size_t i = 0; i < m_pScrollControlVec.size(); ++i )
        m_pScrollControlVec[i]->HandleSelectAction( msgCracker );

    if( msgCracker.IsDeviceMouse() && msgCracker.IsPressDown() )
    {
        // Get the current scroll position
        m_scrollCurPos = NGenFunc::DynCast<CUISlider>(GetSubControl())->GetValue();

        // Set the bounds
        SetStartEndPos();

        // Reposition the scroll controls
        RepositionScrollControls();
    }

    return result;

}   // HandleSelectAction


/************************************************************************
*    desc:  Handle the page scrolling
*
*   param: int scrollVector - direction to scroll
************************************************************************/
void CUIScrollBox::HandlePageScroll( int scrollVector )
{
    if( m_scrollVector == 0 )
    {
        // If there's no controls to select or reposition, do the scroll
        if( !SelectAndRepositionCtrl( scrollVector ) )
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
                InitScrolling( scrollVector, m_controlHeight * visibleCount, true, true );

                // Deactivate the last control if the scrolling has been activated
                if( m_scrollVector )
                {
                    if( m_activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
                        m_pScrollControlVec[m_activeScrollCtrl]->DeactivateControl();
                }
            }
            else
            {
                m_activeScrollCtrl = m_firstScrollCtrlIndex;
                
                if( SCROLL_DOWN )
                    m_activeScrollCtrl += m_visibleCount - 1;
                
                // If the first control can't be selected, then find one that can
                if( !ActivateScrollCtrl( m_activeScrollCtrl ) )
                    SelectNextControl( -scrollVector );
            }
        }
    }
}   // HandlePageScroll


/************************************************************************
*    desc:  Handle the keyboard/Gamepad scrolling
*
*   param: int scrollVector - direction to scroll
************************************************************************/
void CUIScrollBox::HandleKeyboardGamepadScroll( int scrollVector )
{
    // If there's no controls to select or reposition, do the scroll
    if( !SelectAndRepositionCtrl( scrollVector ) )
    {
        // Try to select the next control
        auto scrollResult = SelectNextControl( scrollVector );

        // Scroll the contents of the scroll box if we need to activate a control
        // that's outside of the viewable area of the scroll box.
        if( !scrollResult.IsSet(IN_VIEWABLE_AREA) )
        {
            InitScrolling( scrollVector, m_controlHeight );
        }
    }

}   // HandleKeyboardGamepadScroll


/************************************************************************
*    desc:  Select the next control
*
*   param: int scrollVector - direction to scroll
************************************************************************/
CBitmask<uint> CUIScrollBox::SelectNextControl( int scrollVector )
{
    // Set the active control to the viewable area
    SetActiveCtrlToViewableArea( scrollVector );

    // Scroll to the next control in the viewable area
    CBitmask<uint> scrollResult = ScrollToTheNextCtrlInViewableArea( scrollVector );

    // If we are still in the viewable area but have no active control,
    // try to activate the current control
    if( scrollResult.IsSet(IN_VIEWABLE_AREA) && !scrollResult.IsSet(NEW_ACTIVE_CTRL) )
    {
        NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_ACTIVE,
            (void *)m_pScrollControlVec[m_activeScrollCtrl] );
    }

    return scrollResult;

}   // SelectNextControl


/************************************************************************
*    desc:  Do we need to select and reposition the control
************************************************************************/
bool CUIScrollBox::SelectAndRepositionCtrl( int scrollVector )
{
    // If there's no selected control, don't scroll
    // just select the first selectable control
    if( SetActiveCtrlToViewableArea( scrollVector ) )
    {
        // If the first control can't be selected, then find one that can
        if( !ActivateScrollCtrl( m_activeScrollCtrl ) )
            SelectNextControl( 1 );

        // Get the alignment to see if it needs to be adjusted
        float diff = GetControlAlignment();
        if( diff > 0.1f )
        {
            int pos( m_scrollCurPos / (float)m_controlHeight );
            
            bool nextCtrl( (m_activeScrollCtrl - m_firstScrollCtrlIndex) * m_controlHeight );

            if( nextCtrl || (m_firstScrollCtrlIndex > pos) )
                InitScrolling( 1, m_controlHeight - diff, false );
            else
                InitScrolling( -1, diff, false );
        }
        
        return true;
    }

    return false;

}   // SelectAndRepositionCtrl


/************************************************************************
*    desc:  Select the paged control
************************************************************************/
void CUIScrollBox::SelectPagedControl( int scrollVector )
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
    if( !ActivateScrollCtrl( m_activeScrollCtrl ) )
        SelectNextControl( scrollVector );

}   // SelectPagedControl


/************************************************************************
*    desc:  Set the active control to the viewable area
*           This also deactivates the last known active control
************************************************************************/
bool CUIScrollBox::SetActiveCtrlToViewableArea( int scrollVector )
{
    // If the active control is not within the active area, make it so that it will be the first one selected
    if( (m_activeScrollCtrl < m_firstScrollCtrlIndex) || (m_activeScrollCtrl >= (m_firstScrollCtrlIndex + m_visibleCount)) )
    {
        if( m_activeScrollCtrl != NUIControl::NO_ACTIVE_CONTROL )
            m_pScrollControlVec[m_activeScrollCtrl]->DeactivateControl();

        m_activeScrollCtrl = m_firstScrollCtrlIndex;

        return true;
    }

    return false;

}   // SetActiveCtrlToViewableArea


/************************************************************************
*    desc:  Scroll to the next control in the viewable area
*
*   param: int scrollVector - direction to scroll
************************************************************************/
CBitmask<uint> CUIScrollBox::ScrollToTheNextCtrlInViewableArea( int scrollVector )
{
    int newActiveCtrl(0);
    int inView = InView( m_activeScrollCtrl, scrollVector );

    // Only scroll within the viewable area
    if( inView )
    {
        // Set a temp variable to the active scroll control
        int tmpScrollCtrl = m_activeScrollCtrl;

        // Loop until we hit a selectable control
        do
        {
            tmpScrollCtrl += scrollVector;

            if( ActivateScrollCtrl(tmpScrollCtrl) )
            {
                newActiveCtrl = NEW_ACTIVE_CTRL;

                m_activeScrollCtrl = tmpScrollCtrl;
                break;
            }

            inView = InView( tmpScrollCtrl, scrollVector );
        }
        while( inView );
    }

    CBitmask<uint> result( inView | newActiveCtrl );

    return result;

}   // ScrollToTheNextCtrlInViewableArea


/************************************************************************
*    desc:  See if we can activate this scroll control
************************************************************************/
bool CUIScrollBox::ActivateScrollCtrl( int scrollControlIndex )
{
    if( (scrollControlIndex != NUIControl::NO_ACTIVE_CONTROL) &&
        (scrollControlIndex < (int)m_pScrollControlVec.size()) &&
        !m_pScrollControlVec.at(scrollControlIndex)->IsDisabled() )
    {
        NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_ACTIVE,
            (void *)m_pScrollControlVec[scrollControlIndex] );

        return true;
    }

    return false;

}   // ActivateScrollCtrl


/************************************************************************
*    desc:  Init the variables that scroll the contents of the scroll box
************************************************************************/
void CUIScrollBox::InitScrolling( int scrollVector, float distance, bool endScrollSelection, bool paging )
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

}   // InitScrolling


/************************************************************************
*    desc:  Handle the time based Scrolling of the contents of the scroll box
************************************************************************/
void CUIScrollBox::HandleScrollUpdate()
{
    if( m_scrollVector )
    {
        float dist = CHighResTimer::Instance().GetElapsedTime() * m_scrollSpeed;

        if( m_paging )
            dist = CHighResTimer::Instance().GetElapsedTime() * m_pageSpeed;

        if( m_scrollVector > 0 )
            m_scrollCurPos += dist;
        else
            m_scrollCurPos -= dist;

        NGenFunc::DynCast<CUISlider>(GetSubControl())->SetSlider(m_scrollCurPos);

        m_scrollCounter += dist;

        // Set the bounds
        SetStartEndPos();

        if( m_scrollCounter >= m_scrollDistance )
        {
            if( m_endScroll || !m_scrollMsg || m_paging )
            {
                AlignScrollPostion();

                if( m_endScrollSelection )
                {
                    if( m_paging )
                        SelectPagedControl( m_scrollVector );
                    else
                        SelectNextControl( m_scrollVector );
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
            AlignScrollPostion();
            m_scrollVector = 0;
        }
        
        // Reposition the scroll controls
        RepositionScrollControls();
    }

}   // HandleScrollUpdate


/************************************************************************
*    desc:  Get the fractional amount the controls are off within the scroll box
************************************************************************/
float CUIScrollBox::GetControlAlignment()
{
    float pos = m_scrollCurPos / (float)m_controlHeight;
    return (float)m_controlHeight * (pos - (int)pos);

}   // GetControlAlignment


/************************************************************************
*    desc:  Is the scroll index in view
************************************************************************/
bool CUIScrollBox::InView( int scrollIndex, int scrollVector )
{
    return ((scrollVector < 0) && (scrollIndex > m_firstScrollCtrlIndex)) ||
           (((scrollVector > 0)) && (scrollIndex < (m_firstScrollCtrlIndex + m_visibleCount - 1)));

}   // InView


/************************************************************************
*    desc:  Get the pointer to the subcontrol if found
************************************************************************/
CUIControl * CUIScrollBox::FindSubControl( const std::string & name )
{
    CUIControl * pCtrl = CUISubControl::FindSubControl( name );

    for( int i = m_visStartPos; i < m_visEndPos && (pCtrl == nullptr); ++i )
        pCtrl = m_pScrollControlVec[i]->FindControl( name );

    return pCtrl;

}   // FindSubControl


/************************************************************************
*    desc:  Find the sub control via is pointer
************************************************************************/
CUIControl * CUIScrollBox::FindSubControl( void * pVoid )
{
    CUIControl * pCtrl = CUISubControl::FindSubControl( pVoid );

    for( int i = m_visStartPos; i < m_visEndPos && (pCtrl == nullptr); ++i )
        if( (void *)m_pScrollControlVec[i] == pVoid )
            pCtrl = m_pScrollControlVec[i];

    return pCtrl;

}   // FindSubControl


/************************************************************************
*    desc:  Handle the sub control mouse move
************************************************************************/
bool CUIScrollBox::OnSubControlMouseMove( const SDL_Event & rEvent )
{
    bool result = CUISubControl::OnSubControlMouseMove( rEvent );

    // We only care about the scroll controls if the point is within the scroll box
    if( !result && IsPointInControl( rEvent.motion.x, rEvent.motion.y ) )
    {
        for( int i = m_visStartPos; i < m_visEndPos && !result; ++i )
        {
            result = m_pScrollControlVec[i]->OnMouseMove( rEvent );

            if( result )
                m_activeScrollCtrl = i;
        }
    }

    return result;

}   // OnSubControlMouseMove


/************************************************************************
*    desc:  Deactivate the sub control
************************************************************************/
void CUIScrollBox::DeactivateSubControl()
{
    CUISubControl::DeactivateSubControl();

    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->DeactivateControl();

}   // DeactivateSubControl


/************************************************************************
*    desc:  Set the start and end positions
************************************************************************/
void CUIScrollBox::SetStartEndPos()
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

}   // SetStartEndPos


/************************************************************************
*    desc:  Reposition the scroll controls
************************************************************************/
void CUIScrollBox::RepositionScrollControls()
{
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
    {
        CPoint<float> pos( m_pScrollControlVec[i]->GetPos() );
        pos.y = m_defaultOffsetVec[i] + m_scrollCurPos;
        m_pScrollControlVec[i]->SetPos( pos );
    }

}   // RepositionScrollControls


/************************************************************************
*    desc:  Align the scroll box to it's proper stopping point
*           to account for floating point movement
************************************************************************/
void CUIScrollBox::AlignScrollPostion()
{
    // Do bounds checking just for sanity reasons
    if( m_firstScrollCtrlIndex < 0 )
        m_firstScrollCtrlIndex = 0;

    else if( (m_firstScrollCtrlIndex + m_visibleCount) > static_cast<int>(m_pScrollControlVec.size()) )
        m_firstScrollCtrlIndex = static_cast<int>(m_pScrollControlVec.size() - m_visibleCount);

    // Recalucate the scroll position which will wipe the fractional component
    m_scrollCurPos = m_firstScrollCtrlIndex * m_controlHeight;

}   // AlignScrollPostion


/************************************************************************
*    desc:  Only deactivate sub controls
************************************************************************/
void CUIScrollBox::DeactivateControl()
{
    DeactivateSubControl();

}   // DeactivateControl


/************************************************************************
*    desc:  Get the scroll control vector
************************************************************************/
const std::vector<CUIControl *> & CUIScrollBox::GetScrollCtrlVec()
{
    return m_pScrollControlVec;

}   // GetScrollCtrlVec


/************************************************************************
*    desc:  Set the alpha value of this control
************************************************************************/
void CUIScrollBox::SetAlpha( float alpha )
{
    CUISubControl::SetAlpha( alpha );
            
    for( int i = m_visStartPos; i < m_visEndPos; ++i )
        m_pScrollControlVec[i]->SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
*    desc:  Get the pointer to the active control
************************************************************************/
CUIControl * CUIScrollBox::GetPtrToActiveControl()
{
    CUIControl * pResult( CUISubControl::GetPtrToActiveControl() );
    
    if( pResult == nullptr )
    {
        for( auto iter : m_pScrollControlVec )
        {
            if( iter->GetState() > NUIControl::ECS_INACTIVE )
            {
                pResult = iter->GetPtrToActiveControl();
                break;
            }
        }
    }
    
    return pResult;
    
}   // GetPtrToActiveControl
