///Name: Ben Isenberg
///Date: November 18, 2010
///Email: bji6@pitt.edu

/*This class implements the breadth first search algorithm to solve a maze.  It uses a priority queue.
It displays on a graphic the paths it takes to solve the maze.*/


import java.util.*;
import java.io.*;
import java.awt.*;
import javax.swing.*;


public class BreadthFirst {


	private PriorityQueue heap;

	private JFrame graphic;


	//Constructor
	public BreadthFirst(JFrame f){

		heap = new PriorityQueue();
		graphic = f;

	}


	//Breadth First Search
	public void breadthFirstSearch(Maze matrix){

		int rowNumber = 0;

		int colNumber = 0;

		//Find start and add it to queue
		for(int x = 0; x < matrix.getRowLength(); x++){
			for(int y = 0; y < matrix.getColLength(); y++){

				Point s = matrix.get(x, y);
				if(s.getToken() == 'S'){

					heap.insert(s);
					rowNumber = x;
					colNumber = y;
				}
			}
		}

		int gRow = 0;

		int gCol = 0;

		//Find G to determine every point's distance to it
		for(int x = 0; x < matrix.getRowLength(); x++){
			for(int y = 0; y < matrix.getColLength(); y++){

				Point s = matrix.get(x, y);
				if(s.getToken() == 'G'){

					gRow = x;
					gCol = y;
				}
			}
		}

		//Assign every point its distance to G
		for(int x = 0; x < matrix.getRowLength(); x++){
			for(int y = 0; y < matrix.getColLength(); y++){

				Point k = matrix.get(x, y);

				k.distance(gRow, gCol);


			}
		}


		//Arbitrary variable used to keep while loop going
		boolean checker = false;

		do{

			if(heap.isEmpty()){
				System.out.println("NO SOLUTION");
				break;
			}

			Point test = heap.removeWithPriority();

			test.visit();

			//Slowly updates on screen where it has already visited
			sleep(200);

			graphic.repaint();

			//Breaks out of loop when it finds goal
			if(test.getToken() == 'G'){
				System.out.println("SUCESS!");
				break;
			}

			//According to where it is in the maze, it checks up, down, left, and right to determine its options
			Point up = matrix.get(test.getRowNum() - 1, test.getColNum());

			Point down = matrix.get(test.getRowNum() + 1, test.getColNum());

			Point left = matrix.get(test.getRowNum(), test.getColNum() - 1);

			Point right = matrix.get(test.getRowNum(), test.getColNum() + 1);

			//If up is open and it hasn't been there before, add it to the priority queue
			if(up.getToken() != '*' && up.getVisit() == false){

				heap.insert(up);

			}

			//If down is open and it hasn't been there before, add it to the priority queue
			if(down.getToken() != '*' && down.getVisit() == false){

				heap.insert(down);

			}

			//If left is open and it hasn't been there before, add it to the priority queue
			if(left.getToken() != '*' && left.getVisit() == false){

				heap.insert(left);
			}

			//If right is open and it hasn't been there before, add it to the priority queue
			if(right.getToken() != '*' && right.getVisit() == false){

				heap.insert(right);
			}

		}while(checker != true);



	}




  //Method used to slow down drawing on screen
  public static void sleep(long milliseconds) {
    Date d ;
    long start, now ;
    d = new Date() ;
    start = d.getTime() ;
    do { d = new Date() ; now = d.getTime() ; } while ( (now - start) < milliseconds ) ;
  }





}