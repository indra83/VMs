ECHO "Custom Building"

adb install -r proj.android/bin/$proj_name-debug.apk
adb shell am start -n in.co.divi.$proj_name/org.cocos2dx.cpp.AppActivity
adb logcat
