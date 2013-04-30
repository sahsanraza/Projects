package edu.pitt.cs1635.bji6.twitterapp;

import java.io.IOException;

import com.twitterapime.rest.UserAccountManager;
import com.twitterapime.rest.Credential;
import com.twitterapime.search.LimitExceededException;
import com.twitterapime.xauth.Token;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;

//Takes care of twitter authentication
public class MainActivity extends Activity {

	//Class variables
	private Token token;
	private Credential c;
	private UserAccountManager m;
	private boolean userVerified;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
	
		setContentView(R.layout.activity_main);
		
		userVerified = false;
		
		twitterAuth();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	//Test for twitter authentication
	public void twitterAuth()
	{
		//Uses single token authentication with MY account info
		
		//removed credentials for security reasons
		token = new Token("", "");
		c = new Credential("", "", "", token);

		m = UserAccountManager.getInstance(c);
		 
		//Verify credential throws an exception
		try 
		{
			if (m.verifyCredential()) 
			{
				userVerified = true;
				
				Log.i("BEN", "User's credentials are OK! You're now logged in!");
			} 
			else 
			{
				Log.i("BEN", "Consumer or token keys are invalid!");
			}
		} catch (IOException e) 
		{
			e.printStackTrace();
		} 
		catch (LimitExceededException e) 
		{
			e.printStackTrace();
		}
		
		
	}
	
	//Function to start home activity
	public void goHome(View view)
	{
		// Create intent to send to Home screen activity
    	Intent intent = new Intent(this, Home.class);
		
    	//Start Home screen activity
		startActivityForResult(intent, 0);
		
	}
	
	//Function to go to mentions tweets
	public void goMentions(View view)
	{
		// Create intent to send to mentions tweet activity
    	Intent intent = new Intent(this, MentionsActivity.class);
		
    	//Start mentions activity
		startActivityForResult(intent, 0);
		
	}	

	//Function to go post a new tweet
	public void goPost(View view)
	{
		// Create intent to send to post tweet activity
    	Intent intent = new Intent(this, PostActivity.class);
		
    	//Start post activity
		startActivityForResult(intent, 0);
		
	}
	
	//Closes application
	public void endApp(View view)
	{
		finish();
	}
}



















