///Name: Ben Isenberg
///Date: November 7, 2010
///Email: bji6@pitt.edu


/*This class represents a point on a plane.  Each point has an x and y coordinate
and an angle that it makes with the anchor of the Convex Hull.*/



public class Point{


	private int x;

	private int y;

	private double angle;

	//Constructor
	public Point(int a, int b){
		x = a;
		y = b;


	}

	//Overloaded Constructor
	public Point(Point p){
		x = p.getX();
		y= p.getY();

	}

	//Returns a point's x coordinate
	public int getX(){
		return this.x;

	}

	//Returns a point's y coordinate
	public int getY(){
		return this.y;

	}

	//Sets a point's angle
	public void setAngle(double a){
		angle = a;
	}

	//Returns a point's angle
	public double getAngle(){
		return angle;
	}



}