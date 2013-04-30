package edu.pitt.cs1635.bji6.proj1;

import android.os.Build;
import android.os.Bundle;
import android.annotation.SuppressLint;  //need this for suppresslint line below
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.support.v4.app.NavUtils;

public class DisplayMessageActivity extends Activity {

	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_display_message);
		
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
		{
			// Show the Up button in the action bar.
			getActionBar().setDisplayHomeAsUpEnabled(true);
		}
		
		//Get string from main activity
		Intent intent = getIntent();
		String message = intent.getStringExtra(PostageCalculator.EXTRA_MESSAGE);
		
		//Display price in textview
		TextView tv = (TextView) findViewById(R.id.textView1);
	    tv.setTextSize(25);
	    //System.out.println("Message" + message);
	    tv.setText(message);

	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			// This ID represents the Home or Up button. In the case of this
			// activity, the Up button is shown. Use NavUtils to allow users
			// to navigate up one level in the application structure. For
			// more details, see the Navigation pattern on Android Design:
			//
			// http://developer.android.com/design/patterns/navigation.html#up-vs-back
			//
			NavUtils.navigateUpFromSameTask(this);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	//Return to home screen when go back button is clicked
	public void returnToHome(View view)
	{
		//Go back to postage calculator activity
		Intent intent = new Intent(this, PostageCalculator.class);
		
		startActivity(intent);
	}
	

}
