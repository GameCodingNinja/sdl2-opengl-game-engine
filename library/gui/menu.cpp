
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
#include <script/scriptglobals.h>
#include <2d/sprite2d.h>

// Boost lib dependencies
#include <boost/format.hpp>
#include <boost/bind.hpp>

// AngelScript lib dependencies
#include <angelscript.h>

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
    SetVisible(false);

}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CMenu::~CMenu()
{
    NDelFunc::DeleteVectorPointers( m_pStaticControlVec );
    NDelFunc::DeleteVectorPointers( m_pMouseOnlyControlVec );

    NDelFunc::DeleteVectorPointers( m_pControlNodeVec );
    NDelFunc::DeleteVectorPointers( m_pControlVec );

}   // destructor


/************************************************************************
*    desc:  Load the menu info from file
************************************************************************/
void CMenu::LoadFromXML( const std::string & filePath )
{
    // Open and parse the XML file:
    const XMLNode mainNode = XMLNode::openFileHelper( filePath.c_str(), "menu" );
    
    // Init the script functions
    InitScriptFunctions( mainNode );

    // Load the scroll data from node
    m_scrollParam.LoadFromNode( mainNode.getChildNode( "scroll" ) );

    // Get the static sprite groups
    const XMLNode staticSpriteNode = mainNode.getChildNode( "spriteList" );
    if( !staticSpriteNode.isEmpty() )
    {
        for( int i = 0; i < staticSpriteNode.nChildNode(); ++i )
        {
            XMLNode spriteNode = staticSpriteNode.getChildNode( "sprite", i );

            // Load in the static sprite
            LoadStaticSpriteFromNode( spriteNode );
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
            LoadStaticControlFromNode( controlNode );
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
            LoadMouseOnlyControlFromNode( controlNode );
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
            LoadControlFromNode( controlNode, navNodeMap );
        }

        // Map the controls to their respective nodes
        for( int i = 0; i < menuControlsNode.nChildNode(); ++i )
        {
            XMLNode controlNode = menuControlsNode.getChildNode( "control", i );

            // Find the reference nodes
            FindNodes( controlNode, i, navNodeMap );
        }
    }

}   // LoadFromXML


/************************************************************************
*    desc:  Init the script functions and add them to the map
*           This function loads the attribute info reguardless of what it is
************************************************************************/
void CMenu::InitScriptFunctions( const XMLNode & node )
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
    
}   // InitScriptFunctions


/************************************************************************
*    desc:  Load a static sprite from an XML node
************************************************************************/
void CMenu::LoadStaticSpriteFromNode( const XMLNode & node )
{
    // Get the type of object
    std::string objectName = node.getAttribute( "objectName" );

    // Allocate the static sprite in the container
    m_spriteDeq.emplace_back( CObjectDataMgr::Instance().GetData2D( m_group, objectName ) );

    // Load the transform data
    m_spriteDeq.back().LoadTransFromNode( node );

    // Init the script functions
    m_spriteDeq.back().InitScriptFunctions( node );

}   // LoadStaticSpriteFromNode


/************************************************************************
*    desc:  Load static controls from an XML node
************************************************************************/
void CMenu::LoadStaticControlFromNode( XMLNode & node )
{
    // New up the control with its respected control type
    m_pStaticControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Does this control have a name then add it to the map
    if( !m_pStaticControlVec.back()->GetName().empty() )
    {
        // Map of menu controls
        m_pControlMap.emplace( m_pStaticControlVec.back()->GetName(), m_pStaticControlVec.back() );
    }

}   // LoadControlFromNode


/************************************************************************
*    desc:  Load mouse only controls from an XML node
************************************************************************/
void CMenu::LoadMouseOnlyControlFromNode( const XMLNode & node )
{
    // New up the control with its respected control type
    m_pMouseOnlyControlVec.push_back( NUIControlFactory::Create( node, m_group ) );
    
    // Does this control have a name then add it to the map
    if( !m_pMouseOnlyControlVec.back()->GetName().empty() )
        m_pControlMap.emplace( m_pMouseOnlyControlVec.back()->GetName(), m_pMouseOnlyControlVec.back() );

}   // LoadMouseOnlyControlFromNode


/************************************************************************
*    desc:  Load a control from an XML node
************************************************************************/
void CMenu::LoadControlFromNode( XMLNode & node, NavHelperMap & navNodeMap )
{
    // Add the control to the vector
    m_pControlVec.push_back( NUIControlFactory::Create( node, m_group ) );

    // Get the control name
    const std::string controlName = m_pControlVec.back()->GetName();

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

}   // LoadControlFromNode


/************************************************************************
*    desc:  Load the dynamic offset data from node
************************************************************************/
void CMenu::LoadDynamicOffsetFromNode( const XMLNode & node )
{
    // Load the dynamic offset
    m_dynamicOffset = NParseHelper::LoadDynamicOffset( node );

    // Set the dynamic position
    SetDynamicPos();

}   // LoadDynamicOffsetFromNode


/************************************************************************
*    desc:  Set the dynamic position
************************************************************************/
void CMenu::SetDynamicPos()
{
    // Position the menu based on the dynamic offset
    if( !m_dynamicOffset.IsEmpty() )
        SetPos( m_dynamicOffset.GetPos( CSettings::Instance().GetDefaultSizeHalf() ) );

}   // SetDynamicPos


/************************************************************************
*    desc:  Reset the dynamic position
************************************************************************/
void CMenu::ResetDynamicPos()
{
    SetDynamicPos();
    
    for( auto iter : m_pStaticControlVec )
        iter->SetDynamicPos();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->SetDynamicPos();

    for( auto iter : m_pControlVec )
        iter->SetDynamicPos();

}   // ResetDynamicPos


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CMenu::FindNodes(
    const XMLNode & node,
    int nodeIndex,
    NavHelperMap & navNodeMap )
{
    const XMLNode navNode = node.getChildNode( "navigate" );
    if( !navNode.isEmpty() )
    {
        SetNodes( navNode, nodeIndex, "up",    CUIControlNavNode::ENAV_NODE_UP,    navNodeMap );
        SetNodes( navNode, nodeIndex, "down",  CUIControlNavNode::ENAV_NODE_DOWN,  navNodeMap );
        SetNodes( navNode, nodeIndex, "left",  CUIControlNavNode::ENAV_NODE_LEFT,  navNodeMap );
        SetNodes( navNode, nodeIndex, "right", CUIControlNavNode::ENAV_NODE_RIGHT, navNodeMap );
    }

}   // FindNodes


/************************************************************************
*    desc:  Find the reference nodes
************************************************************************/
void CMenu::SetNodes(
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
            m_pControlNodeVec.at(nodeIndex)->SetNode( navNode, iter->second );
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
*    desc:  Init the menu controls
************************************************************************/
void CMenu::Init()
{
    for( auto iter : m_pStaticControlVec )
        iter->Init();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->Init();

    for( auto iter : m_pControlVec )
        iter->Init();
    
}   // Init


/************************************************************************
*    desc:  Cleanup the menu controls. Currently only for font usage
************************************************************************/
void CMenu::CleanUp()
{
    for( auto iter : m_pStaticControlVec )
        iter->CleanUp();

    for( auto iter : m_pMouseOnlyControlVec )
        iter->CleanUp();

    for( auto iter : m_pControlVec )
        iter->CleanUp();
    
}   // CleanUp


/************************************************************************
*    desc:  Activate this menu because it's probably a root menu
************************************************************************/
void CMenu::ActivateMenu()
{
    m_state = NMenu::EMS_IDLE;
    SetVisible(true);
    SetAlpha(1.f);
    ActivateFirstInactiveControl();

}   // ActivateMenu


/************************************************************************
*    desc:  Update the menu
************************************************************************/
void CMenu::Update()
{
    m_scriptComponent.Update();
    
    if( IsVisible() )
    {
        for( auto & iter : m_spriteDeq )
            iter.Update();

        for( auto iter : m_pStaticControlVec )
            iter->Update();

        for( auto iter : m_pMouseOnlyControlVec )
            iter->Update();

        for( auto iter : m_pControlVec )
            iter->Update();
    }

}   // Update


/************************************************************************
*    desc:  Transform the menu
************************************************************************/
void CMenu::Transform()
{
    if( IsVisible() )
    {
        CObject2D::Transform();

        for( auto & iter : m_spriteDeq )
            iter.Transform( GetMatrix(), WasWorldPosTranformed() );

        for( auto iter : m_pStaticControlVec )
            iter->Transform( *this );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->Transform( *this );

        for( auto iter : m_pControlVec )
            iter->Transform( *this );
    }

}   // Transform

void CMenu::Transform( const CObject2D & object )
{
    if( IsVisible() )
    {
        CObject2D::Transform( object.GetMatrix(), object.WasWorldPosTranformed() );

        for( auto & iter : m_spriteDeq )
            iter.Transform( GetMatrix(), WasWorldPosTranformed() );

        for( auto iter : m_pStaticControlVec )
            iter->Transform( *this );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->Transform( *this );

        for( auto iter : m_pControlVec )
            iter->Transform( *this );
    }

}   // Transform


/************************************************************************
*    desc:  do the render
************************************************************************/
void CMenu::Render( const CMatrix & matrix )
{
    if( IsVisible() )
    {
        for( auto & iter : m_spriteDeq )
            iter.Render( matrix );

        for( auto iter : m_pStaticControlVec )
            iter->Render( matrix );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->Render( matrix );

        for( auto iter : m_pControlVec )
            iter->Render( matrix );
    }

}   // Render


/************************************************************************
*    desc:  Get the name of the menu
************************************************************************/
const std::string & CMenu::GetName() const
{
    return m_name;

}   // GetName


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CMenu::HandleEvent( const SDL_Event & rEvent )
{
    // Have the controls handle events
    for( auto iter : m_pControlVec )
        iter->HandleEvent( rEvent );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->HandleEvent( rEvent );

    if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
    {
        OnTransIn( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_TRANS_OUT )
    {
        OnTransOut( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_REACTIVATE )
    {
        OnReactivate( rEvent );
    }
    else if( m_state == NMenu::EMS_IDLE )
    {
        if( rEvent.type == NMenu::EGE_MENU_SELECT_ACTION )
        {
            OnSelectAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SET_ACTIVE_CONTROL )
        {
            OnSetActiveControl( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_UP )
        {
            OnUpAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_DOWN )
        {
            OnDownAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_LEFT )
        {
            OnLeftAction( rEvent );
        }
        else if( rEvent.type == NMenu::EGE_MENU_SCROLL_RIGHT )
        {
            OnRightAction( rEvent );
        }
        else if( rEvent.type == SDL_MOUSEMOTION )
        {
            OnMouseMove( rEvent );
        }
        else if( (rEvent.type >= NMenu::EGE_MENU_UP_ACTION) &&
                 (rEvent.type <= NMenu::EGE_MENU_RIGHT_ACTION) )
        {
            if( rEvent.user.code == NDefs::EAP_DOWN )
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
        }
    }

    // Handle any smart menu events
    SmartHandleEvent( rEvent );

}   // HandleEvent

/************************************************************************
*    desc:  Handle OnUpAction message
************************************************************************/
void CMenu::OnUpAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_UP );

}   // OnUpAction

/************************************************************************
*    desc:  Handle OnMenuDown message
************************************************************************/
void CMenu::OnDownAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_DOWN );

}   // OnDownAction

/************************************************************************
*    desc:  Handle OnMenuLeft message
************************************************************************/
void CMenu::OnLeftAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_LEFT );

}   // OnLeftAction

/************************************************************************
*    desc:  Handle OnRightAction message
************************************************************************/
void CMenu::OnRightAction( const SDL_Event & rEvent )
{
    NavigateMenu( CUIControlNavNode::ENAV_NODE_RIGHT );

}   // OnRightAction


/************************************************************************
*    desc:  Navigate the menu. Find the next control node that isn't
*           disabled and make it the active control node
************************************************************************/
void CMenu::NavigateMenu( CUIControlNavNode::ENavNode navNodeAction )
{
    if( m_pActiveNode != nullptr )
    {
        CUIControlNavNode * pNavNode = m_pActiveNode;

        do
        {
            pNavNode = pNavNode->GetNode( navNodeAction );
            
            if( pNavNode == nullptr )
            {
                break;
            }
            else if( !pNavNode->GetControl()->IsDisabled() )
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
*    desc:  Handle OnMouseMove message
************************************************************************/
void CMenu::OnMouseMove( const SDL_Event & rEvent )
{
    for( auto iter : m_pControlNodeVec )
    {
        if( iter->GetControl()->OnMouseMove( rEvent ) )
            m_pActiveNode = iter;
        else
            iter->GetControl()->DeactivateControl();
    }

    for( auto iter : m_pMouseOnlyControlVec )
        if( !iter->OnMouseMove( rEvent ) )
            iter->DeactivateControl();

}   // OnMouseMove


/************************************************************************
*    desc:  Handle OnSelectAction message
************************************************************************/
void CMenu::OnSelectAction( const SDL_Event & rEvent )
{
    bool selectionFound(false);
    CSelectMsgCracker msgCracker;
    msgCracker.SetPackedUnit( rEvent.user.code );

    if( (m_pActiveNode != nullptr) &&
        (m_pActiveNode->GetControl()->HandleSelectAction( msgCracker )) )
    {
        selectionFound = true;
        
        // Set the state to active which will block all messages until the state is reset to idle
        if( m_pActiveNode->GetControl()->GetActionType() > NUIControl::ECAT_NULL )
            m_state = NMenu::EMS_ACTIVE;
    }
    else if( msgCracker.IsDeviceMouse() )
    {
        // For mouse only controls
        for( auto iter : m_pMouseOnlyControlVec )
        {
            if( iter->HandleSelectAction( msgCracker ) )
            {
                selectionFound = true;
                
                // Set the state to active which will block all messages until the state is reset to idle
                if( iter->GetActionType() > NUIControl::ECAT_NULL )
                    m_state = NMenu::EMS_ACTIVE;

                break;
            }
        }
    }
    
    // Try to handle touch presses on a non-active control
    // The mouse just happends to be clicked over a non-active control
    if( !selectionFound && msgCracker.IsDeviceMouse() )
    {
        // Deactivate the control that should be active
        if( (m_pActiveNode != nullptr) &&
            (msgCracker.GetPressType() == m_pActiveNode->GetControl()->GetMouseSelectType()) )
        {
            m_pActiveNode->GetControl()->DeactivateControl();
            
            // Go through all the controls on this menu to try to find the one clicked on
            for( auto iter : m_pControlVec )
            {
                if( iter->HandleSelectAction( msgCracker ) )
                {
                    // Set the state to active which will block all messages until the state is reset to idle
                    if( m_pActiveNode->GetControl()->GetActionType() > NUIControl::ECAT_NULL )
                        m_state = NMenu::EMS_ACTIVE;

                    break;
                }
            }
        }
    }

}   // OnSelectAction

/************************************************************************
*    desc:  Handle OnSetActiveControl message
************************************************************************/
void CMenu::OnSetActiveControl( const SDL_Event & rEvent )
{
    // Set the first inactive control to active
    if( rEvent.user.code == NMenu::EAC_FIRST_ACTIVE_CONTROL )
        ActivateFirstInactiveControl();

}   // OnSetActiveControl

/************************************************************************
*    desc:  Handle OnReactivate message
************************************************************************/
void CMenu::OnReactivate( const SDL_Event & rEvent )
{
    m_state = NMenu::EMS_IDLE;

}   // OnReactivate

/************************************************************************
*    desc:  Handle OnTransIn message
************************************************************************/
void CMenu::OnTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenu::ETC_BEGIN )
    {
        Prepare( "transIn" );
        
        m_state = NMenu::EMS_ACTIVE;
    }
    else if( rEvent.user.code == NMenu::ETC_END )
    {
        m_state = NMenu::EMS_IDLE;
    }

}   // OnTransIn

/************************************************************************
*    desc:  Handle OnTransOut message
************************************************************************/
void CMenu::OnTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenu::ETC_BEGIN )
    {
        Prepare( "transOut" );
        
        m_state = NMenu::EMS_ACTIVE;
    }
    else if( rEvent.user.code == NMenu::ETC_END )
    {
        m_state = NMenu::EMS_INACTIVE;
    }

}   // OnTransOut


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
void CMenu::Prepare( const std::string & scriptFuncId )
{
    auto iter = m_scriptFunctionMap.find( scriptFuncId );
    if( iter != m_scriptFunctionMap.end() )
        m_scriptComponent.Prepare(m_group, iter->second, {this});

}   // Prepare


/************************************************************************
*    desc:  Set the first inactive control to be active
************************************************************************/
void CMenu::ActivateFirstInactiveControl()
{
    bool found(false);

    // Activate the first control found and deactivate all the rest
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

}   // ActivateFirstInactiveControl


/************************************************************************
*    desc:  Reset all controls
************************************************************************/
void CMenu::Reset()
{
    for( auto iter : m_pControlVec )
        iter->Reset( true );

    for( auto iter : m_pMouseOnlyControlVec )
        iter->Reset( true );

}   // Reset


/************************************************************************
*    desc:  Get the pointer to the control in question
************************************************************************/
CUIControl * CMenu::GetPtrToControl( const std::string & name )
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

}   // GetPtrToControl


/************************************************************************
*    desc:  Get the pointer to the active control
************************************************************************/
CUIControl * CMenu::GetPtrToActiveControl()
{
    CUIControl * pResult(nullptr);

    for( auto iter : m_pControlVec )
    {
        if( iter->GetState() > NUIControl::ECS_INACTIVE )
        {
            pResult = iter->GetPtrToActiveControl();
            break;
        }
    }

    return pResult;

}   // GetPtrToActiveControl


/************************************************************************
*    desc:  Does this menu use dynamic offsets
************************************************************************/
bool CMenu::IsDynamicOffset()
{
    return !m_dynamicOffset.IsEmpty();

}   // IsDynamicOffset


/************************************************************************
*    desc:  Get the scroll params
************************************************************************/
CScrollParam & CMenu::GetScrollParam( int msg )
{
    if( (m_pActiveNode != nullptr) &&
        m_pActiveNode->GetControl()->CanScroll(msg) )
    {
        return m_pActiveNode->GetControl()->GetScrollParam();
    }

    return m_scrollParam;

}   // GetScrollParam


/************************************************************************
*    desc:  Set the smart menu pointer. This class owns the pointer
************************************************************************/
void CMenu::SetSmartGui( CSmartGuiMenu * pSmartGuiMenu )
{
    m_upSmartGui.reset( pSmartGuiMenu );

}   // SetSmartGui


/************************************************************************
*    desc:  Do any smart create
************************************************************************/
void CMenu::SmartCreate()
{
    if( m_upSmartGui )
        m_upSmartGui->Create();

}   // SmartCreate


/************************************************************************
*    desc:  Do any smart event handling
************************************************************************/
void CMenu::SmartHandleEvent( const SDL_Event & rEvent )
{
    if( m_upSmartGui )
        m_upSmartGui->HandleEvent( rEvent );

}   // SmartHandleEvent


/************************************************************************
*    desc:  Set/Get the alpha value of this menu
************************************************************************/
void CMenu::SetAlpha( float alpha )
{
    if( IsVisible() )
    { 
        for( auto & iter : m_spriteDeq )
            iter.SetAlpha( alpha );

        for( auto iter : m_pStaticControlVec )
            iter->SetAlpha( alpha );

        for( auto iter : m_pMouseOnlyControlVec )
            iter->SetAlpha( alpha );

        for( auto iter : m_pControlVec )
            iter->SetAlpha( alpha );
    }

    m_alpha = alpha;
    
}   // SetAlpha

float CMenu::GetAlpha()
{
    return m_alpha;
    
}   // GetAlpha


/************************************************************************
*    desc:  Is the menu idle
************************************************************************/
bool CMenu::IsIdle()
{
    return (m_state == NMenu::EMS_IDLE);
    
}   // IsIdle


/************************************************************************
*    desc:  Is the menu active
************************************************************************/
bool CMenu::IsActive()
{
    return (m_state == NMenu::EMS_ACTIVE);
    
}   // IsActive


/************************************************************************
*    desc:  Register the class with AngelScript
************************************************************************/
void CMenu::Register( asIScriptEngine * pEngine )
{
    using namespace NScriptGlobals;
    
    // Register CScriptComponent2d reference and methods
    Throw( pEngine->RegisterObjectType(  "CMenu", 0, asOBJ_REF|asOBJ_NOCOUNT) );
    Throw( pEngine->RegisterObjectMethod("CMenu", "void SetAlpha(float alpha)",  asMETHOD(CMenu, SetAlpha), asCALL_THISCALL) );
    Throw( pEngine->RegisterObjectMethod("CMenu", "float GetAlpha()",            asMETHOD(CMenu, GetAlpha), asCALL_THISCALL) );
    
    // Need to use CObject for the below to compile under MSVSC++
    Throw( pEngine->RegisterObjectMethod("CMenu", "void SetVisible(bool visible)", asMETHOD(CObject, SetVisible), asCALL_THISCALL) );
    
}   // Register