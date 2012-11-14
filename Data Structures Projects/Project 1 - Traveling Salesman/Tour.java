
///Name: Ben Isenberg
///Date: September 26, 2010
///Email: bji6@pitt.edu


import java.util.*;
import java.lang.*;

/*This class represents a tour.  It has two ArrayLists as data fields.  One is used to keep
the original city order so the ovals can be drawn on screen and they won't move from their places
after a 2-exchange.  The second ArrayList is used for 2-exchange and draws the lines on screen.*/



public class Tour {

	private ArrayList<City> tourOval;

	private ArrayList<City> tourLine;



	//Constructor
	public Tour() {

		tourOval = new ArrayList<City>();
		tourLine = new ArrayList<City>();


	}



	//This method returns the ArrayList used to draw ovals with
	public ArrayList getOvalTour() {
		return tourOval;
	}

	//This method returns the ArrayList used for 2-exchanges with lines
	public ArrayList getLineTour() {
		return tourLine;
	}

	//This method returns the index of City a in the oval ArrayList
	public int getOvalCityIndex(City a) {
		return tourOval.indexOf(a);
	}

	//This method returns the index of City a in the line ArrayList
	public int getLineCityIndex(City a) {
		return tourLine.indexOf(a);
	}

	//This method returns the city of specified index
	public City getOvalCity(int index){
		return tourOval.get(index);
	}

	//This method returns the city at the specified index
	public City getLineCity(int index) {
		return tourLine.get(index);
	}

	//Method adds a city to the oval ArrayList
	public void addOval(City c) {
		tourOval.add(c);


	}

	//Method adds a city to the line ArrayList
	public void addLine(City c) {
		tourLine.add(c);
	}


	//Displays the locations of the cities in the oval ArrayList
	public void displayOval() {
		for(int x = 0; x < tourOval.size(); x++){
			System.out.println("(" + tourOval.get(x).getXCoord() + "," + tourOval.get(x).getYCoord() + ")");
		}
	}

	//Displays the locations of the cities in the line ArrayList
	public void displayLine() {
		for(int x = 0; x < tourLine.size(); x++){
			System.out.println("(" + tourLine.get(x).getXCoord() + "," + tourLine.get(x).getYCoord() + ")");
		}

	}



	//Method that compares distances between a 2-exchange and the original tour length
	public double delta(int i, int j){


		double a = City.distance(this.getLineCity(i), this.getLineCity(i+1));

		double b = City.distance(this.getLineCity(j), this.getLineCity(j+1));


		double c = City.distance(this.getLineCity(i), this.getLineCity(j));

		double d = City.distance(this.getLineCity(i+1), this.getLineCity(j+1));

		//This is used to prevent a line from comparing itself
		if(a == 0 || b == 0 || c == 0 || d == 0){
			return -1;
		}

		//if a positive number is returned then a 2-exchange should occur
		return ((a+b) - (c+d));




	}



	//This method performs a 2-exchange on City x and City y
	public Tour twoExchange(City x, City y) {


		double yesOrNo = delta(tourLine.indexOf(x), tourLine.indexOf(y));


		//If yesOrNo is positive than a 2-exchange should occur
		if(yesOrNo > 0) {


			int indexX = tourLine.indexOf(x);

			City temp = tourLine.get(indexX + 1);

			int indexY = tourLine.indexOf(y);

			tourLine.set(tourLine.indexOf(x) + 1, y);
			tourLine.set(indexY, temp);


			return this;


		}

		return this;

	}


	//This method calls twoExchange() on every city in the tour
	//This method returns null when every city has been compared for a 2-exchange to end the while loop in the test file
	public Tour improve(Map map){

		for(int f = 0; f < this.getLineTour().size()-1; f++){

			for(int z = f; z < this.getLineTour().size()-1; z++){

				this.twoExchange(this.getLineCity(f), this.getLineCity(z));

				map.repaint();
				sleep(10);
			}


		}

		return null;

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