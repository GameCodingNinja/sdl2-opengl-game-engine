
/************************************************************************
*    FILE NAME:       settings.cpp
*
*    DESCRIPTION:     game settings class
************************************************************************/

// Physical component dependency
#include <utilities/settings.h>

// Game lib dependencies
#include <common/defs.h>
#include <common/worldvalue.h>

// Boost lib dependencies
#include <boost/lexical_cast.hpp>

// SDL lib dependencies
#include <SDL.h>
#include <SDL_mixer.h>

/************************************************************************
*    desc:  Constructor
************************************************************************/
CSettings::CSettings() :
    m_filePath("data/settings/settings.cfg"),
    m_size(1280,768),
    m_default_size(1280,768),
    m_orientation(NDefs::EO_LANDSCAPE),
    m_projectionScale(1),
    m_fullScreen(false),
    m_vSync(false),
    m_major(2),
    m_minor(1),
    m_profile(SDL_GL_CONTEXT_PROFILE_CORE),
    m_viewAngle(45.f),
    m_minZdist(5.f),
    m_maxZdist(1000.f),
    m_gamepadStickDeadZone(2500),
    m_frequency(44100),
    m_sound_channels(MIX_DEFAULT_CHANNELS),
    m_mix_channels(MIX_CHANNELS),
    m_chunksize(1024),
    m_createStencilBuffer(false),
    m_stencilBufferBitSize(0),
    m_clearStencilBuffer(false),
    m_enableDepthBuffer(false),
    m_clearTargetBuffer(true),
    m_minThreadCount(2),
    m_maxThreadCount(0),
    m_sectorSize(512),
    m_sectorSizeHalf(256),
    m_anisotropicLevel(NDefs::ETF_ANISOTROPIC_0X),
    m_projectionType(NDefs::EPT_PERSPECTIVE),
    m_debugStrVisible(false)
{
    CWorldValue::setSectorSize( 512 );
}


/************************************************************************
*    desc:  destructor
************************************************************************/
CSettings::~CSettings()
{
}


/************************************************************************
*    desc:  Init the file path for loading the settings file
************************************************************************/
void CSettings::initFilePath( const std::string & filePath )
{
    m_filePath = filePath;
}


/************************************************************************
*    desc:  Load settings data from xml file
*
*    param: string & _filePath - path to file
*
*    ret: string - class name
************************************************************************/
void CSettings::loadXML()
{
    // Open and parse the XML file:
    m_mainNode = XMLNode::openFileHelper( m_filePath.c_str(), "settings" );

    if( !m_mainNode.isEmpty() )
    {
        const XMLNode displayListNode = m_mainNode.getChildNode("display");
        if( !displayListNode.isEmpty() )
        {
            #if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)

            SDL_DisplayMode dm;
            SDL_GetDesktopDisplayMode(0, &dm);

            m_size.w = dm.w;
            m_size.h = dm.h;

            #else

            // Get the attributes from the "resolution" node
            const XMLNode resolutionNode = displayListNode.getChildNode("resolution");
            if( !resolutionNode.isEmpty() )
            {
                m_size.w = std::atoi(resolutionNode.getAttribute("width"));
                m_size.h = std::atoi(resolutionNode.getAttribute("height"));

                m_fullScreen = ( std::strcmp( resolutionNode.getAttribute("fullscreen"), "true" ) == 0 );
            }

            #endif

            // Get the attributes from the "defaultHeight" node
            const XMLNode defResNode = displayListNode.getChildNode("default");

            if( defResNode.isAttributeSet("orientation") )
            {
                if( std::strcmp( defResNode.getAttribute("orientation"), "portrait" ) == 0 )
                {
                    m_orientation = NDefs::EO_PORTRAIT;

                    #if !defined(__ANDROID__)
                    m_size.swap();
                    #endif
                }
            }

            if( m_orientation == NDefs::EO_PORTRAIT )
            {
                m_native_size.h = m_default_size.h = std::atof(defResNode.getAttribute("width"));

                if( defResNode.isAttributeSet("height") )
                    m_native_size.w = std::atof(defResNode.getAttribute("height"));
            }
            else
            {
                m_native_size.h = m_default_size.h = std::atof(defResNode.getAttribute("height"));

                if( defResNode.isAttributeSet("width") )
                    m_native_size.w = std::atof(defResNode.getAttribute("width"));
            }
        }

        calcRatio();

        const XMLNode deviceNode = m_mainNode.getChildNode("device");
        if( !deviceNode.isEmpty() )
        {
            // Get the attribute for OpenGL node
            const XMLNode OpenGLNode = deviceNode.getChildNode("OpenGL");
            if( !OpenGLNode.isEmpty() )
            {
                m_major = std::atoi( OpenGLNode.getAttribute("major") );
                m_minor = std::atoi( OpenGLNode.getAttribute("minor") );

                const char * pAttr = OpenGLNode.getAttribute("profile");

                if( std::strcmp( pAttr, "core" ) == 0 )
                    m_profile = SDL_GL_CONTEXT_PROFILE_CORE;

                else if( std::strcmp( pAttr, "es" ) == 0 )
                    m_profile = SDL_GL_CONTEXT_PROFILE_ES;

                else if( std::strcmp( pAttr, "compatibility" ) == 0 )
                    m_profile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;

                #if defined(__IOS__) || defined(__ANDROID__) || defined(__arm__)
                m_profile = SDL_GL_CONTEXT_PROFILE_ES;
                #endif
            }

            // Get the projection info
            const XMLNode projNode = deviceNode.getChildNode("projection");
            if( !projNode.isEmpty() )
            {
                if( projNode.isAttributeSet("minZDist") )
                    m_minZdist = std::atof(projNode.getAttribute("minZDist"));

                if( projNode.isAttributeSet("maxZDist") )
                    m_maxZdist = std::atof(projNode.getAttribute("maxZDist"));

                if( projNode.isAttributeSet("view_angle") )
                    m_viewAngle = std::atof(projNode.getAttribute("view_angle"));

                if( projNode.isAttributeSet("projectType") &&
                    std::strcmp( projNode.getAttribute("projectType"), "orthographic" ) == 0 )
                    m_projectionType = NDefs::EPT_ORTHOGRAPHIC;

                if( projNode.isAttributeSet("scale") )
                    m_projectionScale = std::atof(projNode.getAttribute("scale"));
            }

            // Convert to radians
            m_viewAngle *= (float)defs_DEG_TO_RAD;

            // Get the attribute from the "textureFiltering" node
            const XMLNode textFilterNode = deviceNode.getChildNode("anisotropicFiltering");
            if( !textFilterNode.isEmpty() )
            {
                const char * pAttr = textFilterNode.getAttribute("level");

                if( std::strcmp( pAttr, "anisotropic_0X" ) == 0 )
                    m_anisotropicLevel = NDefs::ETF_ANISOTROPIC_0X;

                else if( std::strcmp( pAttr, "anisotropic_2X" ) == 0 )
                    m_anisotropicLevel = NDefs::ETF_ANISOTROPIC_2X;

                else if( std::strcmp( pAttr, "anisotropic_4X" ) == 0 )
                    m_anisotropicLevel = NDefs::ETF_ANISOTROPIC_4X;

                else if( std::strcmp( pAttr, "anisotropic_8X" ) == 0 )
                    m_anisotropicLevel = NDefs::ETF_ANISOTROPIC_8X;

                else if( std::strcmp( pAttr, "anisotropic_16X" ) == 0 )
                    m_anisotropicLevel = NDefs::ETF_ANISOTROPIC_16X;
            }

            // Get the attribute from the "backbuffer" node
            const XMLNode backBufferNode = deviceNode.getChildNode("backbuffer");
            if( !backBufferNode.isEmpty() )
            {
                //tripleBuffering = (backBufferNode.getAttribute("tripleBuffering") == string("true"));
                m_vSync = ( std::strcmp( backBufferNode.getAttribute("VSync"), "true" ) == 0 );
            }

            const XMLNode joypadNode = deviceNode.getChildNode("joypad");
            if( !joypadNode.isEmpty() )
            {
                m_gamepadStickDeadZone = std::atoi(joypadNode.getAttribute("stickDeadZone"));
            }

            const XMLNode threadNode = deviceNode.getChildNode("threads");
            if( !threadNode.isEmpty() )
            {
                if( threadNode.isAttributeSet("minThreadCount") )
                    m_minThreadCount = std::atoi(threadNode.getAttribute("minThreadCount"));

                if( threadNode.isAttributeSet("maxThreadCount") )
                    m_maxThreadCount = std::atoi(threadNode.getAttribute("maxThreadCount"));
            }

            // Get the attribute from the "depthStencilBuffer" node
            const XMLNode depthStencilBufferNode = deviceNode.getChildNode("depthStencilBuffer");
            if( !depthStencilBufferNode.isEmpty() )
            {
                // Do we enable the depth buffer
                if( depthStencilBufferNode.isAttributeSet("enableDepthBuffer") )
                    m_enableDepthBuffer = ( std::strcmp( depthStencilBufferNode.getAttribute("enableDepthBuffer"), "true" ) == 0 );

                // Do we create the stencil buffer
                if( depthStencilBufferNode.isAttributeSet("createStencilBuffer") )
                    m_createStencilBuffer = ( std::strcmp( depthStencilBufferNode.getAttribute("createStencilBuffer"), "true" ) == 0 );

                // Do we clear the stencil buffer
                if( depthStencilBufferNode.isAttributeSet("clearStencilBuffer") )
                    m_clearStencilBuffer = ( std::strcmp( depthStencilBufferNode.getAttribute("clearStencilBuffer"), "true") == 0 );

                // Get the number of bits for the stencil buffer
                if( depthStencilBufferNode.isAttributeSet( "stencilBufferBitSize" ) )
                    m_stencilBufferBitSize = std::atoi( depthStencilBufferNode.getAttribute("stencilBufferBitSize") );
            }

            // Get the sound settings
            const XMLNode soundNode = m_mainNode.getChildNode("sound");
            if( !soundNode.isEmpty() )
            {
                if( soundNode.isAttributeSet("frequency") )
                    m_frequency = std::atoi(soundNode.getAttribute("frequency"));

                if( soundNode.isAttributeSet("sound_channels") )
                    m_sound_channels = std::atoi(soundNode.getAttribute("sound_channels"));

                if( soundNode.isAttributeSet("mix_channels") )
                    m_mix_channels = std::atoi(soundNode.getAttribute("mix_channels"));

                if( soundNode.isAttributeSet("chunksize") )
                    m_chunksize = std::atoi(soundNode.getAttribute("chunksize"));
            }

            // Get world settings
            const XMLNode worldNode = m_mainNode.getChildNode("world");
            if( !worldNode.isEmpty() )
            {
                if( worldNode.isAttributeSet("sectorSize") )
                {
                    m_sectorSize = std::atof(worldNode.getAttribute("sectorSize"));
                    m_sectorSizeHalf = m_sectorSize / 2;
                    CWorldValue::setSectorSize( m_sectorSize );
                }
            }
        }
    }
}


/************************************************************************
*    desc:  Calculate the ratios
************************************************************************/
void CSettings::calcRatio()
{
    // Height and width screen ratio for perspective projection
    m_screenAspectRatio.w = m_size.w / m_size.h;
    m_screenAspectRatio.h = m_size.h / m_size.w;

    // NOTE: The default width is based on the current aspect ratio
    // NOTE: Make sure the width does not have a floating point component
    m_default_size.w = (float)(int)((m_screenAspectRatio.w * m_default_size.h) + 0.5);

    // Get half the size for use with screen boundaries
    m_default_size_half = m_default_size / 2.f;

    // Screen size divided by two
    m_size_half = m_size / 2.f;

    // Pre-calculate the aspect ratios for orthographic projection
    m_orthoAspectRatio.h = m_size.h / m_default_size.h;
    m_orthoAspectRatio.w = m_size.w / m_default_size.w;
}


/************************************************************************
*    desc:  Get/Set game window size
************************************************************************/
const CSize<float> & CSettings::getResolution() const
{
    return m_size;
}

const CSize<float> & CSettings::getSize() const
{
    return m_size;
}

void CSettings::setSize( const CSize<float> & size )
{
    m_size = size;
}


/************************************************************************
*    desc:  Get game window size / 2
*
*    ret: CSize - size of game window / 2
************************************************************************/
const CSize<float> & CSettings::getSizeHalf() const
{
    return m_size_half;
}


/************************************************************************
*    desc:  Get native size specified in config file
************************************************************************/
const CSize<float> & CSettings::getNativeSize() const
{
    return m_native_size;
}


/************************************************************************
*    desc:  Get native size specified in config file and calc width
************************************************************************/
const CSize<float> & CSettings::getDefaultSize() const
{
    return m_default_size;
}


/************************************************************************
*    desc:  Get default size in half
************************************************************************/
const CSize<float> & CSettings::getDefaultSizeHalf() const
{
    return m_default_size_half;
}


/************************************************************************
*    desc:  Height and width screen ratio for orthographic objects
*           The difference between screen and the default size
************************************************************************/
const CSize<float> & CSettings::getOrthoAspectRatio() const
{
    return m_orthoAspectRatio;
}


/************************************************************************
*    desc: Height and width screen ratio for perspective projection
************************************************************************/
const CSize<float> & CSettings::getScreenAspectRatio() const
{
    return m_screenAspectRatio;
}


/************************************************************************
*    desc:  Get/Set vSync?
************************************************************************/
bool CSettings::getVSync() const
{
    return m_vSync;
}

void CSettings::setVSync( bool value )
{
    m_vSync = value;
}


/************************************************************************
*    desc:  Get the OpenGL major version
************************************************************************/
int CSettings::getMajorVersion() const
{
    return m_major;
}


/************************************************************************
*    desc:  Get the OpenGL minor version
************************************************************************/
int CSettings::getMinorVersion() const
{
    return m_minor;
}


/************************************************************************
*    desc:  Get the OpenGL profile type
************************************************************************/
int CSettings::getProfile() const
{
    return m_profile;
}


/************************************************************************
*    desc:  Get the view angle
************************************************************************/
float CSettings::getViewAngle() const
{
    return m_viewAngle;
}


/************************************************************************
*    desc:  Get the minimum z distance
************************************************************************/
float CSettings::getMinZdist() const
{
    return m_minZdist;
}


/************************************************************************
*    desc:  Get the maximum z distance
************************************************************************/
float CSettings::getMaxZdist() const
{
    return m_maxZdist;
}


/************************************************************************
*    desc:  Get the gamepad stick dead zone
************************************************************************/
int CSettings::getGamePadStickDeadZone() const
{
    return m_gamepadStickDeadZone;
}


/************************************************************************
*    desc:  Set the gamepad stick dead zone
************************************************************************/
void CSettings::setGamePadStickDeadZone( int value )
{
    m_gamepadStickDeadZone = value;
}


/************************************************************************
*    desc:  Get/Set full screen
************************************************************************/
bool CSettings::getFullScreen() const
{
    return m_fullScreen;
}

void CSettings::setFullScreen( bool value )
{
    m_fullScreen = value;
}


/************************************************************************
*    desc:  Do we create the depth stencil buffer
************************************************************************/
bool CSettings::getCreateStencilBuffer() const
{
    return m_createStencilBuffer;
}


/************************************************************************
*    desc:  Get the bit size of the stencil buffer
************************************************************************/
int CSettings::getStencilBufferBitSize() const
{
    return m_stencilBufferBitSize;
}


/************************************************************************
*    desc:  Do we clear the stencil buffer
************************************************************************/
bool CSettings::getClearStencilBuffer() const
{
    return m_clearStencilBuffer;
}


/************************************************************************
*    desc:  Is the depth buffer enabled by default
************************************************************************/
bool CSettings::getEnableDepthBuffer() const
{
    return m_enableDepthBuffer;
}


/************************************************************************
*    desc:  Do we clear the target buffer
************************************************************************/
bool CSettings::getClearTargetBuffer() const
{
    return m_clearTargetBuffer;
}


/************************************************************************
*    desc:  Get the sound frequency
************************************************************************/
int CSettings::getFrequency() const
{
    return m_frequency;
}


/************************************************************************
*    desc:  Get the sound channels: mono, stero, quad, suround, etc
************************************************************************/
int CSettings::getSoundChannels() const
{
    return m_sound_channels;
}


/************************************************************************
*    desc:  Get the number of channels used for mixing
************************************************************************/
int CSettings::getMixChannels() const
{
    return m_mix_channels;
}


/************************************************************************
*    desc:  Get the chunk size. The amount of memory used for mixing.
*           The higher the number, the more latency in sound responce
*           stopping and starting
************************************************************************/
int CSettings::getChunkSize() const
{
    return m_chunksize;
}


/************************************************************************
*    desc:  Get the minimum thread count
************************************************************************/
int CSettings::getMinThreadCount() const
{
    return m_minThreadCount;
}


/************************************************************************
*    desc:  Get the maximum thread count
*           Value of zero means use max hardware threads to no of cores
************************************************************************/
int CSettings::getMaxThreadCount() const
{
    return m_maxThreadCount;
}


/************************************************************************
*    desc:  Get/Set the Anisotropic setting
************************************************************************/
int CSettings::getAnisotropicLevel() const
{
    return m_anisotropicLevel;
}

void CSettings::setAnisotropicLevel( int level )
{
    m_anisotropicLevel = (NDefs::ETextFilter)level;
}


/************************************************************************
*    desc:  Get the sector size
************************************************************************/
int CSettings::getSectorSize() const
{
    return m_sectorSize;
}

/************************************************************************
*    desc:  Get half of the sector size
************************************************************************/
int CSettings::getSectorSizeHalf() const
{
    return m_sectorSizeHalf;
}


/************************************************************************
*    desc:  Get the projection type
************************************************************************/
NDefs::EProjectionType CSettings::getProjectionType() const
{
    return m_projectionType;
}


/************************************************************************
*    desc:  Get the projection scale
************************************************************************/
float CSettings::getProjectionScale() const
{
    return m_projectionScale;
}


/************************************************************************
*    desc:  Set/Get debug string visible
************************************************************************/
void CSettings::setDebugStrVisible( bool value )
{
    m_debugStrVisible = value;
}

bool CSettings::getDebugStrVisible() const
{
    return m_debugStrVisible;
}


/************************************************************************
*    desc:  Save the settings file
************************************************************************/
void CSettings::saveSettings()
{
    if( !m_mainNode.isEmpty() )
    {
        XMLNode displayListNode = m_mainNode.getChildNode("display");

        if( !displayListNode.isEmpty() )
        {
            // Get the attributes from the "resolution" node
            XMLNode resolutionNode = displayListNode.getChildNode("resolution");

            {
                std::string tmpStr = boost::lexical_cast<std::string>( m_size.w );
                resolutionNode.updateAttribute(tmpStr.c_str(), "width", "width");

                tmpStr = boost::lexical_cast<std::string>( m_size.h );
                resolutionNode.updateAttribute(tmpStr.c_str(), "height", "height");
            }

            {
                std::string tmpStr = "false";

                if( m_fullScreen )
                    tmpStr = "true";

                resolutionNode.updateAttribute(tmpStr.c_str(), "fullscreen", "fullscreen");
            }
        }

        XMLNode deviceNode = m_mainNode.getChildNode("device");

        if( !deviceNode.isEmpty() )
        {
            {
                // Get the attribute from the "backbuffer" node
                XMLNode backBufferNode = deviceNode.getChildNode("backbuffer");

                std::string tmpStr = "false";

                if( m_vSync )
                    tmpStr = "true";

                backBufferNode.updateAttribute(tmpStr.c_str(), "VSync", "VSync");
            }
            /*{
                // Get the attribute from the "backbuffer" node
                XMLNode textFilterNode = deviceNode.getChildNode("textureFiltering");

                std::string tmpStr = GetTextFilterString();

                textFilterNode.updateAttribute(tmpStr.c_str(), "filter", "filter");
            }*/
        }

        {
            // Get the attribute from the "joypad" node
            XMLNode joypadNode = deviceNode.getChildNode("joypad");

            std::string tmpStr = boost::lexical_cast<std::string>( m_gamepadStickDeadZone );

            joypadNode.updateAttribute(tmpStr.c_str(), "stickDeadZone", "stickDeadZone");
        }

        // Save the settings file
        m_mainNode.writeToFile(m_filePath.c_str(), "utf-8");
    }
}
