
/************************************************************************
*    FILE NAME:       titlescreenstate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// Physical component dependency
#include "titlescreenstate.h"

// Game lib dependencies
#include <objectdata/objectdatamanager.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/xmlpreloader.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/uibutton.h>
#include <gui/uimeter.h>
#include <managers/spritestrategymanager.h>
#include <slot/slotmathmanager.h>
#include <slot/symbolsetviewmanager.h>
#include <slot/betmanager.h>
#include <slot/simplecycleresults.h>

// SDL lib dependencies
#include <SDL.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CTitleScreenState::CTitleScreenState() :
    CCommonState( NGameDefs::EGS_TITLE_SCREEN, NGameDefs::EGS_GAME_LOAD ),
        m_background( CObjectDataMgr::Instance().GetData2D( "(title_screen)", "background" ) ),
        //m_slotGame( "(title_screen)" )
        m_cube( CObjectDataMgr::Instance().GetData3D( "(cube)", "cube" ) )
{
}   // Constructor


/************************************************************************
*    desc:  Do any pre-game loop init's
************************************************************************/
void CTitleScreenState::Init()
{
    // Unblock the menu messaging and activate needed trees
    CMenuManager::Instance().Allow();
    CMenuManager::Instance().ActivateTree( "title_screen_tree" );

    m_cube.SetScaleXYZ( 3, 3, 3 );
    
    m_camera.SetPosXYZ( 0, 0, 20 );
    m_camera.SetRotXYZ( 10, 0, 0 );
    
    // Set the line bet and the total numvber of lines bet
    /*CBetMgr::Instance().SetLineBet(1);
    CBetMgr::Instance().SetTotalLines(1);
    
    // Hook the Play button to the reel group
    CUIButton & rPlayBtn = CMenuManager::Instance().GetMenuControl<CUIButton>( "title_screen_menu", "new_game_btn" );
    rPlayBtn.Connect_ExecutionAction( boost::bind(&CSlotGame::PlayGame, &m_slotGame, _1) );
    
    // Create the slot group
    m_slotGame.CreateSlotGroup(
        NSlotDefs::ED_WHEEL,
        "wheel_strip",
        "wheel_paytable",
        CSlotMathMgr::Instance().GetSlotMath( "(title_screen)", "slot_wheel" ),
        CXMLPreloader::Instance().GetNode( std::get<0>(NTitleScreenState::reelGrpCfg) ),
        CXMLPreloader::Instance().GetNode( std::get<0>(NTitleScreenState::spinProfileCfg) ),
        CSymbolSetViewMgr::Instance().Get( "(title_screen)", "wheel_wedges" ),
        std::move(std::unique_ptr<iCycleResults>(new CSimpleCycleresults)) );
    
    // Clear any preloaded XML files
    CXMLPreloader::Instance().Clear();*/
    
    // Prepare the script to fade in the screen
    m_scriptComponent.Prepare( "(menu)", "Screen_FadeIn" );
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
    
}   // Init


/************************************************************************
*    desc:  Handle events
************************************************************************/
void CTitleScreenState::HandleEvent( const SDL_Event & rEvent )
{
    CCommonState::HandleEvent( rEvent );

    // Check for the "change state" message
    if( rEvent.type == NMenu::EGE_MENU_GAME_STATE_CHANGE )
    {
        // Prepare the script to fade in the screen. The script will send the end message
        if( rEvent.user.code == NMenu::ETC_BEGIN ) 
            m_scriptComponent.Prepare( "(menu)", "Screen_FadeOut" );
    }

}   // HandleEvent


/***************************************************************************
*    desc:  Handle any misc processing before the real work is started
****************************************************************************/
void CTitleScreenState::MiscProcess()
{
    //m_slotGame.ProcessGameState();
    
}   // MiscProcess


/***************************************************************************
*    desc:  Update objects that require them
****************************************************************************/
void CTitleScreenState::Update()
{
    CCommonState::Update();
    
    m_scriptComponent.Update();
    
    //m_slotGame.Update();

    float rot = CHighResTimer::Instance().GetElapsedTime() * 0.04;
    m_cube.IncRotXYZ( 0, rot, 0 );

}   // Update


/***************************************************************************
*    desc:  Transform the game objects
****************************************************************************/
void CTitleScreenState::Transform()
{
    CCommonState::Transform();

    m_background.Transform();
    //m_slotGame.Transform();
    m_cube.Transform();
    
    m_camera.Transform();

}   // Transform


/***************************************************************************
*    desc:  2D/3D Render of game content
****************************************************************************/
void CTitleScreenState::PreRender()
{
    const CMatrix & orthoMatrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );
    m_background.Render( orthoMatrix );
    //m_slotGame.Render( orthoMatrix );
    
    CCommonState::PreRender();
    
    const CMatrix & perMatrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_PERSPECTIVE );
    m_cube.Render( m_camera.GetMatrix() * perMatrix, m_camera.GetRotMatrix() );

}   // PreRender


/***************************************************************************
*    desc: Functions for loading/unloading the assets for this state
****************************************************************************/
namespace NTitleScreenState
{
    /***************************************************************************
    *    desc:  Namespace function for loading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataLoad()
    {
        CObjectDataMgr::Instance().LoadGroup2D( "(title_screen)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
        CObjectDataMgr::Instance().LoadGroup3D( "(cube)", CObjectDataMgr::DONT_CREATE_FROM_DATA );
    }
    
    void CriticalLoad()
    {
        // Create the group's VBO, IBO, textures, etc
        CObjectDataMgr::Instance().CreateFromData2D( "(title_screen)" );
        CObjectDataMgr::Instance().CreateFromData3D( "(cube)" );
    }
    
    void Load()
    {
        // Load the slot group stuff
        //CSymbolSetViewMgr::Instance().LoadGroup( "(title_screen)" );
        //CSlotMathMgr::Instance().LoadGroup( "(title_screen)" );
        //CSlotMathMgr::Instance().LoadPaylineSetFromFile( "data/objects/2d/slot/payline_wheel.cfg" );
        
        // Preload some needed XML files
        //CXMLPreloader::Instance().Clear();
        //CXMLPreloader::Instance().Load( std::get<0>(reelGrpCfg), std::get<1>(reelGrpCfg) );
        //CXMLPreloader::Instance().Load( std::get<0>(spinProfileCfg), std::get<1>(spinProfileCfg) );
        
    }   // ThreadLoad
    
    void CriticalInit()
    {

    }


    /***************************************************************************
    *    desc:  Namespace function for unloading the assets for this state
    *           NOTE: Only call when the class is not allocated
    ****************************************************************************/
    void ObjectDataUnload()
    {
        CObjectDataMgr::Instance().FreeGroup2D( "(title_screen)", CObjectDataMgr::DONT_FREE_OPENGL_OBJECTS );
        CObjectDataMgr::Instance().FreeGroup3D( "(cube)", CObjectDataMgr::DONT_FREE_OPENGL_OBJECTS );
    }
    
    void CriticalUnload()
    {
        CObjectDataMgr::Instance().FreeOpenGL2D( "(title_screen)" );
        CObjectDataMgr::Instance().FreeOpenGL3D( "(cube)" );
        
        // Unload the slot group stuff. Could be using fonts
        //CSymbolSetViewMgr::Instance().Clear();
    }
    
    void Unload()
    {
        // Unload the slot group stuff
        //CSlotMathMgr::Instance().Clear();
        
    }   // Unload

}   // NTitleScreenState
