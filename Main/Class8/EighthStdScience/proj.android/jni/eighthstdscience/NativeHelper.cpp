#include "NativeHelper.h" // class def
#include "platform/android/jni/JniHelper.h"
#include "base/ccMacros.h" // CCAssert
#include "base/CCPlatformMacros.h" // USING_NS_CC

#define NATIVE_HELPER_CLASS "org/cocos2dx/cpp"

USING_NS_CC;

void NativeHelper::vibrate(ssize_t milliseconds)
{
    JniMethodInfo minfo;
    CCAssert(JniHelper::getStaticMethodInfo(minfo, NATIVE_HELPER_CLASS, "vibrate", "(J)V"), "Function doesn't exist");
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, (jlong)milliseconds);
    minfo.env->DeleteLocalRef(minfo.classID);
}

void NativeHelper::vibrate(std::vector<ssize_t> milliseconds, int repeat)
{
    size_t size = milliseconds.size();

    JniMethodInfo minfo;

    jlongArray arr;
    arr = minfo.env->NewLongArray(size);
    CCAssert(arr != NULL, "array couldnt be allocated");

    // fill a temp structure to use to populate the java int array
    jlong *fill = new jlong[milliseconds.size()];
    for (size_t i = 0; i < size; i++)
        fill[i] = milliseconds[i];

    // move from the temp structure to the java structure
    minfo.env->SetLongArrayRegion(arr, 0, size, fill);
    CCAssert(JniHelper::getStaticMethodInfo(minfo, NATIVE_HELPER_CLASS, "vibrate", "([JI)V"), "Function doesn't exist");
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, arr, (jint)repeat);
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

