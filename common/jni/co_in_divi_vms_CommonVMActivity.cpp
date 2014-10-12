#include "co_in_divi_vms_CommonVMActivity.h"
#include "ChallengeData.h"
#include "VmBase.h"
#include "cocos2d.h"
#include <vector>
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

static std::vector<ChallengeInfo> sInfos;
static std::vector<ChallengeAttempt> sAttempts;

std::string getString(JNIEnv *jenv, jstring str)
{
    if ( !str ) std::string();

    const jsize len = jenv->GetStringUTFLength(str);
    const char* strChars = jenv->GetStringUTFChars(str, (jboolean *)0);

    std::string result(strChars, len);

    jenv->ReleaseStringUTFChars(str, strChars);
    return result;
}

JNIEXPORT void JNICALL Java_co_in_divi_vms_CommonVMActivity_setChallengeDetailsNative
  (JNIEnv *jenv, jobject obj, jstring idStr, jstring titleStr, jstring descStr)
{
    auto challengeDetails = ChallengeInfo(getString(jenv, idStr), getString(jenv, titleStr), getString(jenv, descStr));
    auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
    if (base)
        base->addChallengeDetails(challengeDetails);
    else
        sInfos.push_back(challengeDetails);
}

JNIEXPORT void JNICALL Java_co_in_divi_vms_CommonVMActivity_setAttemptNative
  (JNIEnv *jenv, jobject obj, jstring challengeIdStr, jint totalPoints, jint subquestions, jint correctAttempts, jint wrongAttempts, jstring dataStr)
{
    auto attemptData = ChallengeAttempt(getString(jenv, challengeIdStr), totalPoints, subquestions, correctAttempts, wrongAttempts, getString(jenv, dataStr));
    auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
    if (base)
        base->addAttemptDetails(attemptData);
    else
        sAttempts.push_back(attemptData);
}

void clearCachedData()
{
    sInfos.clear();
    sAttempts.clear();
}

const std::vector<ChallengeAttempt> & getCachedAttempts() { return sAttempts; }
const std::vector<ChallengeInfo> & getCachedInfos() { return sInfos; }

#define CLASS_NAME "co/in/divi/vms/BaseVMActivity"

void saveAttemptData(const ChallengeAttempt &ch)
{
    JniMethodInfo minfo;
    CCAssert(JniHelper::getStaticMethodInfo(minfo, CLASS_NAME, "saveCurrentScore", "(Ljava/lang/String;IIIILjava/lang/String;)V"), "Function doesn't exist");
    jstring stringArg1 = minfo.env->NewStringUTF(ch._challengeId.c_str());
    jstring stringArg2 = minfo.env->NewStringUTF(ch._data.c_str());
    minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID,  
                                    stringArg1, (jint)ch._totalPoints, (jint)ch._subquestions, 
                                    (jint)ch._correctAttempts, (jint)ch._wrongAttempts, stringArg2);
    minfo.env->DeleteLocalRef(minfo.classID);
    minfo.env->DeleteLocalRef(stringArg1);
    minfo.env->DeleteLocalRef(stringArg2);
}

