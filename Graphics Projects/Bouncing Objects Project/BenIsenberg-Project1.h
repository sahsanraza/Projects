#ifndef _PROJECTONE_H
#define _PROJECTONE_H

struct Shape; // forward declaration

void glut_setup(void) ;
void gl_setup(void) ;
void my_setup(void);
void my_display(void) ;
void my_reshape(int w, int h) ;
void my_idle(void) ;
void my_mouse(int b, int s, int x, int y) ;
void my_keyboard(unsigned char c, int x, int y) ;
void my_TimeOut(int id) ;
double detect_collision(Shape a, Shape b);  // function calculates distance between the center of two objects

#endif
