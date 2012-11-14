//NAME: Ben Isenberg
//DATE: 10/08/12
//EMAIL: bji6@pitt.edu
//CS1566 Project 3

#ifndef __CS1566_GLOBALS
#define __CS1566_GLOBALS


#ifndef min //In VC++ 2008, Ryan got a warning about min redefinition, so let's not redefine it if something else already did; sigh
#define min(a,b) ((a) < (b)? a:b)
#endif

#define FALSE 0 
#define TRUE  1

/* define index constants into the colors array */
#define BLACK   0
#define RED     1
#define YELLOW  2
#define MAGENTA 3
#define GREEN   4
#define CYAN    5
#define BLUE    6
#define GREY    7
#define WHITE   8


#define HOUSE    0
#define CUBE     1
#define TESCUBE  2
#define CYLINDER 3
#define SPHERE   4
#define TORUS    5
#define CONE	 6
//#define MESH     6
//#define GEODESIC 7


#define NONE_MODE 3
#define AXIS_ROTATION_MODE 4
#define REAL_TRANSLATION_MODE 5
#define REAL_SCALING_MODE 6
#define REAL_ROTATION_MODE 7

#define SPIN_ENABLED 1
#define SPIN_DISABLED 0

//Define Pi
#define PI 3.14159265

GLfloat colors [][3] = {
  {0.0, 0.0, 0.0},  /* black   */
  {1.0, 0.0, 0.0},  /* red     */
  {1.0, 1.0, 0.0},  /* yellow  */
  {1.0, 0.0, 1.0},  /* magenta */
  {0.0, 1.0, 0.0},  /* green   */
  {0.0, 1.0, 1.0},  /* cyan    */
  {0.0, 0.0, 1.0},  /* blue    */
  {0.5, 0.5, 0.5},  /* 50%grey */
  {1.0, 1.0, 1.0}   /* white   */
};

/*note the 4 (!) coordinates per vertex below; 
homogeneous coords with w = 1 (see the lecture notes);*/
GLfloat vertices_house[][4] = {
  {0,2,0,1}, 
  {-1,1,1,1}, {1,1,1,1}, {1,1,-1,1}, {-1,1,-1,1},
  {-1,-1,1,1}, {1,-1,1,1}, {1,-1,-1,1}, {-1,-1,-1,1}
};


GLfloat vertices_cube_brute[][4] = {
  {-1,1,1,1}, {1,1,1,1}, {1,1,-1,1}, {-1,1,-1,1},
  {-1,-1,1,1}, {1,-1,1,1}, {1,-1,-1,1}, {-1,-1,-1,1}
  };


//Particle/vertex structure
struct Particle
{
	//homogeneous coordinates of particle
	double coord[4];
};

//Shape structure
struct Shape 
{
	//All shapes begin centered at the origin
	Particle center;

	//Shape local coordinate system
	Particle local_x;
	Particle local_y;
	Particle local_z;

	Particle vertices[50][50];  //Array of the shape's vertices

	Particle normals[50][50];  //Array of the vertex normals of the shape
	
};


GLfloat vertices_axes[][4] = {
	{0.0, 0.0, 0.0, 1.0},  /* origin */ 
	{5.0, 0.0, 0.0, 1.0},  /* maxx */ 
	{0.0, 5.0, 0.0, 1.0}, /* maxy */ 
	{0.0, 0.0, 5.0, 1.0}  /* maxz */ 

};


void glut_setup(void) ;
void gl_setup(void) ;
void my_setup(void);
void my_display(void) ;
void my_mouse(int button, int state, int mousex, int mousey) ;
void my_reshape(int w, int h) ;
void my_keyboard_up( unsigned char key, int x, int y ) ;
void my_keyboard( unsigned char key, int x, int y ) ;
void my_idle(void) ;
void my_TimeOut(int id) ;

//Functions to compute shape vertices
void make_cube();
void make_house();
void make_cylinder( double height, double ray, int rs, int vs );
void make_sphere( double ray, int rs, int vs );
void make_cone(double ray, double height, int rs, int vs);
void make_torus(double r_torus, double r_tube, int rs, int vs);

//draw sphere prototype
void draw_sphere(int rs, int vs);
//draw cylinder prototype
void draw_cylinder(int rs, int vs);
//draw cone prototype
void draw_cone(int rs, int vs);
//draw torus prototype
void draw_torus(int rs, int vs);
//draw house prototype
void draw_house();
//draw cube prototype
void draw_cube_brute();

//draw a shape's local coordinate system
void draw_local_coord_system();

//Translation function
void real_translation(GLfloat x, GLfloat y, GLfloat z);

//Function finds magnitude of a vector
double find_magnitude(Particle p);

//Function mutiplies matrices together
void matrix_multiplication(double m[4][4], Particle& v, double start[4]);

//Function to reset vertex normals
void reset_normals(Shape& s);

//Cube & house function
void compute_cube_normals(int a, int b, int c);

//Rotates an object around an arbitrary axis
void real_rot_arbitrary_axis_point(GLfloat deg, double ax, double ay, double az, double cx, double cy, double cz, int y_change, int z_change );


#endif
