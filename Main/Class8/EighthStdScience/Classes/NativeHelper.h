#ifndef  _NATIVE_HELPER_H
#define  _NATIVE_HELPER_H

#include <vector>
class NativeHelper
{
    static void vibrate(ssize_t  milliseconds);
    void vibrate(std::vector<ssize_t> milliseconds, int repeat);
    static bool canVibrate();
};
#endif // _NATIVE_HELPER_H
