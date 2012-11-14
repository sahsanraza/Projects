//NAME: Ben Isenberg
//DATE: 9/25/12
//EMAIL: bji6@pitt.edu
//CS1566 Project 2

/*
  This project displays a cube, house, sphere, cone, cylinder, and torus.
  The sphere, cone, cylinder, and torus have their vertices drived through matrix multiplications of various graphical transfomations.
  You can look at the shapes in wire frame or solid color mode.
  In wire frame mode you can use '+' '-' '<' '>' to increase or decrease stack and slice tessellation.  
  Also you can display the vertex normals of the shapes by pressing 'v'.
*/

#include "BenIsenberg-Project2.h"
#include <cstdio>

//Various values
int theta_x;
int theta_y;
int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
double cyl_height=2.0;
double cyl_ray=1.0;
double sph_ray=3.0;

Shape ben_cylinder; //global cylinder shape
Shape ben_sphere; //global sphere object
Shape ben_cone;  //global cone object
Shape ben_torus; //global torus object

GLfloat cube_normals[8][4];  //cube vertex normals

//boolean used to determine whether vertex normals should be displayed
bool display_normals = false;


int main(int argc, char **argv) {

  /* General initialization for GLUT and OpenGL
     Must be called first */
  glutInit( &argc, argv ) ;
  
  /* we define these setup procedures */
  glut_setup() ;  
  gl_setup() ;
  my_setup();

  /* go into the main event loop */
  glutMainLoop() ;

  return(0) ;
}


/* This function sets up the windowing related glut calls */
void glut_setup(void) {

  /* specify display mode -- here we ask for a double buffer and RGB coloring */
  /* NEW: tell display we care about depth now */
  glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB |GLUT_DEPTH);

  /* make a 400x400 window with the title of "Stitcher" placed at the top left corner */
  glutInitWindowSize(600,700);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Ben Isenberg 2");

  /*initialize callback functions */
  glutDisplayFunc( my_display );
  glutReshapeFunc( my_reshape ); 
  glutMouseFunc( my_mouse);
  glutKeyboardFunc(my_keyboard);

  return ;
}


//Set up Opengl
void gl_setup(void) {

  /* specifies a background color: black in this case */
  glClearColor(0,0,0,0) ;

  /* NEW: now we have to enable depth handling (z-buffer) */
  glEnable(GL_DEPTH_TEST);

  /* NEW: setup for 3d projection */
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  // perspective view
  gluPerspective( 20.0, 1.0, 1.0, 100.0);
  return;
}

//Compute shape vertices
void my_setup(void) {
  theta_x = 0;
  theta_y = 0;
  crt_render_mode = GL_LINE_LOOP;
  crt_shape = HOUSE;
  //Initial number of lines of latitude and longitude
  crt_rs = 10;
  crt_vs = 10;

  //Compute cylinder vertices
  make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  //compute sphere vertices
  make_sphere(sph_ray,crt_rs,crt_vs);
  //compute cone vertices
  make_cone(2.0, cyl_height, crt_rs, crt_vs);
  //compute torus vertices
  make_torus(3.0, 1.0, crt_rs, crt_vs);
  
  return;
}


//remaps viewport to the Window Manager's window
void my_reshape(int w, int h) {

  /* define viewport -- x, y, (origin is at lower left corner) width, height */
  glViewport (0, 0, min(w,h), min(w,h));
  return;
}


//Handles keyboard events
void my_keyboard( unsigned char key, int x, int y ) {
  
  switch( key ) 
  {
    //Increase vertical slice tessellation
    case '+': {
				if (crt_rs != 50.0 || crt_shape == CYLINDER)
				{	
					//Cylinder shape has slice and stack tessellation opposite the other shapes
					if (crt_shape == CYLINDER)
					{
						if (crt_vs != 50.0)
							crt_vs++;
					}
					else
						crt_rs++;

					//Compute cylinder vertices
					make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
					//compute sphere vertices
					make_sphere(sph_ray,crt_rs,crt_vs);
					//compute cone vertices
					make_cone(1.0, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(3.0, 1.0, crt_rs, crt_vs);
				}
				//Reset vertex normals when tessellation changes
				for (int x = 0; x < 50; x++)
				{
					for (int y = 0; y < 50; y++)
					{
						ben_cylinder.normals[x][y].coord[0] = 0.0;
						ben_cylinder.normals[x][y].coord[1] = 0.0;
						ben_cylinder.normals[x][y].coord[2] = 0.0;
						ben_cylinder.normals[x][y].coord[3] = 0.0;
						
						ben_cone.normals[x][y].coord[0] = 0.0;
						ben_cone.normals[x][y].coord[1] = 0.0;
						ben_cone.normals[x][y].coord[2] = 0.0;
						ben_cone.normals[x][y].coord[3] = 0.0;
						
						ben_torus.normals[x][y].coord[0] = 0.0;
						ben_torus.normals[x][y].coord[1] = 0.0;
						ben_torus.normals[x][y].coord[2] = 0.0;
						ben_torus.normals[x][y].coord[3] = 0.0;
						
						ben_sphere.normals[x][y].coord[0] = 0.0;
						ben_sphere.normals[x][y].coord[1] = 0.0;
						ben_sphere.normals[x][y].coord[2] = 0.0;
						ben_sphere.normals[x][y].coord[3] = 0.0;
					}
				}

				//Display changes
				glutPostRedisplay();
			  }; 
			
		      break;
    //Decrease vertical slice tessellation
    case '-': {
				if (crt_rs != 5.0 || crt_shape == CYLINDER)
				{
					//Cylinder shape has slice and stack tessellation opposite the other shapes
					if (crt_shape == CYLINDER)
					{
						if (crt_vs != 5.0)
							crt_vs--;
					}
					else
						crt_rs--;

					//Compute cylinder vertices
					make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
					//compute sphere vertices
					make_sphere(sph_ray,crt_rs,crt_vs);
					//compute cone vertices
					make_cone(1.0, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(3.0, 1.0, crt_rs, crt_vs);
				}
				//Reset vertex normals when tessellation changes
				for (int x = 0; x < 50; x++)
				{
					for (int y = 0; y < 50; y++)
					{
						ben_cylinder.normals[x][y].coord[0] = 0.0;
						ben_cylinder.normals[x][y].coord[1] = 0.0;
						ben_cylinder.normals[x][y].coord[2] = 0.0;
						ben_cylinder.normals[x][y].coord[3] = 0.0;
						
						ben_cone.normals[x][y].coord[0] = 0.0;
						ben_cone.normals[x][y].coord[1] = 0.0;
						ben_cone.normals[x][y].coord[2] = 0.0;
						ben_cone.normals[x][y].coord[3] = 0.0;
						
						ben_torus.normals[x][y].coord[0] = 0.0;
						ben_torus.normals[x][y].coord[1] = 0.0;
						ben_torus.normals[x][y].coord[2] = 0.0;
						ben_torus.normals[x][y].coord[3] = 0.0;
						
						ben_sphere.normals[x][y].coord[0] = 0.0;
						ben_sphere.normals[x][y].coord[1] = 0.0;
						ben_sphere.normals[x][y].coord[2] = 0.0;
						ben_sphere.normals[x][y].coord[3] = 0.0;
					}
				}

				//Display changes
				glutPostRedisplay();
			  }; 
		     
		      break;
    //Increase  stack tessellation
    case '<': {
				if (crt_vs != 50.0 || crt_shape == CYLINDER)
				{
					//Cylinder shape has slice and stack tessellation opposite the other shapes
					if (crt_shape == CYLINDER)
					{	
						if (crt_rs != 50.0)
							crt_rs++;
					}
					else
						crt_vs++;

					//Compute cylinder vertices
					make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
					//compute sphere vertices
					make_sphere(sph_ray,crt_rs,crt_vs);
					//compute cone vertices
					make_cone(1.0, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(3.0, 1.0, crt_rs, crt_vs);
				}
				//Reset vertex normals when tessellation changes
				for (int x = 0; x < 50; x++)
				{
					for (int y = 0; y < 50; y++)
					{
						ben_cylinder.normals[x][y].coord[0] = 0.0;
						ben_cylinder.normals[x][y].coord[1] = 0.0;
						ben_cylinder.normals[x][y].coord[2] = 0.0;
						ben_cylinder.normals[x][y].coord[3] = 0.0;
						
						ben_cone.normals[x][y].coord[0] = 0.0;
						ben_cone.normals[x][y].coord[1] = 0.0;
						ben_cone.normals[x][y].coord[2] = 0.0;
						ben_cone.normals[x][y].coord[3] = 0.0;
						
						ben_torus.normals[x][y].coord[0] = 0.0;
						ben_torus.normals[x][y].coord[1] = 0.0;
						ben_torus.normals[x][y].coord[2] = 0.0;
						ben_torus.normals[x][y].coord[3] = 0.0;
						
						ben_sphere.normals[x][y].coord[0] = 0.0;
						ben_sphere.normals[x][y].coord[1] = 0.0;
						ben_sphere.normals[x][y].coord[2] = 0.0;
						ben_sphere.normals[x][y].coord[3] = 0.0;
					}
				}

				//Display changes
				glutPostRedisplay();
			  }; 
		    
		      break;
    //Decrease stack tessellation
    case '>': {
				if (crt_vs != 5.0 || crt_shape == CYLINDER)
				{
					//Cylinder shape has slice and stack tessellation opposite the other shapes
					if (crt_shape == CYLINDER)
					{	
						if (crt_rs != 5.0)
							crt_rs--;
					}
					else
						crt_vs--;

					//Compute cylinder vertices
					make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
					//compute sphere vertices
					make_sphere(sph_ray,crt_rs,crt_vs);
					//compute cone vertices
					make_cone(1.0, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(3.0, 1.0, crt_rs, crt_vs);
				}
				//Reset vertex normals when tessellation changes
				for (int x = 0; x < 50; x++)
				{
					for (int y = 0; y < 50; y++)
					{
						ben_cylinder.normals[x][y].coord[0] = 0.0;
						ben_cylinder.normals[x][y].coord[1] = 0.0;
						ben_cylinder.normals[x][y].coord[2] = 0.0;
						ben_cylinder.normals[x][y].coord[3] = 0.0;
						
						ben_cone.normals[x][y].coord[0] = 0.0;
						ben_cone.normals[x][y].coord[1] = 0.0;
						ben_cone.normals[x][y].coord[2] = 0.0;
						ben_cone.normals[x][y].coord[3] = 0.0;
						
						ben_torus.normals[x][y].coord[0] = 0.0;
						ben_torus.normals[x][y].coord[1] = 0.0;
						ben_torus.normals[x][y].coord[2] = 0.0;
						ben_torus.normals[x][y].coord[3] = 0.0;
						
						ben_sphere.normals[x][y].coord[0] = 0.0;
						ben_sphere.normals[x][y].coord[1] = 0.0;
						ben_sphere.normals[x][y].coord[2] = 0.0;
						ben_sphere.normals[x][y].coord[3] = 0.0;
					}
				}
				
				//Display changes
				glutPostRedisplay();
			  }; 
		      
		      break;
    case 'c':
	case 'C': {
				//Display cylinder
				display_normals = false;
				crt_shape = CYLINDER;
				glutPostRedisplay();
			  }; 
			  break;
	case 'v':
	case 'V': {
				//Display vertex normals of currently displayed shape
				display_normals = true;
				glutPostRedisplay();
			  }; 
			  break;
	case 's':
	case 'S': {
				//Display sphere
				display_normals = false;
				crt_shape = SPHERE;
				glutPostRedisplay();
			  }; 
			  break;
	case 'n':
	case 'N': {
				//Display cone
				display_normals = false;
				crt_shape = CONE;
				glutPostRedisplay();
			  }; 
		      break;
	case 'm':
	case 'M': {
				//Display torus
				display_normals = false;
				crt_shape = TORUS;
				glutPostRedisplay();
			  }; 
		      break;
	case 'y':
	case 'Y': {
				//Turn shape
				theta_y = (theta_y+2) %360;
				glutPostRedisplay(); 
				}; 
		        break;
	case 'x':
	case 'X': {
				//Turn shape
				 theta_x = (theta_x+2) %360;
				 glutPostRedisplay(); 
			  };
		      break;
	case 'B':
	case 'b': {
				//Display cube
				display_normals = false;
				crt_shape = CUBE;
				glutPostRedisplay();
			  }; 
		      break;
	case 'H':
	case 'h': {
				//Display house
				display_normals = false;
				crt_shape = HOUSE;
				glutPostRedisplay();
			  }; 
		      break;
	case 'q': 
	case 'Q':
				exit(0);
	default: break;
  }
  
  return;
}


//Handles mouse events
void my_mouse(int button, int state, int mousex, int mousey) {

  switch( button ) 
  {
	  case GLUT_LEFT_BUTTON:
			if( state == GLUT_DOWN ) 
			{
			  crt_render_mode = GL_LINE_LOOP;
			  glutPostRedisplay();
			}
			if( state == GLUT_UP ) 
			{}
			break ;

	  case GLUT_RIGHT_BUTTON:
			if ( state == GLUT_DOWN ) 
			{
			  crt_render_mode = GL_POLYGON;
			  glutPostRedisplay();
			}
			if( state == GLUT_UP ) 
			{}
			break ;
  }
  
  return ;
}


//Draws 3 vertices
void draw_triangle(GLfloat vertices[][4], int iv1, int iv2, int iv3, int ic) {
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);
    /*note the explicit use of homogeneous coords below: glVertex4f*/
    glVertex4fv(vertices[iv1]);
    glVertex4fv(vertices[iv2]);
    glVertex4fv(vertices[iv3]);
  }
  glEnd();
}

//Draws 4 vertices
void draw_quad(GLfloat vertices[][4], int iv1, int iv2, int iv3, int iv4, int ic) {
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);
    /*note the explicit use of homogeneous coords below: glVertex4f*/
	/*for (int x = 0; x < 4; x ++)
	{
		printf(" Vertex %d -%f\n", x, vertices[iv1][x]);
		printf(" Vertex %d -%f\n", x, vertices[iv2][x]);
		printf(" Vertex %d -%f\n", x, vertices[iv3][x]);
		printf(" Vertex %d -%f\n", x, vertices[iv4][x]);
	}*/
    glVertex4fv(vertices[iv1]);
    glVertex4fv(vertices[iv2]);
    glVertex4fv(vertices[iv3]);
    glVertex4fv(vertices[iv4]);
  }
  glEnd();
}

//Draws 4 vertices of a Shape object in counter clockwise order
void draw_param_quad(Particle vertices[50][50], int line, int col, int ic) {
	glBegin(crt_render_mode);
	{	
		glColor3fv(colors[ic]);
		/*for (int x = 0; x < 4; x ++)
		{
		printf(" Vertex %d -%f\n", x, vertices[line][col]);
		printf(" Vertex %d -%f\n", x, vertices[(line+1)%crt_rs][col]);
		printf(" Vertex %d -%f\n", x, vertices[(line+1)%crt_rs][(col+1)%crt_vs]);
		printf(" Vertex %d -%f\n", x, vertices[line][(col+1)%crt_vs]);
		}*/
		//Draw vertices in counter clockwise order with glvertex for doubles
		glVertex4dv(vertices[line][col].coord);
		//Modulus so it reconnects the end of the rows with the beginning of the rows to create a full circle
		glVertex4dv(vertices[(line+1)%crt_rs][col].coord);
		glVertex4dv(vertices[(line+1)%crt_rs][(col+1)%crt_vs].coord);
		glVertex4dv(vertices[line][(col+1)%crt_vs].coord);
	}
	glEnd();
}


//Draws a house
void draw_house()
{
  draw_triangle(vertices_house,0,1,2,RED);
  draw_triangle(vertices_house,0,2,3,GREEN);
  draw_triangle(vertices_house,0,3,4,WHITE);
  draw_triangle(vertices_house,0,4,1,GREY);


  draw_quad(vertices_house,2,1,5,6, BLUE);
  draw_triangle(vertices_house,2,6,3, CYAN);
  draw_triangle(vertices_house,3,6,7, CYAN);
  draw_triangle(vertices_house,3,7,8, YELLOW);
  draw_triangle(vertices_house,8,3,4, YELLOW);
  draw_triangle(vertices_house,4,8,1, MAGENTA);
  draw_triangle(vertices_house,1,8,5, MAGENTA);

}

//Draws a cube and computes its vertex normals
void draw_cube_brute()
{

  draw_triangle(vertices_cube_brute, 4,5,1,BLUE);
  compute_cube_normals(4, 5, 1);

  draw_triangle(vertices_cube_brute, 0,4,1,BLUE);
  compute_cube_normals(0, 4, 1);
  
  draw_triangle(vertices_cube_brute, 5,6,2,CYAN);
  compute_cube_normals(5, 6, 2);
  
  draw_triangle(vertices_cube_brute, 1,5,2,CYAN);
  compute_cube_normals(1, 5, 2);
  
  draw_triangle(vertices_cube_brute, 3,2,6,YELLOW);
  compute_cube_normals(3, 2, 6);
  
  draw_triangle(vertices_cube_brute, 7,3,6,YELLOW);
  compute_cube_normals(7, 3, 6);
  
  draw_triangle(vertices_cube_brute, 0,3,7,MAGENTA);
  compute_cube_normals(0, 3, 7);
  
  draw_triangle(vertices_cube_brute, 4,0,7,MAGENTA);
  compute_cube_normals(4, 0, 7);

  //Draw a top to the cube with triangles
  draw_triangle(vertices_cube_brute, 0,1,2,GREY);
  compute_cube_normals(0, 1, 2);
  draw_triangle(vertices_cube_brute, 3,0,2,GREY);
  compute_cube_normals(3, 0, 2);

  //Draw a bottom to the cube with triangles
  draw_triangle(vertices_cube_brute, 4,5,6,GREEN);
  compute_cube_normals(4, 5, 6);
  draw_triangle(vertices_cube_brute, 7,4,6,GREEN);
  compute_cube_normals(7, 4, 6);

  float m;

  //Normalize vertex normals
  for (int x = 0; x < 8; x++)
  {
	  //Don't include w coordinate
	  m = (cube_normals[x][0]*cube_normals[x][0]) + (cube_normals[x][1]*cube_normals[x][1]) + (cube_normals[x][2]*cube_normals[x][2]);
	  //take square root
	  m = sqrt(m);
	  cube_normals[x][0] /= m;
	  cube_normals[x][1] /= m;
	  cube_normals[x][2] /= m;
  }
  
  //Draw normals
  if (display_normals)
  {
	float v_prime[4];
	
	v_prime[3] = 1.0;

	for (int x = 0; x < 8; x++)
	{
		//compute v prime, don't include w coordinate
		for (int h = 0; h < 3; h++)
		{
			v_prime[h] = vertices_cube_brute[x][h] + (0.2*cube_normals[x][h]);
		}

		glBegin(crt_render_mode);
		{	
			glColor3fv(colors[MAGENTA]);
		
			//Draw vertices with glvertex for doubles
			glVertex4fv(vertices_cube_brute[x]);
			glVertex4fv(v_prime);
		}
		glEnd();
	}
  }


	  
}

//Compute face normal of plane that vertex a, b, c lie on
void compute_cube_normals(int a, int b, int c)
{
	float p1[4];
	float p2[4];
	float p0[4];
	float answer[4];
	
	//Copy cube vertices into p0, p1, p2
	for (int x = 0; x < 4; x++)
	{ 
		p0[x] = vertices_cube_brute[b][x];
		p1[x] = vertices_cube_brute[a][x];
		p2[x] = vertices_cube_brute[c][x];
	}
  
    //Dont involve w coordinate
	for (int x = 0; x < 3; x++)
	{
		p2[x] -= p0[x];
		p1[x] -= p0[x];
	}

	//Cross product
	answer[0] = (p2[1]*p1[2]) - (p2[2]*p1[1]);
	answer[1] = (p2[2]*p1[0]) - (p2[0]*p1[2]);
	answer[2] = (p2[0]*p1[1]) - (p2[1]*p1[0]);
	answer[3] = 1.0; // w
	//Add cross product vector to normals
	for (int g = 0; g < 3; g++)
	{
		cube_normals[a][g] += answer[g];
		cube_normals[b][g] += answer[g];
		cube_normals[c][g] += answer[g];
	}

	cube_normals[a][3] = 1.0;
	cube_normals[b][3] = 1.0;
	cube_normals[c][3] = 1.0;

	return;
}


//multiply a matrix with a vector in column form
//Set v as the answer
void matrix_multiplication(double m[4][4], Particle& v, double start[4])
{	
	//compute dot products of matrix rows with vertex
	for (int x = 0; x < 4; x++)
	{
		v.coord[x] = (m[x][0]*start[0]) + (m[x][1]*start[1]) + (m[x][2]*start[2]) + (m[x][3]*start[3]);
	}

	return;
}



//Compute vertices of a cylinder
void make_cylinder(double height, double ray, int rs, int vs)
{

	//Angle used for drawing a circle around the y-axis (0 - 2*PI)
	double theta = 0.0; 

	//rotation matrix around y-axis based on theta
	double rotation_y [4][4] = { {cos(theta), 0.0, sin(theta), 0.0},
								 {0.0, 1.0, 0.0, 0.0},
								 {-sin(theta), 0.0, cos(theta), 0.0},
								 {0.0, 0.0, 0.0, 1.0} };
	
	double v_start[4] = {ray, -(height*0.5), 0.0, 1.0};  //starting coordinate

	double h = 0; //height increments for translation
	
	//translation matrix up the y-axis
	double translation_y[4][4] = { {1.0, 0.0, 0.0, 0.0},
							       {0.0, 1.0, 0.0, h},
							       {0.0, 0.0, 1.0, 0.0},
								   {0.0, 0.0, 0.0, 1.0} };

	//Stores coordinates after y rotation to be used later for y translation
	double temp[4];

	//compute vertices for the cylinder
	for (int x = 0; x < rs; x++)
	{

		for (int y = 0; y < vs; y++)
		{
			//Apply rotation
			matrix_multiplication(rotation_y, ben_cylinder.vertices[x][y], v_start);
			
			//Copy rotated coordinates into temp array
			for (int f = 0; f < 4; f++)
			{
				temp[f] = ben_cylinder.vertices[x][y].coord[f];
			}
			
			//Apply translation
			matrix_multiplication(translation_y, ben_cylinder.vertices[x][y], temp);
			
			//Update theta
			theta += (2.0*PI)/vs;
			
			//Update rotation matrix
			rotation_y[0][0] = cos(theta);
			rotation_y[0][2] = sin(theta);
			rotation_y[2][0] = -sin(theta);
			rotation_y[2][2] = cos(theta);
		}

		//Update height
		h += (height/(rs-1));

		//Update translation matrix
		translation_y[1][3] = h;
	}

}


//Computes sphere vertices
void make_sphere(double ray, int rs, int vs)
{
	//Used for rotation around z  (range is -PI/2 -- PI/2)
	double theta = -(PI/2);
	//Used for rotation around y  ( 0 -- 2PI)
	double alpha = 0.0;

	//rotation matrix around z-axis based on theta
	double rotation_z [4][4] = { {cos(theta), -sin(theta), 0.0, 0.0},
								 {sin(theta), cos(theta), 0.0, 0.0},
								 {0.0, 0.0, 1.0, 0.0},
								 {0.0, 0.0, 0.0, 1.0} };

	//rotation matrix around y-axis based on alpha
	double rotation_y [4][4] = { {cos(alpha), 0.0, sin(alpha), 0.0},
								 {0.0, 1.0, 0.0, 0.0},
								 {-sin(alpha), 0.0, cos(alpha), 0.0},
								 {0.0, 0.0, 0.0, 1.0} };


	double v_start[4] = {ray, 0.0, 0.0, 1.0};  //starting coordinate

	//copy of array for applying second rotation after the first one
	double copy[4];

	for (int x = 0; x < vs; x++)
	{
		for (int y = 0; y < rs; y++)
		{
			//Apply rotation around z
			matrix_multiplication(rotation_z, ben_sphere.vertices[y][x], v_start);
			
			//copy rotated coordinates into copy array for next rotation
			for (int c = 0; c < 4; c++)
			{
				copy[c] = ben_sphere.vertices[y][x].coord[c];
			}
			
			//Apply rotation around y
			matrix_multiplication(rotation_y, ben_sphere.vertices[y][x], copy);
			
			//Update alpha
			alpha += ((2.0*PI)/(rs));

			//Update y matrix
			rotation_y[0][0] = cos(alpha);
			rotation_y[0][2] = sin(alpha);
			rotation_y[2][0] = -sin(alpha);
			rotation_y[2][2] = cos(alpha);
		}
		//Reset alpha
		alpha = 0.0;
		
		//Update theta
		//(vs-1) so that it draws the north pole, if I divided by vs it would not compute vertices for north pole (theta = PI/2)
		theta += (PI/(vs-1));
		
		//update z matrix
		rotation_z[0][0] = cos(theta);
		rotation_z[0][1] = -sin(theta);
		rotation_z[1][0] = sin(theta);
		rotation_z[1][1] = cos(theta);

	}

}

//Function to compute vertices of a cone
void make_cone(double ray, double height, int rs, int vs)
{

	double alpha = 0.0;

	//t is used to make the circles that are drawn smaller and smaller to create the cone shape
	double t = 0.0;

	double translation[4][4] =  { {1.0, 0.0, 0.0, (ray*(1-t))},
							      {0.0, 1.0, 0.0, (height*t)},
							      {0.0, 0.0, 1.0, 0.0},
								  {0.0, 0.0, 0.0, 1.0} };

	//rotation matrix around y-axis based on alpha
	double rotation_y [4][4] = { {cos(alpha), 0.0, sin(alpha), 0.0},
								 {0.0, 1.0, 0.0, 0.0},
								 {-sin(alpha), 0.0, cos(alpha), 0.0},
								 {0.0, 0.0, 0.0, 1.0} };
	
	double v_start[4] = {0.0, -0.5, 0.0, 1.0};

	double copy[4];

	//Only x < vs-1 so that theres room left in the 2d array for the vertices of the tip of the cone
	for (int x = 0; x < (vs-1); x++)
	{
		for (int y = 0; y < rs; y++)
		{
			matrix_multiplication(translation, ben_cone.vertices[y][x], v_start);

			for (int j = 0; j < 4; j++)
			{
				copy[j] = ben_cone.vertices[y][x].coord[j];
			}

			matrix_multiplication(rotation_y, ben_cone.vertices[y][x], copy);

			//Update alpha
			alpha += (2*PI)/rs;
			
			//Update y matrix
			rotation_y[0][0] = cos(alpha);
			rotation_y[0][2] = sin(alpha);
			rotation_y[2][0] = -sin(alpha);
			rotation_y[2][2] = cos(alpha);
		}

		//Update t
		t = (1.0*x)/(1.0*vs);
		
		//Update translation matrix
		translation[0][3] = ray*(1-t);
		translation[1][3] = height*t;
	}

	//Draw tip of the cone (t == 1)
	t = 1.0;
	
	//Update translation matrix
	translation[0][3] = ray*(1-t);
	translation[1][3] = height*t;

	for (int y = 0; y < rs; y++)
	{
		matrix_multiplication(translation, ben_cone.vertices[y][vs-1], v_start);

		for (int j = 0; j < 4; j++)
		{
			copy[j] = ben_cone.vertices[y][vs-1].coord[j];
		}

		matrix_multiplication(rotation_y, ben_cone.vertices[y][vs-1], copy);
		
		//Update alpha
		alpha += (2*PI)/rs;
		//Update y matrix
		rotation_y[0][0] = cos(alpha);
		rotation_y[0][2] = sin(alpha);
		rotation_y[2][0] = -sin(alpha);
		rotation_y[2][2] = cos(alpha);
	}




}

//Function to compute the vertices of a torus (donut)
void make_torus(double r_torus, double r_tube, int rs, int vs)
{
	double alpha = 0.0;
	double theta = 0.0;

	//Translation by radius of torus
	double translation[4][4] =  { {1.0, 0.0, 0.0, r_torus},
							      {0.0, 1.0, 0.0, 0.0},
							      {0.0, 0.0, 1.0, 0.0},
								  {0.0, 0.0, 0.0, 1.0} };

	//rotation matrix around y-axis based on alpha
	double rotation_y [4][4] = { {cos(alpha), 0.0, sin(alpha), 0.0},
								 {0.0, 1.0, 0.0, 0.0},
								 {-sin(alpha), 0.0, cos(alpha), 0.0},
								 {0.0, 0.0, 0.0, 1.0} };
	
	//rotation matrix around z-axis based on theta
	double rotation_z [4][4] = { {cos(theta), -sin(theta), 0.0, 0.0},
								 {sin(theta), cos(theta), 0.0, 0.0},
								 {0.0, 0.0, 1.0, 0.0},
								 {0.0, 0.0, 0.0, 1.0} };

	double v_start[4] = {r_tube, 0.0, 0.0, 1.0};

	double copy[4];

	for (int x = 0; x < vs; x++)
	{
		for (int y = 0; y < rs; y++)
		{
			matrix_multiplication(rotation_z, ben_torus.vertices[y][x], v_start);

			for (int h = 0; h < 4; h++)
			{
				copy[h] = ben_torus.vertices[y][x].coord[h];
			}

			matrix_multiplication(translation, ben_torus.vertices[y][x], copy);

			for (int h = 0; h < 4; h++)
			{
				copy[h] = ben_torus.vertices[y][x].coord[h];
			}

			matrix_multiplication(rotation_y, ben_torus.vertices[y][x], copy);
			//Update alpha
			alpha += (2*PI)/rs;
			
			//Update y matrix
			rotation_y[0][0] = cos(alpha);
			rotation_y[0][2] = sin(alpha);
			rotation_y[2][0] = -sin(alpha);
			rotation_y[2][2] = cos(alpha);

		}
		//Update theta
		theta += (2*PI)/vs;
		//update z matrix
		rotation_z[0][0] = cos(theta);
		rotation_z[0][1] = -sin(theta);
		rotation_z[1][0] = sin(theta);
		rotation_z[1][1] = cos(theta);
	}


}


//Sets answer = (p2-p0) X (p1-p0) 
void cross_product(Particle& answer, Particle p0, Particle p1, Particle p2)
{
	//Dont involve w coordinate
	for (int x = 0; x < 3; x++)
	{
		p2.coord[x] -= p0.coord[x];
		p1.coord[x] -= p0.coord[x];
	}

	//Cross product
	answer.coord[0] = (p2.coord[1]*p1.coord[2]) - (p2.coord[2]*p1.coord[1]);
	answer.coord[1] = (p2.coord[2]*p1.coord[0]) - (p2.coord[0]*p1.coord[2]);
	answer.coord[2] = (p2.coord[0]*p1.coord[1]) - (p2.coord[1]*p1.coord[0]);
	answer.coord[3] = 1.0; // w coordinate
}

//Function finds magnitude of a vector
double find_magnitude(Particle p)
{
	double m;

	//dot product of p with itself
	//Don't include w coordinate
	m = (p.coord[0]*p.coord[0]) + (p.coord[1]*p.coord[1]) + (p.coord[2]*p.coord[2]);
	
	//take square root
	m = sqrt(m);

	return m;
}

//normalize all of the shape's vertex normals so they are of length 1
void normalize(Shape& s)
{
	double magnitude;

	for (int x = 0; x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{	
			//Find magnitude of normal
			magnitude = find_magnitude(s.normals[x][y]);
			
			//Normalize by magnitude
			//Don't include w coordinate
			for (int j = 0; j < 3; j++)
			{
				s.normals[x][y].coord[j] /= magnitude;
			}
		}
	}

}

//Draw cylinder vertices in counter-clockwise order
//Compute vertex normals inside draw functions
void draw_cylinder(int rs, int vs)
{
	Particle p0, p1, p2, answer;

	for (int x = 0; x < rs; x++)
	{
		for (int y = 0; y < vs; y++)
		{
			draw_param_quad(ben_cylinder.vertices, x, y, WHITE);

			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = ben_cylinder.vertices[x][y].coord[j];
				p1.coord[j] = ben_cylinder.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = ben_cylinder.vertices[(x+1)%rs][(y+1)%vs].coord[j];
			}

			//Set answer = (p2-p0) X (p1-p0) unless you are on bottom or top stack
			if (x == 0 || x == (rs-1))
			{
				cross_product(answer, p1, p0, p2);
			}
			else
			{
				cross_product(answer, p0, p1, p2);
			}

			//Add answer to all vertices on the face that is orthogonal to answer
			//Vector addition
			for (int j = 0; j < 3; j++)
			{
				ben_cylinder.normals[x][y].coord[j] += answer.coord[j];
				//Skip if x = 0
				if (x > 0)
				{
					ben_cylinder.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
					ben_cylinder.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				}
				
				ben_cylinder.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			ben_cylinder.normals[x][y].coord[3] = 1.0;
			ben_cylinder.normals[(x+1)%rs][y].coord[3] = 1.0;
			ben_cylinder.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			ben_cylinder.normals[x][(y+1)%vs].coord[3] = 1.0;

		}
	}

	//Normalize computed vertex normals
	normalize(ben_cylinder);
}

//Draw normals for a shape
//Draw normal from v to v'
//v' = v + (0.2*normal of v)
void draw_normals(Shape& s, int rs, int vs)
{
	double v_prime[4];
	
	v_prime[3] = 1.0;

	for (int x = 0; x < rs; x++)
	{
		for (int y = 0; y < vs; y++)
		{	
			//compute v prime
			for (int h = 0; h < 3; h++)
			{
				v_prime[h] = s.vertices[x][y].coord[h] + (0.2*s.normals[x][y].coord[h]);
			}

			glBegin(crt_render_mode);
			{	
				glColor3fv(colors[MAGENTA]);
		
				glVertex4dv(s.vertices[x][y].coord);
				glVertex4dv(v_prime);
			}
			glEnd();

		}
	}

}

//Draw sphere vertices
//Compute sphere vertex normals
void draw_sphere(int rs, int vs)
{
	Particle p0, p1, p2, answer;

	for (int x = 0; x < rs; x++)
	{
		//(vs-1) so that the column index doesnt go out of bounds
		for (int y = 0; y < vs; y++)
		{
			glBegin(crt_render_mode);
			{	
				glColor3fv(colors[GREEN]);

				//Draw vertices in counter clockwise order with glvertex for doubles
				glVertex4dv(ben_sphere.vertices[x][y].coord);
				//Modulus so it reconnects the end of the rows with the beginning of the rows to create a full circle
				glVertex4dv(ben_sphere.vertices[(x+1)%rs][y].coord);
				
				//Skip if y = vs-1 so we don't go out of bounds in the array
				if (y != vs-1)
				{
					//No modulus to column index because otherwise the poles will connect with a line
					glVertex4dv(ben_sphere.vertices[(x+1)%rs][(y+1)].coord);
					glVertex4dv(ben_sphere.vertices[x][(y+1)].coord);
				}

			}
			glEnd();


			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = ben_sphere.vertices[x][y].coord[j];
				p1.coord[j] = ben_sphere.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = ben_sphere.vertices[(x+1)%rs][(y+1)%vs].coord[j];
			}
			
			cross_product(answer, p1, p0, p2);

			//Add answer to all vertices that lie on the face that answer is orthogonal to
			//Vector addition
			for (int j = 0; j < 3; j++)
			{
				ben_sphere.normals[x][y].coord[j] += answer.coord[j];
				ben_sphere.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
				ben_sphere.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				ben_sphere.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			ben_sphere.normals[x][y].coord[3] = 1.0;
			ben_sphere.normals[(x+1)%rs][y].coord[3] = 1.0;
			ben_sphere.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			ben_sphere.normals[x][(y+1)%vs].coord[3] = 1.0;
		}
	}

	normalize(ben_sphere);
}

//Draw cone shape
//Compute vertex normals
void draw_cone(int rs, int vs)
{
	Particle p0, p1, p2, answer;

	//Draw vertices
	for (int x = 0; x < rs; x++)
	{
		for (int y = 0; y < vs; y++)
		{
			draw_param_quad(ben_cone.vertices, x, y, YELLOW);

			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = ben_cone.vertices[x][y].coord[j];
				p1.coord[j] = ben_cone.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = ben_cone.vertices[(x+1)%rs][(y+1)%vs].coord[j];
			}
			
			cross_product(answer, p1, p0, p2);

			//Add answer to all vertices that lie on the face that answer is orthogonal to
			//Vector addition
			for (int j = 0; j < 3; j++)
			{
				ben_cone.normals[x][y].coord[j] += answer.coord[j];
				ben_cone.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
				ben_cone.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				ben_cone.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			ben_cone.normals[x][y].coord[3] = 1.0;
			ben_cone.normals[(x+1)%rs][y].coord[3] = 1.0;
			ben_cone.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			ben_cone.normals[x][(y+1)%vs].coord[3] = 1.0;
		}
	}

	normalize(ben_cone);
}

//Draw torus shape
//Compute torus vertex normals
void draw_torus(int rs, int vs)
{
	Particle p0, p1, p2, answer;

	for (int x = 0; x < rs; x++)
	{
		for (int y = 0; y < vs; y++)
		{
			draw_param_quad(ben_torus.vertices, x, y, BLUE);

			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = ben_torus.vertices[x][y].coord[j];
				p1.coord[j] = ben_torus.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = ben_torus.vertices[(x+1)%rs][(y+1)%vs].coord[j];
			}
			
			cross_product(answer, p1, p0, p2);

			//Add answer to all vertices that lie on the face that answer is orthogonal to
			//Vector addition
			for (int j = 0; j < 3; j++)
			{
				ben_torus.normals[x][y].coord[j] += answer.coord[j];
				ben_torus.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
				ben_torus.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				ben_torus.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			ben_torus.normals[x][y].coord[3] = 1.0;
			ben_torus.normals[(x+1)%rs][y].coord[3] = 1.0;
			ben_torus.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			ben_torus.normals[x][(y+1)%vs].coord[3] = 1.0;
		}
	}

	normalize(ben_torus);
}


//Calls object drawing functions
void draw_object(int shape) {

  switch(shape){
  case HOUSE: draw_house(); break;
  case CUBE: draw_cube_brute(); break; 
  case CYLINDER: draw_cylinder(crt_rs, crt_vs);  break;
  case SPHERE: draw_sphere(crt_rs, crt_vs);  break;
  case TORUS: draw_torus(crt_rs, crt_vs); ; break;
  case MESH: /*TODO EC: call your function here*/ ; break;
  case CONE: draw_cone(crt_rs, crt_vs); break;

  default: break;
  }

}



//main drawing function
void my_display(void) {
  
	/* clear the buffer */
  /* NEW: now we have to clear depth as well */
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) ;

  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity();
  gluLookAt(0.0, 5.0, 25.0,  // x,y,z coord of the camera 
	    0.0, 0.0, 0.0,  // x,y,z coord of the origin
	    0.0, 1.0, 0.0); // the direction of up (default is y-axis)

  glRotatef(theta_y,0,1,0);
  glRotatef(theta_x,1,0,0);
  
  //draw selected shape
  draw_object(crt_shape);

  if (display_normals)
  {
	  switch(crt_shape)	//draw normals
	  {
	    //case CUBE: break;
		case CYLINDER: draw_normals(ben_cylinder, crt_rs, crt_vs);  break;
		case SPHERE: draw_normals(ben_sphere, crt_rs, crt_vs);  break;
		case TORUS: draw_normals(ben_torus, crt_rs, crt_vs); ; break;
		case CONE: draw_normals(ben_cone, crt_rs, crt_vs); break;
	  }
  }

  /* buffer is ready */
  glutSwapBuffers();
	
  return ;
}



