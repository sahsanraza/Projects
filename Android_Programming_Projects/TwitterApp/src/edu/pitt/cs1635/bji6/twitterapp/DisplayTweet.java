package edu.pitt.cs1635.bji6.twitterapp;

import java.net.URISyntaxException;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

//Class that displays a tweet with detailed author information and the ability to launch
//a URL in tweet
public class DisplayTweet extends Activity 
{
	private String entireTweet;
	private String URL;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.display_activity);
		
		//Get tweet to be displayed from activity that launched us
		Intent i = getIntent();
		
		entireTweet = i.getStringExtra("Tweet");
		
		URL = i.getStringExtra("URL");
		
		Log.i("BEN", URL);
		
		Button b1 = (Button) findViewById(R.id.urlbutton);
		
		//Gray out button if there is no URL in tweet
		if (URL.equals("Nothing"))
		{
			b1.setEnabled(false);
			b1.setBackgroundResource(R.drawable.no_url);
		}
		
		displayInfo();
	}
	
	//Function to display relevant information 
	public void displayInfo()
	{
		//Display user name
		String temp = "User Name: ";
		int index1 = entireTweet.indexOf("USERACCOUNT_USER_NAME:");
		int index2 = entireTweet.indexOf("USER", index1 + 22);
		
		temp += entireTweet.substring(index1 + 22, index2);
		
		TextView tv = (TextView) findViewById(R.id.username);
		tv.setText(temp);
		
		
		//Display real name
		temp = "Real Name: ";
		
		index1 = entireTweet.indexOf("USERACCOUNT_NAME:");
		index2 = entireTweet.indexOf("USER", index1 + 17);
		
		temp += entireTweet.substring(index1 + 17, index2);
		
		tv = (TextView) findViewById(R.id.realname);
		tv.setText(temp);
		
		//Display number of tweets posted by author
		temp = "Post count: ";
		
		index1 = entireTweet.indexOf("USERACCOUNT_TWEETS_COUNT:");
		index2 = entireTweet.indexOf("USER", index1 + 25);
		
		temp += entireTweet.substring(index1 + 25, index2);
		
		tv = (TextView) findViewById(R.id.tweetcount);
		tv.setTextSize(10);
		tv.setText(temp);
		
		//Display number of followers
		temp = "Followers count: ";
		
		index1 = entireTweet.indexOf("USERACCOUNT_FOLLOWERS_COUNT:");
		index2 = entireTweet.indexOf("USER", index1 + 28);
		
		temp += entireTweet.substring(index1 + 28, index2);
		
		tv = (TextView) findViewById(R.id.followerscount);
		tv.setTextSize(10);
		tv.setText(temp);
		
		//Display number of followees
		temp = "Followees count: ";
		
		index1 = entireTweet.indexOf("USERACCOUNT_FRIENDS_COUNT:");
		index2 = entireTweet.indexOf("USER", index1 + 26);
		
		temp += entireTweet.substring(index1 + 26, index2);
		
		tv = (TextView) findViewById(R.id.friendscount);
		tv.setTextSize(10);
		tv.setText(temp);
		
		//Display tweet
		temp = "Message: ";
		index1 = entireTweet.indexOf("TWEET_CONTENT:");
		index2 = entireTweet.indexOf("TWEET_SOURCE");
		temp += entireTweet.substring(index1+14, index2);
		
		tv = (TextView) findViewById(R.id.tweetmsg);
		tv.setText(temp);
		
	}
	
	//Launch Android browser using URL in tweet with an Intent
	public void launchBrowser(View view)
	{
		Intent intent;
		
		try 
		{
			intent = Intent.parseUri(URL, 0);
			startActivity(intent);
		} 
		catch (URISyntaxException e) 
		{
			e.printStackTrace();
		}
		
		
	}
	
	//Go back to home tweets
	public void backToPrevious(View view)
	{
		finish();
	}
	
	
	
}
