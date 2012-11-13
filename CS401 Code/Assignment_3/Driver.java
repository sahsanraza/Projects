import java.util.*;
import java.io.*;

/*This program is a driver that asks the user questions through the command prompt
and tests any finite-state machine by reading a text file input by the user.*/

//by Ben Isenberg


public class Driver {

	public static void main(String[] args)throws IOException {

		//Displays title of driver
		System.out.println("Finite-state Machine Tool\n");

		//Scanner created to read user input
		Scanner keyboard = new Scanner(System.in);

		//stringInput reads reader input and input1/input2 read the first character of the string
		String stringInput;
		char input1, input2;

		//User asked to run machine
		System.out.print("Do you want to run a finite-state machine? (y or n)--> ");

		stringInput = keyboard.next();

		input1 = stringInput.charAt(0);



		//Driver runs as long as user wants to run a machine
		while (input1 == 'y') {

			if (input1 == 'n') {
				System.exit(0);
			}
			else {
				System.out.print("Enter file where finite-state machine is located--> ");

				//Creates a FSA object by reading the data file that the user enters
				stringInput = keyboard.next();

				FSA x = new FSA(stringInput);



				System.out.println(x.name() + " has been initialized\n");

				System.out.print("Do you want to display this machine? ");

				stringInput = keyboard.next();

				input2 = stringInput.charAt(0);


				//machine displayed if user types 'y'
				if (input2 == 'y') {
					x.displayMachine();
				}

				System.out.println();

				System.out.print("Do you want to process a tape? ");

				stringInput = keyboard.next();

				input2 = stringInput.charAt(0);


				//user enters a tape and its sent to the processTape method to be read
				while (input2 == 'y') {
					System.out.print("Enter the tape from the keyboard--> ");

					stringInput = keyboard.next();

					System.out.println();

					x.processTape(stringInput);

					System.out.println();
					System.out.println();
					System.out.print("Do you want to process a tape? ");

					stringInput = keyboard.next();

					input2 = stringInput.charAt(0);



				}

			}

			//User is asked if he wants to run another machine
			//if the user answers yes than the loop repeats itself
			System.out.print("Do you want to run a finite-state machine? (y or n)--> ");

			stringInput = keyboard.next();

			input1 = stringInput.charAt(0);



		}


	}


}