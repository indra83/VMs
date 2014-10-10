#include "co_in_divi_vms_CommonVMActivity.h"
#include "ChallengeData.h"
#include "VmBase.h"
#include "cocos2d.h"

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
}

JNIEXPORT void JNICALL Java_co_in_divi_vms_CommonVMActivity_setAttemptNative
  (JNIEnv *jenv, jobject obj, jstring challengeIdStr, jint totalPoints, jint subquestions, jint correctAttempts, jint wrongAttempts, jstring dataStr)
{
    auto attemptData = ChallengeAttempt(getString(jenv, challengeIdStr), totalPoints, subquestions, correctAttempts, wrongAttempts, getString(jenv, dataStr));
    auto base = dynamic_cast<VmBase *>(cocos2d::Application::getInstance());
    if (base)
        base->addAttemptDetails(attemptData);
}

// TODO: save score
