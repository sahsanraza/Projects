package edu.pitt.cs1635.bji6.twitterapp;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

import com.twitterapime.rest.Credential;
import com.twitterapime.rest.Timeline;
import com.twitterapime.rest.UserAccountManager;
import com.twitterapime.xauth.Token;
import com.twitterapime.search.LimitExceededException;
import com.twitterapime.search.Tweet;
import com.twitterapime.search.TweetEntity;


import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

public class Home extends Activity
{
	//class variables
	private ArrayList<String> tweets;
	private ArrayList<String> tweetText;
	private String[] url_map;
	private MyListener ml;
	private ListView listView1; 
	private Context my_context;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_home);
		
		//Class that retrieves tweets from Twitter servers
		ml = new MyListener();
		
		tweetText = new ArrayList<String>();
		url_map = new String[100];
		
		my_context = (Context) this;
		
		getTweets();
	}
	
	//Function to get home tweets and display them in listview
	public void getTweets()
	{
		listView1 = (ListView) findViewById(R.id.listView1);
	    
		//Start thread to get tweets from twitter servers
		AsyncTask<String, Integer, Long> mythread = new ServerThread().execute("Go");
	    
		//Wait until we have all the tweets
		while (ml.isReady() == false)
		{
		}
		
		//PLace tweets from listener into array list
		tweets = ml.getTweets();
		
		//Get urls
		url_map = ml.getURLs();
		
		
		//Collect corect content for each tweet.
		for (int x = 0; x < tweets.size(); x++)
		{
			String entire = "";
			String temp = tweets.get(x);
			int index1 = temp.indexOf("USERACCOUNT_NAME:");
			int index2 = temp.indexOf("USERACCOUNT_ID");
			entire += "User: ";
			entire += temp.substring(index1+17, index2);
			
			entire += "\nMessage: ";
			index1 = temp.indexOf("TWEET_CONTENT:");
			index2 = temp.indexOf("TWEET_SOURCE");
			entire += temp.substring(index1+14, index2);
			
			entire += "\nDate: ";
			index1 = temp.indexOf("TWEET_PUBLISH_DATE:");
			index2 = temp.indexOf("TWEET_ID");
			
			Long date1 = Long.valueOf(temp.substring(index1+20, index2-1));
			
			Date d = new Date(date1.longValue());
			
			entire += d.toString();
			
			tweetText.add(entire);
		}
		
		//System.out.println(tweets);
	        
	    ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, tweetText);
	        
	    listView1.setAdapter(adapter);
	    
	    listView1.setOnItemClickListener(new OnItemClickListener() 
	    {
	        public void onItemClick(AdapterView<?> myAdapter, View myView, int myItemInt, long mylng) 
	        {
	          //Launch new display tweet activity when a user clicks on a tweet
	    		String tweet = tweets.get(myItemInt);
	        	
	    		//Try using position number to index original array of tweet data, Might work??
	    		Intent i = new Intent(my_context, DisplayTweet.class);
	    		
	    		i.putExtra("Tweet", tweet);
	    		//Log.i("BEN", myItemInt + "");
	    		
	    		//Also send URL if it has one otherwise send empty string
	    		if (url_map[myItemInt] != null)
	    		{
	    			i.putExtra("URL", url_map[myItemInt]);
	    		}
	    		else
	    		{
	    			i.putExtra("URL", "Nothing");
	    		}
	    		
	    		startActivity(i);
	         }                 
	    });
		
	}
	
	//Refresh for new tweets
	public void Refresh(View view)
	{
		ml = new MyListener();
		tweetText = new ArrayList<String>();
		 
		 url_map = new String[100];
		
		getTweets();
	}
	
	//Go back to main screen
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
					
					//Get home tweets for bji6_ben twitter account
					Timeline tml = Timeline.getInstance(m);
					
					tml.startGetHomeTweets(null, ml);
					
					
				} else 
				{
					Log.i("BEN", "Consumer or token keys are invalid!");
				}
			} catch (IOException e) 
			{
				e.printStackTrace();
			} catch (LimitExceededException e) 
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
