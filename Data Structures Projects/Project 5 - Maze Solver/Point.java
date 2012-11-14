///Name: Ben Isenberg
///Date: November 18, 2010
///Email: bji6@pitt.edu

/*This class represents a Point on the maze.  Each point keeps tracks of it's row number and
column number in the maze.  It also keeps track of what character symbol it is, how far it
is to the goal, and whether it has been visited yet.*/



public class Point{

	private int rowNum;

	private int colNum;

	private char token;

	private boolean visited;

	private int manhattanDistance;

	//Constructor
	public Point(){
		visited = false;
	}

	//Calculates a point's manhattan distance to goal
	public void distance(int row, int col){

		manhattanDistance = Math.abs(row - this.getRowNum()) + Math.abs(col - this.getColNum());

	}

	//Method sets a point's row number to x
	public void setRowNum(int x){
		rowNum = x;
	}

	//Method sets a point's column number to x
	public void setColNum(int x){
		colNum = x;
	}

	//Method sets a point's symbol to x
	public void setToken(char x){
		token = x;
	}

	//Method sets a point's distance to the goal to d
	public void setManDistance(int d){
		manhattanDistance = d;
	}

	//Method returns a point's distance
	public int getManDistance(){
		return manhattanDistance;
	}

	//Method returns a point's character symbol
	public char getToken(){
		return token;
	}

	//Method returns whether a point has been visited or not
	public boolean getVisit(){
		return visited;
	}

	//Method sets a point to visited
	public void visit(){
		visited = true;
	}

	//Method returns a point's row number
	public int getRowNum(){
		return rowNum;
	}

	//Method returns a point's column number
	public int getColNum(){
		return colNum;
	}




}