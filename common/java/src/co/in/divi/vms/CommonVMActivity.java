package co.in.divi.vms;

import android.os.Bundle;

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
        //for (Challenge ch : vm.challenges)
    }

    @Override
	protected void onAttemptsLoaded() {
        //for (String challengeId : attempts.keySet())
    }
}
