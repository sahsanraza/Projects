import java.util.Scanner;
import java.util.Random;
import java.io.*;

//by Ben Isenberg

/* This program asks the user to enter the name of a file of data.
	It reads this data then creates new files for each row of data.
	Each new file has listed in it randomly generated coordinates
	to represent a drunk person walking.  At the end of each file is
	the value of the final distance walked by the drunkard. */

	// I included the file WalkData.txt which has the data I used to test the program
	// You can use that one or make a new one and it will still work

public class RandomWalker {

	public static void main(String[] args) throws IOException{  // For the PrintWriter class

		// For the user to enter the amount of steps the drunk person takes
		// n = number of steps taken
		Scanner keyboard = new Scanner(System.in);

		// So the user can enter a file name and the program will read it
		String fileName;

		System.out.println("Enter a filename to be read: ");

		fileName = keyboard.nextLine();

		// The program opens the file that the user entered and reads it
		File file = new File(fileName);

		Scanner inputFile = new Scanner(file);

		int a, b, c;

		// Reads the data
		while (inputFile.hasNext()) {
			a = inputFile.nextInt();
			b = inputFile.nextInt();
			c = inputFile.nextInt();

			// Creates a new file for each line of data read
			PrintWriter outputFile = new PrintWriter("walk_" + c + ".txt");

			int x, y, z, n, k;
					y = b;
					x = a;


					for (n = 0; n < c; n++) {

						// Prints the data
						outputFile.println("(" + x + "," + y + ")\t" + n);

						// Range of 1-4, for each direction he can travel
						z = 1 + (int) (Math.random()*(5-1));

						// Determines which direction he goes
						if (z < 2) {

							y = y+1;
						}
						else {

							if (z > 1 && z < 3) {

								y = y-1; }


							else {

								if (z > 2 && z < 4) {

									x = x + 1; }

								else {

									x = x - 1; }
							}
						}

					}
					// Prints the randomly generated coordinates
					outputFile.println("(" + x + "," + y + ")\t" + c);

					// Calculates and then displays the final distance
					int finalSquare = x*x + y*y;
					outputFile.println("The square of the final distance is: " + (finalSquare));

					// Closes the file
					outputFile.close();
			}

		// Closes the file
		inputFile.close();


}
}