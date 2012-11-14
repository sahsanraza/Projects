
/*Assignment 1 CS 1566 by Ben Isenberg 9/6/12
This project does 2D animation with circles and rectangles.  They show up on screen whereever one left clicks with the mouse.
A simulation of gravity makes the objects fall to the bottom of the screen.  They bounce up and down and slow down until they stop moving.
Collison detection was added between objects that bouncing ontop of each other.
*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include "BenIsenberg-Project1.h"

//Define Pi
#define PI 3.14159265


// define and initialize some global variables 
double my_x = 0, my_y = 0; // current position 
int window_w = 400, window_h = 300; // window height and width 

//For alternating between displaying rectangles and circles
int click_counter = 0;

//Particle structure
struct Particle
{
	//x and y coordinates of particle
	double x;
	double y;
	//velocity acting on particle
	double velocity;
	// force
	double force;
	//mass
	double mass;
};

//Shape structure
struct Shape 
{
	double curr_x;  //current x coordinate of center of shape
	double curr_y;  //current y coordinate of center of shape
	int is_circle;  // is it a circle or rectangle?
	Particle* p_ptr;  //pointer to array of particles used for physics simulation of object
	
	// for animation realism
	double old_vel2; 
	double old_vel;
	double difference;
};

//array size (how many shapes are allowed to exist on screen)
const int SIZE = 25;

const int ANIMATION_SPEED = 30;  //in milliseconds; 1/1000 of a second

//Array of Shape objects
Shape list_of_shapes[SIZE];

//current time change
double current;


//Typical OpenGL/GLUT Main function  
int main(int argc, char **argv) { // program arguments 

  //initialize shape info
  for (int x = 0; x < SIZE; x++)
  {
	//even shapes are rectangles, odd shapes are circles
	if (x % 2 == 0)
		list_of_shapes[x].is_circle = 0;
	else
		list_of_shapes[x].is_circle = 1;
	
	list_of_shapes[x].curr_x = 0;
	list_of_shapes[x].curr_y = 0;
	list_of_shapes[x].p_ptr = NULL;
  }

  // initialize GLUT and OpenGL; Must be called first 
  glutInit( &argc, argv ) ;
  
  // our own initializations; we'll define these setup procedures 
  glut_setup() ;  
  gl_setup() ;
  my_setup();

  // turn control over to GLUT 
  glutMainLoop() ;

  //Free memory
  for (int x = 0; x < 25; x++)
  {
	  if (list_of_shapes[x].p_ptr != NULL)
	  {
		  delete [] list_of_shapes[x].p_ptr;
	  }
  }

  return(0) ; // make the compiler happy 
}



// Typical function to set up GLUT
void glut_setup(void) {

  // specify display mode -- here we ask for double buffering and RGB coloring 
  glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB);

  // make a window of size window_w by window_h; title of "GLUT Basic Interaction" placed at top left corner 
  glutInitWindowSize(window_w, window_h);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Ben Isenberg Project 1");

 // initialize typical callback functions 
  glutDisplayFunc( my_display );
  glutReshapeFunc( my_reshape ); 
  glutIdleFunc( my_idle );
  glutMouseFunc( my_mouse );	
  glutKeyboardFunc( my_keyboard );	
  glutTimerFunc( ANIMATION_SPEED, my_TimeOut, 0);// schedule a my_TimeOut call with the ID 0 in 20 milliseconds from now 

  return ;
}

// Typical function to set up OpenGL 
// For now, ignore the contents of function 
void gl_setup(void) {

  // specifies a background color: black in this case 
  glClearColor(0,0,0,0) ;

  // setup for simple 2d projection 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  // map unit square to viewport window 
  gluOrtho2D(0.0, 1.0, 0.0, 1.0); 

  return ;
}


void my_idle(void) {
  return ;
}


void my_setup(void) {
  return;
}




// drawing function 
void my_display(void) {

  // clear the buffer 
  glClear(GL_COLOR_BUFFER_BIT) ;

  // buffer is ready; show me the money! 

  glutSwapBuffers();
	
  return ;
}





// called if resize event (i.e., when we create the window and later whenever we resize the window)  
void my_reshape(int w, int h) {
  // define the viewport to be the entire window 
  glViewport (0, 0, w, h); 
  window_w = w;
  window_h = h;
  return;
}





// called if mouse event (i.e., mouse click) 
void my_mouse(int b, int s, int x, int y) {

  switch (b)            // b indicates the button 
  {
  case GLUT_LEFT_BUTTON:
    if (s == GLUT_DOWN)   // button pressed 
	{   
      //printf("Left button pressed at %d %d\n", x, y);  //DEBUG
     
	  my_x = x; 
	  my_y = window_h - y;  //Offset y coordinate from mouse so that it will translate to coordinate system used by my shapes

	  click_counter += 1;  //Even clicks for rectangles, odd clicks for circles
	   
	  //Only allow 25 objects on screen
	  if (click_counter == 26)
	  {
	     click_counter = 1;
      }

	  //printf("Click #%d\n", click_counter); //DEBUG

	  //Allocate memory if need be
	  if (list_of_shapes[click_counter-1].p_ptr == NULL)
	  {
		  //printf("MEMORY ALLOCATION!!!\n");  //DEBUG
		  if (click_counter % 2 == 1)
		  {
			  list_of_shapes[click_counter-1].p_ptr = new Particle[4];  //4 particles for rectangle object
		  }
		  else
		  {
			  list_of_shapes[click_counter-1].p_ptr = new Particle[12];  //12 particles for circle object
		  }
	  }

	  if (list_of_shapes[click_counter-1].p_ptr != NULL && click_counter % 2 == 1)
	  {
		  //Set rectangle coordinates based on mouse click
		  //Length and width will be 10 so technically it is a square
		  list_of_shapes[click_counter-1].p_ptr[0].x = my_x/window_w;
		  
		  list_of_shapes[click_counter-1].p_ptr[0].y = my_y/window_h;

		  list_of_shapes[click_counter-1].p_ptr[1].x = (my_x + 10)/window_w;

		  list_of_shapes[click_counter-1].p_ptr[1].y = my_y/window_h;

		  list_of_shapes[click_counter-1].p_ptr[2].x = (my_x + 10)/window_w;

		  list_of_shapes[click_counter-1].p_ptr[2].y = (my_y - 10)/window_h; 

		  list_of_shapes[click_counter-1].p_ptr[3].x = my_x/window_w;

		  list_of_shapes[click_counter-1].p_ptr[3].y = (my_y - 10)/window_h;

		  //Get center of rectangle for collision detection
		  list_of_shapes[click_counter-1].curr_x = (list_of_shapes[click_counter-1].p_ptr[2].x + list_of_shapes[click_counter-1].p_ptr[0].x)/2.0;
		  list_of_shapes[click_counter-1].curr_y = (list_of_shapes[click_counter-1].p_ptr[2].y + list_of_shapes[click_counter-1].p_ptr[0].y)/2.0;

		  //Initial values before movement has occured
		  list_of_shapes[click_counter-1].old_vel2 = 0.0;
		  list_of_shapes[click_counter-1].old_vel = 0.0;
		  list_of_shapes[click_counter-1].difference = 0.0;

		  //DEBUG
		  //printf("Rectangle Coordinates (%f, %f) changed according to mouse click\n", list_of_shapes[click_counter-1].curr_x, list_of_shapes[click_counter-1].curr_y);

		  //setup starting particle values for force, mass, velocity
		  for (int f = 0; f < 4; f++)
		  {
			  //Gravity = 30
			  list_of_shapes[click_counter-1].p_ptr[f].mass = 20;  //rectangles have more mass than circles
		      //Negative to push objects down
			  list_of_shapes[click_counter-1].p_ptr[f].force = (-30.0/window_h)*list_of_shapes[click_counter-1].p_ptr[f].mass;
			  list_of_shapes[click_counter-1].p_ptr[f].velocity = 0;
		  }

	  }
	  else if (list_of_shapes[click_counter-1].p_ptr != NULL && click_counter % 2 == 0)
	  {
		  //Create circle shape
		  double angle = 0;
		  int k = 0;
		  
		  list_of_shapes[click_counter-1].p_ptr[0].x = my_x/window_w;
		  list_of_shapes[click_counter-1].p_ptr[0].y = my_y/window_h;

		  //get circle center for collision detection
		  list_of_shapes[click_counter-1].curr_x = my_x/window_w;
		  list_of_shapes[click_counter-1].curr_y = my_y/window_h;

		  list_of_shapes[click_counter-1].old_vel = 0.0;
		  list_of_shapes[click_counter-1].old_vel2 = 0.0;
		  list_of_shapes[click_counter-1].difference = 0.0;

		  for (int g = 1; g < 12; g++)
		  {
			  // radius of 10 pixels
			  list_of_shapes[click_counter-1].p_ptr[g].x = ((cos((angle* PI)/180) * 7) + my_x)/window_w;  //particle starting x coordinate
			  list_of_shapes[click_counter-1].p_ptr[g].y = ((sin((angle* PI)/180)*7) + my_y)/window_h;  //particle starting y coordinate

			  k += 2;
			  angle += 36.0;
		  }

		  //setup starting particle values for force, mass, velocity
		  for (int f = 0; f < 12; f++)
		  {
			  list_of_shapes[click_counter-1].p_ptr[f].mass = 10;
			  list_of_shapes[click_counter-1].p_ptr[f].force = (-30.0/window_h)*list_of_shapes[click_counter-1].p_ptr[f].mass;
			  list_of_shapes[click_counter-1].p_ptr[f].velocity = 0;
		  }
		  //DEBUG
		  //printf("Circle Coordinates changed according to mouse click\n");
	  }

    }
    else if (s == GLUT_UP) { // button released 
      printf("Left button released\n");
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (s == GLUT_DOWN)  {    // button pressed
      printf("Right button pressed\n");
    }
    else if (s == GLUT_UP) { // button released
      printf("Right button released\n");
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if (s == GLUT_DOWN)  {    // button pressed
      printf("Middle button pressed at %d %d\n", x, y);
    }
    else if (s == GLUT_UP) { // button released
      printf("Middle button released\n");
    }
    break;

  }
}


// called if keyboard event (keyboard key hit) 
void my_keyboard(unsigned char c, int x, int y) {
  switch (c) {  // c is the key that is being hit
  case 'c': {
    printf("C key is hit\n");
  }
    break;
   case 'y': {
    printf("Y key is hit\n");
  }
    break;
    // and so on
  }


}

// function calculates distance between the centers of two objects
double detect_collision(Shape a, Shape b)
{
	double distance = ((a.curr_x - b.curr_x)*(a.curr_x - b.curr_x) + (a.curr_y - b.curr_y)*(a.curr_y - b.curr_y));
	return sqrt(distance);
}


// called if timer event 
// id is the id of the timer; you can schedule as many timer events as you want in the callback setup section of glut_setup
void my_TimeOut(int id) { 
 
   double accel, vel, y2; //temporary values for comparisons

  //BIG for loop for updating particle coordinates with physics
  for (int aa = 0; aa < 25; aa++)
  {
	  //Exit if shape hasn't been made yet
	  if (list_of_shapes[aa].p_ptr == NULL)
	  {
		  break;
	  }
	  else
	  {
		 //rectangles dropping
		 if (aa % 2 == 0)
		 {
			 current = ANIMATION_SPEED/1000.0;  // get current time change (converted from milliseconds to seconds)
			 
			 //Update particle values using equations from lecture
			 for (int g = 0; g < 4; g++)
			 {
				 accel = list_of_shapes[aa].p_ptr[g].force / list_of_shapes[aa].p_ptr[g].mass;
				 vel = list_of_shapes[aa].p_ptr[g].velocity;
				 //printf("%f - v\n", vel); //DEBUG
				 y2 = list_of_shapes[aa].p_ptr[g].y;
				
				 list_of_shapes[aa].p_ptr[g].velocity = vel + (accel * current);
				 list_of_shapes[aa].p_ptr[g].y = y2 + (vel * current) + ( (accel*0.5) * (current*current));
			 }

			 //Update y coordinate of object's center
			 list_of_shapes[aa].curr_y = (list_of_shapes[aa].p_ptr[2].y + list_of_shapes[aa].p_ptr[0].y)/2.0;

			//check for collisions
			for (int j = 0; j < 25; j++)
			{
			  //Don't check for collisions with itself or nonexistant objects
			  if (j == aa || list_of_shapes[j].p_ptr == NULL)
				  continue;
			  
			  //Distance between two shapes
			  double distance = detect_collision(list_of_shapes[aa], list_of_shapes[j]);
			  
			  //printf("%f distance between shape %d and %d\n", distance, aa, j); //DEBUG
			 
			  //Only go in if statement if current object is still in motion
			  if (distance < 0.03 && list_of_shapes[aa].p_ptr[0].force != 0.0)
			  {
				  //DEBUG
				  //printf("%f, %f\n", abs(list_of_shapes[aa].old_vel2 - list_of_shapes[aa].p_ptr[0].velocity), list_of_shapes[aa].difference);
				
				//IF object keeps colliding with a surface with the same velocity than it should stop moving
				//Animation becomes more realistic
				if ( abs(list_of_shapes[aa].old_vel2 - list_of_shapes[aa].p_ptr[0].velocity) ==  list_of_shapes[aa].difference && list_of_shapes[j].curr_y < list_of_shapes[aa].curr_y)
				{
					//printf("COllision!!\n");  //DEBUG
					for (int r = 0; r < 4; r++)
					{
						//stop shape from moving
						list_of_shapes[aa].p_ptr[r].velocity = 0.0;
						list_of_shapes[aa].p_ptr[r].force = 0.0;
					}
				}
				else  //Slow down object due to collision
				{
					list_of_shapes[aa].difference = abs(list_of_shapes[aa].old_vel2 - list_of_shapes[aa].p_ptr[0].velocity);

					for (int r = 0; r < 4; r++)
					{
						list_of_shapes[aa].p_ptr[r].velocity *= -.75;
						//Send object at top of collision upwards
						if (list_of_shapes[aa].curr_y > list_of_shapes[j].curr_y)
						{
							list_of_shapes[aa].p_ptr[r].y += ((0.03-distance)*.9);
							list_of_shapes[aa].curr_y += ((0.03-distance)*.9);
						}
						else //Send object at bottom of collision downwards
						{
							list_of_shapes[aa].p_ptr[r].y -= ((0.03-distance)*.9);
							list_of_shapes[aa].curr_y -= ((0.03-distance)*.9);
						}
					}
				}

				list_of_shapes[aa].old_vel2 = list_of_shapes[aa].p_ptr[0].velocity;
			  }
			}

			double offset;

			 for (int g = 0; g < 4; g++)
			 {
				 //Collision detection with the bottom
				if (list_of_shapes[aa].p_ptr[g].y < 0.0)
				{   
					offset = abs((0.0 - list_of_shapes[aa].p_ptr[g].y));

					//This stops shapes from bouncing on the ground when they should've stopped moving
					if (abs(list_of_shapes[aa].p_ptr[g].velocity) == list_of_shapes[aa].old_vel)
					{
						for (int r = 0; r < 4; r++)
						{
							//printf("STOPPED\n");  //DEBUG
							//stop shape from moving
							list_of_shapes[aa].p_ptr[r].velocity = 0.0;
							list_of_shapes[aa].p_ptr[r].force = 0.0;
						}
						break;
					}
					else
					{
						for (int r = 0; r < 4; r++)
						{
							//switch velocity sign and decreases by 75% due to collision with floor
							list_of_shapes[aa].p_ptr[r].velocity *= -.75;
							list_of_shapes[aa].p_ptr[r].y += offset;
						}
						break;
					}
					//printf("vel - %f\n", list_of_shapes[aa].p_ptr[g].velocity);  //DEBUG
					list_of_shapes[aa].old_vel = list_of_shapes[aa].p_ptr[g].velocity;
				}
			 }
		 }
		 else if (aa % 2 == 1)	//Circles with PHYSICS
		 {
			 //Update particle values
			 current = ANIMATION_SPEED/1000.0;  // get current time change (convert milliseconds to seconds)

			 //Update particles according to equations from lecture
			 for (int g = 0; g < 12; g++)
			 {
				 accel = list_of_shapes[aa].p_ptr[g].force / list_of_shapes[aa].p_ptr[g].mass;
				
				 // printf("%f\n", accel); //DEBUG
				 
				 vel = list_of_shapes[aa].p_ptr[g].velocity;
				 y2 = list_of_shapes[aa].p_ptr[g].y;

				 list_of_shapes[aa].p_ptr[g].velocity = vel + (accel * current);
				 list_of_shapes[aa].p_ptr[g].y = y2 + (vel * current) + ( (accel*0.5) * (current*current));
			 }
			 
			 //Update y coordinate of object's center
			 list_of_shapes[aa].curr_y = list_of_shapes[aa].p_ptr[0].y;

			//check for collisions
			for (int j = 0; j < 25; j++)
			{
			  //Don't check for collisions with itself or nonexistant objects
			  if (j == aa || list_of_shapes[j].p_ptr == NULL)
				  continue;

			  //Distance between two shapes
			  double distance = detect_collision(list_of_shapes[aa], list_of_shapes[j]);
			  
			  //Only go in if statement if current object is still in motion
			  if (distance < 0.03 && list_of_shapes[aa].p_ptr[0].force != 0.0)
			  {
				//printf("vel - %f\n", list_of_shapes[aa].p_ptr[0].velocity); //DEBUG

				//IF object keeps colliding with a surface with the same velocity than it should stop moving
				//Animation becomes more realistic
				if ( abs(list_of_shapes[aa].old_vel2 - list_of_shapes[aa].p_ptr[0].velocity) ==  list_of_shapes[aa].difference && list_of_shapes[j].curr_y < list_of_shapes[aa].curr_y)
				{
					//printf("COllision!!\n"); //DEBUG
					
					for (int r = 0; r < 12; r++)
					{
						//stop shape from moving
						list_of_shapes[aa].p_ptr[r].velocity = 0.0;
						list_of_shapes[aa].p_ptr[r].force = 0.0;
					}
				}
				else  //Slow down object due to collision
				{
					list_of_shapes[aa].difference = abs(list_of_shapes[aa].old_vel2 - list_of_shapes[aa].p_ptr[0].velocity);

					for (int r = 0; r < 12; r++)
					{
						list_of_shapes[aa].p_ptr[r].velocity *= -.75;
						//Send object at top of collision upwards
						if (list_of_shapes[aa].curr_y > list_of_shapes[j].curr_y)
						{
							list_of_shapes[aa].p_ptr[r].y += ((0.03-distance)*.9);
							list_of_shapes[aa].curr_y += ((0.03-distance)*.9);
						}
						else //Send object at bottom of collision downwards
						{
							list_of_shapes[aa].p_ptr[r].y -= ((0.03-distance)*.9);
							list_of_shapes[aa].curr_y -= ((0.03-distance)*.9);
						}
					}
				}
				list_of_shapes[aa].old_vel2 = list_of_shapes[aa].p_ptr[0].velocity;
			  }
			}

			double offset;

			 for (int g = 0; g < 12; g++)
			 {
				 //Collision detection with the bottom
				if (list_of_shapes[aa].p_ptr[g].y <= 0.0)
				{
					offset = abs((0.0 - list_of_shapes[aa].p_ptr[g].y));
				
					//This stops shapes from bouncing on the ground when it should've stopped moving
					if (abs(list_of_shapes[aa].p_ptr[g].velocity) == list_of_shapes[aa].old_vel)
					{
						for (int r = 0; r < 12; r++)
						{
							//printf("STOPPED\n"); //DEBUG

							//stop shape from moving
							list_of_shapes[aa].p_ptr[r].velocity = 0.0;
							list_of_shapes[aa].p_ptr[r].force = 0.0;
						}
						break;
					}
					else
					{
						for (int r = 0; r < 12; r++)
						{
						  //switch velocity sign and decrease by 75% due to collision with floor
						  list_of_shapes[aa].p_ptr[r].velocity *= -.75;
						  list_of_shapes[aa].p_ptr[r].y += offset;
						}
						break;
					}
					list_of_shapes[aa].old_vel = list_of_shapes[aa].p_ptr[g].velocity;
				}
			 }
		 }
	  }
	}// END of for loop

	// clear the buffer 
    glClear(GL_COLOR_BUFFER_BIT) ;

	 //Animate shapes to drop down to botton of screen Tetris style
	for (int aa = 0; aa < 25; aa++)
    {
	  //If shape doesn't exist then don't animate it
	  if (list_of_shapes[aa].p_ptr == NULL)
	  {
		  break;
	  }
	  else
	  {
		//animate rectangles
		 if (aa % 2 == 0)
		 {
		   glColor3f(0,0,1) ; // Blue rectangles
		    
		   //Draw changes made by simulated gravity
		    glBegin(GL_QUADS);
	   
			for(int g= 0; g < 4; g++)
			{
				glVertex2d(list_of_shapes[aa].p_ptr[g].x, list_of_shapes[aa].p_ptr[g].y);
			}

			glEnd();
		 }
		 else
		 {
			 glColor3f(1,0,0) ; //Red circles
			 
			 //Draw changes made by simulated gravity
			 glBegin(GL_TRIANGLE_FAN);
		 
			 for(int x= 0; x < 12; x++)
			 {
				glVertex2d(list_of_shapes[aa].p_ptr[x].x, list_of_shapes[aa].p_ptr[x].y);
			 }
		 
			 glEnd();
		 }
	  }
	}
 
   // buffer is ready; show me the money! 
   glutSwapBuffers();

  glutTimerFunc(ANIMATION_SPEED, my_TimeOut, 0);// schedule next timer event, according to ANIMATION_SPEED

}
