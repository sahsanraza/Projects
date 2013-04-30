package edu.pitt.cs1635.bji6.proj2;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

//Class that extends View and overwrites onDraw method
public class DrawView extends View implements OnTouchListener{
	
	private ArrayList<Point> list; 
	private Paint p;
	private String message;

	public DrawView(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		this.setClickable(true);
        
		setBackgroundColor(Color.WHITE);    
  
        this.setOnTouchListener(this);
		
		//Make paint object
		 p = new Paint();
		 
		 p.setStrokeWidth(4);
		 
		 p.setColor(Color.BLACK);
		 
		 list = new ArrayList<Point>();
	}
	
	protected void onDraw(Canvas canvas)
	{
		super.onDraw(canvas);
		
		if (canvas != null)
		{
			//Log.i("Ben", "Drawing points from list");
			
			for (int x = 0; x < list.size() - 1; x++)
			{
				//Skip a few calls to drawLine to start a new disconnected line
				if (list.get(x).x < 0 || list.get(x+1).x < 0)
				{
					continue;
				}
				canvas.drawLine(list.get(x).x, list.get(x).y, list.get(x+1).x, list.get(x+1).y, p);
			}
			
		}
		
		
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		if(event.getAction() == MotionEvent.ACTION_DOWN)
		{
			Point point = new Point();
			
	        point.x = (int) event.getX();
	        point.y = (int) event.getY();
	        
	        list.add(point);
	        
		}
		
		if (event.getAction() == MotionEvent.ACTION_MOVE)
		{
			Point temp = new Point();
			
			temp.x = (int) event.getX();
	        temp.y = (int) event.getY();
	        
			list.add(temp);
			
			//Log.i("Ben", "Added" + temp.x + " , " + temp.y);
			
		}
		
		//Add flag point to signal to start of a new line of drawing
		if (event.getAction() == MotionEvent.ACTION_UP)
		{
			Point temp = new Point();
			temp.x = -1;
			temp.y = -1;
			
			list.add(temp);
		}
		
		//Must call to redraw the screen
		invalidate();
		
		return true;
	}
	
	//Clear canvas
	public void clearColors()
	{
		list.clear();
		
		invalidate();
	}
	
	//Change color method
	public void changeColor(int x)
	{
		p.setColor(x);
	}
	
	//Get number of points drawn on canvas
	public int getPointNum()
	{
		return list.size();
	}
	
	//Convert Point coordinates to a string in correct format
	public String toString()
	{
		message = "[";
		
		//Need to start as doubles so division doesn't return 0
		double new_x;
		double new_y;
		
		for (int j = 0; j < list.size(); j++)
		{	
			new_x = (list.get(j).x * (250.0 / getWidth()));
			new_y = (list.get(j).y * (250.0 / getHeight()));
			
			//Log.i("BEN", String.valueOf(new_x));
			//Log.i("BEN", String.valueOf(new_y));
			
			//Signal the end of a stroke
			if (list.get(j).x < 0)
			{
				message += "255, 0";
			}
			else
			{
				message += (String.valueOf((int) new_x) + "," + String.valueOf((int) new_y));
			}
			
			message += ", ";
		}
		
		//Signal end of image
		message += "255, 255]";
		
		return message;
	}
	

}
