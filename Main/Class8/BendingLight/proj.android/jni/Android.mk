LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../../../common/cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../../../common/cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../../../common/cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../../../common)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameLoadScene.cpp \
                   ../../Classes/Challenge.cpp \
                   ../../Classes/GameLayerScene.cpp \
                   ../../Classes/BackGroundLayer.cpp \
                   ../../Classes/SpriteLayer.cpp \
                   ../../Classes/MenuLayer.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../../common/jni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../../common/Classes

ifeq ($(USE_VIBRATE),1)
	LOCAL_CPPFLAGS := -DUSE_VIBRATE=1
endif

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,extensions)
$(call import-module,jni)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
