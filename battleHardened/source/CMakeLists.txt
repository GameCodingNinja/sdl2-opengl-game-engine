
# Add the game level executable files
add_executable(
    ${PROJECT_NAME}
    game/game.cpp
    game/main.cpp
    smartGUI/smartconfirmbtn.cpp
    state/commonstate.cpp
    state/startupstate.cpp
    state/titlescreenstate.cpp
    state/loadstate.cpp
    state/level1state.cpp
    ai/playershipai.cpp
    ai/projectileai.cpp )

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm")
    # List all the libraries to link against
    target_link_libraries(
        ${PROJECT_NAME}
        /opt/vc/lib/libbcm_host.so
        /opt/vc/lib/libbrcmEGL.so
        /opt/vc/lib/libbrcmGLESv2.so
        /opt/vc/lib/libbrcmOpenVG.so
        /opt/vc/lib/libbrcmWFC.so
        /opt/vc/lib/libcontainers.so
        /opt/vc/lib/libdebug_sym.so
        /opt/vc/lib/libdtovl.so
        /opt/vc/lib/libEGL.so
        /opt/vc/lib/libelftoolchain.so
        /opt/vc/lib/libGLESv1_CM.so
        /opt/vc/lib/libGLESv2.so
        /opt/vc/lib/libmmal.so
        /opt/vc/lib/libmmal_components.so
        /opt/vc/lib/libmmal_core.so
        /opt/vc/lib/libmmal_util.so
        /opt/vc/lib/libmmal_vc_client.so
        /opt/vc/lib/libopenmaxil.so
        /opt/vc/lib/libOpenVG.so
        /opt/vc/lib/libvchiq_arm.so
        /opt/vc/lib/libvcos.so
        /opt/vc/lib/libvcsm.so
        /opt/vc/lib/libWFC.so
        ${SDL2_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${CMAKE_BINARY_DIR}/angelscript/libangelscript.a
        ${CMAKE_BINARY_DIR}/Box2D/libBox2D.a
        ${CMAKE_BINARY_DIR}/bulletPhysics/libbulletPhysics.a
        ${CMAKE_BINARY_DIR}/library/liblibrary.a )
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
        /usr/lib/libGLEW.so )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR})
