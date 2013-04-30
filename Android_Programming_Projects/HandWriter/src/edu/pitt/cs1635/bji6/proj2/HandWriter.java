package edu.pitt.cs1635.bji6.proj2;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.util.Log;
import android.view.Menu;
import android.view.View;

public class HandWriter extends Activity {

	 //the key for my intents key value pair for color activity
	 public final static String COLOR_MESSAGE = "edu.pitt.cs1635.bji6.proj2.MESSAGE";
	 
	 //var stores data from server
	 public String result = "";
	 public boolean thread_running = false;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        
        setContentView(R.layout.activity_hand_writer);
        
        
	}
	
	//Use new color once activity gets result from color changer
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		//Change color of paint according to info retrieved from color activity
        Bundle info = data.getExtras();
        
        if (info != null)
        {
        	int new_color = info.getInt("Color");
        	
        	//Change color to new color chosen by user
        	if (new_color != -1)
        	{
        		DrawView d = (DrawView) findViewById(R.id.DrawView);
        		
        		d.changeColor(new_color);
        	}
        }
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_hand_writer, menu);
		return true;
	}
	
	//Method to clear the screen when the Clear button is pressed
	public void clearColors(View view)
	{
		DrawView d = (DrawView) findViewById(R.id.DrawView);
		
		d.clearColors();
		
	}
	
	//Method that starts second activity to choose a new color
	public void changeColor(View view)
	{
		// Get intent to start color activity
    	Intent intent = new Intent(this, ChangeColor.class);
		
    	//Start color activity
		startActivityForResult(intent, 0);
	}
	
	//Method to send data to the character lookup server and display the results
	public void sendData(View view)
	{
		String key = ""; //Value removed for security reasons
		
		DrawView d = (DrawView) findViewById(R.id.DrawView);
		
		String value = d.toString();
		
		//Start thread to get info from server using http request
		AsyncTask<String, Integer, Long> mythread = new ServerThread().execute(key, value);
		
		thread_running = true;
		
		//wait until thread is done executing
		while (result == "" && thread_running == true)
		{}
		
		 //Create an alert dialog to show user the data from the server
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
    	
		//Don't display data if nothing was drawn on screen
		if (d.getPointNum() == 0)
		{
			builder.setMessage("Input invalid!");
			builder.setTitle(ALARM_SERVICE);
			
		}
		else
		{
			builder.setMessage(result);
		    builder.setTitle("Similar Characters");
		}

		//Get the AlertDialog from create()
		AlertDialog dialog = builder.create();
		
		//Show the dialog
		dialog.show();
		
		result = "";
	}
	
	
	//Defining inner Activity thread class for server connection
	private class ServerThread extends AsyncTask<String, Integer, Long> 
	{
	     
		protected Long doInBackground(String... s)
		{	
			// Create a new HttpClient and Post Header
			HttpClient httpclient = new DefaultHttpClient();

			HttpPost httppost = new HttpPost("http://cwritepad.appspot.com/reco/usen");
			
			try {
			      List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(2);
			      
			      //Log.i("BEN", s[0]);
			      //Log.i("BEN", s[1]);
			      
			      nameValuePairs.add(new BasicNameValuePair("key", s[0]));
			      nameValuePairs.add(new BasicNameValuePair("q", s[1]));
			      
			      httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
			 
			      HttpResponse response = httpclient.execute(httppost);
			      
			      BufferedReader rd = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
			      
			      if ((result = rd.readLine()) != null) 
			      {
			    	  //Log.i("BEN", result);
			    	
			      }

			    } catch (IOException e) 
			    {
			      e.printStackTrace();
			    }
			
			thread_running = false; 
			
	         
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




























