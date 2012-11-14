/* Ben Isenberg
/* 7/10/11
/* bji6@pitt.edu */

/*malloc header file with function prototypes*/

#include<stdio.h>

#ifndef MYMALLOC_H
#define MYMALLOC_H


void *my_worstfit_malloc(int size);

void my_free(void *ptr);		


#endif
