import java.util.*;
import java.io.*;

	/*This program is the class representation of a Finite-state Machine.  It reads
	a data file input as an argument to the class constructor and creates a
	finite-state machine based on the data in the file.  Also there are various methods
	that display the transitions and some of the private data fields.  Finally there is a method
	that processes tapes of alphabet characters put in by the user and displays whether the
	tape is accepted and rejected.*/

	//by Ben Isenberg


public class FSA {

	private String machineName;			//name of machine
	private int numStates;				//number of states
	private String alphabet;			//a list of distinct symbols
	private int[][] transitions;		//numStates-by-length_of_alphabet
	private String tape;				//a string over the alphabet
	private int start;					//start-state
	private boolean[] acceptStates;		//set of accept states



	//Constructor that reads a file
	public FSA(String fileName)throws IOException {

		//Creates a file object
		File file = new File(fileName);

		//Opens file object to read it
		Scanner inputFile = new Scanner(file);

		//Data read from file is assigned to the class's fields
		machineName = inputFile.nextLine();
		numStates = inputFile.nextInt();
		start = inputFile.nextInt();
		alphabet = inputFile.next();

		//2D array is created so that the transitions can be read
		transitions = new int[numStates][alphabet.length()];

		//char array created to translate alphabet into the transitions array
		char[] alpha = new char[alphabet.length()];

		//alphabet added into char array
		for(int index = 0; index < alphabet.length(); index++) {

			alpha[index] = alphabet.charAt(index);
		}



		//Value declared outside of loop because it tells the loop when to stop
		int rowNum = 0;

		//Loops through all the transitions in the file and stops once it reaches the final states in the file
		//Inside the loop the 2D array will be assigned the correct values
		while(rowNum != -1){

			int colNum;
			String s;
			char sym;
			int control = 0;

			//First state in transition
			rowNum = inputFile.nextInt();

			//Used to stop loop when inputFile reads a -1
			if (rowNum == -1) {
				continue;
			}

			//Reads the alphabet char as a string and assigns a char to it
			s = inputFile.next();

			sym = s.charAt(0);

			//Variables used for Sequential Search Algorithm
			int index;
			int element;
			boolean found;

			//Starting values given to variables
			index = 0;
			element = -1;
			found = false;

			//Sequential Search Algorithm used to find the correct row and column to put the next read state from the file
			//into the 2D transitions array
			while(!found && index < alphabet.length()) {
				if (alpha[index] == sym) {

					found = true;
					element = index;
				}
				index++;
			}

			colNum = element;

			//Next read state put in the correct spot in the transitions array
			transitions[rowNum][colNum] = inputFile.nextInt();

		}

		//Creates acceptStates array of size of number of possible states
		acceptStates = new boolean[numStates];

		//Create ArrayList to hold final state values
		//I needed to make a list since you wont know how many final states there might be
		ArrayList<Integer> finalStates = new ArrayList<Integer>();

		//Variable used to hold read integers and for looping purposes
		int fs = inputFile.nextInt();

		//Loops through all final states and adds them to the ArrayList
		while (inputFile.hasNext() && fs != -1) {

			finalStates.add(fs);

			fs = inputFile.nextInt();

		}

		//Reads the final state values in the ArrayList and each element in the acceptStates array that corresponds to
		//that value is set to true in order to represent the final state through the subscript in the acceptStates array
		for (int index = 0; index < finalStates.size(); index++) {
			acceptStates[finalStates.get(index)] = true;
		}

		//Goes through the acceptStates array and sets all subscripts that were not equal to final states to false
		//since they are not final states
		for(int index = 0; index < numStates; index++) {
			if (acceptStates[index]) {
				acceptStates[index] = true;
			}
			else {
				acceptStates[index] = false;
			}

		}

		//Tests acceptStates array
		/*for (int index = 0; index < numStates; index++) {
			System.out.println(acceptStates[index]);
		}*/


			//Test ArrayList values
		/*for (int index = 0; index < finalStates.size(); index++) {
			System.out.println(finalStates.get(index));
		}*/


		//Loop used to display values in transtions for testing
		/*for (int row = 0; row < numStates; row++) {
			for(int col = 0; col < alphabet.length(); col++) {
				System.out.println(transitions[row][col]);
			}
		}*/



	}


		//Display the FSM
	   public void displayMachine(){
			//Print Machine Name
		   System.out.println("Machine Name: " + name());

			//Display set of states
		   System.out.print("Set of states: ");
		   System.out.print( "{");

			//Variable used to display last state with a comma
		   int state = 0;

			//Displays first through next to last state in order with a comma after each state
		   for (int index = 0; index < numStates - 1; index++) {
			   System.out.print(index + ", ");
			   state = index + 1;
		   }

			//Displays last state without comma and goes to next line
		   System.out.print(state + "}\n");

			//Displays start state
		   System.out.println("Start state: " + start());

		   //Displays alphabet
		   System.out.print("Alphabet: ");
		   System.out.print("{");

			//Variable used to display last character without a comma
		   char symb = ' ';

			//Displays first through next to last character in order with a comma after each character in alphabet
		   for (int index = 0; index < alphabet.length() - 1; index++) {

				System.out.print(alphabet.charAt(index) + ", ");
				symb = alphabet.charAt(index + 1);
			}

			////Displays last character without comma and goes to next line
			System.out.print(symb + "}\n");

			//Displays accept states
			System.out.print("Accept States: {");

			int fState = 0;

			for (int index = 0; index < numStates - 1; index++) {
				if (acceptStates[index] && index == 0) {
					System.out.print(index);
				}
				else {
					if (acceptStates[index] && index != 0) {
						System.out.print(", " + index);
					}
				}
				fState = index + 1;
			}

			if (acceptStates[fState]) {
				System.out.print(", " + fState + "}\n");
			}
			else {
				System.out.print("}\n");
			}

			System.out.println();

			//Displays all possible transitions
			System.out.print("Transitions Table: ");

			for (int index = 0; index < numStates; index++) {
			    for (int z = 0; z < alphabet.length(); z++) {

			        System.out.print("\n            (" + index + ", " + alphabet.charAt(z) + ")->" + transitions[index][z]);

			     }
			 }



		}

	   //Return the machineName to caller
	   public String name(){
		   return machineName;
		   }

	   //Return number of states to caller
	   public int numberOfStates(){
		   return numStates;
		   }

	   //Return alphabet back to caller
	   public String alphabet(){
		   return alphabet;
		   }

	   //Return start state back to caller
	   public int start(){
		   return start;
		   }

	   //Return next state if in current state and see symbol
	   public int nextState(int current, char symbol){
		   	//Variables needed to find next state
			int index;
			int element;
			boolean found;
			int rowNumber;
			int colNumber;
			//Create same char array as in constructor
			char[] alpha = new char[alphabet.length()];

			//Assign char array values from alphabet
			for(int x = 0; x < alphabet.length(); x++) {

				alpha[x] = alphabet.charAt(x);
			}

			//Initial values for variables
			index = 0;
			element = -1;
			found = false;

			//Find the index in char array where symbol is and use that as the column index in transitions array
			//to find nextState
			while(!found && index < alphabet.length()) {
				if (alpha[index] == symbol) {

					found = true;
					element = index;
				}
				index++;
			}

			return transitions[current][element];

		   }

	   //Return true or false if k is or is not a final state.
	   public boolean finalState(int k){

			//if acceptStates is true at k then return true otherwise return false
		   if (acceptStates[k] == true) {
			   return true;
		   }
		   else {
			   return false;
		   }

   	  }

	   //Process tape, display transitions and determine if accept or reject tape
	   public void processTape(String tape){

			//Display heading
		   System.out.println("Processing tape: ");

			//Reads tape and displays each character in it
			for(int index = 0; index < tape.length(); index++) {

				char partOfTape = tape.charAt(index);

				System.out.print("  " + partOfTape);
			}

			//Makes a new line
			System.out.print("\n");

			//Prints the start state
			System.out.print(start());

			//Variable used to get transitions
			int state = start();

			//Loops through tape reading it and then displays each transition
			//Calls nextState method to find transitions
			for (int x = 0; x < tape.length(); x++) {

				char partOfTape2 = tape.charAt(x);

				System.out.print("->" + nextState(state, partOfTape2));

				state = nextState(state, partOfTape2);
			}

			//Calls finalState method to see if the tape is accepted or rejected
			if(finalState(state)) {
				System.out.print("\n ACCEPT TAPE");
			}
			else {
				System.out.print("\n REJECT TAPE");
			}



		}






}

