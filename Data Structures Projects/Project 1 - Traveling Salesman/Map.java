
///Name: Ben Isenberg
///Date: September 26, 2010
///Email: bji6@pitt.edu



/*This class draws a graphic on a window that shows the tour and cities in the tour on screen.
It also draws all the changes made to the tour.  It has data fields that represent
the color of the shapes drawn, the color of the background plane, the width and heigth of the window,
shape dimensions, and a tour.  This class is based on PaintPanel*/


import java.awt.* ;
import javax.swing.* ;
import java.util.*;

public class Map extends JPanel {

  public Color BACKGROUND = Color.blue ;
  public Color FOREGROUND = Color.red ;

  private int width, height ;
  private int box_x, box_y, box_width, box_height ;
  private int oval_x, oval_y, oval_width, oval_height ;
  private int line_x, line_y, line_xx, line_yy ;

//This tour is refrenced to draw the correct city locations on screen
  private Tour t;

	//Constructor
  public Map(int w, int h, Tour tour) {
    this.width = w ;
    this.height = h ;
    setPreferredSize(new Dimension(w,h)) ;
    t = tour;
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

	//Method that draws cities on a window and the lines connecting them
  public void paintComponent(Graphics g) {
    super.paintComponent(g) ;
    g.setColor(BACKGROUND) ;
    g.fillRect(0,0,width,height) ;
    g.setColor(FOREGROUND) ;

	//Two arrays are used so that when the tour is changed the lines are redrawn but the cities stay in the same spot
	ArrayList<City> ovalList = t.getOvalTour();

	ArrayList<City> lineList = t.getLineTour();

	for(int x = 0; x < ovalList.size()-1; x++){
	  	oval(t.getOvalCity(x).getXCoord(), t.getOvalCity(x).getYCoord(), 3, 3);
	  	g.fillOval( oval_x, oval_y, oval_width, oval_height) ;

	}


	for(int x = 0; x < lineList.size()-1; x++) {
		line(t.getLineCity(x).getXCoord(), t.getLineCity(x).getYCoord(), t.getLineCity(x + 1).getXCoord(), t.getLineCity(x +1).getYCoord());

		g.drawLine( line_x, line_y, line_xx, line_yy) ;

	}

  }


	//This method can be used to slow down how fast shapes are repainted
	public static void sleep(long milliseconds) {
	    Date d ;
	    long start, now ;
	    d = new Date() ;
	    start = d.getTime() ;
	    do { d = new Date() ; now = d.getTime() ; } while ( (now - start) < milliseconds ) ;
  }



}
