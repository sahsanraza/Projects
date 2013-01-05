//Ben Isenberg
//Project 5 - Ray Intersection
//11/2/12

#ifndef __CS1566_GLOBALS
#define __CS1566_GLOBALS

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
// may need to change GLUT/glut.h to GL/glut.h on PC or Linux
#include <GL/glut.h>

#define min(a,b) ((a) < (b)? a:b)

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

//Shapes
#define CUBE     1
#define SPHERE   3
#define CYLINDER 4
#define CONE	 5
#define TORUS    6

//Camera transform modes
#define CHANGE_FAR_CLIPPING 1
#define CHANGE_NEAR_CLIPPING 2
#define CHANGE_ASPECT_RATIO 3
#define ZOOM 4
#define REAL_TRANSLATION_MODE 5
#define CHANGE_HEIGHT_ANGLE 6

//Define Pi
#define PI 3.14159265

//Unit Cube
GLfloat vertices_cube_brute[][4] = {
  {-0.5,0.5,0.5,1}, {0.5,0.5,0.5,1}, {0.5,0.5,-0.5,1}, {-0.5,0.5,-0.5,1},
  {-0.5,-0.5,0.5,1}, {0.5,-0.5,0.5,1}, {0.5,-0.5,-0.5,1}, {-0.5,-0.5,-0.5,1}
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

//Object struct
typedef struct _Object 
{
  int sid;
  //shape name
  std::string name;

  Shape ben_cube;
  Shape ben_sphere;
  Shape ben_cylinder;
  Shape ben_cone;
 
  GLfloat shine;
  GLfloat emi[4];
  GLfloat amb[4];
  GLfloat diff[4];
  GLfloat spec[4];

  //transformations done to the object
  GLfloat translate[4];
  GLfloat scale[4];
  GLfloat rotate[4];

}OBJECT;

//ray struct
struct ray
{
	//start point of ray
	float rayStart[4];
	//direction vector of ray
	float rayDirection[4];
};

void glut_setup(void) ;
void gl_setup(void) ;
void my_init(int argc, char **argv);
void my_setup(int argc, char **argv);
void myabort(void);
void my_display(void) ;
void my_mouse(int button, int state, int mousex, int mousey) ;
void my_reshape(int w, int h) ;
void my_keyboard( unsigned char key, int x, int y ) ;

//normalize camera axes
void normalize(GLfloat *p);

//print camera matrices
void print_matrix(float my_matrix[]);

//read spec file
void read_spec(const char *fname) ;
void parse_light(char *buffer);

//draw world axes
void draw_axes( void );

//Functions to compute shape vertices
void make_cube_smart(OBJECT *po, double size );
void make_cylinder(OBJECT *p, double height, double ray, int rs, int vs );
void make_sphere(OBJECT *p, double ray, int rs, int vs );
void make_cone(OBJECT *p, double ray, double height, int rs, int vs);

//Function finds magnitude of a vector
double find_magnitude(Particle p);

//Function mutiplies matrices together
void matrix_multiplication(double m[4][4], Particle& v, double start[4]);

//transformations
void real_translation(Shape& po, GLfloat x, GLfloat y, GLfloat z);
void real_scaling(Shape& po, GLfloat sx, GLfloat sy, GLfloat sz);
void real_rotation(Shape& po, GLfloat deg, GLfloat x, GLfloat y, GLfloat z);

//draw sphere prototype
void draw_sphere(Shape& po, int rs, int vs);
//draw cylinder prototype
void draw_cylinder(Shape& po, int rs, int vs);
//draw cone prototype
void draw_cone(Shape& po, int rs, int vs);
//draw cube prototype
void draw_cube_brute(Shape& po);

//Draw ray intersection markers (tiny red spheres)
void draw_marker(Shape& s, int rs, int vs);

//draw shape normals
void draw_normals(Shape& s, int rs, int vs);

//Compute face normal of plane that vertex a, b, c lie on
void compute_cube_normals(Shape* s_ptr, int a, int b, int c);

//Recalculates camera local coord system
void recalculate_local_coord();

//transform ray to unit shape location
void apply_inverse_transformations(ray& r, OBJECT* po);
//transform ray to original location
void apply_original_transformations(ray& r, OBJECT* po);

//Used for ray intersection
void matrix_multiplication2(double m[4][4], float* v, double start[4]);


#endif
