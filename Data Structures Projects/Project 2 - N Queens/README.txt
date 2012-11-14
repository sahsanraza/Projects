Name: Ben Isenberg

Email: bji6@pitt.edu

Course: CS 0445 Data Structures

Assignment 2: N-Queens Problem 

Date: October 2, 2010 

Introductory Comments:

	I use one class called Queen which contains all the methods to generate permutations of a set,
display chessboards, and count solutions.  I also measure work by counting the number of recursive
calls are made by the permutations method.  There are two versions of the permutations method.  One 
is called permInt() which checks for diagonal conflicts after all the permutations have been generated.
The other version is called permWithPruning() and this method implements pruning by checking for diagonal 
conflicts before recursion occurs.  I display chessboards that illustrate solutions by creating a 2D Array
of characters and I place the character Q in correct locations in the Array and then I display them on screen.  


Files: Queen.java, Queen.class, TestQueen.java, TestQueen.class

How to run program: 

	To run the program Queen.java and TestQueen.java must be compiled.  I included their class files so that
 you don't have to compile them.  There are many variables (mostly booleans) in the TestQueen.java file that
gives you control on how you want to run the code.  Here is a list of the variable names and what they control:

1. NUMBER_OF_QUEENS - this variable is an integer that represents the chessboard size and the number of queens
			used in the problem.  You can change the number to increase or decrease the problem size.

2. printSolutions - this boolean lets you decide if you want the number of solutions found to be printed on screen.
			Setting it to true means that the number of solutions found is printed on screen.  Setting
			it to false turns off printing of solution number.

3. printRecursion - if this boolean is set to true then the amount of work done is printed on screen.  If it's set
			to false then the amount of work done is not printed.

4. printChessBoards - if this boolean is true then the 2D Arrays of characters representing chessboards with
			queens placed on them is displayed on screen.  Each chessboard is one solution
			to the problem.  Setting this to false turns off printing of chessboards.

5. usePruning - if this boolean is set to true then pruning is used in the permutations method and not as much 
		work is done by the program.  Setting this to false turns off pruning in the permutations method.


Problems:  

	There are no problems with my program.  It runs perfectly.  The only thing is that once you start using 
large numbers like 12 or 14 queens then it takes a couple minuted to find all the solutions since there are so many.
But by calling my permutations method with pruning you can reduce the runtime.

