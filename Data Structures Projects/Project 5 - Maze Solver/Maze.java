///Name: Ben Isenberg
///Date: November 18, 2010
///Email: bji6@pitt.edu

/*This class represents a Maze of points.  It converts the maze of characters read by the
Test program into a maze of points.  Each point is assigned the corresponding character.
I use 2D Arrays to represent mazes.*/



import java.util.*;


public class Maze{

	private Point[][] maze;

	private Point p;


	//Constructor
	public Maze(char[][] matrix){


		maze = new Point[matrix.length][matrix[0].length];

		//Converts a maze of characters into a maze of points
		for(int x = 0; x < matrix.length; x++){

			for(int y = 0; y < matrix[x].length; y++){

				p = new Point();

				p.setRowNum(x);
				p.setColNum(y);

				p.setToken(matrix[x][y]);

				maze[x][y] = p;


			}
		}


	}

	//Method returns the point in the maze at position (x,y)
	public Point get(int x, int y){
		return maze[x][y];
	}

	//Method returns the row length of the 2D Array
	public int getRowLength(){
		return maze.length;
	}

	//Method returns the column length of the 2D Array
	public int getColLength(){
		return maze[0].length;
	}

	//Method returns the maze of points
	public Point[][] getMaze(){
		return maze;
	}

	//Method displays the symbols of every point in the maze in the right order
	public void displayMaze(){
		for(int x = 0; x < maze.length; x++){

			for(int y = 0; y < maze[x].length; y++){
				System.out.print(maze[x][y].getToken());
			}

			System.out.println();
		}


	}




}