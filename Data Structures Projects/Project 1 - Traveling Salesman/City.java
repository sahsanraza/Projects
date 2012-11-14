
///Name: Ben Isenberg
///Date: September 26, 2010
///Email: bji6@pitt.edu



/*City class that represents a city in a tour.
Has a String name as an identifier.
Also has x and y coordinates.*/



public class City {

	private int x_coord;
	private int y_coord;
	private String identifier;

	//Constructor
	public City(String name, int x, int y) {
		this.identifier = name;
		this.x_coord = x;
		this.y_coord = y;

	}

	//This method returns a City's identifier
	public String getName() {
		return identifier;
	}


	//This method returns a City's x-coordinate
	public int getXCoord() {
		return x_coord;
	}

	//This method sets a City's x-coordinate to x
	public void setXCoord(int x) {
		x_coord = x;
	}

	//This method returns a City's y-coordinate
	public int getYCoord() {
		return y_coord;
	}

	//This method sets a City's y-coordinate to y
	public void setYCoord(int y) {
		y_coord = y;
	}

	//This method finds the distance between two points and displays it.
	public static double distance(City a, City b) {
		return Math.sqrt(((b.getXCoord() - a.getXCoord())*(b.getXCoord() - a.getXCoord())) + ((b.getYCoord() - a.getYCoord())*(b.getYCoord() - a.getYCoord())));
	}

	//This method displays the coordinates of a city
	public String toString() {
		String b = this.getXCoord() + "," + this.getYCoord();

		return b;
	}




}