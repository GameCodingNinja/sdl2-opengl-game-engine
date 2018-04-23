
/************************************************************************
*    FILE NAME:       uisubcontrol.cpp
*
*    DESCRIPTION:     Class for user interface controls with sub-controls
************************************************************************/

// Physical component dependency
#include <gui/uisubcontrol.h>

// Game lib dependencies
#include <utilities/deletefuncs.h>
#include <utilities/xmlParser.h>
#include <utilities/genfunc.h>
#include <utilities/exceptionhandling.h>
#include <gui/uicontrolfactory.h>
#include <gui/menudefs.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CUISubControl::CUISubControl( const std::string & group ) :
    CUIControl( group ),
    m_pActiveNode(nullptr),
    m_respondsToSelectMsg(false)
{
    m_type = NUIControl::ECT_SUB_CONTROL;

}   // constructor


/************************************************************************
*    desc:  destructor                                                             
************************************************************************/
CUISubControl::~CUISubControl()
{
    NDelFunc::DeleteVectorPointers( m_pControlNodeVec );
    NDelFunc::DeleteVectorPointers( m_pSubControlVec );

}   // destructor


/************************************************************************
*    desc:  Load the control specific info from XML node
************************************************************************/
void CUISubControl::LoadControlFromNode( const XMLNode & node )
{
    // Have the parent load it's stuff
    CUIControl::LoadControlFromNode( node );

    // Get the sub-control settings
    XMLNode subControlSettingsNode = node.getChildNode( "subControlSettings" );
    if( !subControlSettingsNode.isEmpty() )
    {
        // Does this sub control respond to select? The default is false.
        if( subControlSettingsNode.isAttributeSet( "respondsToSelectMsg" ) )
            m_respondsToSelectMsg = ( std::strcmp( subControlSettingsNode.getAttribute( "respondsToSelectMsg" ), "true") == 0 );
    }

    // Get the menu controls node
    const XMLNode controlListNode = node.getChildNode( "subControlList" );
    if( !controlListNode.isEmpty() )
    {
        // map to help setup the node pointers
        NavHelperMap navNodeMap;

        for( int i = 0; i < controlListNode.nChildNode( "control" ); ++i )
        {
            XMLNode controlNode = controlListNode.getChildNode( "control", i );

            // The pointer is placed within a vector for all controls
            m_pSubControlVec.push_back( NUIControlFactory::Create( controlNode, GetGroup() ) );

            // Get the control name
            const std::string controlName = m_pSubControlVec.back()->GetName();

            // Does this control have a name then create a node and add it to the map
            if( !controlName.empty() )
            {
                // Add a node to the vector with it's control
                m_pControlNodeVec.push_back( new CUIControlNavNode( m_pSubControlVec.back() ) );

                // Map of menu control nodes
                navNodeMap.emplace( controlName, m_pControlNodeVec.back() );
            }
        }

        if( navNodeMap.size() > 0 )
        {
            for( int i = 0; i < controlListNode.nChildNode( "control" ); ++i )
            {
                XMLNode controlNode = controlListNode.getChildNode( "control", i );

                // Find the reference nodes
                FindNodes( controlNode, i, navNodeMap );
            }
        }
    }

}	// LoadControlFromNode


/************************************************************************
*    desc:  Init the control
************************************************************************/
void CUISubControl::Init()
{
    CUIControl::Init();
    
    // Init all controls
    for( auto iter : m_pSubControlVec )
        iter->Init();
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CUISubControl::CleanUp()
{
    CUIControl::CleanUp();
    
    // Init all controls
    for( auto iter : m_pSubControlVec )
        iter->CleanUp();
    
}   // CleanUp


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CUISubControl::FindNodes(
    const XMLNode & node,
    int nodeIndex,
    NavHelperMap & navNodeMap )
{
    const XMLNode navNode = node.getChildNode( "navigate" );
    if( !navNode.isEmpty() )
    {
        SetNodes( navNode, nodeIndex, "up", CUIControlNavNode::ENAV_NODE_UP, navNodeMap );
        SetNodes( navNode, nodeIndex, "down", CUIControlNavNode::ENAV_NODE_DOWN, navNodeMap );
        SetNodes( navNode, nodeIndex, "left", CUIControlNavNode::ENAV_NODE_LEFT, navNodeMap );
        SetNodes( navNode, nodeIndex, "right", CUIControlNavNode::ENAV_NODE_RIGHT, navNodeMap );
    }

}   // FindNodes


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CUISubControl::SetNodes(
    const XMLNode & node,
    int nodeIndex,
    std::string attr,
    CUIControlNavNode::ENavNode navNode,
    NavHelperMap & navNodeMap )
{
    if( node.isAttributeSet( attr.c_str() ) )
    {
        const std::string name = node.getAttribute( attr.c_str() );
        auto iter = navNodeMap.find( name );
        if( iter != navNodeMap.end() )
        {
            m_pControlNodeVec[nodeIndex]->SetNode( navNode, iter->second );
        }
        else
        {
            throw NExcept::CCriticalException("Control Node Find Error!",
                boost::str( boost::format("Control node doesn't exist (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }
    }

}   // SetNodes


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUISubControl::Update()
{
    // Call the parent
    CUIControl::Update();

    // Update all controls
    for( auto iter : m_pSubControlVec )
        iter->Update();

}   // Update


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUISubControl::transform( const CObject2D & object )
{
    // Call the parent
    CUIControl::transform( object );
    
    // Update all controls
    for( auto iter : m_pSubControlVec )
        iter->transform( *this );

}   // Transform


/************************************************************************
*    desc:  Render the sub control
************************************************************************/
void CUISubControl::Render( const CMatrix & matrix )
{
    // Call the parent
    CUIControl::Render( matrix );

    for( auto iter : m_pSubControlVec )
        iter->Render( matrix );

}   // Render


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CUISubControl::HandleEvent( const SDL_Event & rEvent )
{
    // Call the parent
    CUIControl::HandleEvent( rEvent );

    for( auto iter : m_pSubControlVec )
        iter->HandleEvent( rEvent );

    if( IsActive() )
    {
        if( (rEvent.type >= NMenu::EGE_MENU_UP_ACTION) &&
            (rEvent.type <= NMenu::EGE_MENU_RIGHT_ACTION) )
        {
            if( rEvent.type == NMenu::EGE_MENU_UP_ACTION )
            {
                OnUpAction( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_DOWN_ACTION )
            {
                OnDownAction( rEvent );
            }
            if( rEvent.type == NMenu::EGE_MENU_LEFT_ACTION )
            {
                OnLeftAction( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_RIGHT_ACTION )
            {
                OnRightAction( rEvent );
            }
        }
        else if( (rEvent.type >= NMenu::EGE_MENU_SCROLL_UP) &&
                 (rEvent.type <= NMenu::EGE_MENU_SCROLL_RIGHT) )
        {
            if( rEvent.type == NMenu::EGE_MENU_SCROLL_UP )
            {
                OnUpScroll( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_SCROLL_DOWN )
            {
                OnDownScroll( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_SCROLL_LEFT )
            {
                OnLeftScroll( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_SCROLL_RIGHT )
            {
                OnRightScroll( rEvent );
            }
        }
        else if( rEvent.type == NMenu::EGE_MENU_TAB_LEFT )
        {
            OnTabLeft( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_TAB_RIGHT )
        {
            OnTabRight( rEvent );
        }
    }

}   // HandleEvent


/************************************************************************
*    desc:  Handle OnUpAction message
************************************************************************/
void CUISubControl::OnUpAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_UP );

}   // OnUpAction

/************************************************************************
*    desc:  Handle OnMenuDown message
************************************************************************/
void CUISubControl::OnDownAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_DOWN );

}   // OnUpAction

/************************************************************************
*    desc:  Handle OnMenuLeft message
************************************************************************/
void CUISubControl::OnLeftAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_LEFT );

}   // OnLeftAction

/************************************************************************
*    desc:  Handle OnRightAction message
************************************************************************/
void CUISubControl::OnRightAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_RIGHT );

}   // OnRightAction

/************************************************************************
*    desc:  Handle OnUpScroll message
************************************************************************/
void CUISubControl::OnUpScroll( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_UP );

}   // OnUpScroll

/************************************************************************
*    desc:  Handle OnUpScroll message
************************************************************************/
void CUISubControl::OnDownScroll( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_DOWN );

}   // OnDownScroll

/************************************************************************
*    desc:  Handle OnRightScroll message
************************************************************************/
void CUISubControl::OnLeftScroll( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_LEFT );

}   // OnLeftScrol

/************************************************************************
*    desc:  Handle OnRightScroll message
************************************************************************/
void CUISubControl::OnRightScroll( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_RIGHT );

}   // OnRightScroll

/************************************************************************
*    desc:  Handle OnTabLeft message
************************************************************************/
void CUISubControl::OnTabLeft( const SDL_Event & rEvent )
{
    // Do nothing
    
}   // OnTabLeft

/************************************************************************
*    desc:  Handle OnTabRight message
************************************************************************/
void CUISubControl::OnTabRight( const SDL_Event & rEvent )
{
    // Do nothing
    
}   // OnTabRight


/************************************************************************
*    desc:  Navigate the menu. Find the next control node that isn't
*           disabled and make it the active control node
************************************************************************/
void CUISubControl::NavigateMenu( CUIControlNavNode::ENavNode navNode )
{
    if( m_pActiveNode != nullptr )
    {
        CUIControlNavNode * pNavNode = m_pActiveNode;
        
        do
        {
            pNavNode = pNavNode->GetNode( navNode );
            
            if( pNavNode == nullptr )
            {
                break;
            }
            else if( !m_pActiveNode->GetControl()->IsDisabled() )
            {
                m_pActiveNode = pNavNode;

                NGenFunc::DispatchEvent( 
                    NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
                    NUIControl::ECS_ACTIVE,
                    pNavNode->GetControl() );

                break;
            }
        }
        while( true );
    }

}   // NavigateMenu


/************************************************************************
*    desc:  Handle OnStateChange message
************************************************************************/
void CUISubControl::OnStateChange( const SDL_Event & rEvent )
{
    if( m_respondsToSelectMsg )
    {
        CUIControl::OnStateChange( rEvent );
    }
    else
    {
        NUIControl::EControlState state = NUIControl::EControlState(rEvent.user.code);

        CUIControl * pCtrl = FindSubControl( rEvent.user.data1 );

        // Restart the active state of the sub control if something
        // changed in the child controls or their children controls
        if( (state == NUIControl::ECS_ACTIVE) && (pCtrl != nullptr) )
        {
            if( pCtrl->GetState() != state )
            {
                SetState(state, true);

                RecycleContext();

                SetDisplayState();
            }
        }
        // The sub control doesn't respond to selected message
        else if( state < NUIControl::ECS_SELECTED )
            CUIControl::OnStateChange( rEvent );
    }

}   // OnStateChange


/************************************************************************
*    desc:  Reset and recycle the contexts
************************************************************************/
void CUISubControl::Reset( bool complete )
{
    CUIControl::Reset( complete );

    for( auto iter : m_pSubControlVec )
        iter->Reset( complete );

}   // Reset


/************************************************************************
*    desc:  Handle the mouse move
************************************************************************/
bool CUISubControl::OnMouseMove( const SDL_Event & rEvent )
{
    bool result = CUIControl::OnMouseMove( rEvent );

    bool found = OnSubControlMouseMove( rEvent );

    // If the sub control is not found, deactivate them
    if( result && !found )
        DeactivateSubControl();

    return result || found;

}   // HandleMouseMove


/************************************************************************
*    desc:  Handle the sub control mouse move
************************************************************************/
bool CUISubControl::OnSubControlMouseMove( const SDL_Event & rEvent )
{
    bool result(false);
    
    for( size_t i = 0; i < m_pSubControlVec.size() && !result; ++i )
        result = m_pSubControlVec[i]->OnMouseMove( rEvent );

    return result;

}   // HandleSubControlMouseMove


/************************************************************************
*    desc:  Handle the select action
************************************************************************/
bool CUISubControl::HandleSelectAction( const CSelectMsgCracker & msgCracker )
{
    if( m_respondsToSelectMsg )
    {
        return CUIControl::HandleSelectAction( msgCracker );
    }
    else
    {
        for( auto iter : m_pSubControlVec )
            if( iter->HandleSelectAction( msgCracker ) )
                return true;
    }

    return false;

}   // HandleSelectAction


/************************************************************************
*    desc:  Get the pointer to the control if found
*
*    NOTE: This function is mainly for sub controls
************************************************************************/
CUIControl * CUISubControl::FindControl( const std::string & name )
{
    CUIControl * pCtrl = CUIControl::FindControl( name );

    if( pCtrl == nullptr )
        pCtrl = FindSubControl( name );

    return pCtrl;

}   // FindControl

CUIControl * CUISubControl::FindControl( void * pVoid )
{
    CUIControl * pCtrl = CUIControl::FindControl( pVoid );

    if( pCtrl == nullptr )
        pCtrl = FindSubControl( pVoid );

    return pCtrl;

}   // FindControl


/************************************************************************
*    desc:  Get the pointer to the subcontrol if found
*
*    NOTE: This function is mainly for sub controls
************************************************************************/
CUIControl * CUISubControl::FindSubControl( const std::string & name )
{
    CUIControl * pCtrl( nullptr );
    
    for( auto iter : m_pSubControlVec )
        if( (pCtrl = iter->FindControl( name )) != nullptr )
            break;

    return pCtrl;

}   // FindSubControl

CUIControl * CUISubControl::FindSubControl( void * pVoid )
{
    CUIControl * pCtrl( nullptr );
    
    for( auto iter : m_pSubControlVec )
        if( (pCtrl = iter->FindControl( pVoid )) != nullptr )
            break;

    return pCtrl;

}   // FindSubControl


/************************************************************************
*    desc:  Get the sub control via index
*  
*    ret:	CUIControl &
************************************************************************/
CUIControl * CUISubControl::GetSubControl( uint index )
{
    if( index >= m_pSubControlVec.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s")
                % index % m_pSubControlVec.size() % __FUNCTION__ % __LINE__ ));

    return m_pSubControlVec.at(index);

}   // GetSubControl


/************************************************************************
*    desc: Set the first inactive control to be active
*    NOTE: This is mainly here to be virtual for sub controls
************************************************************************/
bool CUISubControl::ActivateFirstInactiveControl()
{
    if( CUIControl::ActivateFirstInactiveControl() )
    {
        bool found(false);
        
        for( auto iter : m_pControlNodeVec )
        {
            if( !found && iter->GetControl()->ActivateFirstInactiveControl() )
            {
                m_pActiveNode = iter;
                
                found = true;
            }
            else
            {
                iter->GetControl()->DeactivateControl();
            }
        }

        return true;
    }

    return false;

}   // ActivateFirstInactiveControl


/************************************************************************
*    desc:  Deactivate the control
************************************************************************/
void CUISubControl::DeactivateControl()
{
    CUIControl::DeactivateControl();
    
    DeactivateSubControl();

}   // DeactivateControl


/************************************************************************
*    desc:  Deactivate the sub control
************************************************************************/
void CUISubControl::DeactivateSubControl()
{
    for( auto iter : m_pSubControlVec )
        iter->DeactivateControl();

}   // DeactivateSubControl


/************************************************************************
*    desc:  Check if control is a sub control
************************************************************************/
bool CUISubControl::IsSubControl() const
{
    return true;

}   // IsSubControl



/************************************************************************
*    desc:  Disable the control
************************************************************************/
void CUISubControl::DisableControl()
{
    CUIControl::DisableControl();
    
    for( auto iter : m_pSubControlVec )
        iter->DisableControl();

}   // DisableControl


/************************************************************************
*    desc:  Enable the control to the inactive state
************************************************************************/
void CUISubControl::EnableControl()
{
    CUIControl::EnableControl();
    
    for( auto iter : m_pSubControlVec )
        iter->EnableControl();

}   // EnableControl


/************************************************************************
*    desc:  Set the alpha value of this control
************************************************************************/
void CUISubControl::SetAlpha( float alpha )
{
    CUIControl::SetAlpha( alpha );
            
    for( auto iter : m_pSubControlVec )
        iter->SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
*    desc:  Get the pointer to the active control
*           This is mostly needed for sub controls
************************************************************************/
CUIControl * CUISubControl::GetPtrToActiveControl()
{
    CUIControl * pResult(nullptr);
    
    for( auto iter : m_pSubControlVec )
    {
        if( iter->GetState() > NUIControl::ECS_INACTIVE )
        {
            pResult = iter->GetPtrToActiveControl();
            break;
        }
    }
    
    return pResult;
    
}   // GetPtrToActiveControl
