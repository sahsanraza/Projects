
///Name: Ben Isenberg
///Date: September 26, 2010
///Email: bji6@pitt.edu



import java.util.*;
import java.awt.* ;
import javax.swing.* ;

//This program tests Tour, City, and PaintPanel 2 classes

public class Test {

	//Variables used to represent the size of the window, number of cities in the tour, and how fast lines are drawn on screen
	public static int WIDTH = 500, HEIGHT = 500, CITIES = 100, WAIT = 3000, PAUSE = 100 ;

	public static void main(String[] args) {


		/// Create a JFrame to show tours:
		JFrame frame = new JFrame("Search for 2-Optimal Tour") ;


		Tour tour = new Tour() ;

		//Random numbers are used as coordinates of the cities in the tour
		Random r = new Random() ;

		//Starting city is created and add to Tour
		City start = new City("start",0,0) ;

		tour.addOval(start) ;
		tour.addLine(start);

		//Randomly generated cities are added into the Tour
		for (int c = 1 ; c <= CITIES ; c++) {
			int width1 = r.nextInt(WIDTH);
			int height1 = r.nextInt(HEIGHT);

			tour.addOval(new City("x", width1, height1)) ;
			tour.addLine(new City("x", width1, height1));
		}

		//The starting city is also the last destination so it is added to the end of the Tour
		tour.addOval(start);
		tour.addLine(start);


		//Map is created to show the Tour and 2-exchanges
		Map map = new Map(WIDTH,HEIGHT, tour) ;

		frame.getContentPane().add(map) ;

		frame.pack() ;

    	frame.setVisible(true) ;

    	frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		map.repaint();


		//Loop that calls 2-exhange until there aren't any left
		while(tour != null) {

			tour = tour.improve(map);

		    map.repaint() ;
		    sleep(PAUSE) ;
   		 }




	}


	//Method used to slow down lines being redrawn on screen
	public static void sleep(long milliseconds) {
	    Date d ;
	    long start, now ;
	    d = new Date() ;
	    start = d.getTime() ;
	    do { d = new Date() ; now = d.getTime() ; } while ( (now - start) < milliseconds ) ;
  }

}