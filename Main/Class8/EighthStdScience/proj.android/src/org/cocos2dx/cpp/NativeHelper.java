
package org.cocos2dx.cpp;

import android.os.Vibrator;
import android.media.ToneGenerator;
import android.media.AudioManager;
import android.content.Context;
import android.util.Log;
import org.cocos2dx.lib.Cocos2dxActivity;

public class NativeHelper
{
    private static final String TAG = "NativeHelper";

    private static volatile Cocos2dxActivity mainActivity;

    public static void setMainActivity(Cocos2dxActivity activity)
    {
        mainActivity = activity;
    }

    public static Cocos2dxActivity getMainActivity()
    {
        if(mainActivity == null)
        {
            Log.w(TAG, "Warning : null main Activity");
        }
        return mainActivity;
    }

    public static void vibrate(long milliseconds)
    {
        Vibrator v = (Vibrator) getMainActivity().getSystemService(Context.VIBRATOR_SERVICE);
        if (v.hasVibrator())
        {
            v.vibrate(milliseconds);
        }
        else
        {
            final ToneGenerator tg = new ToneGenerator(AudioManager.STREAM_NOTIFICATION, 100);
            tg.startTone(ToneGenerator.TONE_PROP_BEEP);
        }
    }

    public static void vibrate(long[] milliseconds, int repeat)
    {
        Vibrator v = (Vibrator) getMainActivity().getSystemService(Context.VIBRATOR_SERVICE);
        if (v.hasVibrator())
        {
            v.vibrate(milliseconds, repeat);
        }
        else
        {
            final ToneGenerator tg = new ToneGenerator(AudioManager.STREAM_NOTIFICATION, 100);
            tg.startTone(ToneGenerator.TONE_PROP_BEEP);
        }
    }

    public static boolean canVibrate()
    {
        Vibrator v = (Vibrator) getMainActivity().getSystemService(Context.VIBRATOR_SERVICE);
        return v.hasVibrator();
    }
}
