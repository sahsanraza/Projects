///Name: Ben Isenberg
///Date: October 1, 2010
///Email: bji6@pitt.edu



import java.util.*;

/*This class has a permutations method that returns the permutations of a set of
integers in an ArrayList.  It also has methods that allow
me to display what the solutions actually look like on a chessboard by using a 2D Array
of characters.  The class also keeps track of the number of recursive calls it makes and
how many solutions it finds.*/



public class Queen {

	//Variable that represents number of solutions
	private int solutions = 0;

	//Determines whether chessboards are printed on screen
	private boolean printBoard;

	//ArrayList that holds initial set of numbers
	private ArrayList<Integer> number;

	//Variable that represents number of recursive calls were maded in permutations method
	private int recursiveCalls = 0;




	//Constructor
	//The list of integers that you want permutations of are placed in ArrayList number
	public Queen(ArrayList<Integer> a, boolean print){
		number = new ArrayList<Integer>();

		for(int x = 0; x < a.size(); x++) {
			number.add(a.get(x));
		}

		printBoard = print;


	}

	//Method that returns number
	public ArrayList<Integer> getArray(){
		return number;
	}

	//Method that displays number
	public void display(){
		System.out.println(number);
	}

	//Method displays number of solutions found
	public void displaySolutionNumber(){
		System.out.println("NUMBER OF SOLUTIONS IS: " + solutions);
	}

	//Method displays number of recursive calls made
	public void displayRecursion(){
		System.out.println("NUMBER OF RECURSIVE CALLS MADE IS: " + recursiveCalls);
	}



	//Method that removes an element from ArrayList s at index i and returns a new ArrayList without that element
	public ArrayList<Integer> remove(ArrayList<Integer> s, int i) {

		ArrayList<Integer> newNumber = new ArrayList<Integer>();

		for (int x = 0; x < s.size(); x++){
			newNumber.add(s.get(x));
		}

		//Checks to see if s is empty so it doesnt try to remove an item from an empty list
		if(newNumber.size() == 0){
			return newNumber;
		}

		newNumber.remove(i);

		return newNumber;

	}

	//Method creates a new ArrayList that is a copy of p, adds int a to it, then returns it
	public ArrayList<Integer> add(ArrayList<Integer> p, int a) {
		ArrayList<Integer> newP = new ArrayList<Integer>();

		for(int x = 0; x < p.size(); x++){
			newP.add(p.get(x));
		}

		newP.add(a);

		return newP;

	}

		//Permutations method with 1 parameter
	public  void permInt(ArrayList<Integer> set){

		ArrayList<Integer> p = new ArrayList<Integer>(set.size());

		//counts recursive calls
		recursiveCalls++;

		permInt(p, set);


	}

	//Overloaded permutations method that returns permutations of a set of integers
	public void permInt(ArrayList<Integer> p, ArrayList<Integer> set) {

			//Base case
			if(set.size() == 0) {

				//Variable used to signal if there is a diagonal conflict in the permutation
				int diagonalChecker = 0;

					//Compares all row numbers in permutation to find diagonal conflicts
					for(int i = 0; i < p.size(); i++){
						for(int j = i+1; j < p.size(); j++){

							int a = Math.abs(p.get(i) - p.get(j));
							int b = Math.abs(i - j);


							if(a == b){

								diagonalChecker = -1;
								return;
							}

						}

					}

				//if there are no diagonal conflicts print solution
				if(diagonalChecker != -1){

					//counts solutions
					solutions++;

					if(printBoard == true){

						Queen.displayArray(Queen.addToArray(p));
						System.out.println("**********************");
					}

					return;
				}


				return;
			}

			//Recursive Case
			for(int x = 0; x < set.size(); x++){

				permInt(this.add(p, set.get(x)), this.remove(set, x));

				//counts recursive calls
				recursiveCalls++;

			}

	}

	//Same permutations method except with pruning implemented before recursive calls
	public void permWithPruning(ArrayList<Integer> set){

			ArrayList<Integer> p = new ArrayList<Integer>(set.size());

			//counts recursive calls
			recursiveCalls++;

			permWithPruning(p, set);

	}

	//Same permutations method except with pruning implemented before recursive calls
	public void permWithPruning(ArrayList<Integer> p, ArrayList<Integer> set) {



			//Base Case
			if(set.size() == 0) {

				int diagonalChecker = 0;

				//Checks for diagonal conflicts one last time
				for(int i = 0; i < p.size(); i++){

					for(int j = i+1; j < p.size(); j++){

						int a = Math.abs(p.get(i) - p.get(j));
						int b = Math.abs(i - j);


						if(a == b){

							diagonalChecker = -1;
							return;
						}

					}

				}

				//counts solutions
				solutions++;

				if(printBoard == true){

					Queen.displayArray(Queen.addToArray(p));
					System.out.println("**********************");
				}

				return;

			}

			//Recursive Case
			for(int x = 0; x < set.size(); x++){

				//Checks for diagonal conflicts before recursion is made
				int diagonalChecker = 0;

				for(int i = 0; i < p.size(); i++){

					for(int j = i+1; j < p.size(); j++){

						int a = Math.abs(p.get(i) - p.get(j));
						int b = Math.abs(i - j);

						if(a == b){

							diagonalChecker = -1;
							return;
						}

					}

				}

				permWithPruning(this.add(p, set.get(x)), this.remove(set, x));

				//counts recursive calls
				recursiveCalls++;


			}

	}


	//Method that makes a 2D Array of characters representing a chessboard and places Queens according to permutation list
	 public static char[][] addToArray(ArrayList<Integer> permutation) {

		char[][] chessboard = new char[permutation.size()][permutation.size()];

		//Goes through a permutation and places Q in a 2D Array where a queen would be placed
		for(int col = 0; col < permutation.size(); col++){

			for(int row = 0; row < permutation.size(); row++){

				if(permutation.get(col) == row){
					chessboard[row][col] = 'Q';
				}
				else{
					chessboard[row][col] = '-';
				}

			}

		}

		return chessboard;

	}

	//Method that displays Queen placement on chessboard
	public static void displayArray(char[][] chess){


		for(int row = 0; row < chess.length; row++){

			System.out.print("\t");

			for(int x = 0; x < chess.length; x++){
				System.out.print(chess[row][x]);
				System.out.print(" ");
			}
			System.out.println();

		}

}







}