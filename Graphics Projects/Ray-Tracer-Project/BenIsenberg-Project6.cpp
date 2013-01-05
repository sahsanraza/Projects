/*Simple Ray Tracer by Ben Isenberg
 *Ray traces Spheres
*/


#include <iostream>
#include <string>

#include "BenIsenberg-Project6.h"

void initGL(void);
void displayScene(void);
void idle(void);
void resizeWindow(int width, int height);

GLuint texId;

color_t screen[WINDOW_WIDTH * WINDOW_HEIGHT];

static int raster_x=0;
static int raster_y=0;

#define my_assert(X,Y) ((X)?(void) 0:(printf("error:%s in %s at %d", Y, __FILE__, __LINE__), myabort()))
#define min(a,b) ((a) < (b)? a:b)
#define max(a,b) ((a) > (b)? a:b)

#define MAX_LIGHTS  8
#define NUM_OBJECTS 8

using namespace std;

//Camera struct
typedef struct _CAM
{
	//position
	GLfloat pos[4];
	//look at
	GLfloat at[4];
	//up
	GLfloat up[4];

	GLfloat dir[4];

	//gluPerspective lens parameters
	double near;
	double far;
	double left;
	double right;
	double top;
	double bottom;
	double zoom_factor;
	double aspect_ratio;
	double fov;

}CAM;


//Light struct
typedef struct _LITE
{
  GLfloat amb[4];
  GLfloat diff[4];
  GLfloat spec[4];
  GLfloat pos[4];
  GLfloat dir[3];
  GLfloat angle;
}LITE;

//List of objects
OBJECT my_objects[NUM_OBJECTS];
//list of lights
LITE my_lights[MAX_LIGHTS];

int num_objects;
int num_lights;

//parallel camera
CAM my_cam;

//ray shot by user
ray my_ray;

//camera matrices
double modelViewMatrix[16];
double projMatrix[16];
int viewport[4];

//Did we get camera matrices from OpenGL?
bool got_matrices = false;

//closest intersection with object
float closestPoint[3] = {1000, 1000, 1000};

//Global diffuse intensity
float g_diff[4] = { 0.4, 0.6, 0.5, 1.0}; 

//Global ambient intensity
float g_amb[4] = { 0.1, 0.1, 0.1, 1.0};

//Current object being ray traced
OBJECT* current;


void myabort(void) 
{
  abort();
  exit(1); /* exit so g++ knows we don't return. */
}

//Shoots a ray based on mouse click and marks the closest object the ray intersects on screen
OBJECT* my_raytrace(int mousex, int mousey)
{
	
	bool foundIntersection = false;
	bool hit = false;
	
	// ray intersection point with far clipping plane
	double farClickPoint[3];
	// ray intersection point with near clipping plane
	double nearClickPoint[3];
	
	//intersection with object
	float intersectionPoint[3] = {1000, 1000, 1000};
	
	//IMPORTANT - reset closest point
	closestPoint[0] = 1000;
	closestPoint[1] = 1000;
	closestPoint[2] = 1000;

	OBJECT *cur;
	OBJECT *close = NULL;

	//Optimization: only need to do this once
	if (!got_matrices)
	{
		// first we need to get the modelview matrix, the projection matrix, and the viewport
		glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		got_matrices = true;
	}
		
	//offset y coordinate of mouse click
	mousey = viewport[3] - mousey;

	// gluUnProject with a Z value of 1 will find the point on the far clipping plane corresponding the mouse click.
	gluUnProject(mousex, mousey, 1.0, modelViewMatrix, projMatrix, viewport, &farClickPoint[0], &farClickPoint[1], &farClickPoint[2]);

	// gluUnProject with a Z value of 0 will find the point on the near clipping plane corresponding the mouse click.
	gluUnProject(mousex, mousey, 0.0, modelViewMatrix, projMatrix, viewport, &nearClickPoint[0], &nearClickPoint[1], &nearClickPoint[2]);
	
	//ray direction vector
	my_ray.rayDirection[0] = farClickPoint[0] - nearClickPoint[0];
	my_ray.rayDirection[1] = farClickPoint[1] - nearClickPoint[1];
	my_ray.rayDirection[2] = farClickPoint[2] - nearClickPoint[2];
	
	//Normalize ray direction vector
	double magnitude = 0;
	
	magnitude = sqrt( (my_ray.rayDirection[0]*my_ray.rayDirection[0]) + (my_ray.rayDirection[1]*my_ray.rayDirection[1]) + (my_ray.rayDirection[2]*my_ray.rayDirection[2]) );
	
	my_ray.rayDirection[0] /= magnitude;
	my_ray.rayDirection[1] /= magnitude;
	my_ray.rayDirection[2] /= magnitude;
	my_ray.rayDirection[3] = 0.0; 
	
	//ray starting point
	my_ray.rayStart[0] = my_cam.pos[0];
	my_ray.rayStart[1] = my_cam.pos[1];
	my_ray.rayStart[2] = my_cam.pos[2];
	my_ray.rayStart[3] = 1.0;
	
	//name of shape being intersected
	string shape_name;
	//distance of intersection to camera
	double distance2;
	
	// now go through the shapes and see if there is a hit
	for (int i = 0; i < num_objects; i++)
	{
		cur = &my_objects[i];
		hit = false;

		switch (cur->sid)
		{
			case 3: //Sphere
				//Get previous distance
				distance2 = sqrt( ((closestPoint[0]-my_ray.rayStart[0])*(closestPoint[0]-my_ray.rayStart[0])) + ((closestPoint[1]-my_ray.rayStart[1])*(closestPoint[1]-my_ray.rayStart[1])) + ((closestPoint[2]-my_ray.rayStart[2])*(closestPoint[2]-my_ray.rayStart[2])) );
				
				//Apply inverse transformations to ray for ray intersection to work correctly with implicit equations
				apply_inverse_transformations(my_ray, cur);
				
				hit = my_raytrace_sphere(my_ray, intersectionPoint, cur, distance2);
				break;
			default:
				break;
		}

		// found intersection
		if (hit)
		{
			closestPoint[0] = intersectionPoint[0];
			closestPoint[1] = intersectionPoint[1];
			closestPoint[2] = intersectionPoint[2];

			foundIntersection = true;
			
			shape_name = cur->name;
			//printf("Getting object\n");
			
			//get object that was hit by ray
			close = &(*cur);
			
			//printf("%x - object address %x - current address\n", cur, close);
			
		}
	}

	//Print closest object intersection location if one occured
	/*if (foundIntersection)
	{
		printf("Intersected with object %s at (%f, %f, %f)\n", shape_name.c_str(), closestPoint[0], closestPoint[1], closestPoint[2]);
		
	}*/
	
	return close;
}


//Checks intersection with sphere
bool my_raytrace_sphere(ray& r, float* result, OBJECT* po, double distance2)
{
	float a = (r.rayDirection[0]*r.rayDirection[0]) + (r.rayDirection[1]*r.rayDirection[1]) + (r.rayDirection[2]*r.rayDirection[2]);
	float b = 2*( (r.rayStart[0]*r.rayDirection[0]) + (r.rayStart[1]*r.rayDirection[1]) + (r.rayStart[2]*r.rayDirection[2]) );
	float c = (r.rayStart[0]*r.rayStart[0]) + (r.rayStart[1]*r.rayStart[1]) + (r.rayStart[2]*r.rayStart[2]) - 1;
	float discriminant = (b*b) - (4*a*c);
	
	if (discriminant < 0)
	{
		//translate ray back to object location before returning
		apply_original_transformations(r, po);
		
		return false;
	} 
	
	float t1 = ((-b) + sqrt(discriminant))/(2.0*a);
	float t2 = ((-b) - sqrt(discriminant))/(2.0*a);
	
	//translate ray back to object location
	apply_original_transformations(r, po);
		
	result[0] = r.rayStart[0] + (min(t1, t2) * r.rayDirection[0]);
	result[1] = r.rayStart[1] + (min(t1, t2) * r.rayDirection[1]);
	result[2] = r.rayStart[2] + (min(t1, t2) * r.rayDirection[2]);
	
	double distance1 = sqrt( ((result[0]-r.rayStart[0])*(result[0]-r.rayStart[0])) + ((result[1]-r.rayStart[1])*(result[1]-r.rayStart[1])) + ((result[2]-r.rayStart[2])*(result[2]-r.rayStart[2])) );
	
	//Not closest object intersection
	if (distance1 > distance2)
	{
		return false;
	}
	
	return true; 
}

//Transform ray back to unit shape location so that ray intersection works correctly
//ASSUMES order of transformations is ALWAYS scale, rotate, translate
void apply_inverse_transformations(ray& r, OBJECT* po)
{	
	double t_inverse[4][4] = { {1.0, 0.0, 0.0, -po->translate[0]},
							   {0.0, 1.0, 0.0, -po->translate[1]},
							   {0.0, 0.0, 1.0, -po->translate[2]},
							   {0.0, 0.0, 0.0, 1.0} };
	
	double theta = po->rotate[0]*(PI/180); //convert degrees to radians
	
	double rx_inverse[4][4] = { {1.0, 0.0, 0.0, 0.0},
								{0.0, cos(theta), sin(theta), 0.0},
								{0.0, -sin(theta),	cos(theta), 0.0},
								{0.0, 0.0, 0.0, 1.0} };
								
	theta = po->rotate[1]*(PI/180); //convert degrees to radians
	
	double ry_inverse[4][4] = { {cos(theta), 0.0, -sin(theta), 0.0}, 
								{0.0, 1.0, 0.0, 0.0},
								{sin(theta), 0.0, cos(theta), 0.0},
								{0.0, 0.0, 0.0, 1.0} };
								
	theta = po->rotate[2]*(PI/180); //convert degrees to radians
								
	double rz_inverse[4][4] = { {cos(theta), sin(theta), 0.0, 0.0},
								{-sin(theta), cos(theta), 0.0, 0.0},	
								{0.0, 0.0, 1.0,	0.0},
								{0.0, 0.0, 0.0, 1.0} };
								
	
	double s_inverse[4][4] = { {1.0/po->scale[0], 0.0, 0.0, 0.0},
							   {0.0, 1.0/po->scale[1], 0.0, 0.0},
						       {0.0, 0.0, 1.0/po->scale[2], 0.0},
						       {0.0, 0.0, 0.0, 1.0} };	
	
	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];
						   
	//Apply in this order: t, rz, ry, rx, scale
	//Inverse translate
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(t_inverse, r.rayStart, copy);
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(t_inverse, r.rayDirection, copy);
	
	//Inverse z rotation
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(rz_inverse, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(rz_inverse, r.rayDirection, copy);
	
	//Inverse y rotation
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(ry_inverse, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(ry_inverse, r.rayDirection, copy);
	
	//Inverse x rotation
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(rx_inverse, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(rx_inverse, r.rayDirection, copy);
	
	//Inverse scale
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(s_inverse, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(s_inverse, r.rayDirection, copy);									
}


//Transforms ray back to original location
void apply_original_transformations(ray& r, OBJECT* po)
{
	double translate[4][4] = { {1.0, 0.0, 0.0, po->translate[0]},
								{0.0, 1.0, 0.0, po->translate[1]},
								{0.0, 0.0, 1.0, po->translate[2]},
								{0.0, 0.0, 0.0, 1.0} };
	
	double theta = po->rotate[0]*(PI/180); //convert degrees to radians
	
	double rx[4][4] = { {1.0, 0.0, 0.0, 0.0},
						{0.0, cos(theta), -sin(theta), 0.0},
						{0.0, sin(theta), cos(theta), 0.0},
						{0.0, 0.0, 0.0, 1.0} };
								
	theta = po->rotate[1]*(PI/180); //convert degrees to radians
	
	double ry[4][4] = { {cos(theta), 0.0, sin(theta), 0.0},
						{0.0, 1.0, 0.0, 0.0},
						{-sin(theta), 0.0, cos(theta), 0.0},
						{0.0, 0.0, 0.0, 1.0} };
								
	theta = po->rotate[2]*(PI/180); //convert degrees to radians
								
	double rz[4][4] = { {cos(theta), -sin(theta), 0.0, 0.0},
						{sin(theta), cos(theta), 0.0, 0.0},
						{0.0, 0.0, 1.0, 0.0},
						{0.0, 0.0, 0.0, 1.0} };
								
	
	double scale[4][4] = { {po->scale[0], 0.0, 0.0, 0.0},
						   {0.0, po->scale[1], 0.0, 0.0},
						   {0.0, 0.0, po->scale[2], 0.0},
						   {0.0, 0.0, 0.0, 1.0} };	
	
	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];
						   
	//Apply in this order: scale, rx, ry, rz, t
	//scale
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(scale, r.rayStart, copy);
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(scale, r.rayDirection, copy);
	
	//x rotation
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(rx, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(rx, r.rayDirection, copy);
	
	//y rotation
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(ry, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(ry, r.rayDirection, copy);
	
	//z rotation
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(rz, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(rz, r.rayDirection, copy);
	
	//translate
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayStart[x];
	}
	matrix_multiplication2(translate, r.rayStart, copy);							
	for (int x = 0; x < 4; x++)
	{
		copy[x] = r.rayDirection[x];
	}
	matrix_multiplication2(translate, r.rayDirection, copy);		
}


void matrix_multiplication2(double m[4][4], float* v, double start[4])
{	
	//compute dot products of matrix rows with vertex
	for (int x = 0; x < 3; x++)
	{
		v[x] = (m[x][0]*start[0]) + (m[x][1]*start[1]) + (m[x][2]*start[2]) + (m[x][3]*start[3]);
	}

	return;
}


void my_setup(int argc, char **argv)
{

  num_objects = num_lights = 0;

  //default camera values
  my_cam.near = 1;
  my_cam.far = 40;
  my_cam.left = -10;
  my_cam.right = 10;
  my_cam.top = 10;
  my_cam.bottom = -10;
  my_cam.zoom_factor = 1.0;
  my_cam.aspect_ratio = 1.0;
  my_cam.fov = 60;
 
  string filename;
  
  cout << "Enter a scene file to parse:" << endl;
  cin >> filename;

  //read spec file (must be in same folder as code)
  read_spec(filename.c_str());

  return;
}

//Parse numbers from text file
void parse_floats(char *buffer, GLfloat nums[]) 
{
  char *ps;

  ps = strtok(buffer, " ");
  
  for (int i=0; ps; i++) 
  {
    nums[i] = atof(ps);
    ps = strtok(NULL, " ");
    //printf("read %f ",nums[i]);
  }

}

//Read in objects from spec file
void parse_obj(char *buffer)
{
  OBJECT *po;
  char *pshape, *pshine, *pemi, *pamb, *pdiff, *pspec, *ptranslate, *pscale, *protate;

  my_assert ((num_objects < NUM_OBJECTS), "too many objects");
  po = &my_objects[num_objects++];

  pshape  = strtok(buffer, " ");
  //printf("pshape is %s\n",pshape);

  ptranslate    = strtok(NULL, "()");  strtok(NULL, "()");
  pscale        = strtok(NULL, "()");  strtok(NULL, "()"); 
  protate       = strtok(NULL, "()");  strtok(NULL, "()");  

  pshine  = strtok(NULL, "()");strtok(NULL, "()");
  //printf("pshine is %s\n",pshine);
 
  pemi    = strtok(NULL, "()");  strtok(NULL, "()"); 
  pamb    = strtok(NULL, "()");  strtok(NULL, "()"); 
  pdiff   = strtok(NULL, "()");  strtok(NULL, "()"); 
  pspec   = strtok(NULL, "()");  strtok(NULL, "()"); 


  po->sid  = atoi(pshape);
  po->shine = atof(pshine);

  parse_floats(ptranslate, po->translate);
  parse_floats(pscale, po->scale);
  parse_floats(protate, po->rotate);

  parse_floats(pemi, po->emi);
  parse_floats(pamb, po->amb);
  parse_floats(pdiff, po->diff);
  parse_floats(pspec, po->spec);
  
  printf("read object\n");
  
  //translate center of object to correct location so that illumination formula can work correctly
  double translation[4][4] = { {1.0, 0.0, 0.0, po->translate[0]},
							   {0.0, 1.0, 0.0, po->translate[1]},
							   {0.0, 0.0, 1.0, po->translate[2]},
							   {0.0, 0.0, 0.0, 1.0} };
							   
	//All objects begin at the world origin						   
	po->center[0] = 0;
	po->center[1] = 0;
	po->center[2] = 0;
	po->center[3] = 1; //homogeneous coordinate
	
	double copy[4] = {0, 0, 0, 1};
	
	//apply translation to center
	matrix_multiplication2(translation, po->center, copy);
	
	
}

//parse camera from spec file
void parse_camera(char *buffer)
{
  CAM *pc;
  char *ppos, *plook, *pup;

  pc = &my_cam;

  strtok(buffer, "()");
  ppos  = strtok(NULL, "()");  strtok(NULL, "()"); 
  plook  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pup  = strtok(NULL, "()");  strtok(NULL, "()"); 

  parse_floats(ppos, pc->pos);
  parse_floats(plook, pc->at);
  parse_floats(pup, pc->up);

  pc->at[0] += pc->pos[0];
  pc->at[1] += pc->pos[1];
  pc->at[2] += pc->pos[2];

  pc->dir[0] = pc->at[0] - pc->pos[0];
  pc->dir[1] = pc->at[1] - pc->pos[1];
  pc->dir[2] = pc->at[2] - pc->pos[2];
  
  normalize(pc->dir);
  printf("read camera\n");

}

//normalize axes
void normalize(GLfloat *p) 
{ 
  double d = 0.0;

  for(int i = 0; i < 3; i++)
  {
	   d+=p[i]*p[i];
  }
  
  d = sqrt(d);
  
  if(d > 0.0) 
  {
	  for(int i = 0; i < 3; i++) 
	  {
		  p[i] /= d;
	  }
  }
}

//Parse light from spec file
void parse_light(char *buffer)
{
  LITE *pl;
  char *pamb, *pdiff, *pspec, *ppos, *pdir, *pang;
  
  my_assert ((num_lights < MAX_LIGHTS), "too many lights");
  
  pl = &my_lights[num_lights++];

  strtok(buffer, "()");
  pamb  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pdiff = strtok(NULL, "()");  strtok(NULL, "()"); 
  pspec = strtok(NULL, "()");  strtok(NULL, "()"); 
  ppos  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pdir  = strtok(NULL, "()");  strtok(NULL, "()"); 
  pang  = strtok(NULL, "()");

  parse_floats(pamb, pl->amb);
  parse_floats(pdiff, pl->diff);
  parse_floats(pspec, pl->spec);
  parse_floats(ppos, pl->pos);
  
  if (pdir)
  {
    parse_floats(pdir, pl->dir);
    pl->angle = atof(pang);
    //printf("angle %f\n", pl->angle);
  }
  else
  {
    pl->dir[0] = pl->dir[1] = pl->dir[2] = 0;
}
  printf("read light\n");

}

//Read spec file
void read_spec(const char *fname) 
{
  char buffer[300];
  FILE *fp;
  char* str;

  fp = fopen(fname, "r");
  my_assert(fp, "can't open spec");

  while(!feof(fp))
  {
    str = fgets(buffer, 300, fp);
    //stop looping when we reach end of file
    if (str == NULL)
    {
		continue;
	}
    
    //printf("read line: %s\n", buffer);
    switch (buffer[0]) 
	{
		case '#':
			break;
		case '3': //sphere
			parse_obj(buffer);
			break;
		case 'l':
			parse_light(buffer);
			break;
		case 'c':
			parse_camera(buffer);
			break;
		default:
			break;
	}
  }
}

//Color a pixel
void plotPixel(color_t *screen, int x, int y, float r, float g, float b, float a)
{
	screen[y*WINDOW_WIDTH + x].r = r;
	screen[y*WINDOW_WIDTH + x].g = g;
	screen[y*WINDOW_WIDTH + x].b = b;
	screen[y*WINDOW_WIDTH + x].a = a;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	my_setup(argc, argv); 
	
	// depth buffer not really needed, but whatever
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Ben Isenberg - Ray Tracer");

	initGL();

	glutDisplayFunc(displayScene);
	glutReshapeFunc(resizeWindow);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

void initGL()
{
	int i;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);

	resizeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	// erase texture (draw pink background)
	for (i=0;i<sizeof(screen)/sizeof(color_t);i++)
	{
		screen[i].r = 1.0f;
		screen[i].g = 0.0f;
		screen[i].b = 1.0f;
		screen[i].a = 1.0f;
	}

	// create texture for drawing
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, (void*)screen);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void displayScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);

	// load orthographic projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();

	// restore projection
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutSwapBuffers();
}

void idle()
{
	//Pixel color values
	float r, g, b, a;
	
	//Ray Trace every pixel on screen
	while (raster_y < WINDOW_HEIGHT)
	{
		if (raster_x < WINDOW_WIDTH && raster_y < WINDOW_HEIGHT)
		{	
			//Check if ray intersects an object
			current = my_raytrace(raster_x, raster_y);
			
			//if there is a hit color the pixel
			if (current != NULL)
			{
				/*Compute Illumination for point*/
				
				//N Vector
				float vector_n[4];
				//printf("closest point = (%f, %f, %f)\n", closestPoint[0], closestPoint[1], closestPoint[2]);
		
				vector_n[0] = closestPoint[0] - current->center[0];
				vector_n[1] = closestPoint[1] - current->center[1];
				vector_n[2] = closestPoint[2] - current->center[2];
				vector_n[3] = 0;
				
				//Normalize N vector
				double magnitude = 0;
				
				magnitude = sqrt( (vector_n[0]*vector_n[0]) + (vector_n[1]*vector_n[1]) + (vector_n[2]*vector_n[2]) );
				
				vector_n[0] /= magnitude;
				vector_n[1] /= magnitude;
				vector_n[2] /= magnitude; 
				
				//printf("N vector - %f, %f, %f\n", vector_n[0], vector_n[1], vector_n[2]);
				
				//L Vector
				float vector_l[4];
				
				float sigma[4] = {0, 0, 0, 0};
				
				//Compute summation for all lights in scene
				for (int x = 0; x < num_lights; x++)
				{	
					vector_l[0] = my_lights[x].pos[0] - closestPoint[0];
					vector_l[1] = my_lights[x].pos[1] - closestPoint[1];
					vector_l[2] = my_lights[x].pos[2] - closestPoint[2];
					vector_l[3] = 0;
					
					//Normalize L vector
					magnitude = sqrt( (vector_l[0]*vector_l[0]) + (vector_l[1]*vector_l[1]) + (vector_l[2]*vector_l[2]) );
				
					vector_l[0] /= magnitude;
					vector_l[1] /= magnitude;
					vector_l[2] /= magnitude;
					
					float dot = (vector_n[0]*vector_l[0]) + (vector_n[1]*vector_l[1]) + (vector_n[2]*vector_l[2]);
					
					sigma[0] += (current->diff[0] * dot * my_lights[x].diff[0]);
					sigma[1] += (current->diff[1] * dot * my_lights[x].diff[1]);
					sigma[2] += (current->diff[2] * dot * my_lights[x].diff[2]);
					
				}
				
				//printf("After for loop\n");
					
				r = (g_amb[0]*current->amb[0]) + sigma[0];
				g = (g_amb[1]*current->amb[1]) + sigma[1];
				b = (g_amb[2]*current->amb[2]) + sigma[2];
				a = 1;
				
			}
			else //otherwise continue loop so that ray tracing is faster (Don't need to recompute background colors)
			{
				if (raster_x < WINDOW_WIDTH)
					raster_x++;

				if (raster_x == WINDOW_WIDTH) // finished line, increment Y raster
				{
					raster_x = 0;
					raster_y++;
				}
				continue;
			}
			
			
			// now we must change the color in our buffer
			plotPixel(screen, raster_x, raster_y, r, g, b, a);

			// now lets upload the changes to the video card (slow)
			// Unfortunately, the final parameter of glTexSubImage2D can only be a 1D array
			// containing the exact pixels you want to transfer (it isn't smart enough to 
			// select a square, for instance)
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexSubImage2D(GL_TEXTURE_2D, 0, raster_x, raster_y, 1, 1, GL_RGBA, GL_FLOAT, (void*)(screen + (raster_y*WINDOW_WIDTH+raster_x)));
		}

		if (raster_x < WINDOW_WIDTH)
			raster_x++;

		if (raster_x == WINDOW_WIDTH) // finished line, increment Y raster
		{
			raster_x = 0;
			raster_y++;
		}
	}//End while

	// redraw
	glutPostRedisplay();
}

void resizeWindow(int width, int height)
{
	if (height == 0) // prevent div/0
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width/(double)height, 0.1, 4096.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
