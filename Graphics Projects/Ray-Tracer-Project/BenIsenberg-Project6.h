#ifndef _RAY_TRACE_H_
#define _RAY_TRACE_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

 #include <GL/glut.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 512

//Define Pi
#define PI 3.14159265

typedef struct color_s
{
	float r, g, b, a;

} color_t;


typedef struct _Object 
{
  int sid;
  //shape name
  std::string name;
 
  //center vertex of object
  float center[4];
 
  //object light properties
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

extern GLuint textureId;
extern color_t screen[WINDOW_WIDTH * WINDOW_HEIGHT];

void plotPixel(color_t *screen, int x, int y, float r, float g, float b, float a);

//transform ray to unit shape location
void apply_inverse_transformations(ray& r, OBJECT* po);
//transform ray to original location
void apply_original_transformations(ray& r, OBJECT* po);

//Used for ray intersection
void matrix_multiplication2(double m[4][4], float* v, double start[4]);

//Checks intersection with sphere
bool my_raytrace_sphere(ray& r, float* result, OBJECT* po, double distance2);

//read spec file
void read_spec(const char *fname) ;
void parse_light(char *buffer);
//parse camera from spec file
void parse_camera(char *buffer);
//Read in objects from spec file
void parse_obj(char *buffer);
//normalize axes
void normalize(GLfloat *p); 

#endif
