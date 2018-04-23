
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
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CUISubControl::~CUISubControl()
{
    NDelFunc::DeleteVectorPointers( m_pControlNodeVec );
    NDelFunc::DeleteVectorPointers( m_pSubControlVec );
}


/************************************************************************
*    desc:  Load the control specific info from XML node
************************************************************************/
void CUISubControl::loadControlFromNode( const XMLNode & node )
{
    // Have the parent load it's stuff
    CUIControl::loadControlFromNode( node );

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
            m_pSubControlVec.push_back( NUIControlFactory::Create( controlNode, getGroup() ) );

            // Get the control name
            const std::string controlName = m_pSubControlVec.back()->getName();

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
                findNodes( controlNode, i, navNodeMap );
            }
        }
    }
}


/************************************************************************
*    desc:  Init the control
************************************************************************/
void CUISubControl::init()
{
    CUIControl::init();

    // Init all controls
    for( auto iter : m_pSubControlVec )
        iter->init();
}


/************************************************************************
*    desc:  Do some cleanup
************************************************************************/
void CUISubControl::cleanUp()
{
    CUIControl::cleanUp();

    // Init all controls
    for( auto iter : m_pSubControlVec )
        iter->cleanUp();
}


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CUISubControl::findNodes(
    const XMLNode & node,
    int nodeIndex,
    NavHelperMap & navNodeMap )
{
    const XMLNode navNode = node.getChildNode( "navigate" );
    if( !navNode.isEmpty() )
    {
        setNodes( navNode, nodeIndex, "up", CUIControlNavNode::ENAV_NODE_UP, navNodeMap );
        setNodes( navNode, nodeIndex, "down", CUIControlNavNode::ENAV_NODE_DOWN, navNodeMap );
        setNodes( navNode, nodeIndex, "left", CUIControlNavNode::ENAV_NODE_LEFT, navNodeMap );
        setNodes( navNode, nodeIndex, "right", CUIControlNavNode::ENAV_NODE_RIGHT, navNodeMap );
    }
}


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CUISubControl::setNodes(
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
            m_pControlNodeVec[nodeIndex]->setNode( navNode, iter->second );
        }
        else
        {
            throw NExcept::CCriticalException("Control Node Find Error!",
                boost::str( boost::format("Control node doesn't exist (%s).\n\n%s\nLine: %s")
                    % name % __FUNCTION__ % __LINE__ ));
        }
    }
}


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUISubControl::update()
{
    // Call the parent
    CUIControl::update();

    // Update all controls
    for( auto iter : m_pSubControlVec )
        iter->update();
}


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
}


/************************************************************************
*    desc:  Render the sub control
************************************************************************/
void CUISubControl::render( const CMatrix & matrix )
{
    // Call the parent
    CUIControl::render( matrix );

    for( auto iter : m_pSubControlVec )
        iter->render( matrix );
}


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CUISubControl::handleEvent( const SDL_Event & rEvent )
{
    // Call the parent
    CUIControl::handleEvent( rEvent );

    for( auto iter : m_pSubControlVec )
        iter->handleEvent( rEvent );

    if( isActive() )
    {
        if( (rEvent.type >= NMenu::EGE_MENU_UP_ACTION) &&
            (rEvent.type <= NMenu::EGE_MENU_RIGHT_ACTION) )
        {
            if( rEvent.type == NMenu::EGE_MENU_UP_ACTION )
            {
                onUpAction( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_DOWN_ACTION )
            {
                onDownAction( rEvent );
            }
            if( rEvent.type == NMenu::EGE_MENU_LEFT_ACTION )
            {
                onLeftAction( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_RIGHT_ACTION )
            {
                onRightAction( rEvent );
            }
        }
        else if( (rEvent.type >= NMenu::EGE_MENU_SCROLL_UP) &&
                 (rEvent.type <= NMenu::EGE_MENU_SCROLL_RIGHT) )
        {
            if( rEvent.type == NMenu::EGE_MENU_SCROLL_UP )
            {
                onUpScroll( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_SCROLL_DOWN )
            {
                onDownScroll( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_SCROLL_LEFT )
            {
                onLeftScroll( rEvent );
            }
            else if( rEvent.type == NMenu::EGE_MENU_SCROLL_RIGHT )
            {
                onRightScroll( rEvent );
            }
        }
        else if( rEvent.type == NMenu::EGE_MENU_TAB_LEFT )
        {
            onTabLeft( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_TAB_RIGHT )
        {
            onTabRight( rEvent );
        }
    }
}


/************************************************************************
*    desc:  Handle OnUpAction message
************************************************************************/
void CUISubControl::onUpAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_UP );
}

/************************************************************************
*    desc:  Handle OnMenuDown message
************************************************************************/
void CUISubControl::onDownAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_DOWN );
}

/************************************************************************
*    desc:  Handle OnMenuLeft message
************************************************************************/
void CUISubControl::onLeftAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_LEFT );
}

/************************************************************************
*    desc:  Handle OnRightAction message
************************************************************************/
void CUISubControl::onRightAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_RIGHT );
}

/************************************************************************
*    desc:  Handle OnUpScroll message
************************************************************************/
void CUISubControl::onUpScroll( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_UP );
}

/************************************************************************
*    desc:  Handle OnUpScroll message
************************************************************************/
void CUISubControl::onDownScroll( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_DOWN );
}

/************************************************************************
*    desc:  Handle OnRightScroll message
************************************************************************/
void CUISubControl::onLeftScroll( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_LEFT );
}

/************************************************************************
*    desc:  Handle OnRightScroll message
************************************************************************/
void CUISubControl::onRightScroll( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_RIGHT );
}

/************************************************************************
*    desc:  Handle OnTabLeft message
************************************************************************/
void CUISubControl::onTabLeft( const SDL_Event & rEvent )
{
    // Do nothing
}

/************************************************************************
*    desc:  Handle OnTabRight message
************************************************************************/
void CUISubControl::onTabRight( const SDL_Event & rEvent )
{
    // Do nothing
}


/************************************************************************
*    desc:  Navigate the menu. Find the next control node that isn't
*           disabled and make it the active control node
************************************************************************/
void CUISubControl::navigateMenu( CUIControlNavNode::ENavNode navNode )
{
    if( m_pActiveNode != nullptr )
    {
        CUIControlNavNode * pNavNode = m_pActiveNode;

        do
        {
            pNavNode = pNavNode->getNode( navNode );

            if( pNavNode == nullptr )
            {
                break;
            }
            else if( !m_pActiveNode->getControl()->isDisabled() )
            {
                m_pActiveNode = pNavNode;

                NGenFunc::DispatchEvent(
                    NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
                    NUIControl::ECS_ACTIVE,
                    pNavNode->getControl() );

                break;
            }
        }
        while( true );
    }
}


/************************************************************************
*    desc:  Handle OnStateChange message
************************************************************************/
void CUISubControl::onStateChange( const SDL_Event & rEvent )
{
    if( m_respondsToSelectMsg )
    {
        CUIControl::onStateChange( rEvent );
    }
    else
    {
        NUIControl::EControlState state = NUIControl::EControlState(rEvent.user.code);

        CUIControl * pCtrl = findSubControl( rEvent.user.data1 );

        // Restart the active state of the sub control if something
        // changed in the child controls or their children controls
        if( (state == NUIControl::ECS_ACTIVE) && (pCtrl != nullptr) )
        {
            if( pCtrl->getState() != state )
            {
                setState(state, true);

                recycleContext();

                setDisplayState();
            }
        }
        // The sub control doesn't respond to selected message
        else if( state < NUIControl::ECS_SELECTED )
            CUIControl::onStateChange( rEvent );
    }
}


/************************************************************************
*    desc:  Reset and recycle the contexts
************************************************************************/
void CUISubControl::reset( bool complete )
{
    CUIControl::reset( complete );

    for( auto iter : m_pSubControlVec )
        iter->reset( complete );
}


/************************************************************************
*    desc:  Handle the mouse move
************************************************************************/
bool CUISubControl::onMouseMove( const SDL_Event & rEvent )
{
    bool result = CUIControl::onMouseMove( rEvent );

    bool found = onSubControlMouseMove( rEvent );

    // If the sub control is not found, deactivate them
    if( result && !found )
        deactivateSubControl();

    return result || found;
}


/************************************************************************
*    desc:  Handle the sub control mouse move
************************************************************************/
bool CUISubControl::onSubControlMouseMove( const SDL_Event & rEvent )
{
    bool result(false);

    for( size_t i = 0; i < m_pSubControlVec.size() && !result; ++i )
        result = m_pSubControlVec[i]->onMouseMove( rEvent );

    return result;
}


/************************************************************************
*    desc:  Handle the select action
************************************************************************/
bool CUISubControl::handleSelectAction( const CSelectMsgCracker & msgCracker )
{
    if( m_respondsToSelectMsg )
    {
        return CUIControl::handleSelectAction( msgCracker );
    }
    else
    {
        for( auto iter : m_pSubControlVec )
            if( iter->handleSelectAction( msgCracker ) )
                return true;
    }

    return false;
}


/************************************************************************
*    desc:  Get the pointer to the control if found
*
*    NOTE: This function is mainly for sub controls
************************************************************************/
CUIControl * CUISubControl::findControl( const std::string & name )
{
    CUIControl * pCtrl = CUIControl::findControl( name );

    if( pCtrl == nullptr )
        pCtrl = findSubControl( name );

    return pCtrl;
}

CUIControl * CUISubControl::findControl( void * pVoid )
{
    CUIControl * pCtrl = CUIControl::findControl( pVoid );

    if( pCtrl == nullptr )
        pCtrl = findSubControl( pVoid );

    return pCtrl;
}


/************************************************************************
*    desc:  Get the pointer to the subcontrol if found
*
*    NOTE: This function is mainly for sub controls
************************************************************************/
CUIControl * CUISubControl::findSubControl( const std::string & name )
{
    CUIControl * pCtrl( nullptr );

    for( auto iter : m_pSubControlVec )
        if( (pCtrl = iter->findControl( name )) != nullptr )
            break;

    return pCtrl;
}

CUIControl * CUISubControl::findSubControl( void * pVoid )
{
    CUIControl * pCtrl( nullptr );

    for( auto iter : m_pSubControlVec )
        if( (pCtrl = iter->findControl( pVoid )) != nullptr )
            break;

    return pCtrl;
}


/************************************************************************
*    desc:  Get the sub control via index
*
*    ret:	CUIControl &
************************************************************************/
CUIControl * CUISubControl::getSubControl( uint index )
{
    if( index >= m_pSubControlVec.size() )
        throw NExcept::CCriticalException("Index out of range",
            boost::str( boost::format("Index out of range of vector size (%d,%d).\n\n%s\nLine: %s")
                % index % m_pSubControlVec.size() % __FUNCTION__ % __LINE__ ));

    return m_pSubControlVec.at(index);
}


/************************************************************************
*    desc: Set the first inactive control to be active
*    NOTE: This is mainly here to be virtual for sub controls
************************************************************************/
bool CUISubControl::activateFirstInactiveControl()
{
    if( CUIControl::activateFirstInactiveControl() )
    {
        bool found(false);

        for( auto iter : m_pControlNodeVec )
        {
            if( !found && iter->getControl()->activateFirstInactiveControl() )
            {
                m_pActiveNode = iter;

                found = true;
            }
            else
            {
                iter->getControl()->deactivateControl();
            }
        }

        return true;
    }

    return false;
}


/************************************************************************
*    desc:  Deactivate the control
************************************************************************/
void CUISubControl::deactivateControl()
{
    CUIControl::deactivateControl();

    deactivateSubControl();
}


/************************************************************************
*    desc:  Deactivate the sub control
************************************************************************/
void CUISubControl::deactivateSubControl()
{
    for( auto iter : m_pSubControlVec )
        iter->deactivateControl();
}


/************************************************************************
*    desc:  Check if control is a sub control
************************************************************************/
bool CUISubControl::isSubControl() const
{
    return true;
}


/************************************************************************
*    desc:  Disable the control
************************************************************************/
void CUISubControl::disableControl()
{
    CUIControl::disableControl();

    for( auto iter : m_pSubControlVec )
        iter->disableControl();
}


/************************************************************************
*    desc:  Enable the control to the inactive state
************************************************************************/
void CUISubControl::enableControl()
{
    CUIControl::enableControl();

    for( auto iter : m_pSubControlVec )
        iter->enableControl();
}


/************************************************************************
*    desc:  Set the alpha value of this control
************************************************************************/
void CUISubControl::setAlpha( float alpha )
{
    CUIControl::setAlpha( alpha );

    for( auto iter : m_pSubControlVec )
        iter->setAlpha( alpha );
}


/************************************************************************
*    desc:  Get the pointer to the active control
*           This is mostly needed for sub controls
************************************************************************/
CUIControl * CUISubControl::getPtrToActiveControl()
{
    CUIControl * pResult(nullptr);

    for( auto iter : m_pSubControlVec )
    {
        if( iter->getState() > NUIControl::ECS_INACTIVE )
        {
            pResult = iter->getPtrToActiveControl();
            break;
        }
    }

    return pResult;
} 
