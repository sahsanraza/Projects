Name: Ben Isenberg

Email: bji6@pitt.edu

Course: CS 0445 Data Structures

Assignment 5: Maze Runner

Date: November 19, 2010

Introductory Comments:
	
	This project is based on the breadth first search algorithm.  My code uses a Priority Queue, implemented
as a Heap, to run the breadth first search algorithm.  It uses this algorithm to solve mazes.  The mazes are
represented as text files.  Priority in the queue is based on manhattan distance to the goal.  That is how it
determines what moves to make.  The mazes are graphically displayed and you can watch where it goes through
the maze to solve it.  If it successfully finds the goal the program prints "Success!" and if there is no path
to the goal it prints "No Solution."  
  

Files: 
	Point.java, Point.class, Maze.java, Maze.class, MatrixPanel.java, MatrixPanel.class, PriorityQueue.java,
PriorityQueue.class, BreadthFirst.java, BreadthFirst.class, Test.java, Test.class, maze1.txt, maze2.txt,
maze3.txt

How to run program: 

	To run the program you need to run Test.class.  This is my driver.  When you run the file
it asks you to enter the file name of a maze file.  I have provided the three maze text files that Dr. Aronis
gave us to use.  So you just enter in the name of one of those files and then the program works to solve it and
displays it on screen.


Problems:  
	
	I have had no problems with my code.  It all works perfectly for me.


