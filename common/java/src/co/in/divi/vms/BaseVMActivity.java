package co.in.divi.vms;

import java.util.HashMap;

import android.app.Activity;
import android.app.LoaderManager.LoaderCallbacks;
import android.content.ContentValues;
import android.content.CursorLoader;
import android.content.Intent;
import android.content.Loader;
import android.database.Cursor;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;
import co.in.divi.vms.common.ChallengeAttempt;
import co.in.divi.vms.common.ChallengeXmlParser;
import co.in.divi.vms.common.VM;

import org.xmlpull.v1.XmlPullParser;

import org.cocos2dx.lib.Cocos2dxActivity;

public abstract class BaseVMActivity extends Cocos2dxActivity implements LoaderCallbacks<Cursor> {
	private static final String					TAG				= BaseVMActivity.class.getSimpleName();
	private static final int					ATTEMPTS_LOADER	= 1;
	private static BaseVMActivity 				instance = null;

	protected String							uid, courseId, bookId, topicId, vmId, vmActivityName, fragment;
	protected String[]							breadcrumbArray;
	protected VM								vm				= null;
	protected HashMap<String, ChallengeAttempt>	attempts		= new HashMap<String, ChallengeAttempt>();

	private LoadVMTask							loadVMTask		= null;

	private boolean 							amnesiaMode 	= false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
        instance = this;
		super.onCreate(savedInstanceState);
	}

	@Override
	protected void onStart() {
		super.onStart();
		uid = getIntent().getStringExtra("UID");
		courseId = getIntent().getStringExtra("COURSE_ID");
		bookId = getIntent().getStringExtra("BOOK_ID");
		topicId = getIntent().getStringExtra("TOPIC_ID");
		vmId = getIntent().getStringExtra("VM_ID");
		vmActivityName = getIntent().getStringExtra("VM_ACTIVITY");
		breadcrumbArray = getIntent().getStringArrayExtra("BREADCRUMB");
		// fragment = getIntent().getStringExtra("FRAGMENT");
		if (uid == null || courseId == null || bookId == null || topicId == null || vmId == null) {
			amnesiaMode = true;
			Log.d(TAG, "launched standalone.. going into amnesia mode");	
		}
		if (!amnesiaMode && !getVMId().equals(vmActivityName)) {
			finish();
			Toast.makeText(this, "Id mismatch!", Toast.LENGTH_SHORT).show();
			return;
		}

		if (vm == null) {
			loadVMTask = new LoadVMTask();
			loadVMTask.execute(new Void[0]);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		// set location to current VM
		// toDO: 
		if (amnesiaMode)
			return;
		Intent intent = new Intent("co.in.divi.intent.LOCATION_BROADCAST");
		intent.putExtra("COURSE_ID", courseId);
		intent.putExtra("BOOK_ID", bookId);
		intent.putExtra("TOPIC_ID", topicId);
		intent.putExtra("VM_ID", vmId);
		intent.putExtra("BREADCRUMB", breadcrumbArray);
		sendBroadcast(intent);
	}

	@Override
	protected void onPause() {
		super.onPause();
		if (amnesiaMode)
			return;
		// clear location
		Intent intent = new Intent("co.in.divi.intent.LOCATION_BROADCAST");
		sendBroadcast(intent);
	}

	@Override
	protected void onStop() {
		super.onStop();
		if (loadVMTask != null)
			loadVMTask.cancel(false);
	}

	/*
	 * This method is called after the VM details are loaded from the /res/xml/challenges.xml file. The protected
	 * variable 'vm' will be filled prior to calling this method.
	 */
	protected abstract void onVMDetailsLoaded();

	/*
	 * This method is called when: 1. start of activity, after call to onVMDetailsLoaded() 2. on saving a new attempt by
	 * calling saveScore The protected variable 'attempts' is filled with latest attempts prior to this call
	 */
	protected abstract void onAttemptsLoaded();

	/*
	 * get the Location of the vmdetails xml file
	 */
	protected abstract XmlPullParser getVMDetailsXml();


	protected final String getVMId() {
		return getClass().getName();
	}

	public static void saveCurrentScore(String challengeId, int points, int subquestions, int correctAttempts, int wrongAttempts, String data) {
		if (instance != null)
			instance.saveScore(challengeId, points, subquestions, correctAttempts, wrongAttempts, data);
	}

	protected final void saveScore(String challengeId, int points, int subquestions, int correctAttempts, int wrongAttempts, String data) {
		Log.d(TAG, "save score called with: " + challengeId + ", " + points + ", " +  subquestions + ", " +  correctAttempts + ", " +  wrongAttempts + ", " + data );
		if (amnesiaMode) {
			// populate attempts with the new data
			ChallengeAttempt c = new ChallengeAttempt(); 
			c.courseId 			= courseId;
			c.bookId 			= bookId;
			c.assessmentId  	= getAssessmentId();
			c.questionId 		= challengeId;
			c.totalPoints 		= points;
			c.subquestions 		= subquestions;
			c.correctAttempts 	= correctAttempts;
			c.wrongAttempts 	= wrongAttempts;
			c.data 				= data;
			c.lastUpdatedAt 	= System.currentTimeMillis();
			if (correctAttempts == subquestions)
				c.solvedAt = c.lastUpdatedAt;
			attempts.put(c.questionId, c);

			// trigger onAttemptsLoaded
			onAttemptsLoaded();
			return;
		}

		ContentValues values = new ContentValues();
		values.put(DiviConstants.Attempt.UID, uid);
		values.put(DiviConstants.Attempt.COURSE_ID, courseId);
		values.put(DiviConstants.Attempt.BOOK_ID, bookId);
		values.put(DiviConstants.Attempt.ASSESSMENT_ID, getAssessmentId());
		values.put(DiviConstants.Attempt.QUESTION_ID, challengeId);
		values.put(DiviConstants.Attempt.TOTAL_POINTS, points);
		values.put(DiviConstants.Attempt.SUBQUESTIONS, subquestions);
		values.put(DiviConstants.Attempt.CORRECT_ATTEMPTS, correctAttempts);
		values.put(DiviConstants.Attempt.WRONG_ATTEMPTS, wrongAttempts);
		values.put(DiviConstants.Attempt.DATA, data);
		if (correctAttempts == subquestions)
			values.put(DiviConstants.Attempt.SOLVED_AT, System.currentTimeMillis());
		getContentResolver().insert(DiviConstants.Attempt.CONTENT_URI, values);
	}

	private String getAssessmentId() {
		return topicId + "_" + vmId;
	}

	@Override
	public Loader<Cursor> onCreateLoader(int id, Bundle args) {
		String mSelectionClause = DiviConstants.Attempt.UID + " = ? AND " + DiviConstants.Attempt.COURSE_ID + " = ? AND "
				+ DiviConstants.Attempt.BOOK_ID + " = ? AND " + DiviConstants.Attempt.ASSESSMENT_ID + " = ? ";
		String[] mSelectionArgs = new String[] { uid, courseId, bookId, getAssessmentId() };
		CursorLoader loader = new CursorLoader(this, DiviConstants.Attempt.CONTENT_URI, null, mSelectionClause, mSelectionArgs, null);
		return loader;
	}

	@Override
	public void onLoadFinished(Loader<Cursor> loader, Cursor cursor) {
		Log.d(TAG, "onLoadFinished");
		attempts.clear();
		while (cursor.moveToNext()) {
			ChallengeAttempt a = ChallengeAttempt.fromCursor(cursor);
			attempts.put(a.questionId, a);
			Log.d(TAG, "got attempt:" + a.questionId + " - " + a.totalPoints + "/" + a.subquestions + " , " + a.correctAttempts + "/"
					+ a.wrongAttempts);
		}
		onAttemptsLoaded();
	}

	@Override
	public void onLoaderReset(Loader<Cursor> loader) {
		// TODO Auto-generated method stub

	}

	private class LoadVMTask extends AsyncTask<Void, Void, Integer> {

		private VM	vm;

		@Override
		protected Integer doInBackground(Void... params) {
			vm = new ChallengeXmlParser().getVMsFromXml(getVMDetailsXml()).get(getVMId());
			return 0;
		}

		@Override
		protected void onPostExecute(Integer result) {
			super.onPostExecute(result);
			if (vm != null) {
				BaseVMActivity.this.vm = vm;
				onVMDetailsLoaded();
				// now load the attempts
				if (!amnesiaMode)
					getLoaderManager().restartLoader(ATTEMPTS_LOADER, null, BaseVMActivity.this);
			} else {
				finish();
				Toast.makeText(BaseVMActivity.this, "VM description missing!", Toast.LENGTH_SHORT).show();
			}
		}
	}
}
