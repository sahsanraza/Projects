import java.util.Random;
import java.io.*;

//by Ben Isenberg

/* This program finds the solution to all the variables in
	this equation: a^5 + b^5 + c^5 + d^5 = e^5
	Then it creates a file called Answer.txt and writes the solutions
	onto the file. */

public class Problem1 {

	public static void main(String[] args) throws IOException { //throws for PrintWriter class

	Random randomInt = new Random(); //To randomly generate numbers

	double f;
	long a, b, c, d, e;
	long a5, b5, c5, d5, e5;

	//Values for a, b, c, and d are randomly generated
	a = randomInt.nextInt(Integer.MAX_VALUE);
	b = randomInt.nextInt(Integer.MAX_VALUE);
	c = randomInt.nextInt(Integer.MAX_VALUE);
	d = randomInt.nextInt(Integer.MAX_VALUE);

	// Method calls to raise the values to the fifth power
	a5 = powFive(a, 5);
	b5 = powFive(b, 5);
	c5 = powFive(c, 5);
	d5 = powFive(d, 5);

	//The original equation
	e5 = a5 + b5 + c5 + d5;

	// Value of e
	e = a + b + c + d;

	//Variable used to continue while loop
	f = e % 1;

	//While e is a negative number and its not a whole number continue looping
	 while ( e < 0.0) {
		while (f > 0.0 && f < 1.0) {

			//Generates new values
			a = randomInt.nextInt(Integer.MAX_VALUE);
			b = randomInt.nextInt(Integer.MAX_VALUE);
			c = randomInt.nextInt(Integer.MAX_VALUE);
			d = randomInt.nextInt(Integer.MAX_VALUE);
			e = a5 + b5 + c5 + d5;
			f = e % 1;

			//Tests to makes sure none of the variables equal one another
			if (a != b || a != c || a != d) {

						}
						else {
							a = randomInt.nextInt(Integer.MAX_VALUE);



								if (b != a || b != c || b != d) {}

						else {
								b = randomInt.nextInt(Integer.MAX_VALUE);


					if (c != a || c != b || c != d) {}


						else {
							c = randomInt.nextInt(Integer.MAX_VALUE);


					if (d != a || d != b || d != c) {}

						else {
								d = randomInt.nextInt(Integer.MAX_VALUE);
								}
								}
							}
						}

			e = a5 + b5 + c5 + d5;
			f = e % 1;
		}
	}


	//Creates a file to write the solutions onto
	PrintWriter outputFile = new PrintWriter("Answer.txt");

	//Writes data onto file
	outputFile.println("e = " + e + "\t a = " + a + "\t b = " + b + "\t c = " + c + "\t d = " + d);

	//Closes the file
	outputFile.close();
}
//Method which calculates the fifth power of a variable and returns that value
public static long powFive(long base, long exp) {
	long x;
	x = 1;
	for(int y = 0; y < exp; y++) {
		x = x*base;}
		return x;
	}


}

