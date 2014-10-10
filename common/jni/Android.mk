LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#$(call import-add-path,$(LOCAL_PATH)/../cocos2d)     

LOCAL_MODULE := divivm_static

LOCAL_MODULE_FILENAME := libdivivm

LOCAL_SRC_FILES := NativeHelper.cpp \
					co_in_divi_vms_CommonVMActivity.cpp \
					../Classes/Util.cpp \
					../Classes/PopUpScene.cpp \
					../Classes/ChallengeMenuScene.cpp \
					../Classes/ChallengeBase.cpp \
					../Classes/VmBase.cpp \
					../Classes/AdvanceSprite.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
	                    $(LOCAL_PATH)/.

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	                    $(LOCAL_PATH)/. \
	                    $(LOCAL_PATH)/../Classes \
	                    $(LOCAL_PATH)/../cocos2d/cocos
ifeq ($(USE_VIBRATE),1)
	LOCAL_CPPFLAGS := -DUSE_VIBRATE=1
endif

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_STATIC_LIBRARY)

#$(call import-module,cocos)
