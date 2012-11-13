import java.util.Scanner;
import java.util.Random;
import java.text.DecimalFormat;

// by Ben Isenberg

/* This program is written to randomly generate places
   on the coordinate plane where a specified number of darts
   would land on a dart board.  The board is a square with a cirle inside it.
   The ratio to the amount of darts thrown with the amount that hit inside the circle
   is equivalent to Pi/4.  This program proves this argument. */

public class Problem2 {

    public static void main(String[] args){

		// For user input
        Scanner keyboard = new Scanner(System.in);

		// Pi is displayed with 10 decimal places
        DecimalFormat formatter = new DecimalFormat("#0.0000000000");

        int k;
        double numInCircle = 0;
        int numOfDarts;
        double x, y;
        double u = -1.0;
        double v = 1.0;

		// Asks user how many darts are thrown
        System.out.println("How many darts will you throw? ");
        numOfDarts = keyboard.nextInt();

		// Randomly generates x and y coordinates for every dart thrown
        for (k=1; k < numOfDarts; k++){

            // The coordinates are generated in the range of -1.0 to 1.0
            x = -1.0 + (double) (Math.random()*(1.0-(-1.0)));
            y = -1.0 + (double) (Math.random()*(1.0-(-1.0)));

			// Tests to see how many darts hit inside the circle
            if(Math.sqrt((x*x) + (y*y)) <= 1.0){
                numInCircle = numInCircle + 1;
            }
        }

		// Calculates Pi with the given data
        double Pi = (numInCircle/numOfDarts)*4;

		// Displays results
		// Pi with 10 decimal places
        System.out.println("numOfDarts = " + numOfDarts);
        System.out.println(" Pi = " + (formatter.format(Pi)));
    }
}