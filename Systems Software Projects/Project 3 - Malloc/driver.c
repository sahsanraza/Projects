/* Ben Isenberg
/* 7/10/11
/* bji6@pitt.edu */


#include "mymalloc.h"

/*My malloc test program*/

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
		/*printf("%x\n", curr);*/

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

			printf("%d is Largest Size\n", largestSize);
			printf("0x%x is curr->next\n", (int*)curr->next);
			printf("%d is bestSpot->free\n", bestSpot->free);

			/*If at end of list and best spot is found set it to not free and return its address*/
			if((int*)curr->next == 0 && largestSize >= size && bestSpot->free == 1){
				bestSpot->free = 0;

				int *p = bestSpot;

				printf("0x%x is best spot\n", p);
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
				printf("0x%x is where p is now\n", p);
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
	/*printf("%d size\n", *ptr2);*/

	ptr2 += 4;
	ptr2 += (size/4);
	int address = ptr2;

	/*if freeing the last node in list reduce brk*/
	if(address == sbrk(0)){
		printf("0x%x SBRK\n", sbrk(0));
		/*printf("0x%x ADDRESS\n", address);*/
		int k = (size + 16);
		sbrk(-k);
		printf("0x%x SBRK\n", sbrk(0));
	}

	ptr -= 12;

	int *p = ptr;
	printf("0x%x is old FREE\n", *p);
	*p = 1;		/*set free to true*/
	printf("0x%x is new FREE\n", *p);
}



#define MALLOC(size) my_worstfit_malloc(size)


#define FREE(ptr) my_free(ptr)


int main(){

	int ptr = sbrk(0);

	int *ptr2 = MALLOC(17);

	int *ptr3;

	int *ptr4 = MALLOC(20);
	int *ptr5;

	printf("0x%x is old brk\n", ptr);
	FREE(ptr2);
	*ptr2 = 7;
	*ptr4 = 50;
	printf("%d is num stored at 0x%x\n", *ptr4, ptr4);

	
	ptr5 = MALLOC(5);

	printf("%d is num stored at 0x%x\n", *ptr2, ptr2);
	*ptr5 = 6;

	printf("%d is num stored at 0x%x\n", *ptr4, ptr4);
	printf("%d is num stored at 0x%x\n", *ptr5, ptr5);

	FREE(ptr4);
	ptr3 = MALLOC(28);
	
	printf("%d is num stored at 0x%x\n", *ptr3, ptr3);
	FREE(ptr3);


	return 0;
}


