
/************************************************************************
*    FILE NAME:       startupstate.cpp
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
#include <managers/cameramanager.h>
#include <objectdata/objectdatamanager.h>
#include <common/color.h>
#include <common/sound.h>
#include <common/build_defs.h>
#include <script/scriptmanager.h>
#include <script/scriptcolor.h>
#include <script/scriptsound.h>
#include <script/scriptplaylist.h>
#include <script/scriptpoint.h>
#include <script/scriptglobals.h>
#include <script/scriptisprite.h>
#include <script/scriptsoundmanager.h>
#include <script/scriptuicontrol.h>
#include <script/scriptmenu.h>
#include <script/scriptshadermanager.h>
#include <script/scriptsize.h>
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
#include <script/scripthighresolutiontimer.h>

// AngelScript lib dependencies
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

// Boost lib dependencies
#include <boost/format.hpp>

// Standard lib dependencies
#include <thread>
#include <chrono>

/************************************************************************
*    DESC:  Constructor
************************************************************************/
CStartUpState::CStartUpState() :
    iGameState( NGameDefs::EGS_STARTUP, NGameDefs::EGS_LOBBY )
{
}


/************************************************************************
*    DESC:  destructor
************************************************************************/
CStartUpState::~CStartUpState()
{
    CObjectDataMgr::Instance().freeGroup2D( "(startup)" );
}


/************************************************************************
*    DESC:  Do any pre-load init's
************************************************************************/
void CStartUpState::init()
{
    // Load the object data list table
    CObjectDataMgr::Instance().loadListTable( "data/objects/2d/objectDataList/dataListTable.lst" );

    // Load the shader
    CShaderMgr::Instance().load( "data/shaders/shader.cfg" );

    // Load the start up animation group
    CObjectDataMgr::Instance().loadGroup2D( "(startup)" );

    // Allocate the sprite to fade in
    m_upSpriteLogo.reset( new CSprite2D( CObjectDataMgr::Instance().getData2D( "(startup)", "logo" ) ) );
    m_upSpriteLogo->transform();

    // Reset the elapsed time before entering game loop
    CHighResTimer::Instance().calcElapsedTime();
}


/************************************************************************
*    DESC:  Fade to color
************************************************************************/
void CStartUpState::fade(
    CSprite2D & sprite,
    float time,
    const CColor & cur,
    const CColor & finalColor )
{
    CColor inc = (finalColor - cur) / time;
    CColor current = cur;

    do
    {
        // Get the elapsed time
        CHighResTimer::Instance().calcElapsedTime();

        time -= CHighResTimer::Instance().getElapsedTime();
        current += inc * CHighResTimer::Instance().getElapsedTime();

        if( time < 0 )
            current = finalColor;

        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );

        CShaderMgr::Instance().setShaderColor( "shader_2d", "additive", current );
        sprite.render( CCameraMgr::Instance().getDefaultProjMatrix() );

        SDL_GL_SwapWindow( CDevice::Instance().getWindow() );

        // Apparently it's a good practice to do this at the end of a render cycle
        CShaderMgr::Instance().unbind();
        CTextureMgr::Instance().unbind();
        CVertBufMgr::Instance().unbind();

        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
    }
    while( time > 0 );
}


/***************************************************************************
*    DESC:  Load the assets
****************************************************************************/
void CStartUpState::assetLoad()
{
    // Load in any fonts
    if( NBDefs::IsMobileDevice() )
        CFontMgr::Instance().load( "data/textures/fonts/font_mobile.lst" );
    else
        CFontMgr::Instance().load( "data/textures/fonts/font.lst" );

    // Load the symbol set view data manager list table
    CSymbolSetViewMgr::Instance().loadListTable( "data/objects/2d/slot/symbolSetListTable.lst" );

    // Load the math data manager list table
    CSlotMathMgr::Instance().loadListTable( "data/objects/2d/slot/mathListTable.lst" );

    // Load the action manager - Must be loaded before memu system
    CActionMgr::Instance().loadActionFromXML( "data/settings/controllerMapping.cfg" );

    // Load menu list table
    CMenuMgr::Instance().loadListTable( "data/objects/2d/menu/menuListTable.lst" );

    // Load the menu action list
    CMenuMgr::Instance().loadMenuAction( "data/objects/2d/menu/menu_action.list" );

    // Load the script list table
    CScriptMgr::Instance().loadListTable( "data/objects/2d/scripts/scriptListTable.lst" );

    // Load sound resources for the menu
    CSoundMgr::Instance().loadListTable( "data/sound/soundListTable.lst" );
    CSoundMgr::Instance().loadGroup("(menu)");
    CSoundMgr::Instance().loadGroup("(standard)");

    // Register AngelScript the script items
    auto pScriptEngine = CScriptMgr::Instance().getEnginePtr();
    RegisterStdString( pScriptEngine );
    RegisterScriptArray( pScriptEngine, false );
    NScriptGlobals::Register();
    NScriptColor::Register();
    NScriptPoint::Register();
    NScriptSize::Register();
    NScriptiSprite::Register();
    NScriptSound::Register();
    NScriptPlayLst::Register();
    NScriptSoundManager::Register();
    NScriptUIControl::Register();
    NScriptMenu::Register();
    NScriptShaderManager::Register();
    NScriptHighResolutionTimer::Register();

    // Load group specific script items
    CScriptMgr::Instance().loadGroup("(menu)");

    // Load all of the meshes and materials in these groups
    CObjectDataMgr::Instance().loadGroup2D("(menu)");

    // Load the menu group
    CMenuMgr::Instance().loadGroup("(menu)");

    // Free up any XML's that have been loaded up to this point
    CXMLPreloader::Instance().clear();

    // Do the state specific load
    NLobby::ObjectDataLoad();
    NLobby::CriticalLoad();
    NLobby::Load();
    NLobby::CriticalInit();
}


/***************************************************************************
*    DESC:  Is the state done
****************************************************************************/
bool CStartUpState::doStateChange()
{
    // Do the fade in
    fade( *m_upSpriteLogo.get(), 500.f, CColor(0,0,0,1), CColor(1,1,1,1) );

    CHighResTimer::Instance().timerStart();

    assetLoad();

    const int time = static_cast<int>(CHighResTimer::Instance().timerStop());

    if( time < 2000.f )
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 - time ) );

    // Do the fade out
    fade( *m_upSpriteLogo.get(), 500.f, CColor(1,1,1,1), CColor(0,0,0,1) );

    return true;
}
