import java.util.*;
import java.io.*;

//by Ben Isenberg

/* This code creates the BallPlayer class.  The class has 8 private data fields which
	hold: the first name, last name, year, hits, doubles, triples, homeruns, and at bats
	of the player for the specified year.  Also their are methods in the class that calculate
	the player's singles, batting average, and slugging percentage.  There is a method that
	manipulates the players name and lists it in the form of "last name, first initial."
	There is a method that displays the heading of a table of data.  Finally I included a
	method that displays all the ballplayer data in the correct format.
	*/

public class BallPlayer
{
	private String fname;  //Data fields for the class
	private String lname;
	private int year;
	private int hits;
	private int doubles;
	private int triples;
	private int homeRuns;
	private int atBats;


	public BallPlayer(String firstName, String lastName, int yr)  //Constructor
	{
		fname = firstName;
		lname = lastName;
		year = yr;

	}


	public void setHits(int h)		//Sets hits
	{
		hits = h;

	}


	public void setName(String firstName, String lastName)  	//Sets first and last name
	{
		fname = firstName;
		lname = lastName;

	}


	public void setYear(int yr)			//Sets year
	{
		year = yr;
	}

	public void setDoubles(int d)			//Sets doubles
	{
		doubles = d;
	}

	public void setTriples(int t)			//Sets triples
	{
		triples = t;
	}

	public void setHomeRuns(int hr)			//Sets homeruns
	{
		homeRuns = hr;
	}

	public void setAtBats(int ab)			//Sets at bats
	{
		atBats = ab;
	}



	public int getYear()				//Gets year
	{
		return year;
	}

	public String getFName()			//Gets the first name
	{
		return fname;
	}

	public String getLName()			//Gets last name
	{
		return lname;
	}

	public int getHits()			//Get hits
	{
		return hits;
	}

	public int getDoubles()    //Gets doubles
	{
		return doubles;
	}

	public int getTriples()				//Get triples
	{
		return triples;
	}

	public int getHomeRuns()			//Get home runs
	{
		return homeRuns;
	}

	public int getAtBats()				//Get at bats
	{
		return atBats;
	}

	public int singles()				//Method that computes and returns singles
	{
		return (hits - doubles - triples - homeRuns);
	}

	public double battingAverage()			//computes batting average and returns it
	{
		double h = hits;
		double ab = atBats;
		double ba = h/ab;
		return ba;
	}

	public double sluggingPercentage()			//computes slugging percentage and returns it
	{
		int singles = (hits - doubles - triples - homeRuns);
		double sp = (singles + (2*doubles) + (3*triples) + (4*homeRuns));

		return sp/atBats;
	}

	public String printableName()		//Method that makes name printable
	{
		char letter;

		letter = fname.charAt(0);

		return lname + ", " + letter + ".";

	}


	public static void heading()		//Displays heading
	{
		String p = "Player";
		String y = "YR";
		String ab = "AB";
		String s = "S";
		String d = "D";
		String t = "T";
		String hr = "HR";
		String ba = "BA";
		String sp = "SP";
		System.out.printf("%-15s %4s %4s %4s %4s %4s %4s %6s %6s\n", p, y, ab, s, d, t, hr, ba, sp);
	}


	public void display()		//Displays all the ball player stats
	{


		System.out.printf("%-15s %5d %4d %4d", printableName(), getYear(), getAtBats(), singles());
		System.out.printf("%5d %3d %4d", getDoubles(), getTriples(), getHomeRuns());
		System.out.printf("%8.3f %8.3f\n", battingAverage(), sluggingPercentage());

	}








}
