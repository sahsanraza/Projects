import java.util.*;  //For scanner class
import java.io.*;	//For the file class

//by Ben Isenberg

/* This program asks the user to enter the name of a data file.  Then it reads the data
	and creates objects of the BallPlayer class and sets the object's stats according
	to the data it reads in the file.  Then it compares all the BallPlayer objects and
	finds the one the the highest slugging percentage.  Finally it displays a table of all
	the ballplayer objects with their stats and then displays the name, year, and slugging
	percentage of the ballplayer with the highest slugging percentage. */

public class StatReader {

	public static void main(String[] args)throws IOException {


		//Opens a file to read
		Scanner keyboard = new Scanner(System.in);

		String fileName;

		System.out.println("Enter a filename: ");

		fileName = keyboard.nextLine();

		File file = new File(fileName);

		Scanner inputFile = new Scanner(file);


		//Creates a ballplayer object to display the heading
		BallPlayer x = new BallPlayer("a", "b", 4);


		//Variables used to compare objects
		double bestSlugger = 0;
		String s1 = "a", s2 = "b";

		//Displays heading
		x.heading();

		//Variables used to hold baseball player stats
		String a, b;
		int c, d, e, f, g, h;

			//Loops to create ballplayer objects and display them
			while (inputFile.hasNext()) {

				a = inputFile.next();
				b = inputFile.next();
				c = inputFile.nextInt();
				d = inputFile.nextInt();
				e = inputFile.nextInt();
				f = inputFile.nextInt();
				g = inputFile.nextInt();
				h = inputFile.nextInt();


				//Creates the ballplayer object
				BallPlayer playa = new BallPlayer(a, b, c);

				//Calls all the ballplayer setter methods to set values for its data fields
				playa.setHits(d);

				playa.setDoubles(e);

				playa.setTriples(f);

				playa.setHomeRuns(g);

				playa.setAtBats(h);

				//Calculates batting average
				playa.battingAverage();

				//Calculates slugging percentage
				playa.sluggingPercentage();

				//Variables used to compare objects
				double sluggingPercent = playa.sluggingPercentage();

				String name = playa.printableName();

				int year = playa.getYear();

				String s3 = sluggingPercent + "";

				String s4 = bestSlugger + "";

				double result  = s3.compareTo(s4);
					// Compares baseball player objects to find the one with the highest slugging percentage
				     if (result > 0) {
				        bestSlugger = sluggingPercent;
				        s1 = name + "";
				        s2 = year + "";
					}


				//Displays all baseball players with their stats
				playa.display();


			}

		//Displays the baseball player with the highest slugging percentage
		System.out.printf("\nMax slugger is %s in %s with a slugging percentage %.3f", s1, s2, bestSlugger);

		//Closes file
		inputFile.close();
	}
}