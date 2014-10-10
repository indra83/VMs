#ifndef  _NATIVE_HELPER_H
#define  _NATIVE_HELPER_H

#include <vector>
#include <ChallengeData.h>

class NativeHelper
{
public :
    static void vibrate(ssize_t  milliseconds);
    static void vibrate(std::vector<ssize_t> milliseconds, int repeat);
    static bool canVibrate();
    static void saveAttemptData(const ChallengeAttempt &ch);
};
#endif // _NATIVE_HELPER_H
