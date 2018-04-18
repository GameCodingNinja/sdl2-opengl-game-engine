LOCAL_PATH := $(call my-dir)

###########################
#
# template shared library
#
###########################

# Removed code
# $(wildcard $(LOCAL_PATH)/library/slot/*.cpp) \

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
ASCRIPT_PATH := $(LOCAL_PATH)/../angelscript

LOCAL_C_INCLUDES := \
	$(SDL_PATH)/include \
	$(LOCAL_PATH)/library \
	$(LOCAL_PATH)/../boost \
	$(ASCRIPT_PATH)/add_on

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_CXXFLAGS := -std=gnu++14 -fexceptions

LOCAL_SRC_FILES := \
    $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/source/game/*.cpp) \
	$(wildcard $(LOCAL_PATH)/source/smartGUI/*.cpp) \
	$(wildcard $(LOCAL_PATH)/source/state/*.cpp) \
	$(wildcard $(LOCAL_PATH)/source/ai/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/gui/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/objectdata/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/system/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/managers/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/physics/physicsworldmanager2d.cpp) \
	$(wildcard $(LOCAL_PATH)/library/physics/physicsworld2d.cpp) \
	$(wildcard $(LOCAL_PATH)/library/physics/physicscomponent2d.cpp) \
	$(wildcard $(LOCAL_PATH)/library/2d/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/3d/object3d.cpp) \
	$(wildcard $(LOCAL_PATH)/library/common/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/utilities/*.cpp) \
	$(wildcard $(LOCAL_PATH)/library/soil/*.c) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptmanager.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptglobals.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptcolor.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptcomponent.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptpoint.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptsprite2d.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptsound.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptplaylist.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptsoundmanager.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptobject.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptmenu.cpp) \
	$(wildcard $(LOCAL_PATH)/library/script/scriptshadermanager.cpp))

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer Box2D angelscript

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)

