/* Ben Isenberg
/* 7/10/11
/* bji6@pitt.edu */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//include your code
#include "mymalloc.h"

/*Size is 16 bytes*/
struct list_node {
	int size;		/*32 bits = 4bytes*/
	int free;		/*32 bits = 4bytes*/
	struct list_node *previous;		/*32 bits = 4bytes*/
	struct list_node *next;		/*32 bits = 4bytes*/
};

struct list_node *start;
struct list_node *curr;

/*Malloc worst fit*/	
void *my_worstfit_malloc(int size){

	int actual_size = size + sizeof(struct list_node); /*calculate amount of space needed in memory*/

	if((int*)start == NULL){ /*if list hasnt been made yet*/
		
		start = sbrk(0);	/*initialize list at beginning of heap*/
		curr = start;		/*curr points to start of list*/
		
		int *p = sbrk(0);

		sbrk(actual_size);		/*grow heap*/

		curr->size = size;
		curr->free = 0;
		curr->previous = NULL;
		curr->next = NULL;

		/*Add 4 to move past where node is stored to where actual data should be placed*/
		p += 4;
		return p;
	}
	else{
		curr = start; /*curr points to start of list*/

		int largestSize = curr->size;
		struct list_node *bestSpot = start;

		while(1){

			/*If at end of list and best spot is found set it to not free and return its address*/
			if((int*)curr->next == 0 && largestSize >= size && bestSpot->free == 1){
				bestSpot->free = 0;

				int *p = bestSpot;
				/*Add 4 to move past where node is stored to where actual data should be placed*/
				return p + 4;
			}

			/*If at end of list and no suitable candidate exits, add new node to end of list and return its address*/
			if((int*)curr->next == 0 && largestSize <= size){
				struct list_node *node = sbrk(0);
			
				sbrk(actual_size);
				node->size = size;
				node->free = 0;
				node->previous = curr;
				node->next = NULL;

				curr->next = node;
				curr = curr->next;

				int *p = curr;
				/*Add 4 to move past where node is stored to where actual data should be placed*/
				return p + 4;
			}

			curr = curr->next;

			if(curr != 0 && curr->size > largestSize && curr->free == 1){
				largestSize = curr->size;
				bestSpot = curr;
			}
		}
	}

	return 0;
}

/*Malloc Free function*/
void my_free(void *ptr){												
	int *ptr2 = ptr;
	ptr2 -= 4;

	int size = *ptr2;	/*size of current allocation spot*/
	ptr2 += 4;
	ptr2 += (size/4);
	int address = ptr2;

	/*if freeing the last node in list reduce brk*/
	if(address == sbrk(0)){
		int k = (size + 16);
		sbrk(-k);
	}
	
	ptr -= 12;									
												
	int *p = ptr;										
	*p = 1;		/*set free to true*/												
}

//replace malloc here with the appropriate version of mymalloc
#define MALLOC(size) my_worstfit_malloc(size)


//replace free here with the appropriate version of myfree
#define FREE(ptr)  my_free(ptr)		


//You can adjust how many things are allocated
#define TIMES 15


//Do not modify below here
struct tree {
	int data;
	struct tree *left;
	struct tree *right;
};

void freetree(struct tree *root)
{
	if(root->left != NULL)
		freetree(root->left);
	if(root->right != NULL)
		freetree(root->right);
	FREE(root);
}

void randominsert(struct tree *head, struct tree *new)
{
	int way;
	struct tree *curr, *prev;
	prev = NULL;
	curr = head;

	while(curr != NULL)
	{
		prev = curr;
		way = rand()%2;
		if(way == 0)
		{
			curr = curr->left;
		}
		else
		{
			curr = curr->right;
		}
	}
	if(way == 0)
		prev->left = new;
	else
		prev->right = new;
}

void printtree(struct tree *head)
{
	struct tree *curr = head;
	if(head == NULL)
		return;

	printtree(curr->left);	
	printf("%d\n", curr->data);
	printtree(curr->right);
}			 

void test1()
{
	int i;
 	
	struct tree *head = (struct tree *)MALLOC(sizeof(struct tree));
	head->data = 0;
	head->left = NULL;
	head->right = NULL;

	for(i=1;i<TIMES;i++)
	{
		struct tree *new = (struct tree *)MALLOC(sizeof(struct tree));
		new->data = i;
		new->left = NULL;
		new->right = NULL;
		randominsert(head, new);
	}

	printtree(head);
	freetree(head);
}

int comp(const void *a, const void *b)
{
	return *((int *)a) - *((int *)b);
}

void test2()
{
	int *a;
	int i;

	a = (int *)MALLOC(TIMES * sizeof(int));

	for(i=0;i<TIMES;i++)
	{
		a[i] = rand()%TIMES + 1;
	}

	qsort(a, TIMES, sizeof(int), comp);

	for(i=0;i<TIMES;i++)
	{
		printf("%d\n", a[i]);
	}

	FREE(a);
	
}

int main() 
{
	srand((unsigned int)time(NULL));
	printf("original val brk: %p\n", sbrk(0));
	test1();
	printf("brk after  test1: %p\n", sbrk(0));
	test2();
	printf("brk after  test2: %p\n", sbrk(0));
	

	return 0;
}
