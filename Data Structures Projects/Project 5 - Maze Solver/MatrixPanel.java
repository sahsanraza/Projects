///Name: Ben Isenberg
///Date: November 18, 2010
///Email: bji6@pitt.edu

/*This file builds the Matrix Graphic.  It scans a 2D Array of points and draws the maze
according to the symbol each point has and where its position is in the 2D Array.*/

import java.awt.*;
import javax.swing.*;
import java.util.*;

public class MatrixPanel extends JPanel {

  public char openCharacter = ' ' ;
  public char closedCharacter = '*' ;
  public char start = 'S';
  public char goal = 'G';

  //Colors assigned to different symbols
  public Color startColor = Color.green;
  public Color goalColor = Color.red;
  public Color openColor = Color.orange;
  public Color closedColor = Color.gray;
  public Color visitColor = Color.blue;
  public int boxSize = 30;

  public Point[][] matrix ;
  public int rows ;
  public int columns ;
  public int height ;
  public int width ;

  //Constructor
  public MatrixPanel(Point[][] m) {
    matrix = m ;
    rows = m.length ;
    columns = m[0].length ;
    width = boxSize * columns ;
    height = boxSize * rows ;
    setPreferredSize( new Dimension(width,height) ) ;
  }

  //Method that draws boxes
  public void drawBox(int column, int row, int side, Color color, Graphics g) {
    int centerX = (column*boxSize) + (boxSize/2) ;
    int centerY = (row*boxSize) + (boxSize/2) ;
    int radius = side / 2 ;
    g.setColor(color) ;
    g.fillRect( centerX-radius , centerY-radius , side , side ) ;
  }

  //Method that draws dots
  public void drawDot(int column, int row, int diameter, Color color, Graphics g) {
    int centerX = (column*boxSize) + (boxSize/2) ;
    int centerY = (row*boxSize) + (boxSize/2) ;
    int radius = diameter / 2 ;
    g.setColor(color) ;
    g.fillOval( centerX-radius , centerY-radius , diameter , diameter ) ;
  }

  //Method draws maze on screen
  public void paintComponent(Graphics g) {
    Point current = new Point();

    for (int r = 0 ; r < rows ; r++) {
		for (int c = 0 ; c < columns ; c++) {
    	  	current = matrix[r][c] ;

    	  	if(current.getToken() == openCharacter){
				drawBox(c,r,boxSize,openColor,g);
			}
    	  	if(current.getToken() == closedCharacter){
				drawBox(c,r,boxSize,closedColor,g);
			}
    	  	if(current.getToken() == start){
				drawBox(c, r, boxSize, startColor, g);
			}
    	  	if(current.getToken() == goal){
				drawBox(c, r, boxSize, goalColor, g);
			}
    	}
    }

	//Colors spaces in the maze blue if they have been visited already
	 for (int r = 0 ; r < rows ; r++) {
		for (int c = 0 ; c < columns ; c++) {

			current = matrix[r][c];

			if( current.getVisit()){
				drawDot(c, r, boxSize, visitColor, g);
			}

		}
	 }



  }




}

