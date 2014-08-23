#include "NativeHelper.h" // class def
#include "platform/android/jni/JniHelper.h"
#include "base/ccMacros.h" // CCAssert
#include "base/CCPlatformMacros.h" // USING_NS_CC

#define NATIVE_HELPER_CLASS "org/cocos2dx/cpp"

USING_NS_CC;

void NativeHelper::vibrate(int milliseconds)
{
    JniMethodInfo minfo;
    CCAssert(JniHelper::getStaticMethodInfo(minfo, NATIVE_HELPER_CLASS, "vibrate", "(I)V"), "Function doesn't exist");
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jint)milliseconds);
    minfo.env->DeleteLocalRef(minfo.classID);
}
bool NativeHelper::canVibrate()
{
    JniMethodInfo minfo;
    CCAssert(JniHelper::getStaticMethodInfo(minfo, NATIVE_HELPER_CLASS, "canVibrate", "()Z"), "Function doesn't exist");
    bool result = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
    minfo.env->DeleteLocalRef(minfo.classID);
    return result;
}

