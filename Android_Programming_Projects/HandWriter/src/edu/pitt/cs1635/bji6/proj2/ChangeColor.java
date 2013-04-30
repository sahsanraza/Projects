package edu.pitt.cs1635.bji6.proj2;

import android.os.Build;
import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

//This class is an activity that lets users change their drawing color
//through the use of a spinner
//In order to write spinner event handlers I had to implement the 
//OnItemSelectedListener interface and the OnItemSelected, OnNothingSelected methods

public class ChangeColor extends Activity implements OnItemSelectedListener
{
	
	Intent i;
	
	
	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_change_color);
		
		//Get string from main activity
		//Intent intent = getIntent();
		//String message = intent.getStringExtra(PostageCalculator.EXTRA_MESSAGE);
		
		
		Spinner spinner = (Spinner) findViewById(R.id.spinner1);
		
		// Create an ArrayAdapter using the string array and a default spinner layout
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
		        R.array.colors_array, android.R.layout.simple_spinner_item);
		
		// Specify the layout to use when the list of choices appears
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		
		// Apply the adapter to the spinner
		spinner.setAdapter(adapter);
		
		//Apply the event listener to my spinner widget
		spinner.setOnItemSelectedListener(this);

	}
	
	//Handles user pressing the go back button
	public void returnHome(View view) 
	{
		Log.i("BEN", "Intent is being sent back");
    	setResult(0, i);
    	finish();
	}
	
	public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) 
	{
		//Create an intent to send color info back to hand writer activity
		i = new Intent(this, HandWriter.class);
		
		// An item was selected. You can retrieve the selected item using
	    //String s = (String) parent.getItemAtPosition(pos);
		switch((int) id)
		{
			case 0:
				i.putExtra("Color", Color.BLACK);
				break;
			case 1:
				i.putExtra("Color", Color.YELLOW);
				break;
			case 2:
				i.putExtra("Color", Color.GREEN);
				break;
			case 3:
				i.putExtra("Color", Color.BLUE);
				break;
			case 4:
				i.putExtra("Color", Color.MAGENTA);
				break;
			case 5:
				i.putExtra("Color", Color.RED);
				break;
			default:
				//-1 means don't change the color
				i.putExtra("Color", -1);
				break;
		}
		
	}

	public void onNothingSelected(AdapterView<?> parent)
	{
	    // Another interface callback
	}

	
	
}
