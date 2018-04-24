
/************************************************************************
*    FILE NAME:       menu.cpp
*
*    DESCRIPTION:     Class for user interface menu
************************************************************************/

// Physical component dependency
#include <gui/menu.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/exceptionhandling.h>
#include <utilities/deletefuncs.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/settings.h>
#include <utilities/genfunc.h>
#include <objectdata/objectdatamanager.h>
#include <gui/menudefs.h>
#include <gui/uicontroldefs.h>
#include <gui/uilabel.h>
#include <gui/uicontrolfactory.h>
#include <gui/ismartguibase.h>
#include <gui/messagecracker.h>
#include <managers/actionmanager.h>
#include <2d/sprite2d.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/bind.hpp>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CMenu::CMenu( const std::string & name, const std::string & group ) :
    m_name(name),
    m_group(group),
    m_pActiveNode(nullptr),
    m_state(NMenu::EMS_INACTIVE),
    m_alpha(0.f)
{
    // The menu needs to default hidden
    setVisible(false);
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CMenu::~CMenu()
{
    NDelFunc::DeleteVectorPointers( m_pStaticControlVec );
    NDelFunc::DeleteVectorPointers( m_pMouseOnlyControlVec );

    NDelFunc::DeleteVectorPointers( m_pControlNodeVec );
    NDelFunc::DeleteVectorPointers( m_pControlVec );
}


/************************************************************************
*    desc:  Load the menu info from file
************************************************************************/
void CMenu::loadFromXML( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "menu" );

    // Init the script functions
    initScriptFunctions( mainNode );

    // Load the scroll data from node
    m_scrollParam.loadFromNode( mainNode.getChildNode( "scroll" ) );

    // Get the static sprite groups
    const XMLNode staticSpriteNode = mainNode.getChildNode( "spriteList" );
    if( !staticSpriteNode.isEmpty() )
    {
        for( int i = 0; i < staticSpriteNode.nChildNode(); ++i )
        {
            XMLNode spriteNode = staticSpriteNode.getChildNode( "sprite", i );

            // Load in the static sprite
            loadStaticSpriteFromNode( spriteNode );
        }
    }

    // Get the static menu controls node
    XMLNode staticControlsNode = mainNode.getChildNode( "staticMenuControls" );
    if( !staticControlsNode.isEmpty() )
    {
        m_pStaticControlVec.reserve( staticControlsNode.nChildNode() );

        for( int i = 0; i < staticControlsNode.nChildNode(); ++i )
        {
            // Get the menu controls
            XMLNode controlNode = staticControlsNode.getChildNode( "control", i );

            // Load static controls from an XML node
            loadStaticControlFromNode( controlNode );
        }
    }

    // Get the mouse only menu controls node
    XMLNode mouseOnlyMenuControlsNode = mainNode.getChildNode( "mouseOnlyControls" );
    if( !mouseOnlyMenuControlsNode.isEmpty() )
    {
        m_pMouseOnlyControlVec.reserve( mouseOnlyMenuControlsNode.nChildNode() );

        for( int i = 0; i < mouseOnlyMenuControlsNode.nChildNode(); ++i )
        {
            // Get the menu controls
            XMLNode controlNode = mouseOnlyMenuControlsNode.getChildNode( "control", i );

            // Load static controls from an XML node
            loadMouseOnlyControlFromNode( controlNode );
        }
    }

    // Get the menu controls
    XMLNode menuControlsNode = mainNode.getChildNode( "menuControls" );
    if( !menuControlsNode.isEmpty() )
    {
        // map to help setup the node pointers
        NavHelperMap navNodeMap;

        m_pControlVec.reserve( menuControlsNode.nChildNode() );

        // Load the controls
        for( int i = 0; i < menuControlsNode.nChildNode(); ++i )
        {
            XMLNode controlNode = menuControlsNode.getChildNode( "control", i );

            // Load in the control
            loadControlFromNode( controlNode, navNodeMap );
        }

        // Map the controls to their respective nodes
        for( int i = 0; i < menuControlsNode.nChildNode(); ++i )
        {
            XMLNode controlNode = menuControlsNode.getChildNode( "control", i );

            // Find the reference nodes
            findNodes( controlNode, i, navNodeMap );
        }
    }
}


/************************************************************************
*    desc:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CMenu::initScriptFunctions( const XMLNode & node )
{
    // Check for scripting - Add an empty string for scripts not defined
    XMLNode scriptLstNode = node.getChildNode( "scriptLst" );
    if( !scriptLstNode.isEmpty() )
    {
        for( int i = 0; i < scriptLstNode.nChildNode(); ++i )
        {
            const XMLNode scriptNode = scriptLstNode.getChildNode(i);

            // Only the first attribute is used
            const XMLAttribute attribute = scriptNode.getAttribute(0);
            const std::string attrName = attribute.lpszName;
            const std::string attrValue = attribute.lpszValue;

            // Add the attribute name and value to the map
            if( !attrValue.empty() )
                m_scriptFunctionMap.emplace( attrName, attrValue );
        }
    }
}


/************************************************************************
*    desc:  Load a static sprite from an XML node
************************************************************************/
void CMenu::loadStaticSpriteFromNode( const XMLNode & node )
{
    // Get the type of object
    std::string objectName = node.getAttribute( "objectName" );

    // Allocate the static sprite in the container
    m_spriteDeq.emplace_back( CObjectDataMgr::Instance().getData2D( m_group, objectName ) );

    // Load the transform data
    m_spriteDeq.back().loadTransFromNode( node );

    // Init the script functions
    m_spriteDeq.back().initScriptFunctions( node );
}


/************************************************************************
*    desc:  Load static controls from an XML node
************************************************************************/
void CMenu::loadStaticControlFromNode( XMLNode & node )
{
    // New up the control with its respected control type
    m_pStaticControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Does this control have a name then add it to the map
    if( !m_pStaticControlVec.back()->getName().empty() )
    {
        // Map of menu controls
        m_pControlMap.emplace( m_pStaticControlVec.back()->getName(), m_pStaticControlVec.back() );
    }
}


/************************************************************************
*    desc:  Load mouse only controls from an XML node
************************************************************************/
void CMenu::loadMouseOnlyControlFromNode( const XMLNode & node )
{
    // New up the control with its respected control type
    m_pMouseOnlyControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Does this control have a name then add it to the map
    if( !m_pMouseOnlyControlVec.back()->getName().empty() )
        m_pControlMap.emplace( m_pMouseOnlyControlVec.back()->getName(), m_pMouseOnlyControlVec.back() );
}


/************************************************************************
*    desc:  Load a control from an XML node
************************************************************************/
void CMenu::loadControlFromNode( XMLNode & node, NavHelperMap & navNodeMap )
{
    // Add the control to the vector
    m_pControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Get the control name
    const std::string controlName = m_pControlVec.back()->getName();

    // Does this control have a name then add it to the map
    if( !controlName.empty() )
    {
        // Map of menu controls
        auto iter = m_pControlMap.emplace( controlName, m_pControlVec.back() );

        // Check for duplicate names
        if( !iter.second )
        {
            throw NExcept::CCriticalException("Control Load Error!",
                boost::str( boost::format("Duplicate control name (%s).\n\n%s\nLine: %s")
                    % controlName % __FUNCTION__ % __LINE__ ));
        }

        // Add a node to the vector with it's control
        m_pControlNodeVec.push_back( new CUIControlNavNode( m_pControlVec.back() ) );

        // Map of menu control nodes
        navNodeMap.emplace( controlName, m_pControlNodeVec.back() );
    }
}


/************************************************************************
*    desc:  Load the dynamic offset data from node
************************************************************************/
void CMenu::loadDynamicOffsetFromNode( const XMLNode & node )
{
    // Load the dynamic offset
    m_dynamicOffset = NParseHelper::LoadDynamicOffset( node );

    // Set the dynamic position
    setDynamicPos();
}


/************************************************************************
*    desc:  Set the dynamic position
************************************************************************/
void CMenu::setDynamicPos()
{
    // Position the menu based on the dynamic offset
    if( !m_dynamicOffset.isEmpty() )
        setPos( m_dynamicOffset.getPos( CSettings::Instance().getDefaultSizeHalf() ) );
}


/************************************************************************
*    desc:  Reset the dynamic position
************************************************************************/
void CMenu::resetDynamicPos()
{
    setDynamicPos();

    for( auto iter : m_pStaticControlVec )
        iter->setDynamicPos();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->setDynamicPos();

    for( auto iter : m_pControlVec )
        iter->setDynamicPos();
}


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CMenu::findNodes(
    const XMLNode & node,
    int nodeIndex,
    NavHelperMap & navNodeMap )
{
    const XMLNode navNode = node.getChildNode( "navigate" );
    if( !navNode.isEmpty() )
    {
        setNodes( navNode, nodeIndex, "up",    CUIControlNavNode::ENAV_NODE_UP,    navNodeMap );
        setNodes( navNode, nodeIndex, "down",  CUIControlNavNode::ENAV_NODE_DOWN,  navNodeMap );
        setNodes( navNode, nodeIndex, "left",  CUIControlNavNode::ENAV_NODE_LEFT,  navNodeMap );
        setNodes( navNode, nodeIndex, "right", CUIControlNavNode::ENAV_NODE_RIGHT, navNodeMap );
    }
}


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CMenu::setNodes(
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
            m_pControlNodeVec.at(nodeIndex)->setNode( navNode, iter->second );
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
*    desc:  Init the menu controls
************************************************************************/
void CMenu::init()
{
    for( auto iter : m_pStaticControlVec )
        iter->init();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->init();

    for( auto iter : m_pControlVec )
        iter->init();
}


/************************************************************************
*    desc:  Cleanup the menu controls. Currently only for font usage
************************************************************************/
void CMenu::cleanUp()
{
    for( auto iter : m_pStaticControlVec )
        iter->cleanUp();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->cleanUp();

    for( auto iter : m_pControlVec )
        iter->cleanUp();
}


/************************************************************************
*    desc:  Activate this menu because it's probably a root menu
************************************************************************/
void CMenu::activateMenu()
{
    m_state = NMenu::EMS_IDLE;
    setVisible(true);
    setAlpha(1.f);
    activateFirstInactiveControl();
}


/************************************************************************
*    desc:  Update the menu
************************************************************************/
void CMenu::update()
{
    m_scriptComponent.update();

    if( isVisible() )
    {
        for( auto & iter : m_spriteDeq )
            iter.update();

        for( auto iter : m_pStaticControlVec )
            iter->update();

        for( auto iter : m_pMouseOnlyControlVec )
            iter->update();

        for( auto iter : m_pControlVec )
            iter->update();
    }
}


/************************************************************************
*    desc:  Transform the menu
************************************************************************/
void CMenu::transform()
{
    if( isVisible() )
    {
        CObject2D::transform();

        for( auto & iter : m_spriteDeq )
            iter.transform( getMatrix(), wasWorldPosTranformed() );

        for( auto iter : m_pStaticControlVec )
            iter->transform( *this );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->transform( *this );

        for( auto iter : m_pControlVec )
            iter->transform( *this );
    }
}

void CMenu::transform( const CObject2D & object )
{
    if( isVisible() )
    {
        CObject2D::transform( object.getMatrix(), object.wasWorldPosTranformed() );

        for( auto & iter : m_spriteDeq )
            iter.transform( getMatrix(), wasWorldPosTranformed() );

        for( auto iter : m_pStaticControlVec )
            iter->transform( *this );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->transform( *this );

        for( auto iter : m_pControlVec )
            iter->transform( *this );
    }
}


/************************************************************************
*    desc:  do the render
************************************************************************/
void CMenu::render( const CMatrix & matrix )
{
    if( isVisible() )
    {
        for( auto & iter : m_spriteDeq )
            iter.render( matrix );

        for( auto iter : m_pStaticControlVec )
            iter->render( matrix );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->render( matrix );

        for( auto iter : m_pControlVec )
            iter->render( matrix );
    }
}


/************************************************************************
*    desc:  Get the name of the menu
************************************************************************/
const std::string & CMenu::getName() const
{
    return m_name;
}


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CMenu::handleEvent( const SDL_Event & rEvent )
{
    // Have the controls handle events
    for( auto iter : m_pControlVec )
        iter->handleEvent( rEvent );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->handleEvent( rEvent );

    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
    {
        onTransIn( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_TRANS_OUT )
    {
        onTransOut( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_REACTIVATE )
    {
        onReactivate( rEvent );
    }
    else if( m_state == NMenu::EMS_IDLE )
    {
        if( rEvent.type == NMenu::EGE_MENU_SELECT_ACTION )
        {
            onSelectAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SET_ACTIVE_CONTROL )
        {
            onSetActiveControl( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_UP )
        {
            onUpAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_DOWN )
        {
            onDownAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_LEFT )
        {
            onLeftAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_RIGHT )
        {
            onRightAction( rEvent );
        }
        else if( rEvent.type == SDL_MOUSEMOTION )
        {
            onMouseMove( rEvent );
        }
        else if( (rEvent.type >= NMenu::EGE_MENU_UP_ACTION) &&
                 (rEvent.type <= NMenu::EGE_MENU_RIGHT_ACTION) )
        {
            if( rEvent.user.code == NDefs::EAP_DOWN )
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
        }
    }

    // Handle any smart menu events
    smartHandleEvent( rEvent );
}

/************************************************************************
*    desc:  Handle OnUpAction message
************************************************************************/
void CMenu::onUpAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_UP );
}

/************************************************************************
*    desc:  Handle OnMenuDown message
************************************************************************/
void CMenu::onDownAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_DOWN );
}

/************************************************************************
*    desc:  Handle OnMenuLeft message
************************************************************************/
void CMenu::onLeftAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_LEFT );
}

/************************************************************************
*    desc:  Handle OnRightAction message
************************************************************************/
void CMenu::onRightAction( const SDL_Event & rEvent )
{
    navigateMenu( CUIControlNavNode::ENAV_NODE_RIGHT );
}


/************************************************************************
*    desc:  Navigate the menu. Find the next control node that isn't
*           disabled and make it the active control node
************************************************************************/
void CMenu::navigateMenu( CUIControlNavNode::ENavNode navNodeAction )
{
    if( m_pActiveNode != nullptr )
    {
        CUIControlNavNode * pNavNode = m_pActiveNode;

        do
        {
            pNavNode = pNavNode->getNode( navNodeAction );

            if( pNavNode == nullptr )
            {
                break;
            }
            else if( !pNavNode->getControl()->isDisabled() )
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
*    desc:  Handle OnMouseMove message
************************************************************************/
void CMenu::onMouseMove( const SDL_Event & rEvent )
{
    for( auto iter : m_pControlNodeVec )
    {
        if( iter->getControl()->onMouseMove( rEvent ) )
            m_pActiveNode = iter;
        else
            iter->getControl()->deactivateControl();
    }

    for( auto iter : m_pMouseOnlyControlVec )
        if( !iter->onMouseMove( rEvent ) )
            iter->deactivateControl();
}


/************************************************************************
*    desc:  Handle OnSelectAction message
************************************************************************/
void CMenu::onSelectAction( const SDL_Event & rEvent )
{
    bool selectionFound(false);
    CSelectMsgCracker msgCracker;
    msgCracker.setPackedUnit( rEvent.user.code );

    if( (m_pActiveNode != nullptr) &&
        (m_pActiveNode->getControl()->handleSelectAction( msgCracker )) )
    {
        selectionFound = true;

        // Set the state to active which will block all messages until the state is reset to idle
        if( m_pActiveNode->getControl()->getActionType() > NUIControl::ECAT_NULL )
            m_state = NMenu::EMS_ACTIVE;
    }
    else if( msgCracker.isDeviceMouse() )
    {
        // For mouse only controls
        for( auto iter : m_pMouseOnlyControlVec )
        {
            if( iter->handleSelectAction( msgCracker ) )
            {
                selectionFound = true;

                // Set the state to active which will block all messages until the state is reset to idle
                if( iter->getActionType() > NUIControl::ECAT_NULL )
                    m_state = NMenu::EMS_ACTIVE;

                break;
            }
        }
    }

    // Try to handle touch presses on a non-active control
    // The mouse just happends to be clicked over a non-active control
    if( !selectionFound && msgCracker.isDeviceMouse() )
    {
        // Deactivate the control that should be active
        if( (m_pActiveNode != nullptr) &&
            (msgCracker.getPressType() == m_pActiveNode->getControl()->getMouseSelectType()) )
        {
            m_pActiveNode->getControl()->deactivateControl();

            // Go through all the controls on this menu to try to find the one clicked on
            for( auto iter : m_pControlVec )
            {
                if( iter->handleSelectAction( msgCracker ) )
                {
                    // Set the state to active which will block all messages until the state is reset to idle
                    if( m_pActiveNode->getControl()->getActionType() > NUIControl::ECAT_NULL )
                        m_state = NMenu::EMS_ACTIVE;

                    break;
                }
            }
        }
    }
}

/************************************************************************
*    desc:  Handle OnSetActiveControl message
************************************************************************/
void CMenu::onSetActiveControl( const SDL_Event & rEvent )
{
    // Set the first inactive control to active
    if( rEvent.user.code == NMenu::EAC_FIRST_ACTIVE_CONTROL )
        activateFirstInactiveControl();
}

/************************************************************************
*    desc:  Handle OnReactivate message
************************************************************************/
void CMenu::onReactivate( const SDL_Event & rEvent )
{
    m_state = NMenu::EMS_IDLE;
}

/************************************************************************
*    desc:  Handle OnTransIn message
************************************************************************/
void CMenu::onTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenu::ETC_BEGIN )
    {
        prepare( "transIn" );

        m_state = NMenu::EMS_ACTIVE;
    }
    else if( rEvent.user.code == NMenu::ETC_END )
    {
        m_state = NMenu::EMS_IDLE;
    }
}

/************************************************************************
*    desc:  Handle OnTransOut message
************************************************************************/
void CMenu::onTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenu::ETC_BEGIN )
    {
        prepare( "transOut" );

        m_state = NMenu::EMS_ACTIVE;
    }
    else if( rEvent.user.code == NMenu::ETC_END )
    {
        m_state = NMenu::EMS_INACTIVE;
    }
}


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
void CMenu::prepare( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
        m_scriptComponent.prepare(m_group, iter->second, {this});
}


/************************************************************************
*    desc:  Set the first inactive control to be active
************************************************************************/
void CMenu::activateFirstInactiveControl()
{
    bool found(false);

    // Activate the first control found and deactivate all the rest
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
}


/************************************************************************
*    desc:  Reset all controls
************************************************************************/
void CMenu::reset()
{
    for( auto iter : m_pControlVec )
        iter->reset( true );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->reset( true );
}


/************************************************************************
*    desc:  Get the pointer to the control in question
************************************************************************/
CUIControl * CMenu::getPtrToControl( const std::string & name )
{
    // See if the control can be found
    auto iter = m_pControlMap.find( name );

    // Make sure control is available
    if( iter == m_pControlMap.end() )
        throw NExcept::CCriticalException("Menu Control Error!",
            boost::str( boost::format("Control being asked for is missing!(%s)\n\n%s\nLine: %s")
                % name.c_str() % __FUNCTION__ % __LINE__ ));

    // Pass back the pointer if found
    return iter->second;
}


/************************************************************************
*    desc:  Get the pointer to the active control
************************************************************************/
CUIControl * CMenu::getPtrToActiveControl()
{
    CUIControl * pResult(nullptr);

    for( auto iter : m_pControlVec )
    {
        if( iter->getState() > NUIControl::ECS_INACTIVE )
        {
            pResult = iter->getPtrToActiveControl();
            break;
        }
    }

    return pResult;
}


/************************************************************************
*    desc:  Does this menu use dynamic offsets
************************************************************************/
bool CMenu::isDynamicOffset()
{
    return !m_dynamicOffset.isEmpty();
}


/************************************************************************
*    desc:  Get the scroll params
************************************************************************/
CScrollParam & CMenu::getScrollParam( int msg )
{
    if( (m_pActiveNode != nullptr) &&
        m_pActiveNode->getControl()->canScroll(msg) )
    {
        return m_pActiveNode->getControl()->getScrollParam();
    }

    return m_scrollParam;
}


/************************************************************************
*    desc:  Set the smart menu pointer. This class owns the pointer
************************************************************************/
void CMenu::setSmartGui( CSmartGuiMenu * pSmartGuiMenu )
{
    m_upSmartGui.reset( pSmartGuiMenu );
}


/************************************************************************
*    desc:  Do any smart create
************************************************************************/
void CMenu::smartCreate()
{
    if( m_upSmartGui )
        m_upSmartGui->create();
}


/************************************************************************
*    desc:  Do any smart event handling
************************************************************************/
void CMenu::smartHandleEvent( const SDL_Event & rEvent )
{
    if( m_upSmartGui )
        m_upSmartGui->handleEvent( rEvent );
}


/************************************************************************
*    desc:  Set/Get the alpha value of this menu
************************************************************************/
void CMenu::setAlpha( float alpha )
{
    if( isVisible() )
    {
        for( auto & iter : m_spriteDeq )
            iter.setAlpha( alpha );

        for( auto iter : m_pStaticControlVec )
            iter->setAlpha( alpha );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->setAlpha( alpha );

        for( auto iter : m_pControlVec )
            iter->setAlpha( alpha );
    }

    m_alpha = alpha;
}

float CMenu::getAlpha()
{
    return m_alpha;
}


/************************************************************************
*    desc:  Is the menu idle
************************************************************************/
bool CMenu::isIdle()
{
    return (m_state == NMenu::EMS_IDLE);
}


/************************************************************************
*    desc:  Is the menu active
************************************************************************/
bool CMenu::isActive()
{
    return (m_state == NMenu::EMS_ACTIVE);
}
