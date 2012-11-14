///Name: Ben Isenberg
///Date: October 1, 2010
///Email: bji6@pitt.edu




import java.util.*;

/*This is a test file that demonstrates my permutations methods.  It solves the N-Queens problem by finding permutations
of row numbers. It has variables that can be used to easily turn off printing, pruning, displaying of chessboards.*/


public class TestQueen {

  public static void main(String[] args) {

	//Integer that represents number of queens
	//Change this number to change the board size and number of queens
	int NUMBER_OF_QUEENS = 6;

	//if printSolutions is true then the number of solutions is printed on screen
	boolean printSolutions = true;

	//if printRecursion is true then the number of recursive calls is printed on screen
	boolean printRecursion = true;

	//if printChessBoards is true then solutions of chessboards with queens on them is printed
	boolean printChessBoards = false;

	//if usePruning is true then pruning is done in the permutation method
	boolean usePruning = true;

	//List that holds first permutation
	ArrayList<Integer> test = new ArrayList<Integer>();



	//specified number of queens is added to the ArrayList
	for(int x = 0; x < NUMBER_OF_QUEENS; x++){
		test.add(x);
	}

	//Queen object is created to use its methods
	Queen y = new Queen(test, printChessBoards);


	System.out.println("Number of Queens is: " + NUMBER_OF_QUEENS);
	System.out.println("****************************");


	if(usePruning == false){

		y.permInt(test);

	}

	if(usePruning == true){

		y.permWithPruning(test);

	}


	if(printRecursion == true){

		y.displayRecursion();
	}

	if(printSolutions == true){

		y.displaySolutionNumber();
	}



 }




}