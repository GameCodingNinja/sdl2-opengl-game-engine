
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
        ${CMAKE_BINARY_DIR}/library/liblibrary.a
        ${CMAKE_BINARY_DIR}/angelscript/libangelscript.a
        ${CMAKE_BINARY_DIR}/Box2D/libBox2D.a
        ${CMAKE_BINARY_DIR}/bulletPhysics/libbulletPhysics.a
        /usr/lib/arm-linux-gnueabihf/libGLESv2.so
        /usr/lib/arm-linux-gnueabihf/libSDL2.so
        /usr/lib/arm-linux-gnueabihf/libSDL2_mixer.so )
else()
    # List all the libraries to link against
    # These need to be listed in order of dependency
    target_link_libraries(
        ${PROJECT_NAME}
        ${CMAKE_BINARY_DIR}/library/liblibrary.a
        ${CMAKE_BINARY_DIR}/angelscript/libangelscript.a
        ${CMAKE_BINARY_DIR}/Box2D/libBox2D.a
        ${CMAKE_BINARY_DIR}/bulletPhysics/libbulletPhysics.a
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARY}
        ${SDL2MIXER_LIBRARY}
        ${GLEW_LIBRARY} )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR})
