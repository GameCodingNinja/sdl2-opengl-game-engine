
/************************************************************************
*    FILE NAME:       classtemplate.cpp
*
*    DESCRIPTION:     Class template
************************************************************************/

// SDL lib dependencies
#include <SDL.h>

#if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
#include "SDL_opengles2.h"
#else
#include <SDL_opengl.h>  // SDL/OpenGL lib dependencies
#endif

// Physical component dependency
#include "startupstate.h"

// Game dependencies
#include "lobbystate.h"

// Game lib dependencies
#include <managers/texturemanager.h>
#include <managers/vertexbuffermanager.h>
#include <managers/shadermanager.h>
#include <managers/fontmanager.h>
#include <managers/soundmanager.h>
#include <managers/actionmanager.h>
#include <managers/spritesheetmanager.h>
#include <objectdata/objectdatamanager.h>
#include <common/color.h>
#include <common/sound.h>
#include <common/build_defs.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptpoint.h>
#include <script/scriptglobals.h>
#include <system/device.h>
#include <gui/menumanager.h>
#include <gui/menu.h>
#include <gui/uicontrol.h>
#include <utilities/genfunc.h>
#include <utilities/highresolutiontimer.h>
#include <utilities/exceptionhandling.h>
#include <utilities/settings.h>
#include <utilities/xmlParser.h>
#include <utilities/xmlpreloader.h>
#include <slot/slotmathmanager.h>
#include <slot/symbolsetviewmanager.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <thread>
#include <chrono>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CStartUpState::CStartUpState() :
    iGameState( NGameDefs::EGS_STARTUP, NGameDefs::EGS_LOBBY )
{
}   // Constructor


/************************************************************************
*    desc:  destructor
************************************************************************/
CStartUpState::~CStartUpState()
{
    CObjectDataMgr::Instance().FreeGroup2D( "(startup)" );
    
}   // destructor


/************************************************************************
*    desc:  Do any pre-load init's
************************************************************************/
void CStartUpState::Init()
{
    // Load the object data list table
    CObjectDataMgr::Instance().LoadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );
    
    // Load the shader
    CShaderMgr::Instance().LoadFromXML( "data/shaders/shader.cfg" );
    
    // Load the start up animation group
    CObjectDataMgr::Instance().LoadGroup2D( "(startup)" );
    
    // Allocate the sprite to fade in
    m_upSpriteLogo.reset( new CSprite2D( CObjectDataMgr::Instance().GetData2D( "(startup)", "logo" ) ) );
    m_upSpriteLogo->Transform();
    
    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().CalcElapsedTime();
 
}   // Init


/************************************************************************
*    desc:  Fade to color                                                             
************************************************************************/
void CStartUpState::Fade(
    CSprite2D & sprite,
    float time,
    const CColor & cur,
    const CColor & finalColor )
{
    CColor inc = (finalColor - cur) / time;
    CColor current = cur;
    
    // Need the projection matrix
    const CMatrix & matrix = CDevice::Instance().GetProjectionMatrix( NDefs::EPT_ORTHOGRAPHIC );

    do
    {
        // Get the elapsed time
        CHighResTimer::Instance().CalcElapsedTime();

        time -= CHighResTimer::Instance().GetElapsedTime();
        current += inc * CHighResTimer::Instance().GetElapsedTime();

        if( time < 0 )
            current = finalColor;

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        CShaderMgr::Instance().SetShaderColor( "shader_2d", "additive", current );
        sprite.Render( matrix );

        SDL_GL_SwapWindow( CDevice::Instance().GetWindow() );

        // Apparently it's a good practice to do this at the end of a render cycle
        CShaderMgr::Instance().Unbind();
        CTextureMgr::Instance().Unbind();
        CVertBufMgr::Instance().Unbind();

        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
    }
    while( time > 0 );

}   // Fade


/***************************************************************************
*    desc:  Load the assets
****************************************************************************/
void CStartUpState::AssetLoad()
{
    // Load in any fonts
    if( NBDefs::IsMobileDevice() )
        CFontMgr::Instance().LoadFromXML( "data/textures/fonts/font_mobile.lst" );
    else
        CFontMgr::Instance().LoadFromXML( "data/textures/fonts/font.lst" );
    
    // Load the symbol set view data manager list table
    CSymbolSetViewMgr::Instance().LoadListTable( "data/objects/2d/slot/symbolSetListTable.lst" );
    
    // Load the math data manager list table
    CSlotMathMgr::Instance().LoadListTable( "data/objects/2d/slot/mathListTable.lst" );

    // Load the action manager - Must be loaded before memu system
    CActionMgr::Instance().LoadActionFromXML( "data/settings/controllerMapping.cfg" );
    
    // Load menu list table
    CMenuManager::Instance().LoadListTable( "data/objects/2d/menu/menuListTable.lst" );
    
    // Load the menu action list
    CMenuManager::Instance().LoadMenuActionFromXML( "data/objects/2d/menu/menu_action.list" );
    
    // Load the script list table
    CScriptManager::Instance().LoadListTable( "data/objects/2d/scripts/scriptListTable.lst" );
    
    // Load sound resources for the menu
    CSoundMgr::Instance().LoadListTable( "data/sound/soundListTable.lst" );
    CSoundMgr::Instance().LoadGroup("(menu)");
    CSoundMgr::Instance().LoadGroup("(standard)");
    
    // Register AngelScript the script items
    auto pScriptEngine = CScriptManager::Instance().GetEnginePtr();
    RegisterStdString( pScriptEngine );
    RegisterScriptArray( pScriptEngine, false );
    NScriptGlobals::Register( pScriptEngine );
    NScriptColor::Register( pScriptEngine );
    NScriptPoint::Register( pScriptEngine );
    NScriptSound::Register( pScriptEngine );
    NScriptPlayLst::Register( pScriptEngine );
    NScriptSoundMgr::Register( pScriptEngine );
    CSprite2D::Register( pScriptEngine );
    CObject::Register( pScriptEngine );
    CMenu::Register( pScriptEngine );
    CShaderMgr::Register( pScriptEngine );
    CScriptManager::Instance().Register();
    
    // Load group specific script items
    CScriptManager::Instance().LoadGroup("(menu)");

    // Load all of the meshes and materials in these groups
    CObjectDataMgr::Instance().LoadGroup2D("(menu)");
    
    // Free the sprite sheet data manager because it's no longer needed
    CSpriteSheetMgr::Instance().Clear();
    
    // Load the menu group
    CMenuManager::Instance().LoadGroup("(menu)");
    
    // Free up any XML's that have been loaded up to this point
    CXMLPreloader::Instance().Clear();
    
    // Do the state specific load
    NLobby::ObjectDataLoad();
    NLobby::CriticalLoad();
    NLobby::Load();
    NLobby::CriticalInit();
    
}   // AssetLoad


/***************************************************************************
*    desc:  Is the state done
****************************************************************************/
bool CStartUpState::DoStateChange()
{
    // Do the fade in
    Fade( *m_upSpriteLogo.get(), 500.f, CColor(0,0,0,1), CColor(1,1,1,1) );
    
    CHighResTimer::Instance().TimerStart();
    
    AssetLoad();
    
    const int time = static_cast<int>(CHighResTimer::Instance().TimerStop());
    
    if( time < 2000.f )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 - time ) );
    
    // Do the fade out
    Fade( *m_upSpriteLogo.get(), 500.f, CColor(1,1,1,1), CColor(0,0,0,1) );

    return true;

}   // DoStateChange

