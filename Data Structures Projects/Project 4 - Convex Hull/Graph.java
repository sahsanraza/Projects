
///Name: Ben Isenberg
///Date: November 7, 2010
///Email: bji6@pitt.edu



/*This class draws a graphic on a window that shows the Convex Hull of points.
It has data fields that represent the color of the shapes drawn, the color of
the background plane, the width and heigth of the window, shape dimensions,
and a convex hull.  This class is based on PaintPanel*/


import java.awt.* ;
import javax.swing.* ;
import java.util.*;

public class Graph extends JPanel {

  public Color BACKGROUND = Color.cyan ;
  public Color FOREGROUND = Color.magenta ;

  private int width, height ;
  private int box_x, box_y, box_width, box_height ;
  private int oval_x, oval_y, oval_width, oval_height ;
  private int line_x, line_y, line_xx, line_yy ;

//This class is refrenced to draw points and lines on screen
  private ConvexHull hull;

	//Constructor
  public Graph(int w, int h, ConvexHull hull) {
    this.width = w ;
    this.height = h ;
    setPreferredSize(new Dimension(w,h)) ;
    hull = hull;
  }

	//Method that creates a box
  public void box(int x, int y, int w, int h) {
    box_x = x ;
    box_y = y ;
    box_width = w ;
    box_height = h ;
  }

	//Method that creates an oval
  public void oval(int x, int y, int w, int h) {
    oval_x = x ;
    oval_y = y ;
    oval_width = w ;
    oval_height = h ;
  }

	//Method that creates a line
  public void line(int x, int y, int xx, int yy) {
    line_x = x ;
    line_y = y ;
    line_xx = xx ;
    line_yy = yy ;
  }

	//Method that draws points on a window and the lines connecting them to make a convex hull
  public void paintComponent(Graphics g) {
    super.paintComponent(g) ;
    g.setColor(BACKGROUND) ;
    g.fillRect(0,0,width,height) ;
    g.setColor(FOREGROUND) ;

	//Array with all the randomized points
	ArrayList<Point> points = hull.getPoints();

	//Draws points on screen
	for(int x = 0; x < points.size(); x++){

	  	oval(points.get(x).getX(), points.get(x).getY(), 7, 7);
	  	g.fillOval( oval_x, oval_y, oval_width, oval_height) ;

	}


	//Draws the lines that make the convex hull
	for(int x = 0; x < hull.hull.size()-1; x++) {
		line(hull.hull.get(x).getX(), hull.hull.get(x).getY(), hull.hull.get(x + 1).getX(), hull.hull.get(x +1).getY());
		g.drawLine( line_x, line_y, line_xx, line_yy) ;

	}




  }



}
