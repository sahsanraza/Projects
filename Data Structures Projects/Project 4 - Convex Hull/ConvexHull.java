///Name: Ben Isenberg
///Date: November 7, 2010
///Email: bji6@pitt.edu


/*This class implements the Graham Scan to find the Convex Hull to a set of
randomly generated numbers.  It first uses a method to find the anchor point,
then it finds the angles all other points make with the anchor.  Then it has a
quick sort method to speedily sort the angles in ascending order.  Finally it
has a method that finds the points that are part of the Convex Hull by computing
the direction of the cross product of two vectors.  Only left turns and colinear points
are added to the hull.*/

import java.util.*;
import java.awt.*;



public class ConvexHull{

	//List to hold all the randomly generated points
	private static ArrayList<Point> points;

	//List that holds the points on convex hull
	public static ArrayList<Point> hull;

	//Stack used to find convex hull
	private static Stack<Point> hullStack;

	//Anchor to convex hull
	public static Point anchor;



	//Constructor
	public ConvexHull(ArrayList<Point> p){

		points = new ArrayList<Point>();

		for(int x = 0; x < p.size(); x++){

			points.add(p.get(x));

		}

	}

	//Method that finds anchor which is points with smallest Y and X coordinates
	public void findAnchor(){

		anchor = new Point(points.get(0));

		for(int x = 0; x < points.size(); x++){

			if(points.get(x).getY() <= anchor.getY()){

				if(points.get(x).getY() < anchor.getY()){

					anchor = points.get(x);
				}
				else{
					if(points.get(x).getX() < anchor.getX()){
						anchor = points.get(x);
					}
				}
			}

		}

		//Sets anchor's angle as 0 since it always has the smallest angle
		anchor.setAngle(0);

	}

	//Method that returns list of points
	public static ArrayList<Point> getPoints(){
		return points;
	}

	//Method that displays anchor
	public void displayAnchor(){
		System.out.println("ANCHOR = (" + anchor.getX() + " , " + anchor.getY()   + ")");
	}

	//Method that displays all the points
	public void displayPoints(){

		for(int x = 0; x < points.size(); x++){
			System.out.println("(" + points.get(x).getX() + " , " + points.get(x).getY()   + ")");
		}

	}

	//Method displays angles found
	public void displayAngles(){

		for(int x = 0; x < points.size(); x++){
			System.out.println("(" + points.get(x).getX() + " , " + points.get(x).getY()   + ")" + "ANGLE IS = " + points.get(x).getAngle());
		}

	}




	//Method that finds angles that all points make with the anchor
	public void findAngles(){

		double distance;

		double angle;

		int deltaX, deltaY;

		for(int x = 0; x < points.size(); x++){

			//Uses distance formula to find distance between 2 points
			distance = Math.sqrt(((points.get(x).getX() - anchor.getX())*(points.get(x).getX() - anchor.getX())) +
									((points.get(x).getY() - anchor.getY())*(points.get(x).getY() - anchor.getY())));

			deltaX = points.get(x).getX() - anchor.getX();

			deltaY = points.get(x).getY() - anchor.getY();

			//Assigns the correct angle to the points based on deltaX and deltaY
			if(deltaX == 0 && deltaY == 0){
				angle = 0;
				points.get(x).setAngle(angle);
			}
			else{
				if(deltaX > 0 && deltaY == 0){
					angle = 0;
					points.get(x).setAngle(angle);
				}
				else{
					if(deltaX < 0 && deltaY == 0){
						angle = Math.PI;
						points.get(x).setAngle(angle);
					}
					else{
						if(deltaX > 0){
							angle = Math.acos(deltaX/distance);
							points.get(x).setAngle(angle);
						}
						else{
							if(deltaX < 0){
								angle = Math.acos(deltaX/distance);
								points.get(x).setAngle(angle);
							}
						}
					}
				}
			}

		}


	}






	//Overloaded quickSort
	public static void quickSortAngles(){
		int left = 0;

		int right = (points.size() - 1);

		quickSortAngles(points, left, right);

	}


	//QuickSort
	public static void quickSortAngles(ArrayList<Point> array, int left, int right){


		//While the whole array hasnt been sorted keep the recursion going
		if(right > left){

			int pivotIndex = right;

			//Final location of pivot is found by calling partion
			int newPivotIndex = partition(array, left, right, pivotIndex);

			//QuickSort is called again to recursively partition the left and right sides until every element is sorted
			quickSortAngles(array, left, newPivotIndex-1);
			quickSortAngles(array, newPivotIndex +1, right);

		}


	}

	//Partitions
	public static int partition(ArrayList<Point> array, int left, int right, int pivotIndex){

		int smaller = left;

		Point i;

		//Goes through array and puts all values less than or equal to pivot left of smaller
		for(int x = left; x < right; x++){

			i = array.get(x);

			if(i.getAngle() < array.get(pivotIndex).getAngle()){

				Point temp = array.get(smaller);

				array.set(smaller, i);

				array.set(x, temp);
				smaller++;

			}

		}

		//Swaps pivot and smaller so that pivot is now in final location
		Point temp = array.get(smaller);
		array.set(smaller, array.get(pivotIndex));
		array.set(pivotIndex, temp);

		//returns final location of pivot
		int newPivotIndex = smaller;

		return newPivotIndex;

	}

	//Method that checks whether three points make a left or right turn
	//if number = 0, points are colinear
	//if number > 0, points make a left turn
	//if number < 0, points make a right turn
	public static double isLeft( Point p1, Point p2, Point p3 ){

		double a = (p2.getX() - p1.getX())*(p3.getY() - p1.getY());

		double b = (p2.getY() - p1.getY())*(p3.getX() - p1.getX());

		double number = a - b;

		return number;
	}



	//Uses Grahams scan to find convex hull
	public static void hull(){

		hullStack = new Stack<Point>();
		hull = new ArrayList<Point>();

		//Automatically add first 2 points onto Convex Hull because they are always a part of it
		hull.add(hullStack.push(anchor));
		hull.add(hullStack.push(points.get(1)));


		double angle1;

		//Start loop on the 3rd point
		int index = 2;

		for(int x = index; x < points.size(); x++){

			angle1 = isLeft(hull.get(hull.size()-2), hull.get(hull.size()-1), points.get(x));

			//Only adds left turns to the stack
			if( angle1 >= 0){

				hull.add(hullStack.push(points.get(x)));
			}

			//Removes second point if a left turn is made by connecting 3 points together
			if(angle1 < 0 && hull.size() != 2){

				hull.remove(hullStack.pop());
				x--;
			}

			//Automatically adds last two points onto the Convex Hull since they must be a part of it
			if(x == points.size() - 2){

				hull.add(hullStack.push(points.get(x+1)));
				break;
			}

		}

		//Adds anchor to Convex Hull again to have the hull end where it started
		hull.add(anchor);

	}

	//Displays points on hull
	public static void displayHull(){
		System.out.println("HULL = ");

		for(int x = 0; x < hull.size(); x++){
			System.out.println("(" + hull.get(x).getX() + " , " + hull.get(x).getY()   + ")");
		}

	}






}














































