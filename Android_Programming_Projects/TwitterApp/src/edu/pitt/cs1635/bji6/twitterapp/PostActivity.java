package edu.pitt.cs1635.bji6.twitterapp;


import java.io.IOException;

import com.twitterapime.rest.Credential;
import com.twitterapime.rest.Timeline;
import com.twitterapime.rest.TweetER;
import com.twitterapime.rest.UserAccountManager;
import com.twitterapime.search.LimitExceededException;
import com.twitterapime.search.Tweet;
import com.twitterapime.xauth.Token;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

//Activity that allows users to post new tweets
public class PostActivity extends Activity
{
	private String tweet;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_post);
	}
	
	public void postTweet(View view)
	{
		//Get text entered by user
		EditText editText = (EditText) findViewById(R.id.editText1);
		
		tweet = editText.getText().toString();
		
		//Start thread to get tweets from twitter servers
		AsyncTask<String, Integer, Long> mythread = new ServerThread().execute("Go");
		
		
		//Clear text field
		editText.setText("");
		
		//Show dialog telling the user that the tweet is posted
		AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
		
		// set title
		alertDialogBuilder.setTitle("Tweet Posted!");
		
		//set message
		alertDialogBuilder.setMessage("Click OK to continue.");
		
		//Set OK button to close dialog box
		alertDialogBuilder.setPositiveButton("OK", new DialogInterface.OnClickListener() 
		{
			public void onClick(DialogInterface dialog,int id) 
			{
				// if this button is clicked close the dialog box
				dialog.cancel();
			}
		});
		
		//Show dialog box
		alertDialogBuilder.show();
		
	}
	
	//Function to go back to welcome screen
	public void goBack(View view)
	{
		// Create intent to send back to main activity
    	Intent intent = new Intent(this, MainActivity.class);
		
    	//Go back to main activity
		setResult(0, intent);
		finish();
		
		
	}
	
	
	//Defining inner Activity thread class for twitter server connection
		private class ServerThread extends AsyncTask<String, Integer, Long> 
		{
		     
			protected Long doInBackground(String... s)
			{
				//Dont post if nothing is typed in
				if (tweet.length() == 0)
				{
					return null;
				}
				
				//removed credentials for security reasons
				Token token = new Token("", "");
				Credential c = new Credential("", "", "", token);
				UserAccountManager m = UserAccountManager.getInstance(c);
				
				//Verify credential throws an exception
				try 
				{
					if (m.verifyCredential()) 
					{
						Log.i("BEN", "User's credentials are OK! You're now logged in!");
						
						//Create a new tweet object
						Tweet t = new Tweet(tweet);
						
						TweetER ter = TweetER.getInstance(m);
						//Post user's tweet
						t = ter.post(t);
						
					} 
					else 
					{
						Log.i("BEN", "Consumer or token keys are invalid!");
					}
				} 
				catch (IOException e) 
				{
					e.printStackTrace();
				} 
				catch (LimitExceededException e) 
				{
					e.printStackTrace();
				}
				
				return null;
			}
		
			 protected void onProgressUpdate(Integer... progress) 
		     {
		        // setProgressPercent(progress[0]);
		     }

		     protected void onPostExecute(Long result) 
		     {
		        //showDialog("Downloaded " + result + " bytes");
		     }
		     
		}

}
