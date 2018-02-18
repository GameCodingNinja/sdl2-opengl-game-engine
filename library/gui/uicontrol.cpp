
/************************************************************************
*    FILE NAME:       uicontrol.cpp
*
*    DESCRIPTION:     class for user interface controls
************************************************************************/

// Physical component dependency
#include <gui/uicontrol.h>

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <utilities/xmlparsehelper.h>
#include <utilities/genfunc.h>
#include <utilities/settings.h>
#include <utilities/exceptionhandling.h>
#include <gui/menudefs.h>
#include <gui/ismartguibase.h>
#include <gui/messagecracker.h>
#include <objectdata/objectdatamanager.h>
#include <objectdata/objectdata2d.h>
#include <system/device.h>
#include <managers/actionmanager.h>
#include <managers/signalmanager.h>
#include <common/fontproperties.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <cstring>
#include <iostream>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CUIControl::CUIControl( const std::string & group ) :
    CControlBase( group ),
    m_state(NUIControl::ECS_NULL),
    m_lastState(NUIControl::ECS_NULL),
    m_actionType(NUIControl::ECAT_NULL),
    m_mouseSelectType(NDefs::EAP_UP)
{
}   // constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CUIControl::~CUIControl()
{
}   // destructor


/************************************************************************
*    desc:  Load the control info from XML node
*
*    param: node - XML node
************************************************************************/
void CUIControl::LoadFromNode( const XMLNode & node )
{
    CControlBase::LoadFromNode( node );

    // Set the default state of the control
    if( node.isAttributeSet( "defaultState" ) )
        SetDefaultState( std::string( node.getAttribute( "defaultState" ) ) );

    // Set if mouse selection is the down message
    if( node.isAttributeSet( "mouseSelectDown" ) )
        if( std::strcmp( node.getAttribute( "mouseSelectDown" ), "true") == 0 )
            m_mouseSelectType = NDefs::EAP_DOWN;

    // Setup the action
    XMLNode actionNode = node.getChildNode( "action" );
    if( !actionNode.isEmpty() )
    {
        // Set the action type
        if( actionNode.isAttributeSet( "actionType" ) )
            SetActionType( actionNode.getAttribute( "actionType" ) );

        // Set the execution action.
        if( actionNode.isAttributeSet( "executionAction" ) )
            SetExecutionAction( std::string(actionNode.getAttribute( "executionAction" )) );
    }

    // Setup the action
    XMLNode stateScriptNode = node.getChildNode( "stateScript" );
    if( !stateScriptNode.isEmpty() )
    {
        if( stateScriptNode.isAttributeSet( "onDisabled" ) )
            m_scriptFunction.emplace( NUIControl::ECS_DISABLED, stateScriptNode.getAttribute( "onDisabled" ) );
        
        if( stateScriptNode.isAttributeSet( "onInactive" ) )
            m_scriptFunction.emplace( NUIControl::ECS_INACTIVE, stateScriptNode.getAttribute( "onInactive" ) );
        
        if( stateScriptNode.isAttributeSet( "onActive" ) )
            m_scriptFunction.emplace( NUIControl::ECS_ACTIVE, stateScriptNode.getAttribute( "onActive" ) );

        if( stateScriptNode.isAttributeSet( "onSelect" ) )
            m_scriptFunction.emplace( NUIControl::ECS_SELECTED, stateScriptNode.getAttribute( "onSelect" ) );
    }

    // Load the scroll data from node
    m_scrollParam.LoadFromNode( node.getChildNode( "scroll" ) );

    // Get the size modifier info
    m_sizeModifier = NParseHelper::LoadRect( node );

    // Init to the default state
    RevertToDefaultState();

}   // LoadFromNode


/************************************************************************
*    desc:  Load the control specific info from XML node
************************************************************************/
void CUIControl::LoadControlFromNode( const XMLNode & node )
{
    // Get the list of object data associated with this button
    const XMLNode spriteLstNode = node.getChildNode( "spriteList" );

    if( !spriteLstNode.isEmpty() )
    {
        size_t fontSpriteCount(0);

        for( int i = 0; i < spriteLstNode.nChildNode(); ++i )
        {
            const XMLNode spriteNode = spriteLstNode.getChildNode( i );

            // Load the sprite from node
            LoadSpriteFromNode( spriteNode, fontSpriteCount );
        }
    }

}   // LoadControlFromNode


/************************************************************************
*    desc:  Load a sprite from an XML node
************************************************************************/
void CUIControl::LoadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount )
{
    // Get the type of object
    std::string objectName = node.getAttribute( "objectName" );

    // Internally allocate the sprite in the deque
    m_spriteDeq.emplace_back( CObjectDataMgr::Instance().GetData2D( m_group, objectName ) );
    auto & rSprite = m_spriteDeq.back();

    // Load the sprite data
    rSprite.Load( node );

    // See if this sprite is used for rendering a font string
    if( rSprite.GetVisualComponent().IsFontSprite() )
    {
        // Set the font string to be created later
        if( !m_stringVec.empty() && (fontSpriteCount < m_stringVec.size()) )
            rSprite.GetVisualComponent().SetFontString( m_stringVec.at(fontSpriteCount) );

        ++fontSpriteCount;
    }
    else
    {
        // Find the largest size width and height of the different sprites for the controls size
        const float width( rSprite.GetObjectData().GetSize().w + std::fabs( rSprite.GetPos().x ) );
        const float height( rSprite.GetObjectData().GetSize().h + std::fabs( rSprite.GetPos().y ) );

        if( width > m_size.w )
            m_size.w = width;

        if( height > m_size.h )
            m_size.h = height;
    }

}   // LoadSpriteFromNode


/************************************************************************
*    desc:  Update the control
************************************************************************/
void CUIControl::Update()
{
    m_scriptComponent.Update();

    for( auto & iter : m_spriteDeq )
        iter.Update();

}   // Update


/************************************************************************
*    desc:  Transform the control
************************************************************************/
void CUIControl::Transform( const CObject2D & object )
{
    CObject2D::Transform( object.GetMatrix(), object.WasWorldPosTranformed() );

    for( auto & iter : m_spriteDeq )
        iter.Transform( GetMatrix(), WasWorldPosTranformed() );

    TransformCollision();

}   // Transform


/************************************************************************
*    desc:  Transform the collision
************************************************************************/
void CUIControl::TransformCollision()
{
    if( WasWorldPosTranformed() && !m_size.IsEmpty() )
    {
        CMatrix finalMatrix( GetMatrix() );
        finalMatrix.Scale( CSettings::Instance().GetOrthoAspectRatio().GetH() );
        finalMatrix.InvertY();

        // Get half the screen size to convert to screen coordinates
        CSize<float> screenHalf = CSettings::Instance().GetSizeHalf();

        // Create the rect of the control based on half it's size
        float halfwidth = m_size.GetW() * 0.5f;
        float halfHeight = m_size.GetH() * 0.5f;

        CQuad quad;
        quad.point[0].x = -halfwidth + -m_sizeModifier.x1;
        quad.point[0].y = -halfHeight + -m_sizeModifier.y1;
        quad.point[1].x = halfwidth + m_sizeModifier.x2;
        quad.point[1].y = -halfHeight + -m_sizeModifier.y1;
        quad.point[2].x = halfwidth + m_sizeModifier.x2;
        quad.point[2].y = halfHeight + m_sizeModifier.y2;
        quad.point[3].x = -halfwidth + -m_sizeModifier.x1;
        quad.point[3].y = halfHeight + m_sizeModifier.y2;

        finalMatrix.Transform( m_collisionQuad, quad );

        // Convert the translated rect to screen coordinates
        m_collisionQuad.point[0].x += screenHalf.GetW();
        m_collisionQuad.point[0].y += screenHalf.GetH();
        m_collisionQuad.point[1].x += screenHalf.GetW();
        m_collisionQuad.point[1].y += screenHalf.GetH();
        m_collisionQuad.point[2].x += screenHalf.GetW();
        m_collisionQuad.point[2].y += screenHalf.GetH();
        m_collisionQuad.point[3].x += screenHalf.GetW();
        m_collisionQuad.point[3].y += screenHalf.GetH();

        finalMatrix.Transform( m_collisionCenter, CPoint<float>() );

        // Convert to screen coordinates
        m_collisionCenter.x += screenHalf.GetW();
        m_collisionCenter.y += screenHalf.GetH();
    }

}   // TransformCollision


/************************************************************************
*    desc:  do the render
************************************************************************/
void CUIControl::Render( const CMatrix & matrix )
{
    for( auto & iter : m_spriteDeq )
        iter.Render( matrix );

}   // Render


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CUIControl::HandleEvent( const SDL_Event & rEvent )
{
    if( rEvent.type == NMenu::EGE_MENU_CONTROL_STATE_CHANGE )
    {
        OnStateChange( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_SELECT_EXECUTE )
    {
        OnSelectExecute( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_SET_ACTIVE_CONTROL )
    {
        OnSetActiveControl( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_REACTIVATE )
    {
        OnReactivate( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_TRANS_IN )
    {
        OnTransIn( rEvent );
    }
    else if( rEvent.type == NMenu::EGE_MENU_TRANS_OUT )
    {
        OnTransOut( rEvent );
    }

    // Do any smart event handling
    SmartHandleEvent( rEvent );

}   // HandleEvent


/************************************************************************
*    desc:  Handle OnTransIn message
************************************************************************/
void CUIControl::OnTransIn( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenu::ETC_BEGIN )
    {
        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            SetDisplayState();
    }

}   // OnTransIn

/************************************************************************
*    desc:  Handle OnTransOut message
************************************************************************/
void CUIControl::OnTransOut( const SDL_Event & rEvent )
{
    if( rEvent.user.code == NMenu::ETC_BEGIN )
    {
        // Reset the control
        Reset();

        // Recycle the contexts
        RecycleContext();

        // Set the script functions for the current displayed state
        if( m_lastState != m_state )
            SetDisplayState();
    }

}   // OnTransOut

/************************************************************************
*    desc:  Handle OnStateChange message
************************************************************************/
void CUIControl::OnStateChange( const SDL_Event & rEvent )
{
    // This control is the focus of the state change
    // The control's "this" pointer is used as a means of identification
    if( rEvent.user.data1 == (void *)this )
        ChangeState( NUIControl::EControlState(rEvent.user.code) );
    else
        DeactivateControl();

}   // OnStateChange

/************************************************************************
*    desc:  Handle OnSelectExecute message
************************************************************************/
void CUIControl::OnSelectExecute( const SDL_Event & rEvent )
{
    if( m_state == NUIControl::ECS_SELECTED )
    {
        if( m_actionType == NUIControl::ECAT_TO_TREE )
            NGenFunc::DispatchEvent( NMenu::EGE_MENU_TO_TREE, 0, &m_executionAction );
        
        else if( m_actionType == NUIControl::ECAT_TO_MENU )
            NGenFunc::DispatchEvent( NMenu::EGE_MENU_TO_MENU, 0, &m_executionAction, this );

        else if( m_actionType == NUIControl::ECAT_BACK )
            NGenFunc::DispatchEvent( NMenu::EGE_MENU_BACK_ACTION );

        else if( m_actionType == NUIControl::ECAT_CLOSE )
            NGenFunc::DispatchEvent( NMenu::EGE_MENU_TOGGLE_ACTION );

        else if( m_actionType == NUIControl::ECAT_GAME_STATE_CHANGE )
            NGenFunc::DispatchEvent( NMenu::EGE_MENU_GAME_STATE_CHANGE, NMenu::ETC_BEGIN, &m_executionAction );

        else if( m_actionType == NUIControl::ECAT_QUIT_GAME )
            NGenFunc::DispatchEvent( SDL_QUIT );

        // Smart gui execution
        SmartExecuteAction();
        
        // Boost signal execute action
        m_executionActionSignal(this);
    }

}   // OnSelectExecute


/************************************************************************
*    desc:  Handle OnSetActiveControl message
************************************************************************/
void CUIControl::OnSetActiveControl( const SDL_Event & rEvent )
{
    // Set the last active control to be active again
    if( (rEvent.user.code == NMenu::EAC_LAST_ACTIVE_CONTROL) &&
        (m_lastState > NUIControl::ECS_INACTIVE))
    {
        m_lastState = m_state = NUIControl::ECS_ACTIVE;

        // Don't animate the control if the mouse was used
        if( !CActionMgr::Instance().WasLastDeviceMouse() )
        {
            RecycleContext();
            SetDisplayState();
        }
    }

}   // OnSetActiveControl

/************************************************************************
*    desc:  Handle OnReactivate message
************************************************************************/
void CUIControl::OnReactivate( const SDL_Event & rEvent )
{
    // Set the last active control to be active again
    if( m_state > NUIControl::ECS_INACTIVE )
    {
        m_lastState = m_state = NUIControl::ECS_ACTIVE;
        
        // Don't animate the control if the mouse was used
        if( !CActionMgr::Instance().WasLastDeviceMouse() ||
            IsPointInControl( CActionMgr::Instance().GetLastMousePos() ) )
        {
            RecycleContext();
            SetDisplayState();
        }
    }

}   // OnReactivate


/************************************************************************
*    desc:  Handle the mouse move
************************************************************************/
bool CUIControl::OnMouseMove( const SDL_Event & rEvent )
{
    bool result(false);

    if( !IsDisabled() && IsPointInControl( rEvent.motion.x, rEvent.motion.y ) )
    {
        result = true;

        // Only send the message if it's not already active
        if( !IsActive() )
        {
            NGenFunc::DispatchEvent(
                NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
                NUIControl::ECS_ACTIVE,
                (void *)this );
        }
    }

    return result;

}   // OnMouseMove


/************************************************************************
*    desc:  Change the control state
************************************************************************/
void CUIControl::ChangeState( NUIControl::EControlState state )
{
    if( m_state != state )
    {
        m_state = state;

        // Prepare any script functions associated with the state change
        PrepareControlScriptFunction( m_state );

        RecycleContext();
        SetDisplayState();

        m_lastState = m_state;
    }

}   // ChangeState


/************************************************************************
*    desc:  Activate the control
************************************************************************/
bool CUIControl::ActivateControl()
{
    // The focus has switched to this control
    if( !IsDisabled() )
    {
        m_lastState = m_state = NUIControl::ECS_ACTIVE;

        RecycleContext();
        SetDisplayState();

        return true;
    }

    return false;

}   // ActivateControl


/************************************************************************
*    desc:  Deactivate the control
************************************************************************/
void CUIControl::DeactivateControl()
{
    // The focus has switched away from this control
    if( (m_lastState == NUIControl::ECS_NULL) ||
        (m_lastState > NUIControl::ECS_INACTIVE) )
    {
        // Reset the control
        Reset();

        RecycleContext();
        SetDisplayState();

        m_lastState = m_state;
    }

}   // DeactivateControl


/************************************************************************
*    desc:  Disable the control
************************************************************************/
void CUIControl::DisableControl()
{
    if( (m_lastState == NUIControl::ECS_NULL) ||
        (m_lastState > NUIControl::ECS_DISABLED) )
    {
        m_lastState = m_state = NUIControl::ECS_DISABLED;

        RecycleContext();
        SetDisplayState();
    }

}   // DisableControl


/************************************************************************
*    desc:  Enable the control to the inactive state
************************************************************************/
void CUIControl::EnableControl()
{
    if( m_lastState <= NUIControl::ECS_DISABLED )
    {
        m_lastState = m_state = NUIControl::ECS_INACTIVE;

        RecycleContext();
        SetDisplayState();
    }

}   // EnableControl


/************************************************************************
*    desc:  Set the sprite's display based on it's current state
************************************************************************/
void CUIControl::SetDisplayState()
{
    // Set the script function
    PrepareSpriteScriptFunction( m_state );

}   // SetControlState


/************************************************************************
*    desc:  Init the control
************************************************************************/
void CUIControl::Init()
{
    // Create any font strings
    // This allows for delayed VBO create so that the fonts can be allocated during the load screen
    for( auto & iter : m_spriteDeq )
    {
        if( iter.GetVisualComponent().IsFontSprite() )
        {
            CSignalMgr::Instance().Broadcast_LoadSignal();
            
            iter.GetVisualComponent().CreateFontString();
        }
    }
    
    // Call any init scripts
    PrepareSpriteScriptFunction( NUIControl::ECS_INIT );
    
}   // Init


/************************************************************************
*    desc:  Do some cleanup. Currently only for font usage
************************************************************************/
void CUIControl::CleanUp()
{
    // Free the font VBO
    // This allows for early VBO delete so that the menu manager can be freed from the load screen
    for( auto & iter : m_spriteDeq )
    {
        if( iter.GetVisualComponent().IsFontSprite() )
        {
            CSignalMgr::Instance().Broadcast_LoadSignal();
            
            iter.GetVisualComponent().DeleteFontVBO();
        }
    }
    
}   // CleanUp


/************************************************************************
*    desc:  Prepare the sprite script function
************************************************************************/
void CUIControl::PrepareSpriteScriptFunction( NUIControl::EControlState controlState )
{
    std::string scriptFuncMapKey;
    bool forceUpdate(false);
    
    switch( controlState )
    {
        case NUIControl::ECS_NULL:
            throw NExcept::CCriticalException("Control State NULL!",
                boost::str( boost::format("Control state can't be NULL!\n\n%s\nLine: %s")
                    %  __FUNCTION__ % __LINE__ ));
        break;
            
        case NUIControl::ECS_INIT:
            scriptFuncMapKey = "init";
            forceUpdate = true;
        break;
        
        case NUIControl::ECS_DISABLED:
            scriptFuncMapKey = "disabled";
            forceUpdate = true;
        break;
        
        case NUIControl::ECS_INACTIVE:
            scriptFuncMapKey = "inactive";
            forceUpdate = true;
        break;
        
        case NUIControl::ECS_ACTIVE:
            scriptFuncMapKey = "active";
        break;
        
        case NUIControl::ECS_SELECTED:
            scriptFuncMapKey = "selected";
        break;
    };
    
    // Force an update for states that just change settings and don't animate
    CallSpriteScriptFuncKey( scriptFuncMapKey, forceUpdate );

}   // PrepareSpriteScriptFunction


/************************************************************************
*    desc:  Call a script function map key for sprite
************************************************************************/
void CUIControl::CallSpriteScriptFuncKey( const std::string & scriptFuncMapKey, bool forceUpdate )
{    
    for( auto & iter : m_spriteDeq )
        iter.PrepareFuncId( scriptFuncMapKey, forceUpdate );
    
}   // CallCustomSpriteScript


/************************************************************************
*    desc:  Prepare the script function to run
************************************************************************/
void CUIControl::PrepareControlScriptFunction( NUIControl::EControlState controlState )
{
    auto iter = m_scriptFunction.find( controlState );

    if( iter != m_scriptFunction.end() )
        m_scriptComponent.Prepare(m_group, iter->second);

}   // PrepareControlScriptFunction


/************************************************************************
*    desc:  Reset and recycle the contexts
************************************************************************/
void CUIControl::Reset( bool complete )
{
    if( m_state > NUIControl::ECS_INACTIVE )
        m_state = NUIControl::ECS_INACTIVE;

    if( complete )
        m_lastState = m_state;

}   // Reset


/************************************************************************
*    desc:  Recycle the contexts
************************************************************************/
void CUIControl::RecycleContext()
{
    for( auto & iter : m_spriteDeq )
        iter.GetScriptComponent().ResetAndRecycle();

}   // RecycleContext


/************************************************************************
*    desc:  Set the default state of this control
************************************************************************/
void CUIControl::SetDefaultState( const std::string & value )
{
    if( value == "inactive" )
        m_defaultState = NUIControl::ECS_INACTIVE;

    else if( value == "active" )
        m_defaultState = NUIControl::ECS_ACTIVE;

    else if( value == "disabled" )
        m_defaultState = NUIControl::ECS_DISABLED;

    else if( value == "selected" )
        m_defaultState = NUIControl::ECS_SELECTED;

}   // SetDefaultState

void CUIControl::SetDefaultState( NUIControl::EControlState value )
{
    m_defaultState = value;

}   // SetDefaultState


/************************************************************************
*    desc:  Set/Get the smart control pointer. This class owns the pointer
************************************************************************/
void CUIControl::SetSmartGui( CSmartGuiControl * pSmartGuiControl )
{
    m_upSmartGui.reset( pSmartGuiControl );

}   // SetSmartGui

CSmartGuiControl * CUIControl::GetSmartGuiPtr()
{
    return m_upSmartGui.get();

}   // GetActionType


/************************************************************************
*    desc:  Do any smart create
************************************************************************/
void CUIControl::SmartCreate()
{
    if( m_upSmartGui )
        m_upSmartGui->Create();

}   // SmartCreate


/************************************************************************
*    desc:  Do any smart event handling
************************************************************************/
void CUIControl::SmartHandleEvent( const SDL_Event & rEvent )
{
    if( m_upSmartGui )
        m_upSmartGui->HandleEvent( rEvent );

}   // SmartHandleEvent


/************************************************************************
*    desc:  Smart execute the action
************************************************************************/
void CUIControl::SmartExecuteAction()
{
    if( m_upSmartGui )
        m_upSmartGui->Execute();

}   // SmartExecuteAction


/************************************************************************
*    desc:  Set the control to their default behavior
************************************************************************/
void CUIControl::RevertToDefaultState()
{
    m_state = m_defaultState;

}   // RevertToDefaultState


/************************************************************************
*    desc:  Get/Set the state of this control
************************************************************************/
NUIControl::EControlState CUIControl::GetState() const
{
    return m_state;

}   // GetState

void CUIControl::SetState( NUIControl::EControlState state, bool setLastState )
{
    m_state = state;

    if( setLastState )
        m_lastState = state;

}   // SetState


/************************************************************************
*    desc:  Get/Set the control's action type
************************************************************************/
NUIControl::EControlActionType CUIControl::GetActionType()
{
    return m_actionType;

}   // GetActionType

void CUIControl::SetActionType( NUIControl::EControlActionType value )
{
    m_actionType = value;

}   // SetActionType

void CUIControl::SetActionType( const std::string & value )
{
    if( value == "action" )
        m_actionType = NUIControl::ECAT_ACTION;
    
    else if( value == "to_tree" )
        m_actionType = NUIControl::ECAT_TO_TREE;

    else if( value == "to_menu" )
        m_actionType = NUIControl::ECAT_TO_MENU;

    else if( value == "back" )
        m_actionType = NUIControl::ECAT_BACK;

    else if( value == "close" )
        m_actionType = NUIControl::ECAT_CLOSE;

    else if( value == "change_focus" )
        m_actionType = NUIControl::ECAT_CHANGE_FOCUS;

    else if( value == "game_state_change" )
        m_actionType = NUIControl::ECAT_GAME_STATE_CHANGE;

    else if( value == "quit_game" )
        m_actionType = NUIControl::ECAT_QUIT_GAME;

}   // SetActionType


/************************************************************************
*    desc:  Get/Set the execution action
************************************************************************/
const std::string & CUIControl::GetExecutionAction()
{
    return m_executionAction;

}   // GetExecutionAction

void CUIControl::SetExecutionAction( const std::string & action )
{
    m_executionAction = action;

}   // SetExecutionAction


/************************************************************************
*    desc:  Create the font string
************************************************************************/
void CUIControl::CreateFontString( const std::string & fontString, int spriteIndex )
{
    int fontSpriteCounter(0);

    for( auto & iter : m_spriteDeq )
    {
        if( iter.GetVisualComponent().IsFontSprite() )
        {
            if( fontSpriteCounter == spriteIndex )
            {
                iter.GetVisualComponent().CreateFontString( fontString );
                break;
            }

            ++fontSpriteCounter;
        }
    }
    
}   // CreateFontString

void CUIControl::CreateFontString( int stringIndex, int spriteIndex )
{
    if( !m_stringVec.empty() )
        CreateFontString( m_stringVec[stringIndex], spriteIndex );

}   // CreateFontString


/************************************************************************
*    desc:  Set the font string
************************************************************************/
void CUIControl::SetFontString( const std::string & fontString, int spriteIndex )
{
    int fontSpriteCounter(0);

    for( auto & iter : m_spriteDeq )
    {
        if( iter.GetVisualComponent().IsFontSprite() )
        {
            if( fontSpriteCounter == spriteIndex )
            {
                iter.GetVisualComponent().SetFontString( fontString );
                break;
            }

            ++fontSpriteCounter;
        }
    }

}   // SetFontString


/************************************************************************
*    desc:  Get the string vector
************************************************************************/
const std::vector<std::string> & CUIControl::GetStringVec() const
{
    return m_stringVec;
}


/************************************************************************
*    desc:  Handle the select action
*           Only process this message if it's keyboard/gamepad down or mouse up
************************************************************************/
bool CUIControl::HandleSelectAction( const CSelectMsgCracker & msgCracker )
{
    if( (IsSelectable() &&
        msgCracker.IsDeviceMouse() &&
        (msgCracker.GetPressType() == m_mouseSelectType) &&
        IsPointInControl( msgCracker.GetPos() ) ) ||

        (IsActive() && !msgCracker.IsDeviceMouse() && msgCracker.IsPressDown()) )
    {
        NGenFunc::DispatchEvent(
            NMenu::EGE_MENU_CONTROL_STATE_CHANGE,
            NUIControl::ECS_SELECTED,
            (void *)this );

        return true;
    }

    return false;

}   // HandleSelectAction


/************************************************************************
*    desc: Set the first inactive control to be active
*    NOTE: This is mainly here to be virtual for sub controls
************************************************************************/
bool CUIControl::ActivateFirstInactiveControl()
{
    // If a mouse was used, set the control as active but don't animate it.
    // This allows us to use the keys to scroll when pressed
    #if !(defined(__IOS__) || defined(__ANDROID__))
    if( CActionMgr::Instance().WasLastDeviceMouse() )
    {
        if( !IsDisabled() )
        {
            m_lastState = m_state = NUIControl::ECS_ACTIVE;

            return true;
        }

        return false;
    }
    #endif

    return ActivateControl();

}   // ActivateInactiveControl


/************************************************************************
*    desc:  Is the point in the control
************************************************************************/
bool CUIControl::IsPointInControl( int x, int y )
{
    return m_collisionQuad.IsPointInQuad( x, y );

}   // IsPointInControl

bool CUIControl::IsPointInControl( const CPoint<float> & pos )
{
    return m_collisionQuad.IsPointInQuad( pos.x, pos.y );

}   // IsPointInControl


/************************************************************************
*    desc:  Get the pointer to the control if found
*
*    NOTE: These function is mainly for sub controls
************************************************************************/
CUIControl * CUIControl::FindControl( const std::string & name )
{
    if( m_name == name )
        return this;

    return nullptr;

}   // FindControl

CUIControl * CUIControl::FindControl( void * pVoid )
{
    if( pVoid == (void *)this )
        return this;

    return nullptr;

}   // FindControl


/************************************************************************
*    desc:  Set the string to vector
************************************************************************/
void CUIControl::SetStringToList( const std::string & str )
{
    m_stringVec.push_back( str );

}   // SetToFontStringList


/************************************************************************
*    desc:  Is this control disabled/active/selected
************************************************************************/
bool CUIControl::IsDisabled()
{
    return m_state == NUIControl::ECS_DISABLED;

}   // IsDisabled

bool CUIControl::IsInactive()
{
    return m_state == NUIControl::ECS_INACTIVE;

}   // IsInactive

bool CUIControl::IsActive()
{
    return (m_state == NUIControl::ECS_ACTIVE);

}   // IsActive

bool CUIControl::IsSelected()
{
    return (m_state == NUIControl::ECS_SELECTED);

}   // IsSelected

bool CUIControl::IsSelectable()
{
    return ((m_state == NUIControl::ECS_INACTIVE) || (m_state == NUIControl::ECS_ACTIVE));

}   // IsSelected




/************************************************************************
*    desc:  Get the collision position
************************************************************************/
const CPoint<float> & CUIControl::GetCollisionPos() const
{
    return m_collisionCenter;

}	// GetCollisionPos


/************************************************************************
*    desc:  Get the scroll params
************************************************************************/
CScrollParam & CUIControl::GetScrollParam()
{
    return m_scrollParam;

}   // GetScrollParam


/************************************************************************
*    desc:  Check if control is a sub control
************************************************************************/
bool CUIControl::IsSubControl() const
{
    return false;

}   // IsSubControl


/************************************************************************
*    desc:  Connect to the execution action signal
************************************************************************/
void CUIControl::Connect_ExecutionAction( const ExecutionActionSignal::slot_type & slot )
{
    m_executionActionSignal.connect(slot);
    
}   // Connect_ExecutionAction


/************************************************************************
*    desc:  Set the alpha value of this control
************************************************************************/
void CUIControl::SetAlpha( float alpha )
{
    for( auto & iter : m_spriteDeq )
        iter.SetAlpha( alpha );

}   // SetAlpha


/************************************************************************
*    desc:  Get the pointer to the active control
*           This is mostly needed for sub controls
************************************************************************/
CUIControl * CUIControl::GetPtrToActiveControl()
{
    return this;
    
}   // GetPtrToActiveControl


/************************************************************************
*    desc:  Get the mouse select type
************************************************************************/
NDefs::EActionPress CUIControl::GetMouseSelectType() const
{
    return m_mouseSelectType;
    
}   // GetMouseSelectType


/************************************************************************
*    desc:  Get the size of this control
************************************************************************/
const CSize<float> & CUIControl::GetSize() const
{
    return m_size;
    
}   // GetSize


/************************************************************************
*    desc:  Can this control scroll?
************************************************************************/
bool CUIControl::CanScroll( int msg )
{
    if( IsActive() && m_scrollParam.CanScroll(msg) )
        return true;
    
    return false;
}
