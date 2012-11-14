//NAME: Ben Isenberg
//DATE: 10/08/12
//EMAIL: bji6@pitt.edu
//CS1566 Project 3

//VisS will not let through math.h constants such as M_PI w/out the line below
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include "BenIsenberg-Project3.h"

//Shape parameters
double cyl_height=2;
double cyl_ray=1;
double sph_ray=1;
double cone_h = 2.0;
double torus_r1 = 2.0;
double torus_r2 = 0.75;

//Camera values
double x_camera = 0, y_camera = 5, z_camera = 30;

//Global variables to keep track of program settings
int crt_render_mode;
int crt_shape, crt_rs, crt_vs;
int crt_transform;
int spin_val = SPIN_DISABLED;

//determines whether to display vertex normals or not
bool display_normals = false;  

Shape ben_sphere;	//global sphere object
Shape ben_cylinder; //global cylinder object
Shape ben_cone;		//global cone object
Shape ben_torus;	//global torus object
Shape ben_cube;		//global cube object
Shape ben_house;	//global house object

Shape* s_ptr; //Shape pointer representing the shape currently being displayed




//main function; starts GL, GLU, GLUT, then loops 
int main(int argc, char** argv)
{	
  glutInit(&argc, argv);
  glut_setup();
  gl_setup();
  my_setup();

  glutMainLoop();

  return(0);

}


// This function sets up the windowing related glut calls 
void glut_setup ()
{
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutInitWindowSize(700,700);
  glutInitWindowPosition(20,20);
  glutCreateWindow("Ben Isenberg - Transformer");


  // set up callback functions
  glutDisplayFunc(my_display);
  glutReshapeFunc(my_reshape);
  glutMouseFunc(my_mouse);
  glutKeyboardFunc(my_keyboard);
  glutKeyboardUpFunc( my_keyboard_up );

  // schedule a my_TimeOut call with the ID 0 in 20 seconds from now
  glutTimerFunc( 20, my_TimeOut, 0 );

  return;

}


//sets up OpenGL (name starts with gl but not followed by capital letter, so it's ours)
void gl_setup(void) 
{
  // enable depth handling (z-buffer)
  glEnable(GL_DEPTH_TEST);

  // define the background color 
  glClearColor(0,0,0,1);

  glMatrixMode(GL_PROJECTION) ;

  glLoadIdentity() ;

  gluPerspective( 20, 1.0, 15, 100.0);

  glMatrixMode(GL_MODELVIEW) ;

  glLoadIdentity() ;  // init modelview to identity

  return ;

}


//Calls functions to pre-compute shape vertices and presets some values
void my_setup()
{
  crt_render_mode = GL_POLYGON;
  
  crt_shape = HOUSE;
  
  //Set pointer to correct initial shape
  s_ptr = &ben_house;

  crt_transform = NONE_MODE;
  
  //Initial number of radial stacks
  crt_rs = 20;
  //Initial number of vertical slices
  crt_vs = 10;
  
  //Create cube vertices
  make_cube();
  //Create house vertices
  make_house();
  //Compute cylinder vertices
  make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);
  //Compute sphere vertices
  make_sphere(sph_ray,crt_rs,crt_vs);
  //Compute con vertices
  make_cone(cone_h, cyl_height, crt_rs, crt_vs);
  //compute torus vertices
  make_torus(torus_r1, torus_r2, crt_rs, crt_vs);
  
  return;

}



//remaps viewport to the Window Manager's window
void my_reshape(int w, int h) 
{
  // ensure a square view port
  glViewport(0,0,min(w,h),min(w,h)) ;
  return ;

}

//makes shape spin around its vertical (local Y) axis
void real_rev(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) 
{
	//Rotate around shape's local axis (either x, y, or z based on arguments)
	if ( x == 1)
	{
		real_rot_arbitrary_axis_point(deg, s_ptr->local_x.coord[0], s_ptr->local_x.coord[1], s_ptr->local_x.coord[2], 
									  s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2], y, z);
	}
	if (y == 1)
	{
		real_rot_arbitrary_axis_point(deg, s_ptr->local_y.coord[0], s_ptr->local_y.coord[1], s_ptr->local_y.coord[2], 
									  s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2], y, z);

		//printf("w vector = %f, %f, %f, %f\n", s_ptr->vertices[0][2].coord[0], s_ptr->vertices[0][2].coord[1], s_ptr->vertices[0][2].coord[2], s_ptr->vertices[0][2].coord[3]);
		//printf("Point = %f, %f, %f, %f\n", s_ptr->vertices[0][8].coord[0], s_ptr->vertices[0][8].coord[1], s_ptr->vertices[0][8].coord[2], s_ptr->vertices[0][8].coord[3]);
		
		//real_rot_arbitrary_axis_point(deg, s_ptr->vertices[0][2].coord[0], s_ptr->vertices[0][2].coord[1], s_ptr->vertices[0][2].coord[2], 
		//							s_ptr->vertices[0][8].coord[0], s_ptr->vertices[0][8].coord[1], s_ptr->vertices[0][8].coord[2], y, z);
		
		
				
	}

	if (z == 1)
	{
		real_rot_arbitrary_axis_point(deg, s_ptr->local_z.coord[0], s_ptr->local_z.coord[1], s_ptr->local_z.coord[2], 
									  s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2], y, z);
	}

}

//makes shape spin around arbitrary axis passing through arbitrary point
void real_rot_arbitrary_axis_point(GLfloat deg, double ax, double ay, double az, double cx, double cy, double cz, int y_change, int z_change ) 
{
	double theta = deg*(PI/180.0); //convert degrees to radians

	//Matrix to translate vertices of shape to origin
	double translation_to_origin[4][4] = { {1.0, 0.0, 0.0, -cx},
											{0.0, 1.0, 0.0, -cy},
											{0.0, 0.0, 1.0, -cz},
											{0.0, 0.0, 0.0, 1.0} };
	
	//Inverse of above matrix
	double undo_translate[4][4] = { {1.0, 0.0, 0.0, cx},
									{0.0, 1.0, 0.0, cy},
									{0.0, 0.0, 1.0, cz},
									{0.0, 0.0, 0.0, 1.0} };
	
	//Represent local coordinate system
	Particle w_vector;
	Particle v_vector;
	Particle u_vector;

	//Set w to arbitrary axis given to us
	w_vector.coord[0] = ax;
	w_vector.coord[1] = ay;
	w_vector.coord[2] = az;
	w_vector.coord[3] = 0.0;

	//Create local coordinate system based on arbitrary axis provided

	double magnitude;

	//Find magnitude of w to normalize it
	magnitude = find_magnitude(w_vector);
	
	//printf("magnitude = %f\n", magnitude);
	//Normalize by magnitude
	//Don't include homogeneous w-coordinate
	for (int j = 0; j < 3; j++)
	{
		w_vector.coord[j] /= magnitude;
		//printf("W coord %d = %f\n", j, w_vector.coord[j]);
	}

	//double big_matrix[4][4] = { { (w_vector.coord[0]*w_vector.coord[0]) + (cos(theta)*(1-(w_vector.coord[0]*w_vector.coord[0]))), ((w_vector.coord[0]*w_vector.coord[1])*(1-cos(theta)))-(w_vector.coord[2]*sin(theta)), ((w_vector.coord[2]*w_vector.coord[0])*(1-cos(theta)))+(w_vector.coord[1]*sin(theta)), 0.0},
	//							{ ((w_vector.coord[0]*w_vector.coord[1])*(1-cos(theta)))+(w_vector.coord[2]*sin(theta)), (w_vector.coord[1]*w_vector.coord[1])+(cos(theta)*(1-(w_vector.coord[1]*w_vector.coord[1]))), (w_vector.coord[1]*w_vector.coord[2]*(1-cos(theta)))-(w_vector.coord[0]*sin(theta)), 0.0},
	//							{ (w_vector.coord[2]*w_vector.coord[0]*(1-cos(theta)))-(w_vector.coord[1]*sin(theta)), (w_vector.coord[1]*w_vector.coord[2]*(1-cos(theta)))+(w_vector.coord[0]*sin(theta)), (w_vector.coord[2]*w_vector.coord[2])+(cos(theta)*(1-(w_vector.coord[2]*w_vector.coord[2]))), 0.0},
	//							{0.0, 0.0, 0.0, 1.0} };


	//Translate to origin first
	//real_translation(-cx, -cy, -cz);

	//printf("w point rotation = %f, %f, %f, %f\n", s_ptr->vertices[0][2].coord[0], s_ptr->vertices[0][2].coord[1], s_ptr->vertices[0][2].coord[2], s_ptr->vertices[0][2].coord[3]);

	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];

	//Apply transformation
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			for (int g = 0; g < 3; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
				//printf("Original vertices - %f\n", copy[g]);
			}
			copy[3] = 1.0;

			//matrix_multiplication(big_matrix, s_ptr->vertices[x][y], copy);
		}
	}

	

	//printf("w point rotation 2 = %f, %f, %f, %f\n", s_ptr->vertices[0][2].coord[0], s_ptr->vertices[0][2].coord[1], s_ptr->vertices[0][2].coord[2], s_ptr->vertices[0][2].coord[3]);

	/*for (int g = 0; g < 4; g++)
	{
		copy[g] = s_ptr->center.coord[g];
		//printf("Original vertices - %f\n", copy[g]);
	}*/

	//matrix_multiplication(big_matrix, s_ptr->center, copy);
	//printf("R1 center coords - %f, %f, %f\n", s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2]);
	
	//Translate back to point of interest
	/*real_translation(cx, cy, cz);

	int f_point_fix[3];

	for (int x = 0; x < 3; x++)
	{
		f_point_fix[x] = int(s_ptr->vertices[0][2].coord[x] + 0.5);
		s_ptr->vertices[0][2].coord[x] = double(f_point_fix[x]);
	}*/

	//printf("R2 center coords - %f, %f, %f\n", s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2]);

	//printf("w point rotation = %f, %f, %f, %f\n", s_ptr->vertices[0][2].coord[0], s_ptr->vertices[0][2].coord[1], s_ptr->vertices[0][2].coord[2], s_ptr->vertices[0][2].coord[3]);

	//V axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose world y-axis as default up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};

	//Up can't be y-axis if that's the axis w is aligned with so make Up vector the x-axis instead
	if (y_change == 1)
	{
		up_vector[1] = 0.0;
		up_vector[0] = 1.0;
	}
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*w_vector.coord[0]) + (up_vector[1]*w_vector.coord[1]) + (up_vector[2]*w_vector.coord[2]);

	//printf("up . w = %f\n", up_dot_w);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (w_vector.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		v_vector.coord[i] = up_vector[i] - w_scaled[i];
		//printf("v vector in creation - %f\n", v_vector.coord[i]);
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(v_vector);
			
	//Normalize by magnitude
	//Don't include homogeneous w-coordinate
	for (int j = 0; j < 3; j++)
	{
		v_vector.coord[j] /= magnitude;
	}

	//U axis = w cross v

	//Cross product
	u_vector.coord[0] = (w_vector.coord[1]*v_vector.coord[2]) - (w_vector.coord[2]*v_vector.coord[1]);
	u_vector.coord[1] = (w_vector.coord[2]*v_vector.coord[0]) - (w_vector.coord[0]*v_vector.coord[2]);
	u_vector.coord[2] = (w_vector.coord[0]*v_vector.coord[1]) - (w_vector.coord[1]*v_vector.coord[0]);
	u_vector.coord[3] = 0.0; // w coordinate

	//Find magnitude of local z to normalize it
	magnitude = find_magnitude(u_vector);
			
	//Normalize by magnitude
	//Don't include homogeneous w-coordinate
	for (int j = 0; j < 3; j++)
	{
		u_vector.coord[j] /= magnitude;
	}

	/*for (int i = 0; i < 4; i++)
	{
		printf("w - %f\n", w_vector.coord[i]);
		printf("v - %f\n", v_vector.coord[i]);
		printf("u - %f\n", u_vector.coord[i]);
	}*/

	//Matrix to align local coordinate system with global system after its been translated
	double rotation[4][4] = { {w_vector.coord[0], w_vector.coord[1], w_vector.coord[2], 0.0},
							  {v_vector.coord[0], v_vector.coord[1], v_vector.coord[2], 0.0},
							  {u_vector.coord[0], u_vector.coord[1], u_vector.coord[2], 0.0},
							  {0.0, 0.0, 0.0, 1.0} };

	//Inverse of above matrix
	double r_inverse[4][4] = { {w_vector.coord[0], v_vector.coord[0], u_vector.coord[0], 0.0},
							   {w_vector.coord[1], v_vector.coord[1], u_vector.coord[1], 0.0},
							   {w_vector.coord[2], v_vector.coord[2], u_vector.coord[2], 0.0},
							   {0.0, 0.0, 0.0, 1.0} };

	//Default is rotation around x
	double r_x[4][4] = { {1.0, 0.0, 0.0, 0.0},
						 {0.0, cos(theta), -sin(theta), 0.0},
						 {0.0, sin(theta), cos(theta), 0.0},
						 {0.0, 0.0, 0.0, 1.0} };


	//Translate all vertices to origin
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
				//printf("Original vertices - %f\n", copy[g]);
			}

			matrix_multiplication(translation_to_origin, s_ptr->vertices[x][y], copy);
		}
	}

	//Align local axes with global coordinate system
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
				//printf("Before multiplication - %f\n", copy[g]);
			}

			matrix_multiplication(rotation, s_ptr->vertices[x][y], copy);
		}
	}

	//Apply desired rotation by degrees given
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
			}

			matrix_multiplication(r_x, s_ptr->vertices[x][y], copy);
		}
	}

	//Undo alignment of local coordinate system with global system
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
			}

			matrix_multiplication(r_inverse, s_ptr->vertices[x][y], copy);
		}
	}


	//Translate all vertices back to their original positions
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
			}

			matrix_multiplication(undo_translate, s_ptr->vertices[x][y], copy);
		}
	}


}



//Translates shapes in space by moving their vertices based on values given
void real_translation(GLfloat x, GLfloat y, GLfloat z) 
{	
	double translation[4][4] = { {1.0, 0.0, 0.0, x},
								 {0.0, 1.0, 0.0, y},
								 {0.0, 0.0, 1.0, z},
								 {0.0, 0.0, 0.0, 1.0} };
	
	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];

	//Apply translation matrix to all vertices
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			if ( x < 1)
			{
				//printf("B4 Translate! = %f, %f, %f\n", s_ptr->vertices[x][y].coord[0], s_ptr->vertices[x][y].coord[1], s_ptr->vertices[x][y].coord[2]);
			}
			//First copy vertex coordinates to copy array
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
			}

			matrix_multiplication(translation, s_ptr->vertices[x][y], copy);
			if ( x < 1)
			{	
				//printf("Translate! = %f, %f, %f\n", s_ptr->vertices[x][y].coord[0], s_ptr->vertices[x][y].coord[1], s_ptr->vertices[x][y].coord[2]);
			}
		}
	}

	//Apply transformation to Particle that represents center of object
	double copy2[4];

	//First copy vertex coordinates to copy array
	for (int g = 0; g < 4; g++)
	{
		copy2[g] = s_ptr->center.coord[g];
	}


	//printf("OLD center coords - %f, %f, %f\n", s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2]);
	matrix_multiplication(translation, s_ptr->center, copy2);
	//printf("NEW center coords - %f, %f, %f\n", s_ptr->center.coord[0], s_ptr->center.coord[1], s_ptr->center.coord[2]);
}


//Scales an object by applying scaling matrix to all of its vertices
void real_scaling(GLfloat sx, GLfloat sy, GLfloat sz) 
{
	double scale[4][4] = { {sx, 0.0, 0.0, 0.0},
						   {0.0, sy, 0.0, 0.0},
						   {0.0, 0.0, sz, 0.0},
						   {0.0, 0.0, 0.0, 1.0} };
	
	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];

	//Apply scaling matrix to all vertices
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			//First copy vertex coordinates to copy array
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[x][y].coord[g];
			}

			matrix_multiplication(scale, s_ptr->vertices[x][y], copy);
		}
	}

	//Apply transformation to Particle that represents center of object
	double copy2[4];

	//First copy vertex coordinates to copy array
	for (int g = 0; g < 4; g++)
	{
		copy2[g] = s_ptr->center.coord[g];
	}

	matrix_multiplication(scale, s_ptr->center, copy2);
	
}

//Rotates an object around either world x, y, or z axis by 'deg' degrees
void real_rotation(GLfloat deg, GLfloat x, GLfloat y, GLfloat z) 
{	
	double theta = deg*(PI/180); //convert degrees to radians
	
	
	double rotation_x[4][4] = { {1.0, 0.0, 0.0, 0.0},
								{0.0, cos(theta), -sin(theta), 0.0},
								{0.0, sin(theta), cos(theta), 0.0},
								{0.0, 0.0, 0.0, 1.0} };

	double rotation_y[4][4] = { {cos(theta), 0.0, sin(theta), 0.0},
								{0.0, 1.0, 0.0, 0.0},
								{-sin(theta), 0.0, cos(theta), 0.0},
								{0.0, 0.0, 0.0, 1.0} };

	double rotation_z[4][4] = { {cos(theta), -sin(theta), 0.0, 0.0},
								{sin(theta), cos(theta), 0.0, 0.0},
								{0.0, 0.0, 1.0, 0.0},
								{0.0, 0.0, 0.0, 1.0} };

	
	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];

	//Apply rotation matrix to all vertices
	for (int j = 0;  j < crt_rs; j++)
	{
		for (int i = 0; i < crt_vs; i++)
		{
			//First copy vertex coordinates to copy array
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr->vertices[j][i].coord[g];
			}
			
			if (x == 1.0)
			{
				matrix_multiplication(rotation_x, s_ptr->vertices[j][i], copy);
			}
			else if (y == 1.0)
			{
				matrix_multiplication(rotation_y, s_ptr->vertices[j][i], copy);
			}
			else if (z == 1.0)
			{
				matrix_multiplication(rotation_z, s_ptr->vertices[j][i], copy);
			}
		}
	}

	//Apply transformation to Particle that represents center of object AND local coordinate axes
	double copy2[4];
	double copy3[4];
	double copy4[4];
	double copy5[4];

	//First copy vertex coordinates to copy arrays
	for (int g = 0; g < 4; g++)
	{
		copy2[g] = s_ptr->center.coord[g];
		copy3[g] = s_ptr->local_x.coord[g];
		copy4[g] = s_ptr->local_y.coord[g];
		copy5[g] = s_ptr->local_z.coord[g];
	}

	if (x == 1.0)
	{
		matrix_multiplication(rotation_x, s_ptr->center, copy2);
		matrix_multiplication(rotation_x, s_ptr->local_x, copy3);
		matrix_multiplication(rotation_x, s_ptr->local_y, copy4);
		matrix_multiplication(rotation_x, s_ptr->local_z, copy5);
	}
	else if (y == 1.0)
	{
		matrix_multiplication(rotation_y, s_ptr->center, copy2);
		matrix_multiplication(rotation_y, s_ptr->local_x, copy3);
		matrix_multiplication(rotation_y, s_ptr->local_y, copy4);
		matrix_multiplication(rotation_y, s_ptr->local_z, copy5);
	}
	else if (z == 1.0)
	{
		matrix_multiplication(rotation_z, s_ptr->center, copy2);
		matrix_multiplication(rotation_z, s_ptr->local_x, copy3);
		matrix_multiplication(rotation_z, s_ptr->local_y, copy4);
		matrix_multiplication(rotation_z, s_ptr->local_z, copy5);
	}

}


//Reset shapes back to the origin
void reset()
{
  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity() ;
  x_camera = 0; y_camera = 5; z_camera =30;

  //Reset shapes
   switch(crt_shape)
  {
		case HOUSE:		s_ptr = &ben_house;
						make_house(); 
						break;
		case CUBE:		s_ptr = &ben_cube;
						make_cube();
						break;
		case CYLINDER:	s_ptr = &ben_cylinder;
						make_cylinder(cyl_height,cyl_ray,crt_rs,crt_vs);	
						break;
		case SPHERE:	s_ptr = &ben_sphere;
						make_sphere(sph_ray,crt_rs,crt_vs); 
						break;
		case TORUS:		s_ptr = &ben_torus;
						make_torus(torus_r1, torus_r2, crt_rs, crt_vs);
						break;
		case CONE:		s_ptr = &ben_cone;
						make_cone(cone_h, cyl_height, crt_rs, crt_vs); 
						break;

		default:		break;
  }

  //Reset vertex normals
  reset_normals(*s_ptr);

}


//Handles keyboard events
void my_keyboard( unsigned char key, int x, int y ) 
{
  //printf("%c - key\n", key);

  //spin_val = SPIN_DISABLED;
  
  switch( key ) 
  {
	//This resets the shape
	case 'O':
	case 'o': {
				reset();
				glutPostRedisplay(); 
			  }; 
			  break;
	//Transformation based on Y axis
	case 'y': {
				switch( crt_transform) 
				{
					case REAL_ROTATION_MODE: real_rotation(-2,0,1,0); 
											 reset_normals(*s_ptr);
											 break;
					
					case REAL_TRANSLATION_MODE: real_translation(0,-1,0); 
												reset_normals(*s_ptr);
												break;
					
					case REAL_SCALING_MODE: real_scaling(1, (1/1.2), 1);
											reset_normals(*s_ptr);
											break;
				}
				glutPostRedisplay(); 
			}; 
			break;
	//Transformation based on Y axis
	case 'Y': {
				switch( crt_transform) 
				{
					case REAL_ROTATION_MODE: real_rotation(2,0,1,0); 
											 reset_normals(*s_ptr);
											 break;
					
					case REAL_TRANSLATION_MODE: real_translation(0,1,0); 
												reset_normals(*s_ptr);
												break;
					
					case REAL_SCALING_MODE: real_scaling(1, 1.2, 1);
											reset_normals(*s_ptr);
											break;
				}
				glutPostRedisplay(); 
			}; 
			break;
	//Transformation based on X axis
	case 'x': {
				//spin_val = SPIN_DISABLED;
				switch( crt_transform) 
				{
					case REAL_ROTATION_MODE: real_rotation(-2,1,0,0); 
											 reset_normals(*s_ptr);
											 break;
					
					case REAL_TRANSLATION_MODE: 
												real_translation(-1,0,0);
												reset_normals(*s_ptr);
												break;
					
					case REAL_SCALING_MODE: real_scaling((1/1.2), 1, 1);
											reset_normals(*s_ptr);
											break;
				}
				glutPostRedisplay();
				//spin_val = SPIN_ENABLED;
			}; 
			break;
	//Transformation based on X axis
	case 'X': {
				switch( crt_transform) 
				{
					case REAL_ROTATION_MODE: real_rotation(2,1,0,0); 
											 reset_normals(*s_ptr);
											 break;
					
					case REAL_TRANSLATION_MODE: real_translation(1,0,0); 
												reset_normals(*s_ptr);
												break;

					case REAL_SCALING_MODE: real_scaling(1.2, 1, 1);
											reset_normals(*s_ptr);
											break;
				}
				glutPostRedisplay(); 
			}; 
			break;
	//Transformation based on Z axis
	case 'z': {
				switch( crt_transform) 
				{
					case REAL_ROTATION_MODE: real_rotation(-2,0,0,1); 
											 reset_normals(*s_ptr);
											 break;
					
					case REAL_TRANSLATION_MODE: real_translation(0,0,-1);
												reset_normals(*s_ptr);
												break;
					
					case REAL_SCALING_MODE: real_scaling(1, 1, (1/1.2));
											reset_normals(*s_ptr);
											break;
				}
				glutPostRedisplay(); 
			}; 
			break;
	//Transformation based on Z axis
	case 'Z': {
				switch( crt_transform) 
				{
					case REAL_ROTATION_MODE: real_rotation(2,0,0,1); 
											 reset_normals(*s_ptr);
											 break;
					
					case REAL_TRANSLATION_MODE: real_translation(0,0,1);
												reset_normals(*s_ptr);
												break;
					
					case REAL_SCALING_MODE: real_scaling(1, 1, 1.2);
											reset_normals(*s_ptr);
											break;
				}
				glutPostRedisplay(); 
			}; 
			break;
	//Draw a different shape based on pressing a number on the keyboard
	case '1': {
				crt_shape = HOUSE;
				glutPostRedisplay();
			  };
			  break;
	case '2': {
				crt_shape = CUBE;
				glutPostRedisplay();
			  }; 
			  break;
	case '3': {
				crt_shape = CYLINDER;
				glutPostRedisplay();
			  }; 
			  break;
	case '4': {
				crt_shape = SPHERE;
				glutPostRedisplay();
			  }; 
			  break;
	case '5': {
				crt_shape = TORUS;
				glutPostRedisplay();
			  }; 
			  break; 
	case '6': {
				crt_shape = CONE;
				glutPostRedisplay();
			  };
			  break;
	//Scaling transformation
	case 'S':
	case 's': {
				crt_transform = REAL_SCALING_MODE;
			  }; 
			  break;
	//Translation transformation
	case 'T':
	case 't': {
				crt_transform = REAL_TRANSLATION_MODE;
			  }; 
			  break;
	//Rotation transformation
	case 'R':
	case 'r': {
				crt_transform = REAL_ROTATION_MODE;
			  }; 
			  break;
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
					make_cone(cone_h, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(torus_r1, torus_r2, crt_rs, crt_vs);
				}
				//Reset vertex normals when tessellation changes
				reset_normals(ben_cylinder);
				reset_normals(ben_sphere);
				reset_normals(ben_cone);
				reset_normals(ben_torus);

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
					make_cone(cone_h, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(torus_r1, torus_r2, crt_rs, crt_vs);
				}

				//Reset vertex normals when tessellation changes
				reset_normals(ben_cylinder);
				reset_normals(ben_sphere);
				reset_normals(ben_cone);
				reset_normals(ben_torus);

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
					make_cone(cone_h, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(torus_r1, torus_r2, crt_rs, crt_vs);
				}
				
				//Reset vertex normals when tessellation changes
				reset_normals(ben_cylinder);
				reset_normals(ben_sphere);
				reset_normals(ben_cone);
				reset_normals(ben_torus);

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
					make_cone(cone_h, cyl_height, crt_rs, crt_vs);
					//compute torus vertices
					make_torus(torus_r1, torus_r2, crt_rs, crt_vs);
				}
				//Reset vertex normals when tessellation changes
				reset_normals(ben_cylinder);
				reset_normals(ben_sphere);
				reset_normals(ben_cone);
				reset_normals(ben_torus);
				
				//Display changes
				glutPostRedisplay();
			  }; 
		      break;
	//Exit program
	case 'q': 
	case 'Q':	exit(0);
				break ;	

	default: break;

  }

  
  //spin_val = SPIN_ENABLED;

  return ;

}

//Resets transform mode when keys aren't pressed anymore
void my_keyboard_up( unsigned char key, int x, int y ) 
{
	switch( key ) 
	{
		case 'S':
		case 's': 
		case 'T':
		case 't':
		case 'R':
		case 'r': {
					crt_transform = NONE_MODE; 
					break;
				  }
	}

}




//Handles mouse events
void my_mouse(int button, int state, int mousex, int mousey) 
{
	switch( button ) 
	{	
		case GLUT_LEFT_BUTTON:	//Left clicks turn on and off spinning of shapes
								if( state == GLUT_DOWN ) 
								{
 									if(spin_val == SPIN_ENABLED)
									{
										spin_val = SPIN_DISABLED;
									}
									else
									{
										spin_val = SPIN_ENABLED;
									}
		
									glutPostRedisplay();
								}

								if( state == GLUT_UP ) 
								{}

								break ;

		case GLUT_RIGHT_BUTTON: //Right clicks display shapes in wire frame with vertex normals
								if( state == GLUT_DOWN ) 
								{
									if(crt_render_mode == GL_LINE_LOOP)
									{
										crt_render_mode = GL_POLYGON;
									}
									else 
									{
										crt_render_mode = GL_LINE_LOOP;
									}

									//Toggle displaying normals
									display_normals = !display_normals;

									glutPostRedisplay();
								}

								if( state == GLUT_UP ) 
								{}
								
								break ;

	}
  
  return ;

}



//Draws a quad in counterclockwise order
void draw_quad(Particle vertices[50][50], int iv1, int iv2, int iv3, int iv4, int ic) 
{
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);
    
    glVertex4dv(vertices[0][iv1].coord);
    glVertex4dv(vertices[0][iv2].coord);
    glVertex4dv(vertices[0][iv3].coord);
    glVertex4dv(vertices[0][iv4].coord);
  }
  glEnd();
}


//Draws 4 vertices of a Shape object in counter clockwise order
void draw_param_quad(Particle vertices[50][50], int line, int col, int ic) 
{
	glBegin(crt_render_mode);
	{	
		glColor3fv(colors[ic]);
		
		//Draw vertices in counter clockwise order with glvertex for doubles
		glVertex4dv(vertices[line][col].coord);
		//Modulus so it reconnects the end of the rows with the beginning of the rows to create a full circle
		glVertex4dv(vertices[(line+1)%crt_rs][col].coord);
		glVertex4dv(vertices[(line+1)%crt_rs][(col+1)%crt_vs].coord);
		glVertex4dv(vertices[line][(col+1)%crt_vs].coord);
	}
	glEnd();
}



//Draws a triangle in counterclockwise order
void draw_triangle(Particle vertices[50][50], int iv1, int iv2, int iv3, int ic) 
{
  glBegin(crt_render_mode); 
  {
    glColor3fv(colors[ic]);

    glVertex4dv(vertices[0][iv1].coord);
    glVertex4dv(vertices[0][iv2].coord);
    glVertex4dv(vertices[0][iv3].coord);
  }
  glEnd();
}


//draws main X, Y, Z axes
void draw_axes( void ) 
{
	//Draw thick lines
    glLineWidth( 5.0 );

    glBegin(GL_LINES); 
	{
		glColor3fv(colors[1]);
		glVertex4fv(vertices_axes[0]);
		glVertex4fv(vertices_axes[1]);

		glColor3fv(colors[4]);
		glVertex4fv(vertices_axes[0]);
		glVertex4fv(vertices_axes[2]);

		glColor3fv(colors[6]);
		glVertex4fv(vertices_axes[0]);
		glVertex4fv(vertices_axes[3]);
	}
	glEnd();
	
	glLineWidth( 1.0 );

}

//Draws currently selected object
void draw_object(int shape) 
{

  switch(shape)
  {
		case HOUSE:		s_ptr = &ben_house;
						draw_house();
						break;
		case CUBE:		s_ptr = &ben_cube;
						draw_cube_brute();
						break;
		case CYLINDER:	s_ptr = &ben_cylinder;
						draw_cylinder(crt_rs, crt_vs);
						break;
		case SPHERE:	s_ptr = &ben_sphere;
						draw_sphere(crt_rs, crt_vs);
						break;
		case TORUS:		s_ptr = &ben_torus;
						draw_torus(crt_rs, crt_vs);
						break;
		case CONE:		s_ptr = &ben_cone;
						draw_cone(crt_rs, crt_vs);
						break;

		default:		break;
  }

}


//Cross product function
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

//Function to draw a shape's local coordinate system
void draw_local_coord_system()
{
	double v_prime[4];
	
	v_prime[3] = 1.0;


	glLineWidth(2.5);

	//Draw the axes as distinct lines
	glBegin(GL_LINES);
	{	
		glColor3fv(colors[RED]);
		
		//compute v prime
		for (int h = 0; h < 3; h++)
		{
			v_prime[h] = s_ptr->center.coord[h] + (3*s_ptr->local_x.coord[h]);
		}

		//Draw x axis
		glVertex4dv(s_ptr->center.coord);
		glVertex4dv(v_prime);

		glColor3fv(colors[GREEN]);

		//compute v prime
		for (int h = 0; h < 3; h++)
		{
			v_prime[h] = s_ptr->center.coord[h] + (3*s_ptr->local_y.coord[h]);
		}

		//Draw y axis
		glVertex4dv(s_ptr->center.coord);
		glVertex4dv(v_prime);

		glColor3fv(colors[BLUE]);

		//compute v prime
		for (int h = 0; h < 3; h++)
		{
			v_prime[h] = s_ptr->center.coord[h] + (3*s_ptr->local_z.coord[h]);
		}

		//Draw z axis
		glVertex4dv(s_ptr->center.coord);
		glVertex4dv(v_prime);
	}
	glEnd();

	glLineWidth(1.0);
}


//Draw normals for a shape
//Draw normal from v to v'
//v' = v + (0.2*normal of v)
void draw_normals(Shape& s, int rs, int vs)
{
	double v_prime[4];
	
	v_prime[3] = 1.0;

	//Cube & House have less vertices than other shapes so don't draw more than is needed
	if (crt_shape == CUBE)
	{
		rs = 1;
		vs = 8;
	}
	if (crt_shape == HOUSE)
	{
		rs = 1;
		vs = 9;
	}

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

//Function to reset vertex normals
void reset_normals(Shape& s)
{
	for (int x = 0; x < 50; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			s.normals[x][y].coord[0] = 0.0;
			s.normals[x][y].coord[1] = 0.0;
			s.normals[x][y].coord[2] = 0.0;
			s.normals[x][y].coord[3] = 0.0;
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


//main drawing function
void my_display()
{
  // clear all pixels, reset depth 
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
  
   // init to identity 
   glLoadIdentity() ;

   gluLookAt(x_camera, y_camera, z_camera,  // x,y,z coord of the camera 
			0.0, 0.0, 0.0,  // x,y,z LookAt
			0.0, 1.0, 0.0); // the direction of Up (default is y-axis)

   //Draw world coordinate system
   draw_axes();

   //Draw selected shape
   draw_object(crt_shape);

   //Draw local coordinate system of shape
   draw_local_coord_system();

   //If true then draw vertex normals
   if (display_normals)
   {
	   draw_normals(*s_ptr, crt_rs, crt_vs);
   }

  // this buffer is ready
  glutSwapBuffers();

}

//handles "timer" events
void my_TimeOut(int id) 
{
  // spin shape counter-clockwise  
  if(spin_val == SPIN_ENABLED)
  {
	//apply rotation around Y axis
	real_rev(3,0,1,0);
    
	//reset normals so they can be recalculated
	reset_normals((*s_ptr));

	glutPostRedisplay();

  }

  // schedule next timer event, 0.2 secs from now
  glutTimerFunc(20, my_TimeOut, 0);

  return ;
}


//handles the "idle" event
void my_idle(void) 
{}



//multiply a matrix with a vector in column form
//Set v as the answer
void matrix_multiplication(double m[4][4], Particle& v, double start[4])
{	
	//compute dot products of matrix rows with vertex
	for (int x = 0; x < 3; x++)
	{
		v.coord[x] = (m[x][0]*start[0]) + (m[x][1]*start[1]) + (m[x][2]*start[2]) + (m[x][3]*start[3]);
	}

	v.coord[3] = 1.0;

	return;
}


//Creates cube shape
void make_cube()
{
	//All shapes are centered at the origin
	ben_cube.center.coord[0] = 0.0;
	ben_cube.center.coord[1] = 0.0;
	ben_cube.center.coord[2] = 0.0;
	ben_cube.center.coord[3] = 1.0; //w

	//Place cube vertices in a Shape struct
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			ben_cube.vertices[0][x].coord[y] = vertices_cube_brute[x][y];
		}
	}

	//Create object's local coordinate system
	
	//Local X-axis
	double v_start[4] = {1.0, 0.0, 0.0, 1.0};

	for (int i = 0; i < 3;  i++)
	{
		ben_cube.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	ben_cube.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(ben_cube.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_cube.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*ben_cube.local_x.coord[0]) + (up_vector[1]*ben_cube.local_x.coord[1]) + (up_vector[2]*ben_cube.local_x.coord[2]) + (up_vector[3]*ben_cube.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (ben_cube.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		ben_cube.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(ben_cube.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_cube.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	ben_cube.local_z.coord[0] = (ben_cube.local_x.coord[1]*ben_cube.local_y.coord[2]) - (ben_cube.local_x.coord[2]*ben_cube.local_y.coord[1]);
	ben_cube.local_z.coord[1] = (ben_cube.local_x.coord[2]*ben_cube.local_y.coord[0]) - (ben_cube.local_x.coord[0]*ben_cube.local_y.coord[2]);
	ben_cube.local_z.coord[2] = (ben_cube.local_x.coord[0]*ben_cube.local_y.coord[1]) - (ben_cube.local_x.coord[1]*ben_cube.local_y.coord[0]);
	ben_cube.local_z.coord[3] = 0.0; // w coordinate
}

//Draws a cube and computes its vertex normals
void draw_cube_brute()
{

  draw_triangle(ben_cube.vertices, 4,5,1,BLUE);
  compute_cube_normals(4, 5, 1);

  draw_triangle(ben_cube.vertices, 0,4,1,BLUE);
  compute_cube_normals(0, 4, 1);
  
  draw_triangle(ben_cube.vertices, 5,6,2,CYAN);
  compute_cube_normals(5, 6, 2);
  
  draw_triangle(ben_cube.vertices, 1,5,2,CYAN);
  compute_cube_normals(1, 5, 2);
  
  draw_triangle(ben_cube.vertices, 3,2,6,YELLOW);
  compute_cube_normals(3, 2, 6);
  
  draw_triangle(ben_cube.vertices, 7,3,6,YELLOW);
  compute_cube_normals(7, 3, 6);
  
  draw_triangle(ben_cube.vertices, 0,3,7,MAGENTA);
  compute_cube_normals(0, 3, 7);
  
  draw_triangle(ben_cube.vertices, 4,0,7,MAGENTA);
  compute_cube_normals(4, 0, 7);

  //Draw a top to the cube with triangles
  draw_triangle(ben_cube.vertices, 0,1,2,GREY);
  compute_cube_normals(0, 1, 2);
  draw_triangle(ben_cube.vertices, 3,0,2,GREY);
  compute_cube_normals(3, 0, 2);

  //Draw a bottom to the cube with triangles
  draw_triangle(ben_cube.vertices, 4,5,6,GREEN);
  compute_cube_normals(4, 5, 6);
  draw_triangle(ben_cube.vertices, 7,4,6,GREEN);
  compute_cube_normals(7, 4, 6);

  float m;

  //Normalize vertex normals
  for (int x = 0; x < 8; x++)
  {
	  //Don't include w coordinate
	  m = (ben_cube.normals[0][x].coord[0]*ben_cube.normals[0][x].coord[0]) + (ben_cube.normals[0][x].coord[1]*ben_cube.normals[0][x].coord[1]) + (ben_cube.normals[0][x].coord[2]*ben_cube.normals[0][x].coord[2]);
	  //take square root
	  m = sqrt(m);
	  ben_cube.normals[0][x].coord[0] /= m;
	  ben_cube.normals[0][x].coord[1] /= m;
	  ben_cube.normals[0][x].coord[2] /= m;
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
		p0[x] = s_ptr->vertices[0][b].coord[x];
		p1[x] = s_ptr->vertices[0][a].coord[x];
		p2[x] = s_ptr->vertices[0][c].coord[x];
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
		s_ptr->normals[0][a].coord[g] += answer[g];
		s_ptr->normals[0][b].coord[g] += answer[g];
		s_ptr->normals[0][c].coord[g] += answer[g];
	}

	s_ptr->normals[0][a].coord[3] = 1.0;
	s_ptr->normals[0][b].coord[3] = 1.0;
	s_ptr->normals[0][c].coord[3] = 1.0;

	return;
}

//Creates house shape
void make_house()
{
	//All shapes are centered at the origin
	ben_house.center.coord[0] = 0.0;
	ben_house.center.coord[1] = 0.0;
	ben_house.center.coord[2] = 0.0;
	ben_house.center.coord[3] = 1.0; //w

	//Place house vertices in a Shape struct
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			ben_house.vertices[0][x].coord[y] = vertices_house[x][y];
		}
	}

	//Create object's local coordinate system
	
	//Local X-axis - v_start/magnitude

	double v_start[4] = {1.0, 0.0, 0.0, 1.0};

	for (int i = 0; i < 3;  i++)
	{
		ben_house.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	ben_house.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(ben_house.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_house.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*ben_house.local_x.coord[0]) + (up_vector[1]*ben_house.local_x.coord[1]) + (up_vector[2]*ben_house.local_x.coord[2]) + (up_vector[3]*ben_house.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (ben_house.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		ben_house.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(ben_house.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_house.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	ben_house.local_z.coord[0] = (ben_house.local_x.coord[1]*ben_house.local_y.coord[2]) - (ben_house.local_x.coord[2]*ben_house.local_y.coord[1]);
	ben_house.local_z.coord[1] = (ben_house.local_x.coord[2]*ben_house.local_y.coord[0]) - (ben_house.local_x.coord[0]*ben_house.local_y.coord[2]);
	ben_house.local_z.coord[2] = (ben_house.local_x.coord[0]*ben_house.local_y.coord[1]) - (ben_house.local_x.coord[1]*ben_house.local_y.coord[0]);
	ben_house.local_z.coord[3] = 0.0; // w coordinate
}

//Draws house object
void draw_house()
{
  //printf("w point draw = %f, %f, %f, %f\n", s_ptr->vertices[0][2].coord[0], s_ptr->vertices[0][2].coord[1], s_ptr->vertices[0][2].coord[2], s_ptr->vertices[0][2].coord[3]);

  draw_triangle(ben_house.vertices,0,1,2,RED);
  compute_cube_normals(0,1,2);
  draw_triangle(ben_house.vertices,0,2,3,GREEN);
  compute_cube_normals(0,2,3);
  draw_triangle(ben_house.vertices,0,3,4,WHITE);
  compute_cube_normals(0,3,4);
  draw_triangle(ben_house.vertices,0,4,1,GREY);
  compute_cube_normals(0,4,1);

  draw_quad(ben_house.vertices,2,1,5,6, BLUE);

  draw_triangle(ben_house.vertices,2,6,3, CYAN);
  compute_cube_normals(2,6,3);
  draw_triangle(ben_house.vertices,3,6,7, CYAN);
  compute_cube_normals(3,6,7);
  draw_triangle(ben_house.vertices,3,7,8, YELLOW);
  compute_cube_normals(3,7,8);
  draw_triangle(ben_house.vertices,8,3,4, YELLOW);
  compute_cube_normals(8,3,4);
  draw_triangle(ben_house.vertices,4,8,1, MAGENTA);
  compute_cube_normals(4,8,1);
  draw_triangle(ben_house.vertices,1,8,5, MAGENTA);
  compute_cube_normals(1,8,5);

  float m;

  //Normalize vertex normals
  for (int x = 0; x < 9; x++)
  {
	  //Don't include w coordinate
	  m = (ben_house.normals[0][x].coord[0]*ben_house.normals[0][x].coord[0]) + (ben_house.normals[0][x].coord[1]*ben_house.normals[0][x].coord[1]) + (ben_house.normals[0][x].coord[2]*ben_house.normals[0][x].coord[2]);
	  //take square root
	  m = sqrt(m);
	  ben_house.normals[0][x].coord[0] /= m;
	  ben_house.normals[0][x].coord[1] /= m;
	  ben_house.normals[0][x].coord[2] /= m;
  }

}


//Compute vertices of a cylinder
void make_cylinder(double height, double ray, int rs, int vs)
{
	//All shapes are centered at the origin
	ben_cylinder.center.coord[0] = 0.0;
	ben_cylinder.center.coord[1] = 0.0;
	ben_cylinder.center.coord[2] = 0.0;
	ben_cylinder.center.coord[3] = 1.0; //w

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
			//printf("%f\n", theta*(180/PI));
		}

		//Update height
		h += (height/(rs-1));
		//printf("\n");

		//Update translation matrix
		translation_y[1][3] = h;
	}


	//Create object's local coordinate system
	
	//Local X-axis
	v_start[1] = 0.0;

	for (int i = 0; i < 3;  i++)
	{
		ben_cylinder.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	ben_cylinder.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(ben_cylinder.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_cylinder.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*ben_cylinder.local_x.coord[0]) + (up_vector[1]*ben_cylinder.local_x.coord[1]) + (up_vector[2]*ben_cylinder.local_x.coord[2]) + (up_vector[3]*ben_cylinder.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (ben_cylinder.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		ben_cylinder.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(ben_cylinder.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_cylinder.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	ben_cylinder.local_z.coord[0] = (ben_cylinder.local_x.coord[1]*ben_cylinder.local_y.coord[2]) - (ben_cylinder.local_x.coord[2]*ben_cylinder.local_y.coord[1]);
	ben_cylinder.local_z.coord[1] = (ben_cylinder.local_x.coord[2]*ben_cylinder.local_y.coord[0]) - (ben_cylinder.local_x.coord[0]*ben_cylinder.local_y.coord[2]);
	ben_cylinder.local_z.coord[2] = (ben_cylinder.local_x.coord[0]*ben_cylinder.local_y.coord[1]) - (ben_cylinder.local_x.coord[1]*ben_cylinder.local_y.coord[0]);
	ben_cylinder.local_z.coord[3] = 0.0; // w coordinate

}



//Computes sphere vertices
void make_sphere(double ray, int rs, int vs)
{
	//All shapes are centered at the origin
	ben_sphere.center.coord[0] = 0.0;
	ben_sphere.center.coord[1] = 0.0;
	ben_sphere.center.coord[2] = 0.0;
	ben_sphere.center.coord[3] = 1.0; //w

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

	//Create object's local coordinate system
	//Local X-axis
	for (int i = 0; i < 3;  i++)
	{
		ben_sphere.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	ben_sphere.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(ben_sphere.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_sphere.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*ben_sphere.local_x.coord[0]) + (up_vector[1]*ben_sphere.local_x.coord[1]) + (up_vector[2]*ben_sphere.local_x.coord[2]) + (up_vector[3]*ben_sphere.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (ben_sphere.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		ben_sphere.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(ben_sphere.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_sphere.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	ben_sphere.local_z.coord[0] = (ben_sphere.local_x.coord[1]*ben_sphere.local_y.coord[2]) - (ben_sphere.local_x.coord[2]*ben_sphere.local_y.coord[1]);
	ben_sphere.local_z.coord[1] = (ben_sphere.local_x.coord[2]*ben_sphere.local_y.coord[0]) - (ben_sphere.local_x.coord[0]*ben_sphere.local_y.coord[2]);
	ben_sphere.local_z.coord[2] = (ben_sphere.local_x.coord[0]*ben_sphere.local_y.coord[1]) - (ben_sphere.local_x.coord[1]*ben_sphere.local_y.coord[0]);
	ben_sphere.local_z.coord[3] = 0.0; // w coordinate

}


//Function to compute vertices of a cone
void make_cone(double ray, double height, int rs, int vs)
{
	//All shapes are centered at the origin
	ben_cone.center.coord[0] = 0.0;
	ben_cone.center.coord[1] = 0.0;
	ben_cone.center.coord[2] = 0.0;
	ben_cone.center.coord[3] = 1.0; //w

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


	//Create object's local coordinate system
	
	//Local X-axis

	v_start[0] = ray;
	v_start[1] = 0.0;

	for (int i = 0; i < 3;  i++)
	{
		ben_cone.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	ben_cone.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(ben_cone.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_cone.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*ben_cone.local_x.coord[0]) + (up_vector[1]*ben_cone.local_x.coord[1]) + (up_vector[2]*ben_cone.local_x.coord[2]) + (up_vector[3]*ben_cone.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (ben_cone.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		ben_cone.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(ben_cone.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_cone.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	ben_cone.local_z.coord[0] = (ben_cone.local_x.coord[1]*ben_cone.local_y.coord[2]) - (ben_cone.local_x.coord[2]*ben_cone.local_y.coord[1]);
	ben_cone.local_z.coord[1] = (ben_cone.local_x.coord[2]*ben_cone.local_y.coord[0]) - (ben_cone.local_x.coord[0]*ben_cone.local_y.coord[2]);
	ben_cone.local_z.coord[2] = (ben_cone.local_x.coord[0]*ben_cone.local_y.coord[1]) - (ben_cone.local_x.coord[1]*ben_cone.local_y.coord[0]);
	ben_cone.local_z.coord[3] = 0.0; // w coordinate

}


//Function to compute the vertices of a torus (donut)
void make_torus(double r_torus, double r_tube, int rs, int vs)
{
	//All shapes are centered at the origin
	ben_torus.center.coord[0] = 0.0;
	ben_torus.center.coord[1] = 0.0;
	ben_torus.center.coord[2] = 0.0;
	ben_torus.center.coord[3] = 1.0; //w

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

	//Create object's local coordinate system
	
	//Local X-axis

	for (int i = 0; i < 3;  i++)
	{
		ben_torus.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	ben_torus.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(ben_torus.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_torus.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*ben_torus.local_x.coord[0]) + (up_vector[1]*ben_torus.local_x.coord[1]) + (up_vector[2]*ben_torus.local_x.coord[2]) + (up_vector[3]*ben_torus.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (ben_torus.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		ben_torus.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(ben_torus.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		ben_torus.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	ben_torus.local_z.coord[0] = (ben_torus.local_x.coord[1]*ben_torus.local_y.coord[2]) - (ben_torus.local_x.coord[2]*ben_torus.local_y.coord[1]);
	ben_torus.local_z.coord[1] = (ben_torus.local_x.coord[2]*ben_torus.local_y.coord[0]) - (ben_torus.local_x.coord[0]*ben_torus.local_y.coord[2]);
	ben_torus.local_z.coord[2] = (ben_torus.local_x.coord[0]*ben_torus.local_y.coord[1]) - (ben_torus.local_x.coord[1]*ben_torus.local_y.coord[0]);
	ben_torus.local_z.coord[3] = 0.0; // w coordinate


}

