
/************************************************************************
*    FILE NAME:       uicontrol.h
*
*    DESCRIPTION:     class for user interface controls
************************************************************************/

#ifndef __ui_control_h__
#define __ui_control_h__

// Physical component dependency
#include <gui/controlbase.h>

// Game lib dependencies
#include <gui/menudefs.h>
#include <gui/scrollparam.h>
#include "gui/messagecracker.h"
#include <common/size.h>
#include <common/quad.h>
#include <common/rect.h>
#include <script/scriptcomponent.h>
#include <2d/sprite2d.h>

// Boost lib dependencies
#include <boost/signals2.hpp>

// SDL/OpenGL lib dependencies
#include <SDL.h>

// Standard lib dependencies
#include <memory>
#include <map>
#include <deque>

// Forward declaration(s)
class CSmartGuiControl;
class CObject2D;

class CUIControl : public CControlBase
{
public:
    
    typedef boost::signals2::signal<void (CUIControl *)> ExecutionActionSignal;

    // Constructor
    CUIControl( const std::string & group );

    // Destructor
    virtual ~CUIControl();

    // Load the initial info from XML node
    virtual void LoadFromNode( const XMLNode & node ) override;
    
    // Init control
    virtual void Init();
    
    // Do some cleanup. Currently only for font usage
    virtual void CleanUp();

    // Update the control
    virtual void Update();

    // Transform the control
    virtual void Transform( const CObject2D & object ) override;

    // do the render
    virtual void Render( const CMatrix & matrix );

    // Handle events
    virtual void HandleEvent( const SDL_Event & rEvent );

    // Set the control to their default behavior
    void RevertToDefaultState();

    // Get/Set the state of this control
    NUIControl::EControlState GetState() const;
    void SetState( NUIControl::EControlState state, bool setLastState = false );

    // Get the type of action
    NUIControl::EControlActionType GetActionType();

    // Get the execution action
    const std::string & GetExecutionAction();

    // Reset the control to inactive if its not disabled
    virtual void Reset( bool complete = false );

    // Set/Get the Smart GUI pointer
    void SetSmartGui( CSmartGuiControl * pSmartGuiControl );
    CSmartGuiControl * GetSmartGuiPtr();

    // Do any smart create
    void SmartCreate();

    // Set the type of action
    void SetActionType( NUIControl::EControlActionType value );

    // Set the execution action
    void SetExecutionAction( const std::string & action );

    // Create the font string
    void CreateFontString( const std::string & fontString, int spriteIndex = 0 );
    void CreateFontString( int stringIndex = 0, int spriteIndex = 0 );
    
    // Set the font string
    void SetFontString( const std::string & fontString, int spriteIndex = 0 );
    
    // Handle the mouse move
    virtual bool OnMouseMove( const SDL_Event & rEvent );

    // Handle the select action
    virtual bool HandleSelectAction( const CSelectMsgCracker & msgCracker );

    // Set the first inactive control to be active
    virtual bool ActivateFirstInactiveControl();

    // Get the pointer to the control if found
    virtual CUIControl * FindControl( const std::string & name );
    virtual CUIControl * FindControl( void * pVoid );

    // Set the string to vector
    void SetStringToList( const std::string & str );

    // Change the control state
    void ChangeState( NUIControl::EControlState state );

    // Is this control active/selected
    bool IsDisabled();
    bool IsInactive();
    bool IsActive();
    bool IsSelected();
    bool IsSelectable();

    // Get the collision position
    const CPoint<float> & GetCollisionPos() const;

    // Activate the control
    bool ActivateControl();

    // Deactivate the control
    virtual void DeactivateControl();
    
    // Disable the control
    virtual void DisableControl();
    
    // Enable the control to the inactive state
    virtual void EnableControl();

    // Get the scroll params
    CScrollParam & GetScrollParam();
    
    // Get the number of strings
    const std::vector<std::string> & GetStringVec() const;
    
    // Check if control is a sub control
    virtual bool IsSubControl() const;
    
    // Connect to the execution action signal
    void Connect_ExecutionAction( const ExecutionActionSignal::slot_type & slot );
    
    // Call a script function map key for sprite
    void CallSpriteScriptFuncKey( const std::string & scriptFuncMapKey, bool forceUpdate = false );
    
    // Set the alpha value of this menu
    virtual void SetAlpha( float alpha );
    
    // Get the pointer to the active control
    virtual CUIControl * GetPtrToActiveControl();
    
    // Get the mouse select type
    NDefs::EActionPress GetMouseSelectType() const;
    
    // Get the size of this control
    const CSize<float> & GetSize() const;
    
    // an this control scroll?
    bool CanScroll( int msg );

protected:

    // Set the type of action
    void SetActionType( const std::string & value );

    // Set the sprite's display based on it's current state
    void SetDisplayState();

    // Prepare the sprite script function
    void PrepareSpriteScriptFunction( NUIControl::EControlState controlState );

    // Prepare the control script function to run
    void PrepareControlScriptFunction( NUIControl::EControlState controlState );

    // Recycle the contexts
    void RecycleContext();

    // Smart execute the action
    void SmartExecuteAction();
    
    // Do any smart event handling
    void SmartHandleEvent( const SDL_Event & rEvent );

    // Handle message
    virtual void OnStateChange( const SDL_Event & rEvent );
    virtual void OnSelectExecute( const SDL_Event & rEvent );
    void OnSetActiveControl( const SDL_Event & rEvent );
    void OnReactivate( const SDL_Event & rEvent );
    void OnTransIn( const SDL_Event & rEvent );
    void OnTransOut( const SDL_Event & rEvent );
    
    // Load the control specific info from XML node
    virtual void LoadControlFromNode( const XMLNode & node ) override;
    
    // Is the point in the control
    bool IsPointInControl( int x, int y );
    bool IsPointInControl( const CPoint<float> & pos );

private:

    // Load a sprite from an XML node
    virtual void LoadSpriteFromNode( const XMLNode & node, size_t & fontSpriteCount ) override;

    // Set the default state of this control
    void SetDefaultState( NUIControl::EControlState value );
    void SetDefaultState( const std::string & value );
    
    // Transform the collision
    void TransformCollision();
    
protected:
    
    // sprite vector
    std::deque<CSprite2D> m_spriteDeq;

private:

    // Script component object
    CScriptComponent m_scriptComponent;

    // control's default state
    NUIControl::EControlState m_defaultState;

    // control's current state
    NUIControl::EControlState m_state;
    NUIControl::EControlState m_lastState;

    // Name of the action to perform under the correct circumstances
    std::string m_executionAction;

    // How the control should respond when selected
    NUIControl::EControlActionType m_actionType;

    // This control's size
    CSize<float> m_size;

    // This is the size modifier
    // when calculating the collision rect
    CRect<float> m_sizeModifier;

    // Collision rect
    CQuad m_collisionQuad;

    // Collision center
    CPoint<float> m_collisionCenter;

    // Base smart Gui control scoped pointer
    std::unique_ptr<CSmartGuiControl> m_upSmartGui;

    // Mouse selection type
    NDefs::EActionPress m_mouseSelectType;

    // On state script function names
    std::map< NUIControl::EControlState, std::string > m_scriptFunction;

    // Scrolling parameters
    CScrollParam m_scrollParam;
    
    // Boost signals
    ExecutionActionSignal m_executionActionSignal;
};

#endif  // __ui_control_h__


