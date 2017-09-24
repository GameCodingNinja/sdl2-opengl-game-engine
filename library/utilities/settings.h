
/************************************************************************
*    FILE NAME:       settings.h
*
*    DESCRIPTION:     game settings class
************************************************************************/

#ifndef __settings_h__
#define __settings_h__

// Game lib dependencies
#include <utilities/xmlParser.h>
#include <common/size.h>
#include <common/defs.h>

// Standard lib dependencies
#include <string>

class CSettings
{
public:

    // Get the instance of the singleton class
    static CSettings & Instance()
    {
        static CSettings settings;
        return settings;
    }

    // Save the settings file
    void SaveSettings();
    
    // Init the file path for loading the settings file
    void InitFilePath( const std::string & filePath );

    // Load settings data from xml file
    void LoadXML();

    // Get game window size
    const CSize<float> & GetResolution() const;
    const CSize<float> & GetSize() const;
    const CSize<float> & GetSizeHalf() const;
    const CSize<float> & GetNativeSize() const;
    const CSize<float> & GetDefaultSize() const;
    const CSize<float> & GetDefaultSizeHalf() const;
    void SetSize( const CSize<float> & size );

    // Do we want vSync?
    bool GetVSync() const;
    void SetVSync( bool value );

    // Get the OpenGL major version
    int GetMajorVersion() const;

    // Get the OpenGL minor version
    int GetMinorVersion() const;

    // Get the OpenGL profile type
    int GetProfile() const;

    // Get the view angle
    float GetViewAngle() const;

    // Get the minimum z distance
    float GetMinZdist() const;

    // Get the maximum z distance
    float GetMaxZdist() const;
    
    // Height and width screen ratio for orthographic objects.
    // The difference between screen and the default size
    const CSize<float> & GetOrthoAspectRatio() const;

    // Height and width screen ratio for perspective projection
    const CSize<float> & GetScreenAspectRatio() const;

    // Get the gamepad stick dead zone
    int GetGamePadStickDeadZone() const;

    // Set the gamepad stick dead zone
    void SetGamePadStickDeadZone( int value );

    // Get/Set full screen
    bool GetFullScreen() const;
    void SetFullScreen( bool value );

    // Calculate the ratio
    void CalcRatio();

    // Do we create the depth stencil buffer
    bool GetCreateStencilBuffer() const;

    // Get the bit size of the stencil buffer
    int GetStencilBufferBitSize() const;

    // Do we clear the stencil buffer
    bool GetClearStencilBuffer() const;

    // Is the depth buffer enabled by default
    bool GetEnableDepthBuffer() const;

    // Do we clear the target buffer
    bool GetClearTargetBuffer() const;
    
    // Get the sound frequency
    int GetFrequency() const;
    
    // Get the sound channels: mono, stero, quad, suround, etc
    int GetSoundChannels() const;
    
    // Get the number of channels used for mixing
    int GetMixChannels() const;
    
    // Get the chunk size.
    int GetChunkSize() const;
    
    // Get the minimum thread count
    int GetMinThreadCount() const;
    
    // Get the maximum thread count
    int GetMaxThreadCount() const;
    
    // Get the sector size
    int GetSectorSize() const;
    
    // Get half of the sector size
    int GetSectorSizeHalf() const;
    
    // Get the Anisotropic setting
    int GetAnisotropicLevel() const;
    
    // Get the projection type
    NDefs::EProjectionType GetProjectionType() const;
    
    // Set/Get debug string visible
    void SetDebugStrVisible( bool value );
    bool GetDebugStrVisible() const;

private:

    // Constructor
    CSettings();

    // Destructor
    virtual ~CSettings();

private:

    // xml node
    XMLNode m_mainNode;

    // file path string
    std::string m_filePath;

    // with and height of game window
    CSize<float> m_size;
    CSize<float> m_size_half;
    CSize<float> m_native_size;
    CSize<float> m_default_size;
    CSize<float> m_default_size_half;
    CSize<float> m_portrait_size;
    CSize<float> m_portrait_size_half;
    CSize<float> m_portrait_native_size;
    CSize<float> m_portrait_default_size;
    CSize<float> m_portrait_default_size_half;
    
    // Orientation of game window
    NDefs::EOrentation m_orientation;
    
    // Height and width screen ratio for perspective projection
    CSize<float> m_screenAspectRatio;

    // Precalculated aspect ratios for orthographic projection
    CSize<float> m_orthoAspectRatio;

    // Full screen flag loaded from file
    bool m_fullScreen;

    // VSync flag
    bool m_vSync;

    // OpenGL versions
    int m_major;
    int m_minor;
    int m_profile;

    // view angle
    float m_viewAngle;

    // minimum Z distance
    float m_minZdist;

    // maximum Z distance
    float m_maxZdist;

    // Gamepad dead zone
    int m_gamepadStickDeadZone;
    
    // Sound members
    int m_frequency;
    int m_sound_channels;
    int m_mix_channels;
    int m_chunksize;

    // Do we create the depth stencil buffer
    bool m_createStencilBuffer;

    // Number of bits in the stencil buffer
    int m_stencilBufferBitSize;

    // Do we clear the stencil buffer
    bool m_clearStencilBuffer;

    // Do we enable the depth buffer
    bool m_enableDepthBuffer;

    // Do we clear the target buffer
    bool m_clearTargetBuffer;
    
    // Minimum thread count
    int m_minThreadCount;
    
    // Max thread count
    // Value of zero means use max hardware threads to cores
    int m_maxThreadCount;
    
    // the sector size
    float m_sectorSize;
    float m_sectorSizeHalf;
    
    // Anisotropic filtering level
    NDefs::ETextFilter m_anisotropicLevel;
    
    // The projection type
    NDefs::EProjectionType m_projectionType;
    
    // Debug string members
    std::string m_debugStrGroup;
    std::string m_debugStrObject;
    std::string m_debugStrFont;
    bool m_debugStrVisible;
};

#endif  // __settings_h__
