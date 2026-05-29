
# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
    game/game.cpp
    game/main.cpp
    ai/ballai.cpp
    smartGUI/smartconfirmbtn.cpp
    smartGUI/smartresolutionbtn.cpp
    smartGUI/smartsettingsmenubtn.cpp
    smartGUI/smartapplysettingsbtn.cpp
    smartGUI/smartfullscreencheckbox.cpp
    smartGUI/smartvsynccheckbox.cpp
    smartGUI/smartdeadzoneslider.cpp
    smartGUI/smartkeybindbtn.cpp
    smartGUI/smartresetkeybind.cpp
    state/commonstate.cpp
    state/startupstate.cpp
    state/titlescreenstate.cpp
    state/loadstate.cpp
    state/runstate.cpp )

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    # List all the libraries to link against
    target_link_libraries(
        ${PROJECT_NAME}
        library
        angelscript
        Box2D
        bulletPhysics
        /usr/lib/arm-linux-gnueabihf/libGLESv2.so
        /usr/lib/arm-linux-gnueabihf/libSDL2.so
        /usr/lib/arm-linux-gnueabihf/libSDL2_mixer.so )
else()
    # List all the libraries to link against
    # These need to be listed in order of dependency
    target_link_libraries(
        ${PROJECT_NAME}
        library
        angelscript
        Box2D
        bulletPhysics
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2MIXER_LIBRARY}
        /usr/lib/libGLEW.so )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR})
