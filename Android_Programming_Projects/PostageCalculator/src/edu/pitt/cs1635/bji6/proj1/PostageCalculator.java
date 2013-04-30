package edu.pitt.cs1635.bji6.proj1;

import java.text.DecimalFormat;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;  //needed for the sendMessage function
import android.content.Intent; //also needed for send message
import android.widget.Button;
import android.widget.EditText; //same as above
import android.widget.RadioButton;


public class PostageCalculator extends Activity {
	
	//the key for my intents key value pair
	 public final static String EXTRA_MESSAGE = "edu.pitt.cs1635.bji6.proj1.MESSAGE";
	 
	 //stores package weight entered by user
	 public double amount = 0.0;
	 public boolean checked = false;
	 public boolean l = false;
	 public boolean e = false;
	 public boolean p = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_postage_calculator);
    }
 

    @Override
    public boolean onCreateOptionsMenu(Menu menu) 
    {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.activity_postage_calculator, menu);
        return true;
    }
    
    /** Called when the user clicks the Calculate button */
    public void sendMessage(View view) 
    {
    	EditText editText = (EditText) findViewById(R.id.editText1);
    	
    	//Only execute method if input has been entered into text view
    	if (!editText.getText().toString().equals(""))
    	{
	    	//Total cost of shipment
	    	double price = 0.0;
	    	String total = "";
	    	//for formatting
	    	DecimalFormat df = new DecimalFormat("#.##");
	    	
	        // Do something in response to button
	    	Intent intent = new Intent(this, DisplayMessageActivity.class);
	    
	    	//Get number value from user input
	    	try
	    	{
	    		amount = Double.parseDouble(editText.getText().toString());
	    		
	    		//Round user input
	        	int amount2 = (int) Math.round(amount);
	        	
	        	//System.out.println(amount2);
	        	
	        	// Check which radio button was clicked and perform correct calculation
	              if (e)
	              {
	        		if (checked && amount2 >= 0 && amount2 <= 13)
	                {   
	                	//System.out.println("Made it in further");
	                	//Calculate envelope prices
	                	if ( amount2 > 1)
	                	{
	                		price = 0.90;
	                		price += (0.2 * (amount2 - 1 ));
	                	}
	                	else
	                	{
	                		price = 0.90 * amount2;
	                	}
	                	
	                	total = "$" + String.valueOf(df.format(price));
	                    
	                    total += " is your total.";
	                }
	                else
	            	{
	            		total = "Invalid amount entered! :(";
	            	}
	              } 
	              if (l)
	              {
	                if (checked && amount2 >= 0 && amount <= 3.5)
	                {
	                	//Calculate letter prices
	                	if (amount2 > 1)
	                	{
	                		price = 0.45;
	                		price += (0.2 * (amount2 - 1 ));
	                	}
	                	else
	                	{
	                		price = 0.45 * amount2;
	                	}
	                	
	                	total = "$" + String.valueOf(df.format(price));
	                    
	                    total += " is your total.";
	                }
	                else
	            	{
	            		total = "Invalid amount entered! :(";
	            	}
	              }
	              if (p)
	              {
	            	if (checked && amount2 >= 0 && amount2 <= 13)
	            	{
	            		//Calculate package prices
	                	if (amount2 > 3)
	                	{
	                		price = 1.95;
	                		price += (0.17 * (amount2 - 3 ));
	                	}
	                	else if(amount2 == 0)
	                	{
	                		price = 0;
	                	}
	                	else
	                	{
	                		price = 1.95;
	                	}
	                	
	                	total = "$" + String.valueOf(df.format(price));
	                    
	                    total += " is your total.";
	                }
	            	else
	            	{
	            		total = "Invalid amount entered! :(";
	            	}
	              }
	    	}
	    	catch (NumberFormatException e)
	    	{
	    		total = "Invalid amount entered! :(";
	    	}
	    	
	    	
	    	
	        //System.out.println("Total = " + total);
	    	//Dont SEND AMOUNT send calculated price
	    	//Extras are key value pairs that intents can pass data to other activities
	    	intent.putExtra(EXTRA_MESSAGE, total);
	    	
	    	//Start the display message activity passing to it my edit_message string
	    	startActivity(intent);
	    	
    	}
    
    }
    
    //Radio button logic
    public void onRadioButtonClicked(View view) {
        // Is the button now checked?
        checked = ((RadioButton) view).isChecked();
        
        EditText editText = (EditText) findViewById(R.id.editText1);
        
        // Check which radio button was clicked
        switch(view.getId()) 
        {
            case R.id.radio_envelope:
                if (checked)
                {   
                	e = true;
                	l = false;
                	p = false;
                	// Display weight restrictions for envelopes
                	editText.setHint(R.string.message_package);
                }
                break;
            case R.id.radio_letter:
                if (checked)
                {
                	l = true;
                	e = false;
                	p = false;
                	// Display weight restrictions for letters
                	editText.setHint(R.string.message_letter);
                }
                break;
            case R.id.radio_package:
            	if (checked)
            	 {
            		p = true;
            		l = false;
            		e = false;
                	// Display weight restrictions for packages
                	editText.setHint(R.string.message_package);
                }
            	break;
            
        }
    }
    
    //Method to exit the application when exit button is clicked
    public void exitApp(View view)
    {
    	Intent intent = new Intent(Intent.ACTION_MAIN);
    	intent.addCategory(Intent.CATEGORY_HOME);
    	intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    	startActivity(intent);
    }
    
}
