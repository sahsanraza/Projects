//Ben Isenberg
//Project 5 - Ray Intersection
//11/2/12

#include <iostream>
#include <string>
#include <vector>

#include "BenIsenberg-Project5.h"

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

	//Vectors representing shape local coordinate system
	Particle w_vector;
	Particle v_vector;
	Particle u_vector;

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

GLfloat colors [][3] = 
{
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

GLfloat vertices_axes[][4] = 
{
	{0.0, 0.0, 0.0, 1.0},  /* origin */ 
	{5.0, 0.0, 0.0, 1.0},  /* maxx */ 
	{0.0, 5.0, 0.0, 1.0}, /* maxy */ 
	{0.0, 0.0, 5.0, 1.0}  /* maxz */ 

};

//List of objects
OBJECT my_objects[NUM_OBJECTS];
//list of lights
LITE my_lights[MAX_LIGHTS];

int num_objects;
int  num_lights;

//array of object ids
int ids[NUM_OBJECTS];

//What mode to transform camera
int transform_mode;

//Window width and height
float window_h = 700;
float window_w = 700;

//variables
int crt_render_mode;
int crt_rs, crt_vs;
int crt_transform;

//Shape parameters
double cyl_height=2.0;
double cyl_ray=1;
double sph_ray=1;
double cone_h = 1.0;
double cone_ray = 1;

//parallel camera
CAM my_cam;

//ray shot by user
ray my_ray;

//markers for intersection points
OBJECT marker1;
OBJECT marker2;

//shrink ray
bool shrink_ray = false;
//grow ray
bool grow_ray = false;


void myabort(void) 
{
  abort();
  exit(1); /* exit so g++ knows we don't return. */
} 

int main(int argc, char** argv)
{ 
  setbuf(stdout, NULL);   /* for writing to stdout asap */
  glutInit(&argc, argv);

  my_setup(argc, argv);  
  glut_setup();
  gl_setup();

  glutMainLoop();
  return(0);
}


void glut_setup (){

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  
  glutInitWindowSize(int(window_w), int(window_h));
  glutInitWindowPosition(20,20);
  glutCreateWindow("Ben Isenberg Project 5");

  /* set up callback functions */
  glutDisplayFunc(my_display);
  glutReshapeFunc(my_reshape);
  glutMouseFunc(my_mouse);
  glutKeyboardFunc(my_keyboard);	

  return;
}

void gl_setup(void) {

  // enable depth handling (z-buffer)
  glEnable(GL_DEPTH_TEST);

  // enable auto normalize
  glEnable(GL_NORMALIZE);

  // define the background color 
  glClearColor(0,0,0,1);

  glMatrixMode(GL_PROJECTION) ;
  glLoadIdentity() ;
  gluPerspective( 40, 1.0, 1, 200.0);
  glMatrixMode(GL_MODELVIEW) ;
  glLoadIdentity() ;  // init modelview to identity

  // toggle to smooth shading (instead of flat)
  glShadeModel(GL_SMOOTH); 


  return ;
}

void my_setup(int argc, char **argv)
{

  num_objects = num_lights = 0;

  // initialize global shape defaults and mode for drawing
  //crt_render_mode = GL_LINE_LOOP;
  crt_render_mode = GL_POLYGON;

  crt_rs = 40;
  crt_vs = 40; 

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

  // use switch to create your objects, cube given as example
  switch (po->sid)
  {
	case 1: //cube
		po->name = "Cube";
		make_cube_smart(po, 1);
		 // scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
		real_scaling(po->ben_cube, po->scale[0], po->scale[1], po->scale[2]);  
		real_rotation(po->ben_cube, po->rotate[0], 1, 0, 0);
		real_rotation(po->ben_cube, po->rotate[1], 0, 1, 0);
		real_rotation(po->ben_cube, po->rotate[2], 0, 0, 1);
		real_translation(po->ben_cube, po->translate[0], po->translate[1], po->translate[2]);
		break;
	case 3:
		po->name = "Sphere";
		//Compute sphere vertices
		make_sphere(po, sph_ray,crt_rs,crt_vs);
		// scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
		real_scaling(po->ben_sphere, po->scale[0], po->scale[1], po->scale[2]);  
		real_rotation(po->ben_sphere, po->rotate[0], 1, 0, 0);
		real_rotation(po->ben_sphere, po->rotate[1], 0, 1, 0);
		real_rotation(po->ben_sphere, po->rotate[2], 0, 0, 1);
		real_translation(po->ben_sphere, po->translate[0], po->translate[1], po->translate[2]);
		break;
	case 4:
		po->name = "Cylinder";
		//Compute cylinder vertices
		make_cylinder(po, cyl_height,cyl_ray,crt_rs,crt_vs);
		// scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
		real_scaling(po->ben_cylinder, po->scale[0], po->scale[1], po->scale[2]);  
		real_rotation(po->ben_cylinder, po->rotate[0], 1, 0, 0);
		real_rotation(po->ben_cylinder, po->rotate[1], 0, 1, 0);
		real_rotation(po->ben_cylinder, po->rotate[2], 0, 0, 1);
		real_translation(po->ben_cylinder, po->translate[0], po->translate[1], po->translate[2]);
		break;
	case 5:
		po->name = "Cone";
		//Compute cone vertices
		make_cone(po, cone_ray, cone_h, crt_rs, crt_vs);
		// scale, rotate, translate using your real tranformations from assignment 3 depending on input from spec file
		real_scaling(po->ben_cone, po->scale[0], po->scale[1], po->scale[2]);  
		real_rotation(po->ben_cone, po->rotate[0], 1, 0, 0);
		real_rotation(po->ben_cone, po->rotate[1], 0, 1, 0);
		real_rotation(po->ben_cone, po->rotate[2], 0, 0, 1);
		real_translation(po->ben_cone, po->translate[0], po->translate[1], po->translate[2]);
		break;
  }
  
  printf("read object\n");
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

  //Build camera local coordinate axes
  pc->w_vector.coord[0] = pc->at[0] - pc->pos[0];
  pc->w_vector.coord[1] = pc->at[1] - pc->pos[1];
  pc->w_vector.coord[2] = pc->at[2] - pc->pos[2];
  pc->w_vector.coord[3] = 0.0;

  double m;

  //dot product
  //Don't include w coordinate
  m = (pc->w_vector.coord[0]*pc->w_vector.coord[0]) + (pc->w_vector.coord[1]*pc->w_vector.coord[1]) + (pc->w_vector.coord[2]*pc->w_vector.coord[2]);

  //take square root
  m = sqrt(m);
  if (m != 0)
  {
    //normalize
    pc->w_vector.coord[0] /= m;
    pc->w_vector.coord[1] /= m;
    pc->w_vector.coord[2] /= m;
  }

  //V axis - formula = (up - ((up dot w)*w))/magnitude

  double up_dot_w;

  //(up dot w)*w)
  double w_scaled[4];

  up_dot_w = (pc->up[0]*pc->w_vector.coord[0]) + (pc->up[1]*pc->w_vector.coord[1]) + (pc->up[2]*pc->w_vector.coord[2]);

  //printf("up . w = %f\n", up_dot_w);

  for (int i = 0; i < 4; i++)
  {
    w_scaled[i] = (pc->w_vector.coord[i]*up_dot_w);
  }

  //up - ((up dot w)*w)
  for (int i = 0; i < 4; i++)
  {
    pc->v_vector.coord[i] = pc->up[i] - w_scaled[i];
    //printf("v vector in creation - %f\n", v_vector.coord[i]);
  }

      
  //Normalize by magnitude
  //dot product
  //Don't include w coordinate
  m = (pc->v_vector.coord[0]*pc->v_vector.coord[0]) + (pc->v_vector.coord[1]*pc->v_vector.coord[1]) + (pc->v_vector.coord[2]*pc->v_vector.coord[2]);

  //take square root
  m = sqrt(m);
  if (m != 0)
  {
    //normalize
    pc->v_vector.coord[0] /= m;
    pc->v_vector.coord[1] /= m;
    pc->v_vector.coord[2] /= m;
  }

  //U axis = w cross v

  //Cross product
  pc->u_vector.coord[0] = (pc->w_vector.coord[1]*pc->v_vector.coord[2]) - (pc->w_vector.coord[2]*pc->v_vector.coord[1]);
  pc->u_vector.coord[1] = (pc->w_vector.coord[2]*pc->v_vector.coord[0]) - (pc->w_vector.coord[0]*pc->v_vector.coord[2]);
  pc->u_vector.coord[2] = (pc->w_vector.coord[0]*pc->v_vector.coord[1]) - (pc->w_vector.coord[1]*pc->v_vector.coord[0]);
  pc->u_vector.coord[3] = 0.0; // w coordinate

  /*for (int x = 0; x < 4; x++)
  {
    printf("w - %f, %f, %f, %f\n", pc->w_vector.coord[0], pc->w_vector.coord[1], pc->w_vector.coord[2], pc->w_vector.coord[3]);
    printf("v - %f, %f, %f, %f\n", pc->v_vector.coord[0], pc->v_vector.coord[1], pc->v_vector.coord[2], pc->v_vector.coord[3]);
    printf("u - %f, %f, %f, %f\n", pc->u_vector.coord[0], pc->u_vector.coord[1], pc->u_vector.coord[2], pc->u_vector.coord[3]);
  }*/

}

//Parse light from spec file
void parse_light(char *buffer){
  LITE *pl;
  char *pamb, *pdiff, *pspec, *ppos, *pdir, *pang;
  my_assert ((num_lights < MAX_LIGHTS), "too many lights");
  pl = &my_lights[++num_lights];

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
  if (pdir) {
    parse_floats(pdir, pl->dir);
    pl->angle = atof(pang);
    //printf("angle %f\n", pl->angle);
  }
  else
    pl->dir[0]= pl->dir[1]= pl->dir[2] =0;
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
		case '1': //cube
		case '3': //sphere
		case '4': //cylinder
		case '5': //cone
		case '6': //torus
			//read in the cube
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


void my_reshape(int w, int h) {
  // ensure a square view port
  glViewport(0,0,min(w,h),min(w,h)) ;
  return ;
}

//Rotates an object around either world x, y, or z axis by 'deg' degrees
void real_rotation(Shape& s_ptr, GLfloat deg, GLfloat x, GLfloat y, GLfloat z) 
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
				copy[g] = s_ptr.vertices[j][i].coord[g];
			}
			
			if (x == 1.0)
			{
				matrix_multiplication(rotation_x, s_ptr.vertices[j][i], copy);
			}
			else if (y == 1.0)
			{
				matrix_multiplication(rotation_y, s_ptr.vertices[j][i], copy);
			}
			else if (z == 1.0)
			{
				matrix_multiplication(rotation_z, s_ptr.vertices[j][i], copy);
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
		copy2[g] = s_ptr.center.coord[g];
		copy3[g] = s_ptr.local_x.coord[g];
		copy4[g] = s_ptr.local_y.coord[g];
		copy5[g] = s_ptr.local_z.coord[g];
	}

	if (x == 1.0)
	{
		matrix_multiplication(rotation_x, s_ptr.center, copy2);
		matrix_multiplication(rotation_x, s_ptr.local_x, copy3);
		matrix_multiplication(rotation_x, s_ptr.local_y, copy4);
		matrix_multiplication(rotation_x, s_ptr.local_z, copy5);
	}
	else if (y == 1.0)
	{
		matrix_multiplication(rotation_y, s_ptr.center, copy2);
		matrix_multiplication(rotation_y, s_ptr.local_x, copy3);
		matrix_multiplication(rotation_y, s_ptr.local_y, copy4);
		matrix_multiplication(rotation_y, s_ptr.local_z, copy5);
	}
	else if (z == 1.0)
	{
		matrix_multiplication(rotation_z, s_ptr.center, copy2);
		matrix_multiplication(rotation_z, s_ptr.local_x, copy3);
		matrix_multiplication(rotation_z, s_ptr.local_y, copy4);
		matrix_multiplication(rotation_z, s_ptr.local_z, copy5);
	}

}

//Translates shapes in space by moving their vertices based on values given
void real_translation(Shape& s_ptr, GLfloat x, GLfloat y, GLfloat z) 
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
				copy[g] = s_ptr.vertices[x][y].coord[g];
			}

			matrix_multiplication(translation, s_ptr.vertices[x][y], copy);
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
		copy2[g] = s_ptr.center.coord[g];
	}


	//printf("OLD center coords - %f, %f, %f\n", s_ptr.center.coord[0], s_ptr.center.coord[1], s_ptr.center.coord[2]);
	matrix_multiplication(translation, s_ptr.center, copy2);
	//printf("NEW center coords - %f, %f, %f\n", s_ptr.center.coord[0], s_ptr.center.coord[1], s_ptr.center.coord[2]);
}

//Scales an object by applying scaling matrix to all of its vertices
void real_scaling(Shape& s_ptr, GLfloat sx, GLfloat sy, GLfloat sz) 
{
	double scale[4][4] = { {sx, 0.0, 0.0, 0.0},
						   {0.0, sy, 0.0, 0.0},
						   {0.0, 0.0, sz, 0.0},
						   {0.0, 0.0, 0.0, 1.0} };
	
	//Need this so matrix multiplication works correctly without having values update before others have
	double copy[4];

	/*double c1, c2, c3;
	c1 = s_ptr.center.coord[0];
	c2 = s_ptr.center.coord[1];
	c3 = s_ptr.center.coord[2];*/

	//translate to origin first before scaling
	//real_translation(s_ptr, -c1, -c2, -c3);

	//Apply scaling matrix to all vertices
	for (int x = 0;  x < crt_rs; x++)
	{
		for (int y = 0; y < crt_vs; y++)
		{
			//First copy vertex coordinates to copy array
			for (int g = 0; g < 4; g++)
			{
				copy[g] = s_ptr.vertices[x][y].coord[g];
			}

			matrix_multiplication(scale, s_ptr.vertices[x][y], copy);
		}
	}

	//Apply transformation to Particle that represents center of object
	/*double copy2[4];

	//First copy vertex coordinates to copy array
	for (int g = 0; g < 4; g++)
	{
		copy2[g] = s_ptr.center.coord[g];
	}*/

	//matrix_multiplication(scale, s_ptr.center, copy2);
	
	//translate back from origin
	//real_translation(s_ptr, c1, c2, c3);
}



void my_keyboard( unsigned char key, int x, int y ) 
{
    //Variables used for printing
	double width = my_cam.right - my_cam.left;
	double height = my_cam.top - my_cam.bottom;

	float scale[16] = { (2/width), 0, 0, 0, 0, (2/height), 0, 0, 0, 0, (1/my_cam.far), 0, 0, 0, 0, 1};

	float a = (-(my_cam.pos[0] + (my_cam.near*my_cam.w_vector.coord[0]))*my_cam.u_vector.coord[0]) + (-(my_cam.pos[1] + (my_cam.near*my_cam.w_vector.coord[1]))*my_cam.u_vector.coord[1]) +  (-(my_cam.pos[2] + (my_cam.near*my_cam.w_vector.coord[2]))*my_cam.u_vector.coord[2]);
	float b = (-(my_cam.pos[0] + (my_cam.near*my_cam.w_vector.coord[0]))*my_cam.v_vector.coord[0]) + (-(my_cam.pos[1] + (my_cam.near*my_cam.w_vector.coord[1]))*my_cam.v_vector.coord[1]) +  (-(my_cam.pos[2] + (my_cam.near*my_cam.w_vector.coord[2]))*my_cam.v_vector.coord[2]);
	float c = (-(my_cam.pos[0] + (my_cam.near*my_cam.w_vector.coord[0]))*my_cam.w_vector.coord[0]) + (-(my_cam.pos[1] + (my_cam.near*my_cam.w_vector.coord[1]))*my_cam.w_vector.coord[1]) +  (-(my_cam.pos[2] + (my_cam.near*my_cam.w_vector.coord[2]))*my_cam.w_vector.coord[2]);

	float r_t[16] = { my_cam.u_vector.coord[0], my_cam.v_vector.coord[0], my_cam.w_vector.coord[0], 0.0,
						my_cam.u_vector.coord[1], my_cam.v_vector.coord[1], my_cam.w_vector.coord[1], 0.0,
						my_cam.u_vector.coord[2], my_cam.v_vector.coord[2], my_cam.w_vector.coord[2], 0.0, 
						a, b, c, 1.0
					};

	switch (key)
	{
		//Activate the shrink ray!
		case '1':
			shrink_ray = !shrink_ray;
			grow_ray = false;
			break;
		//Activate the grow ray!
		case '2':
			grow_ray = !grow_ray;
			shrink_ray = false;
			break;
			
		case 'P':	//print out camera info
		case 'p':

			printf("****INFO*****\n");
			printf("Eye point - (%.2f, %.2f, %.2f)\n", my_cam.pos[0], my_cam.pos[1], my_cam.pos[2]);
			printf("Look vector - (%.2f, %.2f, %.2f)\n", my_cam.w_vector.coord[0], my_cam.w_vector.coord[1], my_cam.w_vector.coord[2]);
			printf("Up vector - (%.2f, %.2f, %.2f)\n", my_cam.up[0], my_cam.up[1], my_cam.up[2]);
			printf("Height angle - %.2f\n", my_cam.top - my_cam.bottom);
			printf("Apect ratio - %.2f:%.2f -- (%.2f)\n", (my_cam.right - my_cam.left), (my_cam.top - my_cam.bottom), (my_cam.right - my_cam.left)/(my_cam.top - my_cam.bottom));
			printf("Projection Matrix :\n");
			print_matrix(scale);
			printf("Modelview Matrix :\n");
			print_matrix(r_t);

			break;
		case 'T':	//translate
		case 't':
			transform_mode = REAL_TRANSLATION_MODE;
			break;
		case 'X':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				my_cam.pos[0] += (100/window_w);
				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'x':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				my_cam.pos[0] -= (100/window_w);
				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'Y':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				my_cam.pos[1] += (100/window_h);
				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'y':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				my_cam.pos[1] -= (100/window_h);
				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'Z':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				my_cam.pos[2] += (100/window_w);
				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'z':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				my_cam.pos[2] -= (100/window_w);
				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'W':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				for (int x = 0; x < 3; x++)
				{
					my_cam.pos[x] += ((100*my_cam.w_vector.coord[x])/window_w);
					//printf("%f\n", ((10*my_cam.w_vector[x])/window_w));
					my_cam.at[x] += ((100*my_cam.w_vector.coord[x])/window_w);
				}

				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'w':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				for (int x = 0; x < 3; x++)
				{
					my_cam.pos[x] -= ((100*my_cam.w_vector.coord[x])/window_w);
					my_cam.at[x] -= ((100*my_cam.w_vector.coord[x])/window_w);
				}

				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'V':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				for (int x = 0; x < 3; x++)
				{
					my_cam.pos[x] += ((100*my_cam.v_vector.coord[x])/window_h);
					my_cam.at[x] += ((100*my_cam.v_vector.coord[x])/window_h);
				}

				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'v':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				for (int x = 0; x < 3; x++)
				{
					my_cam.pos[x] -= ((100*my_cam.v_vector.coord[x])/window_h);
					my_cam.at[x] -= ((100*my_cam.v_vector.coord[x])/window_h);
				}

				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'U':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				for (int x = 0; x < 3; x++)
				{
					my_cam.pos[x] += ((100*my_cam.u_vector.coord[x])/window_w);
					my_cam.at[x] += ((100*my_cam.u_vector.coord[x])/window_w);
				}

				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'u':
			if (transform_mode == REAL_TRANSLATION_MODE)
			{
				for (int x = 0; x < 3; x++)
				{
					my_cam.pos[x] -= ((100*my_cam.u_vector.coord[x])/window_w);
					my_cam.at[x] -= ((100*my_cam.u_vector.coord[x])/window_w);
				}

				recalculate_local_coord();
			}
			glutPostRedisplay() ;
			break;
		case 'L':
		case 'l':
			transform_mode = ZOOM;
			break;
		case 'A':
		case 'a':
			transform_mode = CHANGE_ASPECT_RATIO;
			break;
		case 'H':
		case 'h':
			transform_mode = CHANGE_HEIGHT_ANGLE;
			break;
		case 'F':
		case 'f':
			transform_mode = CHANGE_FAR_CLIPPING;
			break;
		case 'N':
		case 'n':
			transform_mode = CHANGE_NEAR_CLIPPING;
			break;
		case '+':
			switch(transform_mode)
			{
				double w;
				double h;

				case ZOOM:
					//to zoom in decrease the height and width of lens
					my_cam.zoom_factor -= 0.2;

					glutPostRedisplay();
					break;
				case CHANGE_ASPECT_RATIO:
					//increase aspect ratio means increasing width by 1/3*width and height by 1/4*height
					w = (my_cam.right - my_cam.left)*(1.0/6.0);
					my_cam.right += w;
					my_cam.left -= w;

					h = (my_cam.top - my_cam.bottom)*(1.0/8.0);
					my_cam.top += h;
					my_cam.bottom -= h;
					
					my_cam.aspect_ratio = (my_cam.right - my_cam.left)/(my_cam.top - my_cam.bottom);

					//printf("aspect ratio %f:%f\n", my_cam.right - my_cam.left, my_cam.top - my_cam.bottom);

					glutPostRedisplay();
					break;
				case CHANGE_HEIGHT_ANGLE:
					//Increase the height of our view volume of the parallel camera
					my_cam.fov += 2;
					
					glutPostRedisplay();
					break;
				case CHANGE_FAR_CLIPPING:
					my_cam.far += 1;
					glutPostRedisplay();
					break;
				case CHANGE_NEAR_CLIPPING:
					if ((my_cam.near - 1) <= my_cam.far)
					{
						my_cam.near -= 1;
					}
					glutPostRedisplay();
					break;
			}
			break;
		case '-':
			switch(transform_mode)
			{
				double h, w;

				case ZOOM:
					//to zoom out increase the height and width of lens
					my_cam.zoom_factor += 0.2;
					
					glutPostRedisplay();
					break;
				case CHANGE_ASPECT_RATIO:
					//decrease aspect ratio means decreasing width by 1/3*width and height by 1/4*height
					w = (my_cam.right - my_cam.left)*(1.0/6.0);
					my_cam.right -= w;
					my_cam.left += w;

					h = (my_cam.top - my_cam.bottom)*(1.0/8.0);
					my_cam.top -= h;
					my_cam.bottom += h;

					my_cam.aspect_ratio = (my_cam.right - my_cam.left)/(my_cam.top - my_cam.bottom);
					//printf("aspect ratio %f:%f\n", my_cam.right - my_cam.left, my_cam.top - my_cam.bottom);

					glutPostRedisplay();
					break;
				case CHANGE_HEIGHT_ANGLE:
					//Decrease the height of our view volume of the parallel camera
					my_cam.fov -= 2;
					
					glutPostRedisplay();
					break;
				case CHANGE_FAR_CLIPPING:
					if ((my_cam.far - 1) >= my_cam.near)
					{
						my_cam.far -= 1;
					}
					glutPostRedisplay();
					break;
				case CHANGE_NEAR_CLIPPING:
					if ((my_cam.near + 1) <= my_cam.pos[2] && (my_cam.near + 1) <= 0.0)
					{
						my_cam.near += 1;
					}
					glutPostRedisplay();
					break;
			}
			break;
		case 'q': 
		case 'Q':
			exit(0) ;
			break ;	
		default: break;
	}
  
  return ;
}

//Bubble sort function 
void bubbleSort(float *array,int length)
{
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(array[i] < array[j])
            {
                float temp = array[i]; //swap 
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}


//Checks if there was an intersection with a cube
bool my_raytrace_cube(ray& r, float* result, OBJECT* po, double distance2)
{
	
	float t1, t2, t3, t4, t5, t6;
	
	if (r.rayDirection[0] != 0)
	{
		t1 = ((-r.rayStart[0]) - 0.5)/r.rayDirection[0];
		t2 = ((-r.rayStart[0]) + 0.5)/r.rayDirection[0];
	}
	else
	{
		t1 = ((-r.rayStart[0]) - 0.5);
		t2 = ((-r.rayStart[0]) + 0.5);
	}
	
	if (r.rayDirection[1] != 0)
	{ 
	  t3 = ((-r.rayStart[1]) - 0.5)/r.rayDirection[1];
	  t4 = ((-r.rayStart[1]) + 0.5)/r.rayDirection[1];
    }
	else
	{
		t3 = ((-r.rayStart[1]) - 0.5);
		t4 = ((-r.rayStart[1]) + 0.5);
	}
	
	if (r.rayDirection[2] != 0)
	{
		t5 = ((-r.rayStart[2]) - 0.5)/r.rayDirection[2];
		t6 = ((-r.rayStart[2]) + 0.5)/r.rayDirection[2];
	}
	else
	{
		t5 = ((-r.rayStart[2]) - 0.5);
		t6 = ((-r.rayStart[2]) + 0.5);
	}
	
	float list[6] = {t1, t2, t3, t4, t5, t6};
	
	float min1 = 0;
	
	//Sort t's so we can check them from smallest to largest
	bubbleSort(list, 6);
	
	min1 = list[0];
	
	vector<float> mins;
	
	//Loop thru the t values to see if you are intersecting the restricted planes
	for (int x = 0; x < 6; x++)
	{	
		min1 = list[x];
		
		if (mins.size() == 2)
		{
			break;
		}
		
		result[0] = r.rayStart[0] + (min1 * r.rayDirection[0]);
		result[1] = r.rayStart[1] + (min1 * r.rayDirection[1]);
		result[2] = r.rayStart[2] + (min1 * r.rayDirection[2]);
		
		//printf("result - %f, %f, %f\n", result[0], result[1], result[2]);
		
		//right plane (x= 1)
		if (min1 == t1)
		{
			//Get rid of floating point issues
			float value = roundf(result[0]*10.0f)/10.0f;
			//printf("value - %f\n", value);
			
			if (value == -0.5 and result[1] <= 0.5 and result[1] >= -0.5 and result[2] <= 0.5 and result[2] >= -0.5)
			{
				//printf("left plane!\n");
				mins.push_back(min1);
			}
		}
		//left plane (x = -1)
		if (min1 == t2)
		{
			//Get rid of floating point issues
			float value = roundf(result[0]*10.0f)/10.0f;
			//printf("value - %f\n", value);
			
			if (value == 0.5 and result[1] <= 0.5 and result[1] >= -0.5 and result[2] <= 0.5 and result[2] >= -0.5)
			{
				//printf("right plane!\n");
				mins.push_back(min1);
			}
		}
		//top plane (y= 1)
		if (min1 == t3)
		{
			//Get rid of floating point issues
			float value = roundf(result[1]*10.0f)/10.0f;
			//printf("value - %f\n", value);
			
			if (value == -0.5 and result[0] <= 0.5 and result[0] >= -0.5 and result[2] <= 0.5 and result[2] >= -0.5)
			{
				//printf("bottom plane!\n");
				mins.push_back(min1);
			}
		}
		if (min1 == t4) //Bottom plane (y = -1)
		{
			//Get rid of floating point issues
			float value = roundf(result[1]*10.0f)/10.0f;
			//printf("value - %f\n", value);
			
			if (value == 0.5 and result[0] <= 0.5 and result[0] >= -0.5 and result[2] <= 0.5 and result[2] >= -0.5)
			{
				//printf("top plane!\n");
				mins.push_back(min1);
			}
		}
		if (min1 == t5) //Front plane (z = 1)
		{
			//Get rid of floating point issues
			float value = roundf(result[2]*10.0f)/10.0f;
			//printf("value - %f\n", value);
			
			if (value == -0.5 and result[0] <= 0.5 and result[0] >= -0.5 and result[1] <= 0.5 and result[1] >= -0.5)
			{
				//printf("back plane!\n");
				mins.push_back(min1);
			}
		}
		if(min1 == t6)//Back plane (z = -1)
		{
			//Get rid of floating point issues
			float value = roundf(result[2]*10.0f)/10.0f;
			//printf("value - %f\n", value);
			
			if (value == 0.5 and result[0] <= 0.5 and result[0] >= -0.5 and result[1] <= 0.5 and result[1] >= -0.5)
			{
				//printf("front plane!\n");
				mins.push_back(min1);
			}
		}
	}
	
	//No hit was found
	if (mins.size() < 1)
	{
		//translate ray back to object location before returning
		apply_original_transformations(r, po);
		
		return false;
	}
	
	//Only one marker needed
	if (mins.size() < 2) 
	{
		//translate ray back to object location
		apply_original_transformations(r, po);
		
		result[0] = r.rayStart[0] + (mins.front() * r.rayDirection[0]);
		result[1] = r.rayStart[1] + (mins.front() * r.rayDirection[1]);
		result[2] = r.rayStart[2] + (mins.front() * r.rayDirection[2]);	
		
		double distance1 = sqrt( ((result[0]-r.rayStart[0])*(result[0]-r.rayStart[0])) + ((result[1]-r.rayStart[1])*(result[1]-r.rayStart[1])) + ((result[2]-r.rayStart[2])*(result[2]-r.rayStart[2])) );
	
		//Not closest intersection
		if (distance1 > distance2)
		{
			return false;
		}
	
		make_sphere(&marker1, 0.1, crt_rs, crt_vs);
		
		//translate markers to ray intersection points
		real_translation(marker1.ben_sphere, r.rayStart[0] + (mins.front() * r.rayDirection[0]), r.rayStart[1] + (mins.front() * r.rayDirection[1]), r.rayStart[2] + (mins.front() * r.rayDirection[2]));
				

		return true;
	}
	
	//printf("True min was = %f\n", min(mins.front(), mins.back()));
	
	//translate ray back to object location
	apply_original_transformations(r, po);
	
	result[0] = r.rayStart[0] + (min(mins.front(), mins.back()) * r.rayDirection[0]);
	result[1] = r.rayStart[1] + (min(mins.front(), mins.back()) * r.rayDirection[1]);
	result[2] = r.rayStart[2] + (min(mins.front(), mins.back()) * r.rayDirection[2]);
	
	double distance1 = sqrt( ((result[0]-r.rayStart[0])*(result[0]-r.rayStart[0])) + ((result[1]-r.rayStart[1])*(result[1]-r.rayStart[1])) + ((result[2]-r.rayStart[2])*(result[2]-r.rayStart[2])) );
	
	//Not closest intersection
	if (distance1 > distance2)
	{
		return false;
	}
	
	make_sphere(&marker1, 0.1, crt_rs, crt_vs);
	make_sphere(&marker2, 0.1, crt_rs, crt_vs);
	
	//translate markers to ray intersection points
	real_translation(marker1.ben_sphere, r.rayStart[0] + (mins.front() * r.rayDirection[0]), r.rayStart[1] + (mins.front() * r.rayDirection[1]), r.rayStart[2] + (mins.front() * r.rayDirection[2]));
	real_translation(marker2.ben_sphere, r.rayStart[0] + (mins.back() * r.rayDirection[0]), r.rayStart[1] + (mins.back() * r.rayDirection[1]), r.rayStart[2] + (mins.back() * r.rayDirection[2]));
		

	return true; 
}

//Checks if ray intersects cone
bool my_raytrace_cone(ray& r, float* result, OBJECT* po, double distance2)
{
	
	float a = (r.rayDirection[0]*r.rayDirection[0]) - (r.rayDirection[1]*r.rayDirection[1]) + (r.rayDirection[2]*r.rayDirection[2]);
	float b = 2*( (r.rayStart[0]*r.rayDirection[0]) - (r.rayStart[1]*r.rayDirection[1]) + (r.rayStart[2]*r.rayDirection[2]) + r.rayDirection[1] );
	float c = (r.rayStart[0]*r.rayStart[0]) - (r.rayStart[1]*r.rayStart[1]) + (r.rayStart[2]*r.rayStart[2]) + (2*r.rayStart[1]) - 1;
	float discriminant = (b*b) - (4*a*c);
	
	//No hit
	if (discriminant < 0)
	{
		//translate ray back to object location before returning
		apply_original_transformations(r, po);
		
		return false;
	}
	
	float t1 = ((-b) + sqrt(discriminant))/(2.0*a);
	float t2 = ((-b) - sqrt(discriminant))/(2.0*a);
	
	result[0] = r.rayStart[0] + (min(t1, t2) * r.rayDirection[0]);
	result[1] = r.rayStart[1] + (min(t1, t2) * r.rayDirection[1]);
	result[2] = r.rayStart[2] + (min(t1, t2) * r.rayDirection[2]);	
	
	//Check y is in between 1 and 0
	if (result[1] > 1 or result[1] < 0)
	{
		//translate ray back to object location before returning
		apply_original_transformations(r, po);
		
		return false;
	}
	
	//translate ray back to object location
	apply_original_transformations(r, po);
	
	//Get new results after translating ray back to object
	result[0] = r.rayStart[0] + (max(t1, t2) * r.rayDirection[0]);
	result[1] = r.rayStart[1] + (max(t1, t2) * r.rayDirection[1]);
	result[2] = r.rayStart[2] + (max(t1, t2) * r.rayDirection[2]);
	
	double distance1 = sqrt( ((result[0]-r.rayStart[0])*(result[0]-r.rayStart[0])) + ((result[1]-r.rayStart[1])*(result[1]-r.rayStart[1])) + ((result[2]-r.rayStart[2])*(result[2]-r.rayStart[2])) );

	//Not closest object intersection
	if (distance1 > distance2)
	{
		return false;
	}

	make_sphere(&marker1, 0.1, crt_rs, crt_vs);
	make_sphere(&marker2, 0.1, crt_rs, crt_vs);
	
	//translate markers to ray intersection points
	real_translation(marker1.ben_sphere, r.rayStart[0] + (t1 * r.rayDirection[0]), r.rayStart[1] + (t1 * r.rayDirection[1]), r.rayStart[2] + (t1 * r.rayDirection[2]));
	real_translation(marker2.ben_sphere, r.rayStart[0] + (t2 * r.rayDirection[0]), r.rayStart[1] + (t2 * r.rayDirection[1]), r.rayStart[2] + (t2 * r.rayDirection[2]));
	
	return true;
	
}

//Checks ray intersection with cylinder
bool my_raytrace_cylinder(ray& r, float* result, OBJECT* po, double distance2)
{
	float a = (r.rayDirection[0]*r.rayDirection[0]) + (r.rayDirection[2]*r.rayDirection[2]);
	float b = 2*( (r.rayStart[0]*r.rayDirection[0]) + (r.rayStart[2]*r.rayDirection[2]) );
	float c = (r.rayStart[0]*r.rayStart[0]) +(r.rayStart[2]*r.rayStart[2]) - 1;
	float discriminant = (b*b) - (4*a*c);
	
	if (discriminant < 0)
	{
		//translate ray back to object location before returning
		apply_original_transformations(r, po);
		
		return false;
	} 
	
	float t1 = ((-b) + sqrt(discriminant))/(2.0*a);
	float t2 = ((-b) - sqrt(discriminant))/(2.0*a);
	
	result[0] = r.rayStart[0] + (min(t1, t2) * r.rayDirection[0]);
	result[1] = r.rayStart[1] + (min(t1, t2) * r.rayDirection[1]);
	result[2] = r.rayStart[2] + (min(t1, t2) * r.rayDirection[2]);
	
	
	//try other point if first doesnt work
	if (result[1] > 1 or result[1] < -1)
	{
		result[0] = r.rayStart[0] + (max(t1, t2) * r.rayDirection[0]);
		result[1] = r.rayStart[1] + (max(t1, t2) * r.rayDirection[1]);
		result[2] = r.rayStart[2] + (max(t1, t2) * r.rayDirection[2]);
		
		if (result[1] > 1 or result[1] < -1)
		{
			//translate ray back to object location before returning
			apply_original_transformations(r, po);
			
			return false;
		}
		
		//translate ray back to object location
		apply_original_transformations(r, po);
		
		//Get new results after translating ray back to object
		result[0] = r.rayStart[0] + (max(t1, t2) * r.rayDirection[0]);
		result[1] = r.rayStart[1] + (max(t1, t2) * r.rayDirection[1]);
		result[2] = r.rayStart[2] + (max(t1, t2) * r.rayDirection[2]);
		
		double distance1 = sqrt( ((result[0]-r.rayStart[0])*(result[0]-r.rayStart[0])) + ((result[1]-r.rayStart[1])*(result[1]-r.rayStart[1])) + ((result[2]-r.rayStart[2])*(result[2]-r.rayStart[2])) );
		
		//Not closest object intersection
		if (distance1 > distance2)
		{
			return false;
		}
		
		make_sphere(&marker1, 0.1, crt_rs, crt_vs);
		real_translation(marker1.ben_sphere, r.rayStart[0] + (max(t1, t2) * r.rayDirection[0]), r.rayStart[1] + (max(t1, t2) * r.rayDirection[1]), r.rayStart[2] + (max(t1, t2) * r.rayDirection[2]));
	
		//only draw one marker so lets just scale it till we cant see it
		real_scaling(marker2.ben_sphere, 0.001, 0.001, 0.001);
	}
	else
	{
		//translate ray back to object location
		apply_original_transformations(r, po);
		
		//Get new results after translating ray back to object
		result[0] = r.rayStart[0] + (max(t1, t2) * r.rayDirection[0]);
		result[1] = r.rayStart[1] + (max(t1, t2) * r.rayDirection[1]);
		result[2] = r.rayStart[2] + (max(t1, t2) * r.rayDirection[2]);
		
		double distance1 = sqrt( ((result[0]-r.rayStart[0])*(result[0]-r.rayStart[0])) + ((result[1]-r.rayStart[1])*(result[1]-r.rayStart[1])) + ((result[2]-r.rayStart[2])*(result[2]-r.rayStart[2])) );
		
		//Not closest object intersection
		if (distance1 > distance2)
		{
			return false;
		}
		
		make_sphere(&marker1, 0.1, crt_rs, crt_vs);
		make_sphere(&marker2, 0.1, crt_rs, crt_vs);
		
		//translate markers to ray intersection points
		real_translation(marker2.ben_sphere, r.rayStart[0] + (t2 * r.rayDirection[0]), r.rayStart[1] + (t2 * r.rayDirection[1]), r.rayStart[2] + (t2 * r.rayDirection[2]));
		real_translation(marker1.ben_sphere, r.rayStart[0] + (t1 * r.rayDirection[0]), r.rayStart[1] + (t1 * r.rayDirection[1]), r.rayStart[2] + (t1 * r.rayDirection[2]));
	}
	
	return true; 
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
	
	make_sphere(&marker1, 0.1, crt_rs, crt_vs);
	make_sphere(&marker2, 0.1, crt_rs, crt_vs);
	
	//translate markers to ray intersection points
	real_translation(marker1.ben_sphere, r.rayStart[0] + (t1 * r.rayDirection[0]), r.rayStart[1] + (t1 * r.rayDirection[1]), r.rayStart[2] + (t1 * r.rayDirection[2]));
	real_translation(marker2.ben_sphere, r.rayStart[0] + (t2 * r.rayDirection[0]), r.rayStart[1] + (t2 * r.rayDirection[1]), r.rayStart[2] + (t2 * r.rayDirection[2]));

	
	return true; 
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

//Shrinks object when shrink ray is used
void shrink(Shape& s, OBJECT& o)
{
	//translate to origin
	real_translation(s, -o.translate[0], -o.translate[1], -o.translate[2]);
	
	//shrink shape
	real_scaling(s, 0.2, 0.2, 0.2);
	
	//translate back
	real_translation(s, o.translate[0], o.translate[1], o.translate[2]);
	
}

//Grows object when grow ray is used
void grow(Shape& s, OBJECT& o)
{
	//translate to origin
	real_translation(s, -o.translate[0], -o.translate[1], -o.translate[2]);
	
	//grow shape
	real_scaling(s, 1/0.2, 1/0.2, 1/0.2);
	
	//translate back
	real_translation(s, o.translate[0], o.translate[1], o.translate[2]);
	
}

//Shoots a ray based on mouse click and marks the closest object the ray intersects on screen
void my_raytrace(int mousex, int mousey)
{
	//camera matrices
	double modelViewMatrix[16];
	double projMatrix[16];
	int viewport[4];
	
	bool foundIntersection = false;
	bool hit = false;
	
	// ray intersection point with far clipping plane
	double farClickPoint[3];
	// ray intersection point with near clipping plane
	double nearClickPoint[3];
	
	//intersection with object
	float intersectionPoint[3] = {1000, 1000, 1000};
	//closest intersection with object
	float closestPoint[3] = {1000, 1000, 1000};

	OBJECT *cur;
	OBJECT *toShrink = NULL;
	Shape* s = NULL;

	// first we need to get the modelview matrix, the projection matrix, and the viewport
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
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
		case 1: //Cube
			//Get previous distance
			distance2 = sqrt( ((closestPoint[0]-my_ray.rayStart[0])*(closestPoint[0]-my_ray.rayStart[0])) + ((closestPoint[1]-my_ray.rayStart[1])*(closestPoint[1]-my_ray.rayStart[1])) + ((closestPoint[2]-my_ray.rayStart[2])*(closestPoint[2]-my_ray.rayStart[2])) );
			
			//Apply inverse transformations to ray for ray intersection to work correctly with implicit equations
			apply_inverse_transformations(my_ray, cur);
			
			hit = my_raytrace_cube(my_ray, intersectionPoint, cur, distance2);
			break;
		case 3: //Sphere
			//Get previous distance
			distance2 = sqrt( ((closestPoint[0]-my_ray.rayStart[0])*(closestPoint[0]-my_ray.rayStart[0])) + ((closestPoint[1]-my_ray.rayStart[1])*(closestPoint[1]-my_ray.rayStart[1])) + ((closestPoint[2]-my_ray.rayStart[2])*(closestPoint[2]-my_ray.rayStart[2])) );
			
			//Apply inverse transformations to ray for ray intersection to work correctly with implicit equations
			apply_inverse_transformations(my_ray, cur);
			
			hit = my_raytrace_sphere(my_ray, intersectionPoint, cur, distance2);
			break;
		case 4: //Cylinder
			//Get previous distance
			distance2 = sqrt( ((closestPoint[0]-my_ray.rayStart[0])*(closestPoint[0]-my_ray.rayStart[0])) + ((closestPoint[1]-my_ray.rayStart[1])*(closestPoint[1]-my_ray.rayStart[1])) + ((closestPoint[2]-my_ray.rayStart[2])*(closestPoint[2]-my_ray.rayStart[2])) );
			
			//Apply inverse transformations to ray for ray intersection to work correctly with implicit equations
			apply_inverse_transformations(my_ray, cur);
			
			hit = my_raytrace_cylinder(my_ray, intersectionPoint, cur, distance2);
			break;
		case 5: //Cone
			//Get previous distance
			distance2 = sqrt( ((closestPoint[0]-my_ray.rayStart[0])*(closestPoint[0]-my_ray.rayStart[0])) + ((closestPoint[1]-my_ray.rayStart[1])*(closestPoint[1]-my_ray.rayStart[1])) + ((closestPoint[2]-my_ray.rayStart[2])*(closestPoint[2]-my_ray.rayStart[2])) );
			
			//Apply inverse transformations to ray for ray intersection to work correctly with implicit equations
			apply_inverse_transformations(my_ray, cur);
			
			hit = my_raytrace_cone(my_ray, intersectionPoint, cur, distance2);
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
			
			//For shrinking and growing
			switch(cur->sid)
			{
				case 1:
					s = &cur->ben_cube;
					toShrink = cur;
					break;
				case 3:
					s = &cur->ben_sphere;
					toShrink = cur;
					break;
				case 4:
					s = &cur->ben_cylinder;
					toShrink = cur;
					break;
				case 5:
					s = &cur->ben_cone;
					toShrink = cur;
					break;
			}
		}
	}

	//Print closest object intersection location if one occured
	if (foundIntersection)
	{
		printf("Intersected with object %s at (%f, %f, %f)\n", shape_name.c_str(), closestPoint[0], closestPoint[1], closestPoint[2]);
		foundIntersection = false;
		
		//shrink object if shrink ray is enabled
		if (shrink_ray)
		{
			shrink(*s, *toShrink);
		}
		//Grow object if grow ray enabled
		if (grow_ray)
		{
			grow(*s, *toShrink);
		}
		
	}
	else
	{
		printf("No intersection\n");
	}
}

//Handles mouse clicks
void my_mouse(int button, int state, int mousex, int mousey)
{
	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			if( state == GLUT_DOWN )
			{
				//Shoot ray into scene	
				my_raytrace(mousex, mousey);
				glutPostRedisplay();
			}
			if( state == GLUT_UP ) 
			{}
			break;
		case GLUT_RIGHT_BUTTON:
			if ( state == GLUT_DOWN )
			{}
			if( state == GLUT_UP )
			{}
			break;
	}
  
	return;
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

//print matrix
void print_matrix(float my_matrix[])
{ 
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      printf ("%f ", my_matrix[i+j*4]);
    }
    printf ("\n");
  }
  printf ("\n");
}


//Make cube shape
void make_cube_smart(OBJECT *po, double size)
{
    //Place cube vertices in a Shape struct
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			po->ben_cube.vertices[0][x].coord[y] = vertices_cube_brute[x][y];
		}
		//printf("cube coords - %f, %f,%f\n", po->ben_cube.vertices[0][x].coord[0], po->ben_cube.vertices[0][x].coord[1], po->ben_cube.vertices[0][x].coord[2]);
	}
}


//Computes sphere vertices
void make_sphere(OBJECT* p, double ray, int rs, int vs)
{
	//All shapes are centered at the origin
	p->ben_sphere.center.coord[0] = 0.0;
	p->ben_sphere.center.coord[1] = 0.0;
	p->ben_sphere.center.coord[2] = 0.0;
	p->ben_sphere.center.coord[3] = 1.0; //w

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
			matrix_multiplication(rotation_z, p->ben_sphere.vertices[y][x], v_start);
			
			//copy rotated coordinates into copy array for next rotation
			for (int c = 0; c < 4; c++)
			{
				copy[c] = p->ben_sphere.vertices[y][x].coord[c];
			}
			
			//Apply rotation around y
			matrix_multiplication(rotation_y, p->ben_sphere.vertices[y][x], copy);
			
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
		p->ben_sphere.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	p->ben_sphere.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(p->ben_sphere.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		p->ben_sphere.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*p->ben_sphere.local_x.coord[0]) + (up_vector[1]*p->ben_sphere.local_x.coord[1]) + (up_vector[2]*p->ben_sphere.local_x.coord[2]) + (up_vector[3]*p->ben_sphere.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (p->ben_sphere.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		p->ben_sphere.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(p->ben_sphere.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		p->ben_sphere.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	p->ben_sphere.local_z.coord[0] = (p->ben_sphere.local_x.coord[1]*p->ben_sphere.local_y.coord[2]) - (p->ben_sphere.local_x.coord[2]*p->ben_sphere.local_y.coord[1]);
	p->ben_sphere.local_z.coord[1] = (p->ben_sphere.local_x.coord[2]*p->ben_sphere.local_y.coord[0]) - (p->ben_sphere.local_x.coord[0]*p->ben_sphere.local_y.coord[2]);
	p->ben_sphere.local_z.coord[2] = (p->ben_sphere.local_x.coord[0]*p->ben_sphere.local_y.coord[1]) - (p->ben_sphere.local_x.coord[1]*p->ben_sphere.local_y.coord[0]);
	p->ben_sphere.local_z.coord[3] = 0.0; // w coordinate

}



//Compute vertices of a cylinder
void make_cylinder(OBJECT* p, double height, double ray, int rs, int vs)
{
	//All shapes are centered at the origin
	p->ben_cylinder.center.coord[0] = 0.0;
	p->ben_cylinder.center.coord[1] = 0.0;
	p->ben_cylinder.center.coord[2] = 0.0;
	p->ben_cylinder.center.coord[3] = 1.0; //w

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
			matrix_multiplication(rotation_y, p->ben_cylinder.vertices[x][y], v_start);
			
			//Copy rotated coordinates into temp array
			for (int f = 0; f < 4; f++)
			{
				temp[f] = p->ben_cylinder.vertices[x][y].coord[f];
			}
			
			//Apply translation
			matrix_multiplication(translation_y, p->ben_cylinder.vertices[x][y], temp);
			
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
		p->ben_cylinder.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	p->ben_cylinder.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(p->ben_cylinder.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		p->ben_cylinder.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*p->ben_cylinder.local_x.coord[0]) + (up_vector[1]*p->ben_cylinder.local_x.coord[1]) + (up_vector[2]*p->ben_cylinder.local_x.coord[2]) + (up_vector[3]*p->ben_cylinder.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (p->ben_cylinder.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		p->ben_cylinder.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(p->ben_cylinder.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		p->ben_cylinder.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	p->ben_cylinder.local_z.coord[0] = (p->ben_cylinder.local_x.coord[1]*p->ben_cylinder.local_y.coord[2]) - (p->ben_cylinder.local_x.coord[2]*p->ben_cylinder.local_y.coord[1]);
	p->ben_cylinder.local_z.coord[1] = (p->ben_cylinder.local_x.coord[2]*p->ben_cylinder.local_y.coord[0]) - (p->ben_cylinder.local_x.coord[0]*p->ben_cylinder.local_y.coord[2]);
	p->ben_cylinder.local_z.coord[2] = (p->ben_cylinder.local_x.coord[0]*p->ben_cylinder.local_y.coord[1]) - (p->ben_cylinder.local_x.coord[1]*p->ben_cylinder.local_y.coord[0]);
	p->ben_cylinder.local_z.coord[3] = 0.0; // w coordinate

}


//Function to compute vertices of a cone
void make_cone(OBJECT* p, double ray, double height, int rs, int vs)
{
	//All shapes are centered at the origin
	p->ben_cone.center.coord[0] = 0.0;
	p->ben_cone.center.coord[1] = 0.0;
	p->ben_cone.center.coord[2] = 0.0;
	p->ben_cone.center.coord[3] = 1.0; //w

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
	
	double v_start[4] = {0.0, 0.0, 0.0, 1.0};

	double copy[4];

	//Only x < vs-1 so that theres room left in the 2d array for the vertices of the tip of the cone
	for (int x = 0; x < (vs-1); x++)
	{
		for (int y = 0; y < rs; y++)
		{
			matrix_multiplication(translation, p->ben_cone.vertices[y][x], v_start);

			for (int j = 0; j < 4; j++)
			{
				copy[j] = p->ben_cone.vertices[y][x].coord[j];
			}

			matrix_multiplication(rotation_y, p->ben_cone.vertices[y][x], copy);

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
		matrix_multiplication(translation, p->ben_cone.vertices[y][vs-1], v_start);

		for (int j = 0; j < 4; j++)
		{
			copy[j] = p->ben_cone.vertices[y][vs-1].coord[j];
		}

		matrix_multiplication(rotation_y, p->ben_cone.vertices[y][vs-1], copy);
		
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
		p->ben_cone.local_x.coord[i] = v_start[i];
	}
	//w coordinate is 0 for vectors
	p->ben_cone.local_x.coord[3] = 0.0;

	double magnitude;

	//Find magnitude of local x to normalize it
	magnitude = find_magnitude(p->ben_cone.local_x);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		p->ben_cone.local_x.coord[j] /= magnitude;
	}

	//Local Y-axis - formula = (up - ((up dot w)*w))/magnitude

	//Choose y-axis as up vector
	double up_vector[4] = {0.0, 1.0, 0.0, 0.0};
	
	double up_dot_w;

	//(up dot w)*w)
	double w_scaled[4];
	
	up_dot_w = (up_vector[0]*p->ben_cone.local_x.coord[0]) + (up_vector[1]*p->ben_cone.local_x.coord[1]) + (up_vector[2]*p->ben_cone.local_x.coord[2]) + (up_vector[3]*p->ben_cone.local_x.coord[3]);

	for (int i = 0; i < 4; i++)
	{
		w_scaled[i] = (p->ben_cone.local_x.coord[i]*up_dot_w);
	}

	//up - ((up dot w)*w)
	for (int i = 0; i < 4; i++)
	{
		p->ben_cone.local_y.coord[i] = up_vector[i] - w_scaled[i];
	}

	//Find magnitude of local y to normalize it
	magnitude = find_magnitude(p->ben_cone.local_y);
			
	//Normalize by magnitude
	//Don't include w coordinate
	for (int j = 0; j < 3; j++)
	{
		p->ben_cone.local_y.coord[j] /= magnitude;
	}

	//Local z-axis = x cross y

	//Cross product
	p->ben_cone.local_z.coord[0] = (p->ben_cone.local_x.coord[1]*p->ben_cone.local_y.coord[2]) - (p->ben_cone.local_x.coord[2]*p->ben_cone.local_y.coord[1]);
	p->ben_cone.local_z.coord[1] = (p->ben_cone.local_x.coord[2]*p->ben_cone.local_y.coord[0]) - (p->ben_cone.local_x.coord[0]*p->ben_cone.local_y.coord[2]);
	p->ben_cone.local_z.coord[2] = (p->ben_cone.local_x.coord[0]*p->ben_cone.local_y.coord[1]) - (p->ben_cone.local_x.coord[1]*p->ben_cone.local_y.coord[0]);
	p->ben_cone.local_z.coord[3] = 0.0; // w coordinate

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


//Draw world axes
void draw_axes( void ) 
{
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

void draw_objects()
{

  for(int i=0; i<num_objects; i++)
  {
    OBJECT *cur;
    cur = &my_objects[i];

	//printf("%d object - spec %f, %f, %f, %f\n", i, cur->spec[0], cur->spec[1], cur->spec[2], cur->spec[3]);
    glMaterialfv(GL_FRONT, GL_AMBIENT, cur->amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cur->diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cur->spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, &cur->shine);
    glMaterialfv(GL_FRONT, GL_EMISSION, cur->emi);

	switch(cur->sid)
	{
		case 1: //cube
			//printf("Draw cube!\n");
			draw_cube_brute(cur->ben_cube);
			//draw_normals(cur->ben_cube, 1, 8);
			break;
		case 3: //sphere
			//printf("Draw sphere!\n");
			draw_sphere(cur->ben_sphere, crt_rs, crt_vs);
			//draw_normals(cur->ben_sphere, crt_rs, crt_vs);
			break;
		case 4: //cylinder
			//printf("Draw cylinder!\n");
			draw_cylinder(cur->ben_cylinder, crt_rs, crt_vs);
			//draw_normals(cur->ben_cylinder, crt_rs, crt_vs);
			break;
		case 5: //cone
			//printf("Draw cone!\n");
			draw_cone(cur->ben_cone, crt_rs, crt_vs);
			//draw_normals(cur->ben_cone, crt_rs, crt_vs);
			break;
		default: break;
	}

  }
}


void my_display() 
{

  // clear all pixels, reset depth 
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

  //Load onto matrix stack default perspective camera
  glMatrixMode(GL_PROJECTION);
  //Load identity matrix
  glLoadIdentity();

  //Push Perspective matrix onto stack
  gluPerspective(my_cam.fov*my_cam.zoom_factor, my_cam.aspect_ratio, my_cam.near, my_cam.far);

  glMatrixMode(GL_MODELVIEW);
  //Load Identity matrix
  glLoadIdentity();

  gluLookAt(my_cam.pos[0], my_cam.pos[1], my_cam.pos[2], my_cam.at[0], my_cam.at[1], my_cam.at[2], my_cam.up[0], my_cam.up[1], my_cam.up[2]);
  
  //draw the objects
  draw_axes();
  draw_objects();
  
  float end_point[4];
  
  end_point[0] = my_ray.rayStart[0] + 30*my_ray.rayDirection[0];
  end_point[1] = my_ray.rayStart[1] + 30*my_ray.rayDirection[1];
  end_point[2] = my_ray.rayStart[2] + 30*my_ray.rayDirection[2];
  end_point[3] = 1.0;
  
  //draw ray
  glLineWidth( 2.5 );
  glBegin(GL_LINES); 
  {
	glColor3fv(colors[GREY]);
	glVertex4fv(my_ray.rayStart);
	glVertex4fv(end_point);

  }
  glEnd();
  glLineWidth( 1.0 );
	  
  draw_marker(marker1.ben_sphere, crt_rs, crt_vs);
  draw_marker(marker2.ben_sphere, crt_rs, crt_vs);

  // this buffer is ready
  glutSwapBuffers();
}

//Recalculates camera local coord system
void recalculate_local_coord()
{
  CAM *pc = &my_cam;

  //Build camera local coordinate axes
  pc->w_vector.coord[0] = pc->at[0] - pc->pos[0];
  pc->w_vector.coord[1] = pc->at[1] - pc->pos[1];
  pc->w_vector.coord[2] = pc->at[2] - pc->pos[2];
  pc->w_vector.coord[3] = 0.0;

  double m;

  //dot product
  //Don't include w coordinate
  m = (pc->w_vector.coord[0]*pc->w_vector.coord[0]) + (pc->w_vector.coord[1]*pc->w_vector.coord[1]) + (pc->w_vector.coord[2]*pc->w_vector.coord[2]);

  //take square root
  m = sqrt(m);
  if (m != 0)
  {
    //normalize
    pc->w_vector.coord[0] /= m;
    pc->w_vector.coord[1] /= m;
    pc->w_vector.coord[2] /= m;
  }

    //V axis - formula = (up - ((up dot w)*w))/magnitude

  double up_dot_w;

  //(up dot w)*w)
  double w_scaled[4];

  up_dot_w = (pc->up[0]*pc->w_vector.coord[0]) + (pc->up[1]*pc->w_vector.coord[1]) + (pc->up[2]*pc->w_vector.coord[2]);

  //printf("up . w = %f\n", up_dot_w);

  for (int i = 0; i < 4; i++)
  {
    w_scaled[i] = (pc->w_vector.coord[i]*up_dot_w);
  }

  //up - ((up dot w)*w)
  for (int i = 0; i < 4; i++)
  {
    pc->v_vector.coord[i] = pc->up[i] - w_scaled[i];
    //printf("v vector in creation - %f\n", v_vector.coord[i]);
  }

      
  //Normalize by magnitude
  //dot product
  //Don't include w coordinate
  m = (pc->v_vector.coord[0]*pc->v_vector.coord[0]) + (pc->v_vector.coord[1]*pc->v_vector.coord[1]) + (pc->v_vector.coord[2]*pc->v_vector.coord[2]);

  //take square root
  m = sqrt(m);
  if (m != 0)
  {
    //normalize
    pc->v_vector.coord[0] /= m;
    pc->v_vector.coord[1] /= m;
    pc->v_vector.coord[2] /= m;
  }

  //U axis = w cross v

  //Cross product
  pc->u_vector.coord[0] = (pc->w_vector.coord[1]*pc->v_vector.coord[2]) - (pc->w_vector.coord[2]*pc->v_vector.coord[1]);
  pc->u_vector.coord[1] = (pc->w_vector.coord[2]*pc->v_vector.coord[0]) - (pc->w_vector.coord[0]*pc->v_vector.coord[2]);
  pc->u_vector.coord[2] = (pc->w_vector.coord[0]*pc->v_vector.coord[1]) - (pc->w_vector.coord[1]*pc->v_vector.coord[0]);
  pc->u_vector.coord[3] = 0.0; // w coordinate

  /*for (int x = 0; x < 4; x++)
  {
    printf("w - %f, %f, %f, %f\n", pc->w_vector.coord[0], pc->w_vector.coord[1], pc->w_vector.coord[2], pc->w_vector.coord[3]);
    printf("v - %f, %f, %f, %f\n", pc->v_vector.coord[0], pc->v_vector.coord[1], pc->v_vector.coord[2], pc->v_vector.coord[3]);
    printf("u - %f, %f, %f, %f\n", pc->u_vector.coord[0], pc->u_vector.coord[1], pc->u_vector.coord[2], pc->u_vector.coord[3]);
  }*/

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

void matrix_multiplication2(double m[4][4], float* v, double start[4])
{	
	//compute dot products of matrix rows with vertex
	for (int x = 0; x < 3; x++)
	{
		v[x] = (m[x][0]*start[0]) + (m[x][1]*start[1]) + (m[x][2]*start[2]) + (m[x][3]*start[3]);
	}

	return;
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

//Draws a cube and computes its vertex normals
void draw_cube_brute(Shape& p)
{

  draw_triangle(p.vertices, 4,5,1,BLUE);
  compute_cube_normals(&p, 4, 5, 1);

  draw_triangle(p.vertices, 0,4,1,BLUE);
  compute_cube_normals(&p, 0, 4, 1);
  
  draw_triangle(p.vertices, 5,6,2,CYAN);
  compute_cube_normals(&p, 5, 6, 2);
  
  draw_triangle(p.vertices, 1,5,2,CYAN);
  compute_cube_normals(&p, 1, 5, 2);
  
  draw_triangle(p.vertices, 3,2,6,YELLOW);
  compute_cube_normals(&p, 3, 2, 6);
  
  draw_triangle(p.vertices, 7,3,6,YELLOW);
  compute_cube_normals(&p, 7, 3, 6);
  
  draw_triangle(p.vertices, 0,3,7,MAGENTA);
  compute_cube_normals(&p, 0, 3, 7);
  
  draw_triangle(p.vertices, 4,0,7,MAGENTA);
  compute_cube_normals(&p, 4, 0, 7);

  //Draw a top to the cube with triangles
  draw_triangle(p.vertices, 0,1,2,GREY);
  compute_cube_normals(&p, 0, 1, 2);
  draw_triangle(p.vertices, 3,0,2,GREY);
  compute_cube_normals(&p, 3, 0, 2);

  //Draw a bottom to the cube with triangles
  draw_triangle(p.vertices, 4,5,6,GREEN);
  compute_cube_normals(&p, 4, 5, 6);
  draw_triangle(p.vertices, 7,4,6,GREEN);
  compute_cube_normals(&p, 7, 4, 6);

  float m;

  //Normalize vertex normals
  for (int x = 0; x < 8; x++)
  {
	  //Don't include w coordinate
	  m = (p.normals[0][x].coord[0]*p.normals[0][x].coord[0]) + (p.normals[0][x].coord[1]*p.normals[0][x].coord[1]) + (p.normals[0][x].coord[2]*p.normals[0][x].coord[2]);
	  //take square root
	  m = sqrt(m);
	  p.normals[0][x].coord[0] /= m;
	  p.normals[0][x].coord[1] /= m;
	  p.normals[0][x].coord[2] /= m;
  }
    
}

//Compute face normal of plane that vertex a, b, c lie on
void compute_cube_normals(Shape* s_ptr, int a, int b, int c)
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


//Draw cylinder vertices in counter-clockwise order
//Compute vertex normals inside draw functions
void draw_cylinder(Shape& s, int rs, int vs)
{
	Particle p0, p1, p2, answer;

	for (int x = 0; x < rs; x++)
	{
		for (int y = 0; y < vs; y++)
		{
			draw_param_quad(s.vertices, x, y, WHITE);

			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = s.vertices[x][y].coord[j];
				p1.coord[j] = s.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = s.vertices[(x+1)%rs][(y+1)%vs].coord[j];
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
				s.normals[x][y].coord[j] += answer.coord[j];
				//Skip if x = 0
				if (x > 0)
				{
					s.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
					s.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				}
				
				s.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			s.normals[x][y].coord[3] = 1.0;
			s.normals[(x+1)%rs][y].coord[3] = 1.0;
			s.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			s.normals[x][(y+1)%vs].coord[3] = 1.0;

		}
	}

	//Normalize computed vertex normals
	normalize(s);
}

//Draw ray intersection markers (tiny red spheres)
void draw_marker(Shape& s, int rs, int vs)
{
	for (int x = 0; x < rs; x++)
	{
		//(vs-1) so that the column index doesnt go out of bounds
		for (int y = 0; y < vs; y++)
		{
			glBegin(GL_POLYGON);
			{	
				glColor3fv(colors[RED]);

				//Draw vertices in counter clockwise order with glvertex for doubles
				glVertex4dv(s.vertices[x][y].coord);
				//Modulus so it reconnects the end of the rows with the beginning of the rows to create a full circle
				glVertex4dv(s.vertices[(x+1)%rs][y].coord);
				
				//Skip if y = vs-1 so we don't go out of bounds in the array
				if (y != vs-1)
				{
					//No modulus to column index because otherwise the poles will connect with a line
					glVertex4dv(s.vertices[(x+1)%rs][(y+1)].coord);
					glVertex4dv(s.vertices[x][(y+1)].coord);
				}

			}
			glEnd();
		}
	}
}



//Draw sphere vertices
//Compute sphere vertex normals
void draw_sphere(Shape& s, int rs, int vs)
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
				glVertex4dv(s.vertices[x][y].coord);
				//Modulus so it reconnects the end of the rows with the beginning of the rows to create a full circle
				glVertex4dv(s.vertices[(x+1)%rs][y].coord);
				
				//Skip if y = vs-1 so we don't go out of bounds in the array
				if (y != vs-1)
				{
					//No modulus to column index because otherwise the poles will connect with a line
					glVertex4dv(s.vertices[(x+1)%rs][(y+1)].coord);
					glVertex4dv(s.vertices[x][(y+1)].coord);
				}

			}
			glEnd();
		
			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = s.vertices[x][y].coord[j];
				p1.coord[j] = s.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = s.vertices[(x+1)%rs][(y+1)%vs].coord[j];
			}
			
			cross_product(answer, p1, p0, p2);

			//Add answer to all vertices that lie on the face that answer is orthogonal to
			//Vector addition
			for (int j = 0; j < 3; j++)
			{
				s.normals[x][y].coord[j] += answer.coord[j];
				s.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
				s.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				s.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			s.normals[x][y].coord[3] = 1.0;
			s.normals[(x+1)%rs][y].coord[3] = 1.0;
			s.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			s.normals[x][(y+1)%vs].coord[3] = 1.0;
		}
	}

	normalize(s);
}


//Draw cone shape
//Compute vertex normals
void draw_cone(Shape& s, int rs, int vs)
{
	Particle p0, p1, p2, answer;

	//Draw vertices
	for (int x = 0; x < rs; x++)
	{
		for (int y = 0; y < vs; y++)
		{
			draw_param_quad(s.vertices, x, y, YELLOW);

			//Get points used to compute vertex normal
			for (int j = 0; j < 4; j++)
			{
				p0.coord[j] = s.vertices[x][y].coord[j];
				p1.coord[j] = s.vertices[(x+1)%rs][y].coord[j];
				p2.coord[j] = s.vertices[(x+1)%rs][(y+1)%vs].coord[j];
			}
			
			cross_product(answer, p1, p0, p2);

			//Add answer to all vertices that lie on the face that answer is orthogonal to
			//Vector addition
			for (int j = 0; j < 3; j++)
			{
				s.normals[x][y].coord[j] += answer.coord[j];
				s.normals[(x+1)%rs][y].coord[j] += answer.coord[j];
				s.normals[(x+1)%rs][(y+1)%vs].coord[j] += answer.coord[j];
				s.normals[x][(y+1)%vs].coord[j] += answer.coord[j];
			}

			//Set w coordinate
			s.normals[x][y].coord[3] = 1.0;
			s.normals[(x+1)%rs][y].coord[3] = 1.0;
			s.normals[(x+1)%rs][(y+1)%vs].coord[3] = 1.0;
			s.normals[x][(y+1)%vs].coord[3] = 1.0;
		}
	}

	normalize(s);
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
