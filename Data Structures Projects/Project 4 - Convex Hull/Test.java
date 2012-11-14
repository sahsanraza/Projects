///Name: Ben Isenberg
///Date: November 7, 2010
///Email: bji6@pitt.edu

/*This is a test file that demonstrates the Graham scan to find a Convex Hull of
a set of random points.  To change the number of points generated on screen,
just set the variable POINTS to your desired number of points.*/


import java.util.*;
import java.awt.* ;
import javax.swing.* ;




public class Test{

  public static int WIDTH = 500;
  public static int HEIGHT = 500;
  public static int POINTS = 100;


	public static void main(String[] args) {

	    // Create random set of points:
	    Random r = new Random() ;

	    ArrayList<Point> points = new ArrayList<Point>() ;

		//Add random points to array
	    for (int p = 0 ; p < POINTS ; p++) {
	      points.add(new Point(r.nextInt(WIDTH),r.nextInt(HEIGHT))) ;
	    }

		//Send array of points to ConvexHull class
		ConvexHull test = new ConvexHull(points);

		//Finds anchor from random points
		test.findAnchor();

		//Finds angles the anchor makes with all other points
		test.findAngles();

		//Sorts the angles in ascending order
		test.quickSortAngles();

		//Creates window to display Convex Hull
		JFrame frame = new JFrame("Convex Hull") ;

		Graph map = new Graph(WIDTH,HEIGHT, test) ;

		frame.getContentPane().add(map) ;

		frame.pack() ;

		frame.setVisible(true) ;

		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		map.repaint();

		//Finds Convex Hull from points
		test.hull();

		//Updates window with convex hull
		map.repaint();




	}





}