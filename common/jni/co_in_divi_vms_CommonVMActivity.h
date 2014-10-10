/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class co_in_divi_vms_CommonVMActivity */

#ifndef _Included_co_in_divi_vms_CommonVMActivity
#define _Included_co_in_divi_vms_CommonVMActivity
#ifdef __cplusplus
extern "C" {
#endif
#undef co_in_divi_vms_CommonVMActivity_MODE_PRIVATE
#define co_in_divi_vms_CommonVMActivity_MODE_PRIVATE 0L
#undef co_in_divi_vms_CommonVMActivity_MODE_WORLD_READABLE
#define co_in_divi_vms_CommonVMActivity_MODE_WORLD_READABLE 1L
#undef co_in_divi_vms_CommonVMActivity_MODE_WORLD_WRITEABLE
#define co_in_divi_vms_CommonVMActivity_MODE_WORLD_WRITEABLE 2L
#undef co_in_divi_vms_CommonVMActivity_MODE_APPEND
#define co_in_divi_vms_CommonVMActivity_MODE_APPEND 32768L
#undef co_in_divi_vms_CommonVMActivity_MODE_MULTI_PROCESS
#define co_in_divi_vms_CommonVMActivity_MODE_MULTI_PROCESS 4L
#undef co_in_divi_vms_CommonVMActivity_MODE_ENABLE_WRITE_AHEAD_LOGGING
#define co_in_divi_vms_CommonVMActivity_MODE_ENABLE_WRITE_AHEAD_LOGGING 8L
#undef co_in_divi_vms_CommonVMActivity_BIND_AUTO_CREATE
#define co_in_divi_vms_CommonVMActivity_BIND_AUTO_CREATE 1L
#undef co_in_divi_vms_CommonVMActivity_BIND_DEBUG_UNBIND
#define co_in_divi_vms_CommonVMActivity_BIND_DEBUG_UNBIND 2L
#undef co_in_divi_vms_CommonVMActivity_BIND_NOT_FOREGROUND
#define co_in_divi_vms_CommonVMActivity_BIND_NOT_FOREGROUND 4L
#undef co_in_divi_vms_CommonVMActivity_BIND_ABOVE_CLIENT
#define co_in_divi_vms_CommonVMActivity_BIND_ABOVE_CLIENT 8L
#undef co_in_divi_vms_CommonVMActivity_BIND_ALLOW_OOM_MANAGEMENT
#define co_in_divi_vms_CommonVMActivity_BIND_ALLOW_OOM_MANAGEMENT 16L
#undef co_in_divi_vms_CommonVMActivity_BIND_WAIVE_PRIORITY
#define co_in_divi_vms_CommonVMActivity_BIND_WAIVE_PRIORITY 32L
#undef co_in_divi_vms_CommonVMActivity_BIND_IMPORTANT
#define co_in_divi_vms_CommonVMActivity_BIND_IMPORTANT 64L
#undef co_in_divi_vms_CommonVMActivity_BIND_ADJUST_WITH_ACTIVITY
#define co_in_divi_vms_CommonVMActivity_BIND_ADJUST_WITH_ACTIVITY 128L
#undef co_in_divi_vms_CommonVMActivity_CONTEXT_INCLUDE_CODE
#define co_in_divi_vms_CommonVMActivity_CONTEXT_INCLUDE_CODE 1L
#undef co_in_divi_vms_CommonVMActivity_CONTEXT_IGNORE_SECURITY
#define co_in_divi_vms_CommonVMActivity_CONTEXT_IGNORE_SECURITY 2L
#undef co_in_divi_vms_CommonVMActivity_CONTEXT_RESTRICTED
#define co_in_divi_vms_CommonVMActivity_CONTEXT_RESTRICTED 4L
#undef co_in_divi_vms_CommonVMActivity_RESULT_CANCELED
#define co_in_divi_vms_CommonVMActivity_RESULT_CANCELED 0L
#undef co_in_divi_vms_CommonVMActivity_RESULT_OK
#define co_in_divi_vms_CommonVMActivity_RESULT_OK -1L
#undef co_in_divi_vms_CommonVMActivity_RESULT_FIRST_USER
#define co_in_divi_vms_CommonVMActivity_RESULT_FIRST_USER 1L
#undef co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_DISABLE
#define co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_DISABLE 0L
#undef co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_DIALER
#define co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_DIALER 1L
#undef co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_SHORTCUT
#define co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_SHORTCUT 2L
#undef co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_SEARCH_LOCAL
#define co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_SEARCH_LOCAL 3L
#undef co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_SEARCH_GLOBAL
#define co_in_divi_vms_CommonVMActivity_DEFAULT_KEYS_SEARCH_GLOBAL 4L
#undef co_in_divi_vms_CommonVMActivity_ATTEMPTS_LOADER
#define co_in_divi_vms_CommonVMActivity_ATTEMPTS_LOADER 1L
/*
 * Class:     co_in_divi_vms_CommonVMActivity
 * Method:    setChallengeDetailsNative
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_co_in_divi_vms_CommonVMActivity_setChallengeDetailsNative
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     co_in_divi_vms_CommonVMActivity
 * Method:    setAttemptNative
 * Signature: (Ljava/lang/String;IIIILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_co_in_divi_vms_CommonVMActivity_setAttemptNative
  (JNIEnv *, jobject, jstring, jint, jint, jint, jint, jstring);

#ifdef __cplusplus
}
#endif
struct ChallengeAttempt;
void saveAttemptData(const ChallengeAttempt &ch);
#endif
