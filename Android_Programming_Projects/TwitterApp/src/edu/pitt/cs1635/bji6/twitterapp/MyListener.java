package edu.pitt.cs1635.bji6.twitterapp;

import java.util.ArrayList;

import android.util.Log;

import com.twitterapime.search.SearchDeviceListener;
import com.twitterapime.search.Tweet;
import com.twitterapime.search.TweetEntity;

public class MyListener implements SearchDeviceListener
{
	private ArrayList<String> tweets;
	private String[] url_map;	
	private boolean searchDone;
	
	public MyListener()
	{
		tweets = new ArrayList<String>();
		url_map = new String[100];
		searchDone = false;
	}
	
	//Add tweets to arraylist of tweets
	public void tweetFound(Tweet tweet)
	{
		String temp = tweet.toString();
		
		
		tweets.add(temp);
		
		//System.out.println(tweet);
	}

	//All tweets have been collected
	public void searchCompleted() {
		searchDone = true;
		
	}
	
	public void searchFailed(Throwable arg0) {
		
	}
	
	//Get list of tweets
	public ArrayList<String> getTweets()
	{	
		return tweets;
	}
	
	//Get tweet objects
	public String[] getURLs()
	{
		String temp, temp1;
		
		for (int x = 0; x < tweets.size(); x++)
		{
		
			temp1 = tweets.get(x);
			
			int index1 = temp1.indexOf("TWEET_CONTENT:");
			int index2 = temp1.indexOf("TWEET_SOURCE");
			
			temp = temp1.substring(index1, index2);
			
			//Get URLs from tweet if it has any
			int index3 = temp.indexOf("http");
			
			if (index3 > -1)
			{
				int index4 = temp.indexOf(" ", index3);
				
				if (index4 < 0)
				{
					index4 = temp.length() - 1;
				}
			
				//Log.i("BEN", tweets.size());
				
				url_map[x] = temp.substring(index3, index4);
				
			}
		}
		
		return url_map;
	}
	
	public boolean isReady()
	{
		return searchDone;
	}
	

}
