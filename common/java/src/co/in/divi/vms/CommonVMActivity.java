package co.in.divi.vms;

import android.os.Bundle;
import co.in.divi.vms.common.Challenge;
import co.in.divi.vms.common.ChallengeAttempt;

abstract public class CommonVMActivity extends BaseVMActivity
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        NativeHelper.setMainActivity(this);
    }

	@Override
	protected void onVMDetailsLoaded() {
        for (Challenge ch : vm.challenges) {
            setChallengeDetails(ch);
        }
    }

    @Override
	protected void onAttemptsLoaded() {
        for (String challengeId : attempts.keySet()) {
            ChallengeAttempt att = attempts.get(challengeId);
            setAttemptNative(challengeId, att.totalPoints, att.subquestions, 
                             att.correctAttempts, att.wrongAttempts, att.data);
        }
    }

    private void setChallengeDetails(Challenge ch) {
        setChallengeDetailsNative(ch.id, ch.title, ch.description);
    }

    private native void setChallengeDetailsNative(String id, String title, String description); 

    private native void setAttemptNative(String challengeId, int totalPoints, int subquestions, 
                                         int correctAttempts, int wrongAttempts, String data);
}
