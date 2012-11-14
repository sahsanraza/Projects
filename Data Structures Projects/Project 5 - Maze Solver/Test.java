///Name: Ben Isenberg
///Date: November 18, 2010
///Email: bji6@pitt.edu

/*This is a test file that reads a maze file and displays the maze as a graphic.  It
then runs Breadth First Search with a priority queue to find the goal of the maze.*/


import java.util.*;
import java.io.*;
import java.awt.*;
import javax.swing.*;

public class Test{


public static void main(String[] args) throws IOException{


		//Use scanner to read file name specified by the user
		Scanner keyboard = new Scanner(System.in);

		String fileName;

		System.out.println("Enter a file name to be read: ");

		fileName = keyboard.nextLine();

		File file = new File(fileName);

		Scanner inputFile = new Scanner(file);

		//frontLine is used to find how many columns will be needed in the 2D Array
		String frontLine = inputFile.nextLine();


		inputFile.reset();

		//rowNumber is used to determine how many rows will be needed in the 2D Array
		int rowNumber = 1;

		while(inputFile.hasNextLine()){

			String l = inputFile.nextLine();

			rowNumber++;

		}


		inputFile = new Scanner(file);

		char[][] matrix = new char[rowNumber][frontLine.length() + 1];

		int row = 0;

		int col = 0;

		//Creates a 2D Array of characters which is a copy of the maze in the text file
		while(inputFile.hasNextLine()){

			String line = inputFile.nextLine();

			for(int x = 0; x < line.length(); x++){
				matrix[row][col] = line.charAt(x);
				col++;
			}

			row++;

			col = 0;

		}


		//Creates a maze of points that is a copy of matrix
		Maze m = new Maze(matrix);

		//Creates a graphical display to display the maze
		JFrame frame = new JFrame("Matrix Panel") ;

		MatrixPanel panel = new MatrixPanel(m.getMaze()) ;

		frame.getContentPane().add(panel) ;

		frame.pack() ;

		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    	frame.setVisible(true) ;

		frame.repaint();

		//Breadth First Search is used to solve to maze
		BreadthFirst test = new BreadthFirst(frame);

		test.breadthFirstSearch(m);

		frame.repaint();


	}



}